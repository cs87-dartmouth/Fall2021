import math
import os
import shutil
from xml.dom.minidom import Document

import bpy
import bpy_extras
from bpy.props import BoolProperty, IntProperty, StringProperty
from bpy_extras.io_utils import ExportHelper
from mathutils import Matrix
from mathutils import Vector
import json

bl_info = {
    "name": "Export as DARTS scene",
    "author": "Shaojie Jiao",
    "version": (0, 1),
    "blender": (2, 80, 0),
    "location": "File > Export > DARTS exporter (.json)",
    "description": "Export DARTS scene format (.json)",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Import-Export"}

class DartsWriter:

    def __init__(self, context, filepath):
        self.context = context
        self.filepath = filepath
        self.working_dir = os.path.dirname(self.filepath)
        self.items = [thing for thing in self.context.scene.objects]


    def export_camera(self, cam):
        print('exporting camera')

        #default camera
        cam_json = {
        "transform": {
        "from": [
        1, 1, 1
        ],
        "at": [
        0, 0, 0
        ],
        "up": [0, 1, 0]
        },
        "vfov": 30.0,
        "resolution": [1280, 1280]
        }

        #fov & resolution
        cam_json["vfov"]= cam.data.angle * 180 / math.pi
        percent = self.context.scene.render.resolution_percentage / 100.0
        cam_json["resolution"][0] = int(self.context.scene.render.resolution_x * percent)
        cam_json["resolution"][1] = int(self.context.scene.render.resolution_y * percent)

        #up, lookat, and position 
        up = cam.matrix_world.to_quaternion() @ Vector((0.0, 1.0, 0.0))
        direction = cam.matrix_world.to_quaternion() @ Vector((0.0, 0.0, -1.0))
        loc = cam.location

        up[1],up[2] = up[2],up[1]
        direction[1],direction[2] = direction[2],direction[1]
        loc[1],loc[2] = loc[2],loc[1]

        #set the values and return
        for i in range(0,3):
            cam_json["transform"]["from"][i] = loc[i]
            cam_json["transform"]["up"][i] = up[i]
            cam_json["transform"]["at"][i] = loc[i] + direction[i]
        return cam_json


    #adds default values to make the scene complete
    def make_misc(self,jso):
        json_sampler ={
        "type": "independent",
        "samples": 100}
        json_background = [5, 5, 5]
        json_accelerator = {"type": "bbh"}

        json_materials= [
        {
            "type": "lambertian",
            "name": "default",
            "albedo": 0.2
        }
        ]

        jso["sampler"] = json_sampler 
        jso["background"] = json_background
        jso["accelerator"] = json_accelerator
        jso["materials"] = json_materials



    # export meshes to "meshes/" and then point to them in the scene file
    def export_mesh(self, mesh):
        print('exporting mesh')
        viewport_selection = self.context.selected_objects
        bpy.ops.object.select_all(action='DESELECT')

        obj_name = mesh.name + ".obj"
        obj_path = os.path.join(self.working_dir, 'meshes', obj_name)
        mesh.select_set(True)
        bpy.ops.export_scene.obj(filepath=obj_path, check_existing=False,
                                    use_selection=True, use_edges=False, use_smooth_groups=False,
                                    use_materials=False, use_triangles=True, use_mesh_modifiers=True)
        
        #using a default lambertian material
        obj_json = {
        "type": "mesh",
        "name": "",
        "filename": "",
        "material": "default"
        }
        
        obj_json["name"]=mesh.name
        obj_json["filename"]="meshes/%s.obj"%mesh.name
        
        for ob in viewport_selection:
            ob.select_set(True)

        mesh.select_set(False)
        return obj_json



    def write(self):
        """Main method to write the blender scene into DARTS format"""

        data_all ={}

        n_samples = 32

        #only exporting one camera
        cameras = [cam for cam in self.context.scene.objects
            if cam.type in {'CAMERA'}]
        
        if(len(cameras) == 0):
            print("WARN: No camera to export")
        else:
            if(len(cameras) > 1):
                print("WARN: Multiple cameras found, only exporting the active one")

        data_cam = self.export_camera(self.context.scene.camera)
        data_all["camera"] = data_cam

        #adding defaults
        self.make_misc(data_all)

        #export meshes
        if not os.path.exists(self.working_dir + "/meshes"):
            os.makedirs(self.working_dir + "/meshes")

        meshes = [obj for obj in self.context.scene.objects
                  if obj.type in {'MESH', 'FONT', 'SURFACE', 'META'}]
        print(meshes)

        #point to the exported meshes         
        data_all["surfaces"]=[]

        for mesh in meshes:
            cur_obj_json = self.export_mesh(mesh)
            data_all["surfaces"].append(cur_obj_json)

        # write the json file 
        with open(self.filepath, "w") as dump_file:
            exported_json_string = json.dumps(data_all, indent=4)
            dump_file.write(exported_json_string)
            dump_file.close

        


class DARTSExporter(bpy.types.Operator, ExportHelper):
    """Export a blender scene into DARTS scene format"""

    # add to menu
    bl_idname = "export_scene.darts"
    bl_label = "Export DARTS scene"

    filename_ext = ".json"
    filter_glob: StringProperty(default="*.json", options={'HIDDEN'})

    def execute(self, context):
        nori = DartsWriter(context, self.filepath)
        nori.write()

        print(nori.filepath)


        return {'FINISHED'}




def menu_func_export(self, context):
    self.layout.operator(DARTSExporter.bl_idname, text="Export DARTS scene...")

def register():
    bpy.utils.register_class(DARTSExporter)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(DARTSExporter)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()