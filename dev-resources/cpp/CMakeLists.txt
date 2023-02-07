cmake_minimum_required(VERSION 3.14)

project(anatomycpp)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (HEADERS anata1.h anata2.h anaterm.h anatermid.h anatermstrings.h anatomy.h qutools.h qutree.h quuidtree.h qudefs.h)
set (SOURCES anata1.cpp anata2.cpp anaterm.cpp anatermid.cpp anatermstrings.cpp anatomy.cpp qutools.cpp)

add_library(anatomycpp SHARED ${HEADERS} ${SOURCES})


# target_include_directories(anatomycpp PUBLIC )
# target_link_libraries(anatomycpp PUBLIC)

install(TARGETS anatomycpp DESTINATION lib)
