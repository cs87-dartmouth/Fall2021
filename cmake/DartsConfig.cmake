# ============================================================================
# Include guards
# ============================================================================

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ============================================================================
# Set a default build configuration (Release)
# ============================================================================
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Choose the type of build." FORCE
  )
  set_property(
    CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
  )
endif()
string(TOUPPER "${CMAKE_BUILD_TYPE}" U_CMAKE_BUILD_TYPE)

# ============================================================================
# Add dependencies via CPM (cmake/tools.cmake includes cmake/CPM.cmake)
#
# see https://github.com/TheLartians/CPM.cmake for more info
# ============================================================================
include(cmake/tools.cmake)

# PackageProject.cmake will be used to make our target installable
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.6.0")

# CPMAddPackage("gh:TheLartians/Format.cmake@1.7.0")

CPMAddPackage(
  NAME nlohmann_json
  VERSION 3.9.1 # the git repo is incredibly large, so we download the archived include directory
  URL https://github.com/nlohmann/json/releases/download/v3.9.1/include.zip
  URL_HASH SHA256=6bea5877b1541d353bd77bdfbdb2696333ae5ed8f9e8cc22df657192218cad91
  DOWNLOAD_ONLY YES
)
if(nlohmann_json_ADDED)
  add_library(nlohmann_json INTERFACE IMPORTED)
  target_include_directories(nlohmann_json INTERFACE ${nlohmann_json_SOURCE_DIR}/single_include)
endif()

CPMAddPackage(
  NAME filesystem
  GITHUB_REPOSITORY wkjarosz/filesystem
  GIT_TAG 5e0eb2a6160201de38d076c085641742ee86a8f6
  DOWNLOAD_ONLY YES
)
if(filesystem_ADDED)
  add_library(filesystem INTERFACE IMPORTED)
  target_include_directories(filesystem INTERFACE "${filesystem_SOURCE_DIR}")
endif()

CPMAddPackage(
  NAME pcg32
  GITHUB_REPOSITORY wjakob/pcg32
  GIT_TAG 70099eadb86d3999c38cf69d2c55f8adc1f7fe34
  DOWNLOAD_ONLY YES
)
if(pcg32_ADDED)
  add_library(pcg32 INTERFACE IMPORTED)
  target_include_directories(pcg32 INTERFACE "${pcg32_SOURCE_DIR}")
endif()

CPMAddPackage("gh:nothings/stb#a0a939058c579ddefd4c5671b046f29d12aeae01")
if(stb_ADDED)
  add_library(stb INTERFACE IMPORTED)
  target_include_directories(stb INTERFACE "${stb_SOURCE_DIR}")
endif()

CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 02ad5e11da5b2702cc1c9a8fdf750486beee08fe
  OPTIONS "FMT_INSTALL YES" # create an installable target
  # DOWNLOAD_ONLY YES
)
# if(fmt_ADDED) add_library(fmt INTERFACE IMPORTED) target_include_directories(fmt INTERFACE
# "${fmt_SOURCE_DIR}/include") endif()

CPMAddPackage("gh:sgorsten/linalg#a3e87da35e32b781a4b6c01cdd5efbe7ae51c737")
if(linalg_ADDED)
  add_library(linalg INTERFACE IMPORTED)
  target_include_directories(linalg INTERFACE "${linalg_SOURCE_DIR}")
endif()

CPMAddPackage(
  NAME cli11
  URL https://github.com/CLIUtils/CLI11/archive/v1.9.1.zip
  DOWNLOAD_ONLY YES
)
if(cli11_ADDED)
  add_library(cli11 INTERFACE IMPORTED)
  target_include_directories(cli11 INTERFACE "${cli11_SOURCE_DIR}/include")
endif()

# CPMAddPackage("gh:madler/zlib@1.2.11") CPMAddPackage( NAME Imath GITHUB_REPOSITORY
# AcademySoftwareFoundation/Imath VERSION 3.1.0 OPTIONS "BUILD_SHARED_LIBS OFF" "PYTHON OFF" )

# CPMAddPackage( NAME OpenEXR GITHUB_REPOSITORY AcademySoftwareFoundation/openexr VERSION 3.0.5
# OPTIONS "OPENEXR_BUILD_SHARED_LIBS OFF" "OPENEXR_BUILD_UTILS NO" "OPENEXR_INSTALL_TOOLS OFF"
# "OPENEXR_FORCE_INTERNAL_ZLIB ON" )

CPMAddPackage(
  NAME tinyobjloader
  GITHUB_REPOSITORY tinyobjloader/tinyobjloader
  GIT_TAG 94d2f7fe1f7742818dbcd0917d11679d055a33de
  DOWNLOAD_ONLY YES
)
if(tinyobjloader_ADDED)
  add_library(tinyobjloader INTERFACE IMPORTED)
  target_include_directories(tinyobjloader INTERFACE "${tinyobjloader_SOURCE_DIR}/")
endif()

CPMAddPackage(
  NAME tinyexr
  GITHUB_REPOSITORY syoyo/tinyexr
  VERSION 1.0.0
  # OPTIONS "TINYEXR_BUILD_SAMPLE OFF"
  DOWNLOAD_ONLY YES
)
if(tinyexr_ADDED)
  add_library(tinyexr INTERFACE IMPORTED)
  target_include_directories(tinyexr INTERFACE "${tinyexr_SOURCE_DIR}/")
endif()

CPMAddPackage(
  NAME spdlog
  GITHUB_REPOSITORY gabime/spdlog
  VERSION 1.9.2
  OPTIONS "SPDLOG_INSTALL YES" # create an installable target
          "SPDLOG_FMT_EXTERNAL YES" # use the fmt library we added above instead of the bundled one
)

if(USE_FLIP)
  include(FetchContent)
  FetchContent_Declare(
    flip
    GIT_REPOSITORY https://github.com/NVlabs/flip.git
    GIT_TAG c13d9701b8a6dd4e3725ab0fcac775f9b91e3480
  )

  message("-- Adding FLIP image comparison tool")
  FetchContent_MakeAvailable(flip)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# ============================================================================
# compile the rest of the codebase using C++17
# ============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CPMAddPackage( NAME CPMLicenses.cmake GITHUB_REPOSITORY cpm-cmake/CPMLicenses.cmake VERSION 0.0.5
# )

# cpm_licenses_create_disclaimer_target( write-licenses
# "${CMAKE_CURRENT_BINARY_DIR}/third_party.txt" "${CPM_PACKAGES}" )
