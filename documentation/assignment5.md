@page assignment5 A5: Sampling lights

@m_footernavigation
@tableofcontents

@m_class{m-note m-danger}

@par Note
    Make sure to do a `git pull upstream remote` to retrieve any updates to the base code before you begin!

In this final assignment, you will use the sampling and integrator interfaces from the previous assignment to build powerful new integrators that can handle lights with much less noise than you've seen so far.

This assignment consists of three parts. In the first, you will implement sampling routines for different geometries, and visualize the resulting points. Then you will modify the existing shapes in darts to allow sampling them and evaluate their PDFs. Finally, you will implement new integrators that make use of the ability to sample emissive geometry in the scene.

Task 1: Sampling Geometry
=========================
@m_span{m-label m-primary m-flat} 3 pts @m_endspan

In this part, you will implement new functions for sampling basic geometry, which will make the later parts of the assignment easier. Similar to last time, you should write a standalone program to generate and save a few hundred points from your function, and visualize them using your favorite plotting tool (we will show screenshots from [plot.ly](https://chart-studio.plot.ly/create/#/)). You can reuse the same program you used in the previous assignment.

Spheres
-------

@m_div{m-col-s-6 m-col-m-5 m-right-s}
\image html images/sphere-points.png
@m_enddiv

<!-- **FIXME** -->

As a warm-up, begin by implementing a `Vec3f sampleSphere(Vec3f center, float radius)` function that samples a point on a sphere with the given center and radius. You can use your `randomOnUnitSphere()` function from the previous assignment; all you need to do is scale and offset the points from this function.

Visualize the points you get for a sphere with center (0.5, 1, 2) and radius 3. It should look something like the image shown here.

@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

Rectangles
----------

@m_div{m-col-s-6 m-col-m-5 m-left-s}
\image html images/quad-points.png
@m_enddiv

Implement a function `sampleRect(Vec3f center, Vec3f v0, Vec3f v1)` that produces random points on a rectangle with corners `center ± v0 ± v1.`

@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

@m_class{m-block m-success}

@par Hint
    All points on this rectangle can be parametrized by `center + a*v0 + b*v1`, where `a` and `b` range from -1 to 1. Generate random `a` and `b` in this range and use this formula to turn them into points on the rectangle.

Visualize the points from this function for a center of (0, 1, 0) and sides (1, 0, 0), (0, 1, 0.6). You should get a point set like the one shown in the image here.

Triangles
---------

@m_div{m-col-s-6 m-col-m-5 m-right-s}
\image html images/triangle-points.png
@m_enddiv

Implement a function `sampleTriangle(Vec3f v0, Vec3f v1, Vec3f v2)` that produces points uniformly at random on a triangle with the specified coordinates. There are different ways you can do this; the easiest way is to first generate random barycentric coordinates (a, b, c), and then return the corresponding interpolated point `v0*a + v1*b + v2*c`.

@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

@m_class{m-block m-success}

@par Hint
    You can generate random barycentric coordinates by computing `a` and `b` uniformly at random in \[0,1\], and set `c = 1 - a - b`. However, if `a+b>1`, this will give you an invalid coordinate for `c`. You can either do rejection sampling and repeat this procedure until you get valid coordinates, or you can flip the coordinates (i.e. set `a = 1 - a`, `b = 1 - b`) if `a+b` is greater than 1.

Visualize points from your function for a triangle with corners (0,0,0), (1,1,1) and (0.5, 1, 2). You should get points like the ones shown in the image here.

Spherical Cap
-------------

@m_div{m-col-s-6 m-col-m-5 m-right-s}
\image html images/spherical-cap-points.png
@m_enddiv

Finally, add a method `randomSphericalCap(float cos_theta_max)` for sampling points uniformly at random from a spherical cap. You've already (implicitly) implemented this method for your previous assignment: Sampling the sphere and the hemisphere are just special cases of sampling different sections of the sphere. Copy-paste the code for sampling the hemisphere from your previous assignment; all that needs to change is the computation of cosTheta: It should be distributed uniformly between 1 and `cos_theta_max`; this is the cosine of the largest angle points are allowed to have with the normal. In pseudo code, `cosTheta = lerp(cos_theta_max, 1.0f, randf())`.

Visualize your code for an angle of pi/4 (hint: You pass the cosine of the angle to this function, not the angle directly). It should look something like the image shown here.

Task 2: Making Lights Samplable
===============================
@m_span{m-label m-primary m-flat} 5 pts @m_endspan

In this task, you will extend the surface base class in darts with a new sampling interface. Open `surface.h` and add two new methods to `Surface`:

@m_div{m-col-s-10 m-col-m-9 m-center-s}
\image html images/sampling-interface.png
@m_enddiv

`Surface::sample()` will generate a random _direction_ that points from `o` towards the surface. The direction is guaranteed to hit the surface from `o`. Usually, you will do this by first generating a point on the surface (using the functions from task 1) and then computing the direction from `o` to the point. However, for certain shapes (like the sphere), we will sample directions directly.

`Surface::pdf()` will return the probability density of generating a certain direction `v`, seen from point `o`. Usually, you would implement this by tracing a ray from `o` in direction `v` and checking if it hits the surface (and returning 0 if not - we never generate directions not towards the surface); the PDF is then the PDF of generating the hitpoint (usually 1/surface area) times the geometry factor (squared distance to `o` divided by the cosine of `v` with the surface normal at the hit point).

Similar to last assignment, we provide an updated tool for testing your sampling code. This is in `src/04_sample_test.cpp`. Add it to your project by modifying the `CMakeLists.txt` file like you did in the previous assignment.

Rectangles
----------

Open `quad.h` and `quad.cpp`, and implement the `sample` and `pdf` functions for the `Quad` class.

For `sample`, you should generate a random point on the rectangle (using your function from earlier) and return the normalized direction from `o` to the point you generated.

@m_class{m-block m-success}

@par Hint
    You can get the vectors for the two sides of the rectangle using

        v0 = m_xform.vector({m_size.x, 0, 0})  
        v1 = m_xform.vector({0, m_size.y, 0})


For `pdf`, you should check if the given direction hits the quad from the given origin. If not, return 0; otherwise, return 1/area times the geometry factor.


@m_class{m-block m-success}

@par Hint
    The length of the cross product of `v0` and `v1` is exactly 1/4th of the surface area.

After you've implemented these functions, test your code using the sample tester tool. You should get images like these for `quad-pdf.png` and `quad-sampled.png`:

@m_div{twentytwenty-container}
    <img src="quad-pdf.png" alt="Analytic distribution">
    <img src="quad-sampled.png" alt="Observed distribution">
@m_enddiv

Triangles
---------

Go to `mesh.h` and `mesh.cpp`, and implement `sample` and `pdf` for the `Triangle` class. Use exactly the same recipe as you did for the quad - all that needs to change is how to get a point on the surface, and the computation of the surface area. Look at `Triangle::intersect` for inspiration for how to get the three corners of the triangle (p0, p1 and p2). Hint: The length of the cross product of any two sides of the triangle is twice the triangle area.

@m_div{twentytwenty-container}
    <img src="triangle-pdf.png" alt="Analytic distribution">
    <img src="triangle-sampled.png" alt="Observed distribution">
@m_enddiv

Spheres
-------

Open `sphere.h` and `sphere.cpp`, and implement `sample` and `pdf` for the `Sphere` class.

We will use a different approach for sampling the sphere: Instead of sampling a point on the sphere first and computing the direction towards it, we will directly sample the spherical cap of directions that all point towards the sphere. To do this, you can use your function for sampling spherical caps from the first task. However, to use this function you need to first figure out cos_theta_max, the angular extent of the sphere at `o`. You can compute it with `cos_theta_max = sqrt(d*d - r*r)/d` where `d` is the distance from the center of sphere to `o`, and `r` is the radius of the sphere. Hint: Can this formula fail? When does this happen? What should the value of `cos_theta_max` be in that case?

The sampling function you implemented in task 1 assumes the spherical cap is aligned with the z axis. You should use the ONB to transform the sampled direction, so that the spherical cap points from `o` towards the center of the sphere instead.

For `pdf`, you should first check if the given origin/direction intersect with the sphere. If they do, the pdf is simply 1/solid angle of the spherical cap (no geometry factor needed - we generated directions directly!). The solid angle of a spherical cap with opening angle of thetaMax is `2*pi*(1-cos_theta_max)`.

Test your code with the sample tester tool. You should get images like these:

@m_div{twentytwenty-container}
    <img src="sphere-pdf.png" alt="Analytic distribution">
    <img src="sphere-sampled.png" alt="Observed distribution">
@m_enddiv

Surface Groups
--------------

Open `surface_group.h` and `surface_group.cpp` and implement the sampling interface for the `SurfaceGroup` class. This class represents a collection of shapes (e.g. the list of all lights in the scene), which are stored in the array `m_surfaces`. For `sample`, you should randomly generate an integer index in range `0...m_surfaces.size() - 1` (inclusive) and call the `sample` method of the shape at that index in the array. For `pdf`, you should call the `pdf` method of each shape in the list, and return the average of all the pdf evaluations. Hint: You can use the following syntax to iterate over all shapes:

~~~{.cpp}
for (auto surface : m_surfaces)  
    surface->do_something(...);
~~~
  

Task 3: Integrating Lights
==========================
@m_span{m-label m-primary m-flat} 7 pts @m_endspan

In this task, you will implement better integrators for direct lighting. You will use the sampling interfaces you implemented in the earlier task to explicitly guide rays towards light sources.

Note that the algorithm described in this assignment is slightly different from the pseudo-code shown in class: Both algorithms will work, and you are free to implement either version, as long as it gives you the correct image.

Direct Lighting Material Integrator
-----------------------------------

@m_div{m-col-s-6 m-col-m-5 m-right-s}
<a href="veach_mats-ref.png"><img src="veach_mats-ref.png"/></a>
@m_enddiv

<!-- **FIXME: call this PathMats from the very beginning** -->

Begin by implementing a baseline `DirectMats` integrator that integrates direct lighting by simply sampling the material. This is a simpler version of your integrator from last time - remove the recursive part, and only shoot a single ray after the first hit to return `eval/pdf*emission`. Make sure to register this integrator with the `DirtsFactory` by adding the macro at the bottom of your implementation file. Associate it with the string `"direct_mats"`.

Test your integrator by running it on `scenes/assignment5/veach_mats.json`. You should get an image like the one shown here.

Most of this image looks extremely noisy, especially the background and the reflections in the lower right. We can do better than this!


Creating a list of scene emitters
---------------------------------

Instead of asking the `Material` to generate ray directions for us, we could instead generate ray directions by sampling points on all surfaces in the scene using our new `Surface::sample()` function. As it is now, however, this would not be so useful, since most surfaces in the scene do not emit light. We'd like to sample rays towards surfaces that emit light. To do that we first need to add some plumbing to our code to allow a `Scene` to maintain a list of only the emissive surfaces.


<!-- **FIXME add details here **-->


Next Event Integrator
---------------------

Add a new integrator class called `DirectNEE`, and register it with the factory using the string `"direct_nee"`. The "NEE" part stands for _Next Event Estimation_, which is a fancy name for sampling light sources directly.

@m_div{m-col-s-6 m-col-m-5 m-right-s}
<a href="veach_nee-ref.png"><img src="veach_nee-ref.png"/></a>
@m_enddiv

On a basic level, this integrator looks very similar to the material-based integrator from the previous subtask (and you can start by copy-pasting that code). However, instead of calling `Material::sample` to produce a direction, it should call `scene.emitters().sample()`. `scene.emitters()` is a `SurfaceGroup` that contains all emissive shapes in the scene. Similarly, instead of dividing by `Material::pdf`, you should divide by `scene.emitters().pdf()`.

This (for the first time) shows the flexibility of the Monte Carlo approach: All you had to change was the sampling and PDF routine, and you get a noise-free image much faster! This would not have been possible with the renderer you wrote in Assignment 1. Convince yourself that this integrator has much less noise by running your new code on `scenes/assignment5/veach_nee.json`. You should get an image looking like the one shown here.

This looks _much_ better than the `DirectMats` integrator for most of the image... except for the upper left, where the NEE estimator does much worse. Just like discussed in class, we can fix this by using both estimators, and combining them with MIS.

MIS Integrator
--------------

Add a new integrator class called `DirectMIS`, and add it to the `parseIntegrator` function for type `"direct_mis"`. The MIS part stands for _Multiple Importance Sampling_, which is a technique for efficiently combining multiple integration techniques.

Begin by copy-pasting either your `DirectMIS` or your `DirectNEE` integrator from before. Instead of always sampling from the material, or always from the light source, you should sample from a _mixture_ of both of them. With probability of 0.5, generate a direction by sampling the material; otherwise, generate a direction by sampling the lights. After you've generated a direction, trace a ray and evaluate the emission and material like before; however, instead of dividing by just the material pdf or the light PDF, divide by the _average_ of the two PDFs (since you randomly sample from either one).

Test your integrator on `scenes/assignment5/veach_mis.json`. You should get an image like below:

@m_div{twentytwenty-container}
    <img src="veach_mats-ref.png" alt="Material sampling">
    <img src="veach_nee-ref.png" alt="Next event estimation">
    <img src="veach_mis-ref.png" alt="MIS">
@m_enddiv

The integrator now works robustly in all parts of the scene, and combines the good traits of both material sampling and light sampling.

Also test your integrators on `scenes/assignment5/odyssey_XXX.json`. Here is our comparison:

@m_div{twentytwenty-container}
    <img src="odyssey_mats-ref.png" alt="Material sampling">
    <img src="odyssey_nee-ref.png" alt="Next event estimation">
    <img src="odyssey_mis-ref.png" alt="MIS">
@m_enddiv

Full Path Tracing
-----------------

Extend the `DirectMIS` integrator from the previous subtask to do not just direct lighting, but all direct and indirect lighting. You can start by copy-pasting your code for `DirectMIS`: This code will currently find the closest hit point along the given ray, estimate direct lighting at that location and exit. All you need to do in order to support indirect light is to add recursion before exiting: After computing direct lighting, sample the material to obtain the next ray along the path, and call the function recursively with the new ray to obtain an estimate of indirect lighting. Divide that estimate by the material sampling PDF, and add this estimate to the direct lighting estimate. That's it! In our code, we've created a new integrator class called `"PathTracerMIS"` to do this. You could instead also add the recursive functionality to your current direct lighting integrator, and add a JSON parameter for limiting the recursion depth. No recursion gives you direct lighting only, and anything beyond that gives you global illumination with increasing numbers of interreflections. Either way works - pick the one that you prefer.

**Grad Students: Optimize your DirectMIS integrator**. In the current version, you sample the material and trace a ray once for direct lighting, and a second time for the recursive ray. This is wasteful - they can both use the same ray and intersection (intersecting the scene is expensive!). Optimize your implementation to reuse the material ray and intersection for the recursive call. You can check the lecture slides for ideas on how to do this.

@m_class{m-block m-success}

@par Hint
    With the randomized MIS approach, you only sample the material 50% of the time, so you don't always have that ray available. You can simplify it by removing randomness and always taking one light sample and one material sample, and averaging the direct light estimates from both. Then, reuse the ray and intersection sampled from the material for the recursive call.

<!-- FIXME: finalize scenes, and add renderings here  -->

Test your new integrators on `scenes/assignment5/glossy-cornell-box_mis.json`.


@m_class{m-row m-container-inflate}

@parblock

@m_div{m-col-s-4}
<a href="sphere_light_small-ref.png"><img src="sphere_light_small-ref.png"/></a>
@m_enddiv

@m_div{m-col-s-4}
<a href="sphere_light_medium-ref.png"><img src="sphere_light_medium-ref.png"/></a>
@m_enddiv

@m_div{m-col-s-4}
<a href="sphere_light_large-ref.png"><img src="sphere_light_large-ref.png"/></a>
@m_enddiv

@endparblock


What to submit
==============

In your report, make sure to include:

* Visualized point sets for all distributions from task 1
* Rendered images of all the scenes in `scenes/assignment5`

Then submit according to the instructions in the \ref submitting-on-canvas section of \ref getting-started guide.

@m_footernavigation
