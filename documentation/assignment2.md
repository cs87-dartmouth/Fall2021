@page assignment2 A2: %Ray tracing faster

@m_footernavigation
@tableofcontents

@m_class{m-note m-danger}

@par Get the latest base code updates!
    Make sure to do a `git pull upstream main` to retrieve any updates to the base code before you begin!

In this assignment, you will learn how to accelerate ray tracing with an acceleration data structure. We will primarily be referring to chapters of [Ray Tracing - The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html) in this assignment. Make sure to read through it.

@m_class{m-note m-danger}

@par Download textures and meshes!
    We will be dealing with large textures and meshes in this assignment (roughly 1GB). We won't add these to GitHub to avoid bloating your repository. Instead, you will need to download [this DropBox folder](https://www.dropbox.com/sh/e8svsmy22xoan8y/AACxsE8_LpUDohmDYx8flEIta?dl=0) as a zip file and extract into your `scenes/` directory (so that it creates a `scenes/assets` directory within your darts codebase). **DO NOT** commit these files to your GitHub repo (our `.gitignore` should prevent you from accidentally doing this unless you try to explicitly circumvent it).

In the previous assignment, each time you traced a ray the code would perform an intersection test against every single primitive in your scene. This is a reasonable solution for scenes with a few primitives, but it quickly becomes impractical in scenes that contain meshes with thousands to billions of triangles. This is because with naive ray tracing the cost of tracing a ray grows linearly with the number of primitives. Acceleration structures allow us to keep this cost sub-linear, providing tremendous speed improvements for typical scenes.

In this assignment you will implement an acceleration structure called a _bounding volume hierarchy_ (BVH), specifically a variant that uses axis-aligned bounding boxes and hence called a _bounding box hierarchy_ (BBH). The base code for this assignment is an addition to the base code from the previous assignment. You can get the new code by doing a `git pull` in your `darts-basecode` working directory.

@m_class{m-block m-danger}

@par Warning
    %Ray tracing is a popular programming exercise, and there are countless implementations of ray tracing and acceleration structures available online. You are allowed to use the book and other online material to help you learn and understand the concepts, but once you have a grasp on it, you must put non-course sources of code away and write your own code.

Task 1: Ray-triangle intersection  {#ray-triangleintersection}
=================================

Triangles are the default modeling primitives in graphics and hence essential for rendering a variety of scenes. The darts basecode already provides the ability to load OBJ triangle meshes with potentially thousands or millions of triangles, so you can find nice models online and load them in your renderer. Currently, however, darts doesn't know how to intersect a ray with a single triangle. In this task, you will implement a ray-triangle intersection routine.

Open up the `src/triangle.cpp` file and take a look at `single_triangle_intersect()`. This method is passed three vertex positions and (optionally) three vertex normals and should test the intersection of the ray against the triangle.

There are many established ray-triangle intersection algorithms out there, and you are free to pick one you like. You can check the [PBRT book](https://www.pbr-book.org/3ed-2018/Shapes/Triangle_Meshes#TriangleIntersection) for inspiration (the Shirley mini-books don't cover this). Personally I recommend the Möller-Trumbore test, which is simple and widely used. A Google search should bring up a lot of resources for this algorithm.

Once you've finished writing up the intersection code, you can go ahead and run the `build/test_tri_intersection` application from the terminal, which will test your intersection code.

@m_div{m-col-s-6 m-col-m-5 m-col-l-5 m-right-s}
<a href="simple-geometry-ref.png"><img src="simple-geometry-ref.png"></a>
@m_enddiv

Once your tests pass, you can test it on a slightly more complex example: Run the `darts` application from the terminal on the `assignment2/simple-geometry-no-bvh.json` scene. This renders a scene with a few simple triangle meshes, which should look like the image shown (the amount of noise in the image will depend on the number of samples used, ours uses more than the default value in the scene).

This render takes quite a while! It's about time that we fix this using a BBH.

If this scene doesn't render correctly, then the long render times can become a problem when you're trying to debug. It might help you to reduce the `"samples"` and the camera `"resolution"` fields in the JSON scene file to render smaller and noisier images more quickly while you're debugging.

Task 2: Ray-AABB intersection
=============================

[Chapter 3](https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies) of the book discusses how to build a bounding volume hierarchy so you can intersect rays quickly against a large collection of triangles or other geometric primitives.

The first step in this approach is to choose what kind of bounding volume to use. We will stick with axis-aligned bounding boxes (or AABBs). Go read Sections [3.3](<https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies/axis-alignedboundingboxes(aabbs)>)–[3.5](https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies/anoptimizedaabbhitmethod) to understand the idea.

We already provide you an implementation of an axis-aligned bounding `Box` class in `<darts/box.h>`. Our implementation is similar to what is described in the book, but make sure to read through and understand also how our code works.

@m_class{m-block m-success}

@par Tip
    A neat feature of axis-aligned bounding boxes (and rays) is that the concept works in arbitrary dimensions. Our implementation is generic, providing a single `template` class that works for N-dimensional boxes. Take a look at how this is accomplished in `<darts/box.h>`. We also provide an implementation of a ray-box intersection routine, similar to the one described in [Section 3.5](https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies/anoptimizedaabbhitmethod). But again, our code works in arbitrary dimensions (even though we will only use the 3D version in this class).

Task 3: Hierarchy construction and traversal
============================================

Read Sections [3.6](https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies/constructingboundingboxesforhittables)–[3.10](https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies/theboxcomparisonfunctions) of the book to understand how the internals of a bounding volume hierarchy work.

Now open up `src/bbh.cpp` and take a look at `BBHNode::intersect()` and `BBHNode::BBHNode()`. The former traverses the hierarchy and tests for intersections with the ray, and the latter is the constructor that builds the hierarchy given a list of primitives.

We largely follow the design in the book, so it should be relatively easy to implement these methods if you follow the book. Note that C++11 allows us to do certain things easier than the code style used in the book (e.g. sorting a lists of things based on a custom criterion). Take a look at the hints in the base code for more information.

Once you think your code is correct, you can test your implementation on the test scenes we provide. Run the `darts` application from the terminal on the `assignment2/simple-geometry.json` scene. This is the same scene as before, but this time using a %BBH. You should produce the exact same image, but much faster. Also take a look at the *Average number of intersection tests per ray* statistic that the code outputs after rendering; this should be around 20X lower compared to the image you rendered in [Task 1](#ray-triangleintersection).

If your image is correct, you can go ahead and stress test your code on some more complex scenes. Run the darts application on all the scenes in the `scenes/assignment2` folder. You should get images similar to the reference images below (just noisier):


@m_class{m-row m-container-inflate}

@parblock

@m_div{m-col-t-6}
<a href="bunny-dragons-ref.png"><img src="bunny-dragons-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-6}
<a href="sponza-buddha-ref.png"><img src="sponza-buddha-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-6}
<a href="buddha-box-ref.png"><img src="buddha-box-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-6}
<a href="loewenfeld-in-box-ref.png"><img src="loewenfeld-in-box-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-4 m-col-s-4}
<a href="ajax-white-ref.png"><img src="ajax-white-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-4 m-col-s-4}
<a href="nefertiti-white-ref.png"><img src="nefertiti-white-ref.png"/></a>
@m_enddiv

@m_div{m-col-t-4 m-col-s-4}
<a href="loewenfeld-white-ref.png"><img src="loewenfeld-white-ref.png"/></a>
@m_enddiv

@endparblock


Task 4: Extensions
==================

The %BBH presented in the book is simple to implement, but has a lot of potential for improvement (i.e., it's a bit slow).

For your final task, it is your job to optimize your %BBH and measure the improvements you've made. You should implement at least **two** ideas for improving your %BBH, and measure the reduction in render time (while your computer is not doing anything else) as well as the reduction in the average number of intersection tests, which darts will report when it finishes rendering an image.

To help get you started, here is a list of potential ideas you could implement to improve your code:

- If you followed the book, then you will pick the splitting axis randomly while you construct the %BBH. There are a couple problems with this: 1) using a _global_ random number generator like this becomes problematic once you consider improving performance via parallelism, and 2) you can construct much better BBHs with just a few small changes. Think about how you could cleverly pick your splitting axis to improve performance. For example, you could split the axis along which the bounding box is the longest, or you could pick a fixed axis at each level of recursion (e.g. x-axis at the root, y-axis in the direct children of the root, z-axis in the grand-children, x-axis in the great-grand children etc.). Which method performs the best?

- The book recommend using [`std::sort()`](https://en.cppreference.com/w/cpp/algorithm/sort) to sort the primitives along the chosen axis. However, we don't actually need to fully sort the primitives along the axis. All we need is to identify the primitive which should be the _median_ element (the primitive in the middle of the list _if_ it were sorted), and put all primitives left of this median (along the chosen axis) in the first half of the list, and all those right of the median in the second half of the list. This theoretically should require less work because the left and right sublists themselves don't need to be sorted -- we just need to identify which primitives should go into each sublist. C++ has a ready-made algorithm for this called [`std::nth_element()`](https://en.cppreference.com/w/cpp/algorithm/nth_element). The good news is that its runtime complexity is linear in the number of elements in the list, while sorting is log-linear. Figure out how to use this instead of `std::sort()` by reading the [documentation](https://en.cppreference.com/w/cpp/algorithm/nth_element). Once you have it working, compare the time it takes to build the %BBH. The resulting tree should still be exactly the same as before, but it should build considerably faster.

- The book splits the list of primitives so that the left and right child contain the same number or primitives. This is called the median or _equal_ split, and it produces a balanced binary tree. From your data structures class you learned that balanced trees are often what you want when you're searching in a list -- it leads to good worst-case runtime. In ray tracing, however, balanced trees can perform poorly if the primitives are not distributed uniformly, but cluster on one side. In that case, you could to better by splitting a node at the spatial midpoint or _middle_. Pick the center of the bounding box of the primitives of the node, and _partition_ the primitives so that all those whose centroids lie to the left of the midpoint (along the splitting axis) go in one child, and all those to the right go in the other child (you will likely end up with a different number of primitives for each child). Again, C++ has a ready-made function for this, suitably named [`std::partition()`](https://en.cppreference.com/w/cpp/algorithm/partition) Allow the user to choose between the `"equal"` and `"middle"` strategies by specifying a `"split_method"` field in the JSON file -- our basecode already reads this field and stores it in the %BBH class. Does this improve things? Under which circumstances?

- Following the book, each `BBHNode` always has two children. This is fine for internal nodes, but you can lose some efficiency in the leaf nodes, which might benefit from storing more than just two primitives. Modify your implementation such that leaf nodes store a list of primitives instead of only two children (there are different ways to do this). Whenever the number primitives is less than a certain threshold during %BBH node construction, turn it into a leaf node; otherwise, an internal node. Play around with this threshold. Does this make things faster? For which thresholds and which scenes?

- Building the %BBH can still be pretty time consuming for very big scenes (sometimes it can even take longer then rendering the scene!). There is a lot of opportunity to further accelerate this through thread-level parallelism. Note that once we split a node, we recursively build both the left and right children. These two computations are independent and could be performed in parallel. Make your %BBH construction multi-threaded. A simple way to do this in C++11 is to use the [`std::async`](https://en.cppreference.com/w/cpp/thread/async) and [`std::future`](https://en.cppreference.com/w/cpp/thread/future). You can take a look at `<darts/parallel_for.h>` in the base code for an example how to use these concepts to execute multiple functions in parallel.

These are just examples. If you can come up with your own ideas that are on a similar scope as the suggestions, feel free to implement those ideas instead.

Task 5: Surface area heuristic (grad students only)
===================================================

On top of the improvements in the previous task, implement the surface area heuristic (SAH) in your %BBH construction.

You can check the [BVH section of the PBRT textbook](https://www.pbr-book.org/3ed-2018/Primitives_and_Intersection_Acceleration/Bounding_Volume_Hierarchies#) for more information on how it works. The basic idea is to split the list of primitives not simply in the middle or at the median, but split it at a point that minimizes an approximate expression for the cost intersecting the tree.

Task 6: Evaluation scene and leaderboard
========================================

We've designed some scene for you to evaluate the performance of all your %BBH improvements. TBA <!-- **FIXME: complete this section** -->

What to submit
==============

In your report, make sure to include:

- Images of all provided scenes as produced by your raytracer: run `darts` on all scenes in `scenes/assignment2`, and add the images to your report.
- Paste the output of your `test_triangle_intersection` application.
- The ray traversal stats (average ray-primitive intersection tests) of your %BBH on the different scenes provided (don't change the view location or other scene parameters).

Then submit according to the instructions in the \ref submitting-on-canvas section of \ref getting-started guide.

@m_footernavigation
