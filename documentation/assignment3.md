@page assignment3 A3: Textures

@m_footernavigation
@tableofcontents

@m_class{m-note m-danger}

@par Note
    Make sure to do a `git pull upstream remote` to retrieve any updates to the base code before you begin!

<!-- **FIXME: add scenes using meshes (since BBH is already done now). add complex textured scenes. ** -->

We will primarily be referring to chapters of [Ray Tracing - The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html) in this assignment.

Task 1: The Texture class
=========================
@m_span{m-label m-primary m-flat} 1 pt @m_endspan

Read [Ray Tracing - The Next Week: Chapter 4](https://raytracing.github.io/books/RayTracingTheNextWeek.html#solidtextures), and create a `Texture` base class following this chapter (perhaps in a new `texture.h` file within your `include/darts` directory. Remember to add this file to the list of headers or source files in `CMakeLists.txt`). Instead of accepting `u`, `v`, and `p` parameters, we suggest you have your `Texture::value` function take a `const HitInfo &` as the parameter (you can then retrieve `u`, `v`, and `p` from within this `HitInfo`).

Constant textures
-----------------

Create a derived `class ConstantTexture : public Texture` which has a member variable `Color3f color;` and whose `value(...)` function just returns it. Make a constructor that takes a `const json &j` and tries to assign the parameter named "color" to your color member variable by using `j.value("color", color);`.

Retrofitting the parser
-----------------------

To make this work with the rest of darts, we need to refactor our code a bit so that `Materials` use `Texture`s instead of `Color3f`s, and so that it plays nice with our JSON parser.

First, change all the `Color3f` members of your `Material`s to `Texture`s, e.g. `Color3f albedo;` in `Lambertian` becomes `shared_ptr<const Texture> albedo`. You'll also need to change the corresponding constructors of any `Material` to call `DartsFactory::create()` instead of reading a color directly from the passed `json` object, e.g. this:
    
~~~{.cpp}
albedo = j.value("albedo", albedo);
~~~

becomes:

~~~{.cpp}
albedo      = DartsFactory<Texture>::create(j.at("albedo"));
~~~

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

One nuisance with our current solution is that we would have to re-write all our previous scene files (that directly specified a color) to instead use a "constant" texture. Previously we could have simply written:  

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

To do that, we'll need to create a specialization of the `DartsFactory::create` method that is used specifically when the template type is a `Texture`. Create a method in `factory.h` with the following signature (just below the current `DartsFactory::create` implementation):

\code{.cpp}
template <>
inline shared_ptr<Texture> DartsFactory<Texture>::create(const json &j)
{
    // your code here
}
\endcode

Start by copying over the implementation of the generic `DartsFactory::create` method.

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

Add a CheckerTexture class following the description in [Section 4.3](https://raytracing.github.io/books/RayTracingTheNextWeek.html#solidtextures/acheckertexture) of the book, but adapted to the darts framework. Your constructor should look for fields named `"odd"` and `"even"` and use the **FIXME** `parseTexture` function you've already defined. You'll also need to extend your `parseTexture` function to handle this new type of texture.

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

Read [Chapter 5](https://raytracing.github.io/books/RayTracingTheNextWeek.html#perlinnoise) of the book. Implement the `Perlin` class. Also implement the `noise_texture` class from the book, but call this `MarbleTexture` instead. Allow the JSON scene file to specify the `scale` member variable of your `MarbleTexture`, and instead of always interpolating between black and white as in the book, allow the user to specify these two colors in the JSON scene file just like we did with `"odd"` and `"even"` for the checkerboard. Look for the named fields `"veins"` and `"base"` in the JSON object for this purpose.

Render `scenes/assignment3/marble.json`.

@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

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

Extend your `Sphere::intersect` function to output the UV coordinates as [described in the book](https://raytracing.github.io/books/RayTracingTheNextWeek.html#solidtextures/texturecoordinatesforspheres). You should now be able to render `scenes/assignment3/earth.json`.

Next, extend your `Quad::intersect` function to output UV coordinates. Render `scenes/assignment3/textured-box.json`. 

<!-- **FIXME** -->
Finally, extend `Triangle::intersect` to output UV coordinates. You will need to extend `single_triangle_intersect` to take three `Vec2f` pointers (the uv coordinates for each vertex), in addition to the positions and normals. As with the normals, if the mesh doesn't provide per-vertex UV coorinates, we'll need to fall back to some reasonable default. In this case, you should just store the barycentric coordinates in place of interpolated UV coordinates.

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

Task 4: Blend material
======================
@m_span{m-label m-primary m-flat} 1 pt @m_endspan

<!-- **FIXME: make this FresnelBlend** -->
Now let's create a `BlendMaterial` that is a blend between two other materials. This material should store two `shared_ptrs` to `Materials` (specified as `"a"` and `"b"` in the JSON file), and a `shared_ptr` to a `Texture` (`"amount"` in JSON) specifying the amount of blend between "a" and "b". You'll need to update the parser to support this material type.

In the `scatter()` function, you will need to evaluate the blend amount texture, convert this value to a scalar (e.g. using `luminance()`) and then draw a random number using `randf()`. If `randf()` is less than the blend amount, then call the `scatter()` function of material b, otherwise, call the `scatter()` function of material a.

Render `scenes/assignment3/blend.json`.

<a href="blend-ref.png"><img src="blend-ref.png"/></a>

Task 5: Interesting scene
=========================
@m_span{m-label m-primary m-flat} 2 pts @m_endspan

Now create an interesting scene (or scenes) showcasing all the features you've implemented. Be creative. Find some interesting textures online, or create some new procedural textures by combining your existing functionality. Consider applying textures to various parts of a material, like the blend factor, the roughness, and emission.

What to submit
==============

In your report, make sure to include:

* Rendered images of all the scenes in `scenes/assignment3` and your interesting scene

Then submit according to the instructions in the \ref submitting-on-canvas section of \ref getting-started guide.

@m_footernavigation
