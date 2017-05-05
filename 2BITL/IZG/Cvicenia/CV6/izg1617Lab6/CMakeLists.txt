cmake_minimum_required(VERSION 3.1.0)

PROJECT(Izg2016Lab6)

set(APPLICATION_NAME izg2016Lab6)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

if(MSVC)
	set(USE_PREBUILD_LIB_PACKAGE ON CACHE BOOL "Use prebuild lib package.")

	if(USE_PREBUILD_LIB_PACKAGE)
		set(SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/VisualStudio/SDL2/include CACHE STRING "" FORCE)
		set(SDL2_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/VisualStudio/SDL2/lib/SDL2.lib CACHE STRING "" FORCE)
		set(SDL2MAIN_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/VisualStudio/SDL2/lib/SDL2main.lib CACHE STRING "" FORCE)
		set(SDL2_LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/VisualStudio/SDL2/lib/SDL2.lib CACHE STRING "" FORCE)
		file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/bin/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	endif() 
endif() 

find_package(SDL2          REQUIRED)

set(STUDENT_SOURCES
  student/student.c
  student/linearAlgebra.c
  student/main.c
  student/camera.c
  student/bunny.c
  student/mouseCamera.c
  student/program.c
  )

set(STUDENT_INCLUDES
  student/student.h
  student/camera.h
  student/bunny.h
  student/mouseCamera.h
  student/program.h
  )

set(EXAMPLE_SOURCES
  examples/quadExample.c
  )

set(EXAMPLE_HEADERS
  examples/quadExample.h
  )

set(OPENGL_INCLUDES
  opengl/opengl.h
  )

set(OPENGL_SOURCES
  opengl/opengl.c
  )

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)

IF(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-W)
  add_compile_options(-Wall)
  add_compile_options(-Wconversion)
  add_compile_options(-pedantic)
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCC)
  add_compile_options(-W)
  add_compile_options(-Wall)
  add_compile_options(-Wconversion)
  add_compile_options(-pedantic)
ENDIF()


add_executable(${APPLICATION_NAME} ${OPENGL_SOURCES} ${OPENGL_INCLUDES} ${STUDENT_SOURCES} ${STUDENT_INCLUDES} ${EXAMPLE_SOURCES} ${EXAMPLE_HEADERS})

if(MSVC)
  target_link_libraries(${APPLICATION_NAME} SDL2)
else()
  target_link_libraries(${APPLICATION_NAME} SDL2 m)
endif()

include_directories(${Izg2016Lab6_SOURCE_DIR})
