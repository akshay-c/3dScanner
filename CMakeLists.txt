cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

# Auto version pickup
file(READ "${CMAKE_SOURCE_DIR}/config/version" PROJECT_VERSION_STRING)
string(STRIP "${PROJECT_VERSION_STRING}" PROJECT_VERSION_STRING)

project(3dScanner VERSION ${PROJECT_VERSION_STRING})  #Major.Minor.Patch.Tweak

message(STATUS "Welcome to 3dScanner Version ${PROJECT_VERSION_STRING}")

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SYSTEM_DEFINES)
set(PROJECT_DEFINES)
set(PROJECT_INCLUDES)
set(AUTOGEN_INCLUDES ${PROJECT_SOURCE_DIR}/include/autogen)
set(COMMON_FILES)
message(STATUS  "Checking System requirements...")

if(WIN32)
    message(FATAL_ERROR "Cannot build for windows, exiting")
    list(APPEND SYSTEM_DEFINES "-D__WIN32__")
elseif(MSVC)
    message(FATAL_ERROR "Cannot build for MacOS, exiting")
    list(APPEND SYSTEM_DEFINES "-D__MSVC__")
elseif(UNIX)
    message(STATUS "Unix like operating system found. Initializing.." )
    list(APPEND SYSTEM_DEFINES "-D__UNIX__")

endif()

# Common Definitions independent of platform
math(EXPR BUS_SIZE "8* ${CMAKE_SIZEOF_VOID_P}")
list(APPEND SYSTEM_DEFINES "-D_SYS_${BUS_SIZE}")

message(STATUS "Found ${BUS_SIZE} bit system. System Defines: ${SYSTEM_DEFINES}")


list(APPEND PROJECT_INCLUDES "${PROJECT_SOURCE_DIR}/include")
file(GLOB COMMON_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
message(STATUS "Attached Includes: ${PROJECT_INCLUDES}\nAttached Sources: ${COMMON_FILES}\n")

# Export software version from Cmake to Code
configure_file("${PROJECT_SOURCE_DIR}/config/config.h.in" "${AUTOGEN_INCLUDES}/config.h" @ONLY)
message(STATUS "Version file created.\n\n${PROJECT_BINARY_DIR}/config.h")
file(READ "${AUTOGEN_INCLUDES}/config.h" file_contents)
message("${file_contents}")


# Qt specific setup
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

# Target configurations
qt_add_executable(3dScanner ${COMMON_FILES})
target_compile_definitions(3dScanner PRIVATE ${SYSTEM_DEFINES})
target_compile_definitions(3dScanner PRIVATE ${PROJECT_DEFINES})
target_include_directories(3dScanner PRIVATE ${PROJECT_INCLUDES} ${AUTOGEN_INCLUDES})
target_link_libraries(3dScanner PRIVATE Qt6::Widgets)


# # Set properties to ensure it builds as a proper GUI application (e.g., no console window on Windows)
# set_target_properties(HelloWorld PROPERTIES
#     WIN32_EXECUTABLE ON
#     MACOSX_BUNDLE ON
# )