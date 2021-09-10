
@page darts-overview Code

\image html images/darts-wordmark.svg

**darts** is a minimalistic skeleton for a Monte Carlo ray tracer, written in C++17. The name is an acronym for _The Dartmouth Academic %Ray Tracer Skeleton_ while also being a nod to the random dart-throwing-like process involved in Monte Carlo ray tracing. It runs on recent versions of Windows, Linux, and macOS and provides the foundation for the programming assignments we'll be doing in this class.

Design philosophy
=================

The primary goal of darts is educational. As such, its design aims to balance several educational objectives

* minimize "time to first pixel" -- allowing you to start producing images quickly
* don't provide many rendering features "out of the box", instead allow you to learn by implementing these yourself while steering you towards a modern full-featured ray tracer design
* keep the core rendering-related code base small but extensible, while relying on a few, self-contained libraries for supporting code
* keep the build system simple and reliable across major platforms

You can think of darts, and the accompanying assignments, as filling the gap between the simplicity of Peter Shirley's [_%Ray Tracing in One Weekend_](https://raytracing.github.io) series of books, and the complexity and completeness of more sophisticated research-oriented renderers like [PBRT](https://www.pbr-book.org) or Mitsuba.

The assignments are designed to help you explore and extend the darts code, while simultaneously progressing through the Shirley books and towards the PBR book. In the first few assignments, you will fill in empty methods left in the starter code, and test them using the tools provided with each assignment. In latter assignments, you will need to figure out what functions and classes you will need to add to the code to accomplish the tasks.

While darts cannot (yet) produce images, after stepping through the assignments you will steadily add features that allow you to render images like the ones below, with the photorealistic lighting characteristic of ray tracing:


<!-- | @image html images/jensen_box.png | @image html images/classroom.png | @image html images/nefertiti.png | @image html images/ajax-ao-ref.png |
|---------|----------|---------|----------|
|         |          |         |          | -->


@m_class{m-row m-container-inflate}

@parblock

@m_div{m-col-s-6}
<a href="classroom.png"><img src="classroom.png"/></a>
@m_enddiv

@m_div{m-col-s-6}
<a href="shoe.png"><img src="shoe.png"/></a>
@m_enddiv

@m_div{m-col-s-6}
<a href="ajax-ao-ref.png"><img src="ajax-ao-ref.png"/></a>
@m_enddiv

@m_div{m-col-s-3 m-col-t-6}
<a href="nefertiti.png"><img src="nefertiti.png"/></a>
@m_enddiv

@m_div{m-col-s-3 m-col-t-6}
<a href="loewenfeld.png"><img src="loewenfeld.png"/></a>
@m_enddiv

@m_div{m-col-s-6}
<a href="jensen_box.png"><img src="jensen_box.png"/></a>
@m_enddiv

@endparblock

Features
========

Darts provides many features that are tedious to implement from scratch, including:

*   A JSON-based scene file loader
*   A nice command-line interface with option parsing, and a text-based progress bar for long operations
*   Basic color/vector/matrix/ray/bounding box classes
*   A high-quality pseudorandom number generator
*   Support for loading and saving images in a variety of formats (.png, .jpg, .exr, .hdr, etc.)
*   Support for loading Wavefront OBJ files

The codebase has the following high-level structure:

Directory         | Description
------------------|------------
`include/darts`   | Contains the header files for the core classes, algorithms and data structures of darts
`src/`            | The core C++ source files, as well as most added files will go here
`scenes/`         | Example scenes and test datasets to validate your implementation
`reports/`        | The skeletons for webpage reports for each of the assignments
`documentation/`  | The source and configuration files for this course website, and the Doxygen documentation
`CMakeLists.txt`  | The main CMake build file which specifies how to compile and link the code
`cmake/`          | Additional CMake files (you likely won't have to look at this)


Darts uses modern CMake, and the [CPM.cmake package manager](https://github.com/cpm-cmake/CPM.cmake) for a particularly clean and simple build set up. At configuration time, the CMake build system automatically downloads (from other GitHub repositories) several open source libraries, including:

Dependency                                              | Description
--------------------------------------------------------|------------
[`cli11`](https://github.com/CLIUtils/CLI11)            | Command line parser for C++11
[`filesystem`](https://github.com/wjakob/filesystem)    | A tiny self-contained library for manipulating paths on various platforms
[`flip`](https://github.com/NVlabs/flip)                | A tool for visualizing and communicating errors in rendered images
[`fmt`](https://github.com/fmtlib/fmt)                  | A modern formatting library/replacement for C's printf and C++ streams
[`linalg`](https://github.com/sgorsten/linalg)          | Single header C++11 vector math library inspired by the syntax of shading languages
[`nlohmann-json`](https://github.com/nlohmann/json)	    | A JSON parsing library for modern C++
[`pcg32`](https://github.com/wjakob/pcg32)              | A tiny self-contained pseudorandom number generator
[`spdlog`](https://github.com/gabime/spdlog)            | Fast C++ logging library
[`stb_image`](https://github.com/nothings/stb)	        | Header-only public domain library for image loading and saving.
[`tinyexr`](https://github.com/syoyo/tinyexr)           | Tiny OpenEXR image loader/saver library
[`tinyobjloader`](https://github.com/tinyobjloader/tinyobjloader)      | Tiny but powerful single file wavefront obj loader
 

The code is also integrated into GitHub's actions for automated continuous integration/build testing.

