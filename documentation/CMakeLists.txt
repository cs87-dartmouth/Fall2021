cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(documentation)

# ---- Dependencies ----

include(../cmake/CPM.cmake)
CPMAddPackage("gh:wkjarosz/m.css#51a02af8132b3ba1f632fa7928f46a411e6d844e")

# ---- Doxygen variables ----

# set Doxyfile variables
set(DOXYGEN_PROJECT_NAME Darts)
set(DOXYGEN_PROJECT_VERSION ${Darts_VERSION})
set(DOXYGEN_PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
set(DOXYGEN_MCSS_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doxygen-mcss")

configure_file(${CMAKE_CURRENT_LIST_DIR}/Doxyfile-mcss ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile-mcss)
configure_file(${CMAKE_CURRENT_LIST_DIR}/conf.py ${CMAKE_CURRENT_BINARY_DIR}/conf.py)

add_custom_target(
  website
  ${CMAKE_COMMAND} -E rm -rf "${DOXYGEN_MCSS_OUTPUT_DIRECTORY}/html"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${DOXYGEN_MCSS_OUTPUT_DIRECTORY}"
  COMMAND "${m.css_SOURCE_DIR}/documentation/doxygen.py" "--debug"
          "${CMAKE_CURRENT_BINARY_DIR}/conf.py"
  COMMAND echo "Docs written to: ${DOXYGEN_MCSS_OUTPUT_DIRECTORY}"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
