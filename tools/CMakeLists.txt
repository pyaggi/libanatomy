cmake_minimum_required(VERSION 3.1)
set(CMAKE_LINK_DEPENDS_NO_SHARED ON)

project(Tools)

#QuillTemplate(MiRutina)


add_subdirectory(AnaGenerator)
add_subdirectory(AnaViewer)
add_subdirectory(AnaTranslator)
add_subdirectory(tamixer)
