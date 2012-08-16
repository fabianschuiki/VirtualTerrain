cmake_minimum_required(VERSION 2.6)
project(VirtualTerrain)

# Default to debug build.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

# Executable name.
set(EXECUTABLE_NAME VirtualTerrain)

# Debugging definitions.
if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions("-DBUILD_DEBUG")
endif()


# Gather the source files.
file(GLOB_RECURSE sources RELATIVE ${PROJECT_SOURCE_DIR} source/*.cpp)


###  PLATFORM-SPECIFIC STUFF
###  =======================

# Mac OS X
if (APPLE)
	add_definitions("-DTARGET_APPLE")
	
	# Find the paths of all resources.
	file(GLOB_RECURSE resources RELATIVE ${PROJECT_SOURCE_DIR} data/*)

	# Make sure the resources are copied to the right bundle.
	foreach(r ${resources})
		get_filename_component(r_abs ${r} ABSOLUTE)
		get_filename_component(r_dir ${r_abs} PATH)
		file(RELATIVE_PATH r_rel ${PROJECT_SOURCE_DIR}/data ${r_dir})
		set_source_files_properties(${r} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/${r_rel})
	endforeach()

	# Executable.
	add_executable(
		${EXECUTABLE_NAME} MACOSX_BUNDLE
		${sources}
		${resources}
	)

# other systems
else ()
	add_executable(
		${EXECUTABLE_NAME}
		${sources}
	)
endif ()


###  PACKAGES AND LIBRARIES
###  ======================

# Tell CMake to also search the thirdparty directory for modules.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/thirdparty" ${CMAKE_MODULE_PATH})

# SFML
find_package(SFML REQUIRED system window audio network)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

# Copy the SFML frameworks into the application bundle on Mac OS X.
if (APPLE)
	install(DIRECTORY ${SFML_LIBRARIES} DESTINATION ${CMAKE_BINARY_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks)
endif ()

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})