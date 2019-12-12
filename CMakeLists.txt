cmake_minimum_required(VERSION 3.15)
project(Linux_System_Monitor C)

set(CMAKE_C_STANDARD 11)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
add_link_options(-rdynamic)
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(Linux_System_Monitor src/main.c)

target_link_libraries(Linux_System_Monitor ${GTK3_LIBRARIES})