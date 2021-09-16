@page assignment1 A1: %Ray tracing

@m_footernavigation
@tableofcontents

@m_class{m-note m-danger}

@par Note
    Make sure to do a `git pull upstream remote` to retrieve any updates to the base code before you begin! If you haven't set a remote upstream, go back to the relevant instructions in the \ref getting-started guide.

In this assignment, you will implement the basics of a ray tracer and render your first images. This assignment assumes you've completed the previous one - do not start here before you've completed [Assignment 0](\ref assignment0)!

This assignment relies heavily on Peter Shirley's book [_%Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html), and (to a lesser extent) on [_%Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html). Make sure you have copies of these at hand; you can get them for free on the author's website. We recommend reading the entirety of the first in One Weekend book before starting this assignment. 

As always, all the places where you are expected to insert or edit code are marked with `TODO`. To help you with debugging, we provide you with reference images for every image you are expected to create in the `references` folder.

Task 1: Generating rays and using JSON
======================================

Open up `%src/darts_tutorial1.cpp`. In `%main()`, the code will call a series of functions. For now, all are commented out.

Each one of those functions expects you to implement something and will then test that functionality, and they will output information when you run the program. We've commented them out for now so as not to spam your terminal with too much information.

@m_div{m-col-s-4 m-right-s m-center-t}
<a href="01_manual_ray_image-ref.png"><img src="01_manual_ray_image-ref.png"/></a>
@m_enddiv

Go ahead and uncomment `%test_manual_camera_image()`, and have a look at that function further down in the file. Follow the instructions in the code to generate rays over the image plane. The base code will convert your ray directions into pixel colors and write the image to  `scenes/assignment1/01_manual_ray_image.png`.

@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

Uncomment `%test_JSON` and have a look at that function. This function teaches you about JSON, but it has a bug. Can you spot it? Fix it and have a look at `%function_with_JSON_parameters`, which asks you to read some parameters.

@m_div{m-col-s-4 m-right-s m-center-t}
<a href="01_camera_ray_image-ref.png"><img src="01_camera_ray_image-ref.png"/></a>
@m_enddiv

Finally, uncomment `%test_camera_class_image()` and go to that function. This function also generates rays, but this time it does it by calling the `Camera::generate_ray()` method. Head over to `camera.h`. You will have to implement two things here: The camera constructor, which should read the `"vfov"` parameter from JSON to get the vertical field of view, and compute the size of the image plane and assign it to `m_size`; follow [Chapter 11](https://raytracing.github.io/books/RayTracingInOneWeekend.html#positionablecamera) in the Shirley book for this. Then you should implement `Camera::generate_ray()`. This function gets passed a pixel coordinate, and should output a ray, similar to what you did in `%test_manual_camera_image()` – except that now, the size of the image plane is given by `m_size`. After you complete this task, you will have an output image in `scenes/assignment1/01_camera_ray_image.png`.

When you first implement `Camera::generate_ray()` you may set all rays to start at the origin, but before you render some of the scenes at the end of this assignment you should read [Chapter 12](https://raytracing.github.io/books/RayTracingInOneWeekend.html#defocusblur) and implement defocus blur by sampling the origin randomly from a disk.


@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

Task 2: Transforms
==================

In this task, you will learn how to use transforms.

Go ahead and uncomment `%test_transforms()` and take a look at that function. This function makes use of the transform class, which knows how to transform points, vectors, normals and rays between spaces. This function will test your code, you don't have to do anything here – instead, open up `transform.h` and fill in the methods marked as `TODO`. Follow the instructions in the code. After you're done, run your code and make sure the output is correct (the base code will tell you).


@m_div{m-col-s-4 m-right-s m-center-t}
<a href="01_xformed_camera_ray_image-ref.png"><img src="01_xformed_camera_ray_image-ref.png"/></a>
@m_enddiv

After your transforms work correctly, uncomment `%test_xformed_camera_image()` and head over to that function. This will write out another image of ray directions, but this time it will do it using a transformed camera. Open up `camera.h` to `Camera::generate_ray` and make sure you transform the ray into world space after you have generated it (hint: the camera transform is in `m_xform`). Multiply the ray by the transform.

If you did this correctly, `scenes/assignment1/01_xformed_camera_ray_image.png` will match the reference image.

@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

Task 3: Spheres
===============

With rays being generated correctly, you are now ready to intersect things!

Uncomment `%test_ray_sphere_intersection()` and head on over to that function. This function will intersect two different spheres and check if the results are correct. Open up `sphere.cpp` and look at `Sphere::intersect`. You will need to implement code that intersects the sphere and, if the sphere was hit, fills in the `hit` struct with information about the intersection. You will need to figure out the hit distance, the intersection point and the hit normal. If you're unsure how to do this, check the lecture slides and Chapters [4](https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere)–[5](https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects) of the Shirley book.

The test code first checks your code for an untransformed sphere, and then for a sphere with a transform applied. To make the transform work, you will first need to transform the ray into the local coordinate system of the sphere, and then transform the hit normal/hitpoint back to the world coordinate system. If you are unsure how to transform things correctly, we have implemented an intersect routine for a quad in `quad.cpp` which you can use as inspiration.

@m_class{m-note m-warning}

@par Important!
    To make later tasks work correctly, you should only return true if the hit distance lies within the `mint`/`maxt` interval of the ray. You can look at `Quad::intersect()` to see how you can do this.

@m_div{m-col-s-4 m-right-s m-center-t}
<a href="01_xformed_camera_sphere_image-ref.png"><img src="01_xformed_camera_sphere_image-ref.png"/></a>
@m_enddiv

Uncomment `%test_sphere_image()` and take a look at that function. It will raytrace an image of a sphere and save it as `scenes/assignment1/01_xformed_camera_sphere_image.png`; comparing that image to the reference might help you debug your sphere implementation if something isn't working right.

@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv

Task 4: Materials
=================

Uncomment `%test_materials()` and take a look at that function. It instantiates a `Lambertian` and a `Metal` material and generates a scattered ray from it.

There are two functions you need to implement: `Lambertian::scatter()` and `Metal::scatter()` in `%src/materials/lambertian.cpp` and `%src/materials/metal.cpp`, respectively. Read the lecture slides and Chapters [8](https://raytracing.github.io/books/RayTracingInOneWeekend.html#diffusematerials) and [9](https://raytracing.github.io/books/RayTracingInOneWeekend.html#metal) of the Shirley book to get an idea for what you need to do. Please take a close look at the comments in the code - there is a small error in the text book, and we tell you how to avoid it.

The function `%test_materials()` will generate a single scattered ray and check if the ray origin, direction and attenuation are correct. Because the scatter method is random, it is difficult to test, and there are multiple correct ways to implement this method. We provide test data using the "obvious" implementation, but maybe you did it differently - in that case you won't lose points if the next task still renders correctly. Take the results of our rudimentary autotester with a grain of salt.

You also need to implement `Dielectric::scatter()`. You can follow the Shirley book (Chapter 9) for your implementation. After you have completed the entire assignment, you can run your renderer on scenes with glass to test/debug your implementation.

Task 5: Recursive Ray Tracing    {#recursiveraytracing}
=============================

If you've come this far, you have reached the home stretch of this assignment!

Go ahead and uncomment `%test_recursive_raytracing()`. This function generates the first "real" scene with materials and light. Because the scattering off of materials is random, this function should trace multiple rays and average their results.

In `%test_recursive_raytracing()`, you should implement a for loop that makes `num_samples` calls to `%recursive_color()` in each pixel and averages the results.


@m_div{m-col-s-5 m-col-m-4 m-right-s m-center-t}
<a href="01_recursive_raytracing-ref.png"><img src="01_recursive_raytracing-ref.png"/></a>
@m_enddiv

Take a look at `recursive_color`. This function should trace a ray into the scene, and if it hits something, it should scatter off of the material that was hit and call `recursive_color` again to get the color of the scattered ray. You can take the pseudo code as inspiration for what you need to do.

If you've implemented it correctly, it will save an output image in `scenes/assignment1/01_recursive_raytracing.png` that should match the reference.

This task might be slow to run - if it takes too long, make sure you are compiling in _Release_ mode rather than _Debug_ mode.

@m_div{m-clearfix-s} @m_enddiv
@m_div{m-clearfix-m} @m_enddiv
@m_div{m-clearfix-l} @m_enddiv


Task 6: Scene Renderer
======================

Finally, you can combine everything from the previous assignments by filling in the code in `%scene.cpp`. Go to `Scene::recursive_color()` and fill in the recursive ray tracing function.

This code is very similar (in fact, almost identical) to the code you've had in [Task 5](#recursiveraytracing). You can copy in your code from task 5 to start you off. The `Scene` class has a dedicated `Scene::intersect()` method that you should use to intersect geometry. Also your code should accumulate emission (if any) at each hit point. See the pseudocode for more details.

Finally, you should fill in `Scene::raytrace()`. You can base this on `%test_recursive_raytracing()` from the previous task to get an idea of how to implement this.

We provide you with a standalone renderer, `darts`, and a few test scenes that you can test your implementation with. Once you've implemented this task, take a look at the `*.json` files in `scenes/assignment1`. You can run the `darts` renderer on these scene files, using the terminal like so:

\image html images/run-darts.png

There are also a few example scenes built into darts, you can render those by running

@code{.bash}
darts example_sceneN
@endcode

where `N` should be replaced with a number from 0 to 3.

Run your renderer on each of these built-in scenes and the JSON scene files and compare the output to the reference images. If you've implemented all the tasks correctly, the images should match.

Be aware that depending on how you implemented your code, you might get differences in the noise patterns between your image and the reference. This is normal. However, if you "squint your eyes", the images should match.

@m_class{m-row m-container-inflate}

@parblock

@m_div{m-col-t-12 m-col-s-6 m-col-m-4}
<a href="example_scene0-ref.png"><img src="example_scene0-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-12 m-col-s-6 m-col-m-4}
<a href="example_scene1-ref.png"><img src="example_scene1-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-12 m-col-s-6 m-col-m-4}
<a href="example_scene1-1spp.png"><img src="example_scene1-1spp.png"/></a>
@m_enddiv

@m_div{m-col-t-12 m-col-s-6 m-col-m-4}
<a href="example_scene2-ref.png"><img src="example_scene2-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-12 m-col-s-6 m-col-m-4}
<a href="example_scene2-1spp.png"><img src="example_scene2-1spp.png"/></a>
@m_enddiv

@m_div{m-col-t-12 m-col-s-6 m-col-m-4}
<a href="example_scene3-ref.png"><img src="example_scene3-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-12 m-col-s-6 m-col-m-4}
<a href="example_scene3-1spp.png"><img src="example_scene3-1spp.png"/></a>
@m_enddiv

@m_div{m-col-t-12 m-col-s-6 m-col-m-4}
<a href="01_plane-ref.png"><img src="01_plane-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-12 m-col-s-6 m-col-m-4}
<a href="02_balls-ref.png"><img src="02_balls-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-12 m-col-s-6 m-col-m-4}
<a href="03_refl-ref.png"><img src="03_refl-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-12 m-col-s-6 m-col-m-4}
<a href="04_refr-ref.png"><img src="04_refr-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-12 m-col-s-6 m-col-m-4}
<a href="05_quad-light-ref.png"><img src="05_quad-light-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-12 m-col-s-6 m-col-m-4}
<a href="06_jensen_box-ref.png"><img src="06_jensen_box-ref.png"/></a>
@m_enddiv

@endparblock

What to submit
==============

Submit according to the instructions in the \ref submitting-on-canvas section of \ref getting-started guide.

@m_footernavigation
