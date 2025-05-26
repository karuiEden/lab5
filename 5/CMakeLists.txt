cmake_minimum_required(VERSION 3.27)
project(lab5 C)
set(CMAKE_C_STANDARD 23)

find_library(GVC_LIB gvc)
find_library(CGRAPH_LIB cgraph)

include_directories(/usr/include/graphviz)
include_directories("../misc")
include_directories("src")

#add_library(TABLE STATIC ../misc/maps/table.c ../misc/maps/ks.c)
add_library(IO SHARED ../misc/io/io.c)
add_library(ERROR SHARED ../misc/error/error_handle.c)
add_library(MENU SHARED ../misc/menu/menu.c)

add_library(GRAPH STATIC
        src/vertex.c
        src/vertex.h
        src/graph.c
        src/graph.h
        src/graph.h
        src/hash_table.c
        src/adj_node.c
        src/adj_node.h
        src/graphic_out.c
        src/info.c
        src/info.h


)
set(CMAKE_C_FLAGS "-O0")

add_executable(lab5
        main.c
)
target_link_libraries(lab5 IO ERROR GRAPH MENU ${GVC_LIB} ${CGRAPH_LIB})
