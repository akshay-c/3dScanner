cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Scanner)

#Check requirements for OpenCv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#find_package(GLUT REQUIRED)
#include_directories(${GLUT_INCLUDE_DIRS})
#link_directories(${GLUT_LIBRARY_DIRS})
#add_definitions(${GLUT_DEFINITIONS})
#if(NOT GLUT_FOUND)
#    message(ERROR " GLUT not found!")
#endif(NOT GLUT_FOUND)

# Use PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
include_directories(${GTK2_INCLUDE_DIRS})
link_directories(${GTK2_LIBRARY_DIRS})
add_definitions(${GTK2_CFLAGS_OTHER})

pkg_check_modules(LIBSERIAL REQUIRED libserial)
include_directories(${LIBSERIAL_INCLUDE_DIRS})
link_directories(${LIBSERIAL_LIBRARY_DIRS})
add_definitions(${LIBSERIAL_CFLAGS_OTHER})

# Use PkgConfig to detect epoxy headers/library files
pkg_check_modules(EPOXY REQUIRED epoxy)
include_directories(${EPOXY_INCLUDE_DIRS})
link_directories(${EPOXY_LIBRARY_DIRS})
add_definitions(${EPOXY_CFLAGS_OTHER})


# Use PkgConfig to detect gtkglarea
pkg_check_modules(GTKGL REQUIRED gtkgl-2.0)
include_directories(${GTKGL_INCLUDE_DIRS})
message(STATUS "${GTKGL_INCLUDE_DIRS}")
link_directories(${GTKGL_LIBRARY_DIRS})
add_definitions(${GTKGL_CFLAGS_OTHER})

# Check PThreads 
find_package (Threads)


#Check OpenGl
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

file(GLOB_RECURSE Scanner_SOURCES "src/*.cpp")
file(GLOB_RECURSE Scanner_HEADERS "include/*.hpp")

set (Scanner_INCLUDE_DIRS "")
foreach (_headerFile ${Scanner_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND Scanner_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES Scanner_INCLUDE_DIRS)

include_directories(${Scanner_INCLUDE_DIRS})
add_executable (Scanner ${Scanner_SOURCES})
set(OPENCV_LIBS opencv_core opencv_imgproc opencv_calib3d opencv_video
opencv_features2d opencv_ml opencv_highgui opencv_objdetect )
#set(OPENCV_LIBS opencv_highgui)

target_link_libraries (Scanner ${OPENCV_LIBS} ${OPENGL_LIBRARIES}
	${GLUT_LIBRARY} ${GTK2_LIBRARIES} ${GTKGL_LIBRARIES} ${EPOXY_LIBRARIES}
	${GDK3_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${LIBSERIAL_LIBRARIES})
