@page assignment3 A3: Textures

@m_footernavigation
@tableofcontents

@m_class{m-note m-danger}

@par Note
    Make sure to do a `git pull upstream remote` to retrieve any updates to the base code before you begin!

We will primarily be referring to chapters of [Ray Tracing - The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html) in this assignment.

Task 1: The Texture class
=========================
@m_span{m-label m-primary m-flat} 1 pt @m_endspan

Read [Ray Tracing - The Next Week: Chapter 4](https://raytracing.github.io/books/RayTracingTheNextWeek.html#solidtextures), and create a `Texture` base class following this chapter (perhaps in a new `texture.h` file within your `include/darts` directory. Remember to add this file to the list of headers or source files in `CMakeLists.txt`. Instead of accepting `u`, `v`, and `p` parameters as done in the book, we suggest you have your `Texture::value()` function take a `const HitInfo &` as the parameter (you can then retrieve `u`, `v`, and `p` from within this `HitInfo`).

Here is what our interface looks like:

@code{.cpp}
// Base class for all textures
class Texture
{
public:
    Texture(const json &j = json::object());
    virtual ~Texture() = default;

    virtual Color3f value(const HitInfo &hit) const = 0;
};
@endcode

We put our implementation file in `%src/textures/texture.cpp`.

Constant textures
-----------------

Create a derived `class ConstantTexture : public Texture` which has a member variable `Color3f color;` and whose `value(...)` function just returns it. Make a constructor that takes a `const json &j` and tries to assign the parameter named `"color"` to your color member variable by using `j.value("color", color);`. We put both our class definition and implementation into the file `src/textures/constant.cpp`.

Retrofitting the parser
-----------------------

To make this work with the rest of darts, we need to refactor our code a bit so that `Material`s use `Texture`s instead of `Color3f`s, and so that it plays nice with our JSON parser.

We first need to inform our JSON parser how to create each type of texture, and what type string (e.g. `"constant"`) to look for when creating it. Calling the following macro at the bottom of `%src/textures/constant.cpp` will do this for the new `ConstantTexture`:

@code{.cpp}
DARTS_REGISTER_CLASS_IN_FACTORY(Texture, ConstantTexture, "constant")
@endcode


Now, change all the `Color3f` members of your `Material`s to `Texture`s, e.g. `Color3f albedo;` in `Lambertian` becomes `shared_ptr<const Texture> albedo;`. You'll also need to change the corresponding constructors of any `Material` to call `DartsFactory::create()` instead of reading a color directly from the passed `json` object, e.g. this:
    
~~~{.cpp}
albedo = j.value("albedo", albedo);
~~~

becomes:

~~~{.cpp}
albedo      = DartsFactory<Texture>::create(j.at("albedo"));
~~~

`DartsFactory::create()` will look for a `"type"` field in the passed-in `json` object, and create the appropriate `Texture` subclass. This works because of the `DARTS_REGISTER_CLASS_IN_FACTORY` macro we called earlier.

You'll also want to support texturing the `Metal::roughness` parameter. This is currently a float, but you can still turn it into a `Texture`, and when calling its `value(...)` function, just convert to a scalar using the `luminance()` function in `darts/vec.h`. An alternative approach would have been to treat color and grayscale textures separately, or allow roughness to differ across the three color channels, but we'll stick with the simpler approach.


@m_div{m-col-s-6 m-col-m-4 m-right-s}
<a href="constant-cornell-box-ref.png"><img src="constant-cornell-box-ref.png"/></a>
@m_enddiv


Once you have this implemented, you should be able to render the scene `scenes/assignment3/constant-cornell-box.json`. This scene looks similar to the Cornell box from the previous assignment, except all albedos are now specified with a constant texture.

@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

Backwards scene compatibility
-----------------------------

One nuisance with our current solution is that we would have to re-write all our previous scene files (that directly specified a color) to instead use a `"constant"` texture. Previously we could have simply written:  

~~~{.json}
"albedo": [0.8, 1.0, 0.3],
~~~

in the scene file, but now we have to write the more clunky:  

~~~{.json}
"albedo":
{
    "type": "constant",
    "color": [0.8, 1.0, 0.3]
}
~~~

Let's add backwards compatibility so that the scene can specify colors directly too.

Take a look at `DartsFactory::create()` in `darts/factory.h`. Recall that this method of our `DartsFactory` is responsible for creating instances of materials, surfaces, etc. (determined by the base class `T`) by checking for a `"type"` field in the passed-in `json` object. We'd like the factory to do something a bit more sophisticated when we try to create a subclass of `Texture`.

To do that, we'll need to create a specialization of the `DartsFactory::create()` method that is used specifically when the template type is a `Texture`. Create a method in `factory.h` with the following signature (just below the current `DartsFactory::create()` implementation):

\code{.cpp}
template <>
inline shared_ptr<Texture> DartsFactory<Texture>::create(const json &j)
{
    // your code here
}
\endcode

Start by copying over the implementation of the generic `DartsFactory::create()` method.

To allow backwards compatibility, you can inspect the `const json &j` by calling `j.is_object()`, `j.is_array()` or `j.is_number()`. If `j` is an object, you can proceed just as in the generic implementation to construct the appropriate `Texture`. However, if `j` is an array or number, you should create a `ConstantTexture` directly (you may also need to modify the `ConstantTexture` constructor to handle this).

Once you have this implemented, you should be able to render all the scenes from the previous assignment. Give it a try.

Task 2: Solid textures
======================
@m_span{m-label m-primary m-flat} 2.5 pts @m_endspan

You now have the framework to easily add other types of textures, and these textures can be used any place you previously used a fixed `Color3f`! 

Checkerboard
------------
@m_span{m-label m-primary m-flat} 1 pt @m_endspan

@m_div{m-col-s-6 m-col-m-4 m-right-s}
<a href="checker-ref.png"><img src="checker-ref.png"/></a>
@m_enddiv

Add a `CheckerTexture` class following the description in [Section 4.3](https://raytracing.github.io/books/RayTracingTheNextWeek.html#solidtextures/acheckertexture) of the book, but adapted to the darts framework. Your constructor should look for fields named `"odd"` and `"even"` and use those together with `DartsFactory<Texture>::create()` to create the corresponding odd and even textures. Make sure to register the new texture with the factory using the `DARTS_REGISTER_CLASS_IN_FACTORY` macro like before, but this time associate it with the type string `"checker"`.

Once you have this implemented, you should be able to render the scene `scenes/assignment3/checker.json` producing an image like the one shown here.

@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

 <!-- **FIXME: OPTIONAL?** -->
Perlin noise
------------
@m_span{m-label m-primary m-flat} 1.5 pts @m_endspan

@m_div{m-col-s-6 m-col-m-4 m-right-s}
<a href="marble-128spp.png"><img src="marble-128spp.png"/></a>
@m_enddiv

Read [Chapter 5](https://raytracing.github.io/books/RayTracingTheNextWeek.html#perlinnoise) of the book. Implement the `Perlin` class. Also implement the `noise_texture` class from the book, but call this `MarbleTexture` instead. Allow the JSON scene file to specify the `scale` member variable of your `MarbleTexture`, and instead of always interpolating between black and white as in the book, allow the user to specify these two colors in the JSON scene file just like we did with `"odd"` and `"even"` for the checkerboard. Look for the named fields `"veins"` and `"base"` in the JSON object for this purpose. Make sure to register the texture with the `DartsFactory` and add your `Perlin` and `MarbleTexture` file to your `CMakeLists.txt`.

Render `scenes/assignment3/marble.json`.

@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

Transforming texture lookups (optional)
---------------------------------------

Particularly with procedural textures, you might find it useful to be able to scale, rotate, and otherwise transform how the textures are placed on your surfaces. You could accomplish this by adding a `Transform xform;` member variable in any `Texture` that needs this, and transform the lookup point (`hit.p`) by `xform` in the class's `value(const HitInfo & hit)` method. To avoid duplicating this functionality in each derived class that needs it, we simply added the `Transform xform;` member variable directly to the `Texture` base class, so all textures inherit it. We read this in from the `json` object in the `Texture` base class constructor `Texture::Texture(const json & j)`, but only look for it if `j.is_object() == true`. This allows our shorthand for specifying constant colors directly as `json` arrays to continue working.


Task 3: Image texture mapping
=============================
@m_span{m-label m-primary m-flat} 2.5 pts @m_endspan

The ImageTexture class
----------------------
@m_span{m-label m-primary m-flat} 1 pt @m_endspan

Read [Chapter 6](https://raytracing.github.io/books/RayTracingTheNextWeek.html#imagetexturemapping) of the book and implement image texturing mapping. We already provide you with image loading support, which you should leverage using the `Image3f` class defined in `include/darts/image.h` and `src/image.cpp`. Your `ImageTexture` class should accept a `"filename"` string parameter from the `json` object. Once you read this string, use the global file resolver to resolve this filename into a path, like so:

~~~{.cpp}
string path = get_file_resolver().resolve(filename).str();
~~~

`get_file_resolver()` is defined in `darts/common.h`.


Texture coordinates
-------------------
@m_span{m-label m-primary m-flat} 1.5 pts @m_endspan

Extend your `Sphere::intersect()` function to output the UV coordinates as [described in the book](https://raytracing.github.io/books/RayTracingTheNextWeek.html#solidtextures/texturecoordinatesforspheres). Since there are many different conventions for spherical coordinates and uv coordinates, we already provide handy functions to consistently perform the Cartesian-to-spherical or spherical-to-Cartesian coordinate mappings for you. They are `direction_to_spherical_uv()` and `spherical_uv_to_direction()` in `include/darts/common.h`. You should now be able to render `scenes/assignment3/earth.json`.

Next, extend your `Quad::intersect()` function to output UV coordinates. Render `scenes/assignment3/textured-box.json`. 

Finally, extend `single_triangle_intersect()` to output UV coordinates. The function already receives three parameters (`t0`, `t1`, and `t2`) that specify the UV texture coordinates at the three triangle vertices. As with the normals, if the mesh doesn't provide per-vertex UV coordinates `Triangle::intersect()` passes `nullptr`s for the `t0`, `t1`, and `t2` parameters and you'll need to fall back to some reasonable default. In this case, you should just store the barycentric coordinates in place of interpolated UV coordinates in `hit.uv`.

If you render `scenes/02/triangles_textures/teapot.json` you should see a scene with a teapot mesh that has texture coordinates and a texture applied, as shown below.

@m_class{m-row m-container-inflate}

@parblock

@m_div{m-col-t-6 m-col-s-4}
<a href="earth-ref.png"><img src="earth-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-6 m-col-s-4}
<a href="textured-box-ref.png"><img src="textured-box-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-6 m-col-s-4}
<a href="teapot-ref.png"><img src="teapot-ref.png"/></a>
@m_enddiv

@endparblock

Task 4: Fresnel blend material
==============================
@m_span{m-label m-primary m-flat} 1 pt @m_endspan

Many real-world materials have a more complex structure because the surface actually consist of multiple layers: e.g. a clear varnish on top of wood. There are many sophisticated techniques in graphics that try to model such multiple layers in a physically based way. We will instead take a physically inspired but simple approach to blend between a dielectric varnish and a substrate layer based on the index of refraction of the varnish.

Implement the class  `FresnelBlendMaterial` in a new file `src/materials/fresnel_blend.cpp`. This material will compute the percentage of light that should reflect vs. refract off the surface using the Fresnel equations, and then call a different `scatter()` function for each. To handle this, the material should store two `shared_ptrs` to `Materials` (specified as `"refl"` and `"refr"` in the JSON file) and a `shared_ptr` to a `Texture` (`"ior"` in JSON) specifying the index of refraction of the varnish layer (which will indirectly influence the blend amount between the reflection vs. refraction materials). Make sure to register the new material with the `DartsFactory` and add it to your `CMakeLists.txt` file.

In the `FresnelBlendMaterial::scatter()` function, evaluate the Fresnel equations using the index of refraction in the `ior` texture (e.g. converted to a scalar using `luminance()`). The book favors the fast approximation to the Fresnel equations proposed by Schlick. You are free to implement/use this if you like. We also provide an implementation of the full Fresnel equations as `fresnel()` in `include/darts/common.h`. Using the calculated reflection percentage, draw a random number using `randf()`. If `randf()` is less than the reflection amount, then call the `scatter()` function of material `refl`, otherwise, call the `scatter()` function of material `refr`.

Render `scenes/assignment3/fresnel_blend.json`.

@m_class{m-row m-container-inflate}

@parblock

@m_div{m-col-t-12}
<a href="fresnel_blend-ref.png"><img src="fresnel_blend-ref.png"/></a>
@m_enddiv

@endparblock

Task 5: Interesting scene
=========================
@m_span{m-label m-primary m-flat} 2 pts @m_endspan

Now create an interesting scene (or scenes) showcasing all the features you've implemented. Be creative. Find some interesting meshes and textures online, or create some new procedural textures by combining your existing functionality. Consider applying textures to various parts of a material, like the blend factor, the roughness, and emission. [Sketchfab.com](https://sketchfab.com/feed) and [polyhaven.com](https://polyhaven.com/models) have thousands of high-quality textured meshes available for free. Here's a few that we created and provide the scene files for:

@m_class{m-row m-container-inflate}

@parblock

@m_div{m-col-t-4 m-col-s-4}
<a href="ajax-textured-ref.png"><img src="ajax-textured-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-4 m-col-s-4}
<a href="nefertiti-textured-ref.png"><img src="nefertiti-textured-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-4 m-col-s-4}
<a href="loewenfeld-textured-ref.png"><img src="loewenfeld-textured-ref.png"/></a>
@m_enddiv

@endparblock

What to submit
==============

In your report, make sure to include:

* Rendered images of all the scenes in `scenes/assignment3` and your interesting scene

Then submit according to the instructions in the \ref submitting-on-canvas section of \ref getting-started guide.

@m_footernavigation
