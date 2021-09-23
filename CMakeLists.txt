cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(
  darts
  VERSION 1.0
  LANGUAGES CXX
)

# ============================================================================
# Configure dependencies
# ============================================================================
include(cmake/DartsConfig.cmake)

# ============================================================================
# Create darts library
# ============================================================================
add_library(
  # cmake-format: off
  darts_lib OBJECT
  include/darts/array2d.h
  include/darts/common.h
  include/darts/fwd.h
  include/darts/image.h
  include/darts/parallel_for.h
  include/darts/progress.h
  include/darts/ray.h
  include/darts/vec.h
  src/common.cpp
  src/image.cpp
  src/progress.cpp
  include/darts/test.h
  include/darts/box.h
  include/darts/mesh.h
  include/darts/triangle.h
  src/surfaces/mesh.cpp
  src/surfaces/triangle.cpp
  src/surfaces/bbh.cpp
  src/tests/test.cpp
  src/tests/intersection_test.cpp
  src/materials/diffuse_light.cpp
  include/darts/camera.h
  include/darts/material.h
  include/darts/scene.h
  include/darts/sphere.h
  include/darts/surface.h
  include/darts/surface_group.h
  include/darts/sampling.h
  include/darts/transform.h
  include/darts/factory.h
  include/darts/json.h
  src/materials/dielectric.cpp
  src/materials/lambertian.cpp
  src/materials/material.cpp
  src/materials/metal.cpp
  src/surfaces/quad.cpp
  src/surfaces/sphere.cpp
  src/surfaces/surface.cpp
  src/surfaces/surface_group.cpp
  src/parser.cpp
  src/scene.cpp
  src/example_scenes.cpp
  # cmake-format: on
)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(darts_lib PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

# Link dependencies
target_link_libraries(
  darts_lib
  PRIVATE stb Threads::Threads tinyobjloader tinyexr
  PUBLIC nlohmann_json filesystem pcg32 cli11 linalg spdlog::spdlog
)

# Windows' math include does not define constants by default. Set this definition so it does. Also
# set NOMINMAX so the min and max functions are not overwritten with macros.
if(MSVC)
  target_compile_definitions(darts_lib PUBLIC -D_USE_MATH_DEFINES -DNOMINMAX -DWIN32_LEAN_AND_MEAN)
endif()

target_include_directories(
  darts_lib PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                   $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

# ============================================================================
# Add executables and link with darts_lib
# ============================================================================

add_executable(darts_tutorial0 src/darts_tutorial0.cpp)
target_link_libraries(darts_tutorial0 PRIVATE darts_lib)

add_executable(darts_tutorial1 src/darts_tutorial1.cpp)
target_link_libraries(darts_tutorial1 PRIVATE darts_lib)

add_executable(darts src/darts.cpp)
target_link_libraries(darts PRIVATE darts_lib)

add_executable(img_compare src/img_compare.cpp)
target_link_libraries(img_compare PRIVATE darts_lib)

add_executable(img_avg src/img_avg.cpp)
target_link_libraries(img_avg PRIVATE darts_lib)


# ============================================================================
# Create an installable target
#
# this allows users to install and find darts_lib via `find_package()`.
# ============================================================================

string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
  NAME darts_lib
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
  DEPENDENCIES ""
)

# add the documentation/website subproject
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/documentation ${CMAKE_BINARY_DIR}/documentation)
