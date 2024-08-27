cmake_minimum_required(VERSION 3.16)

project(SudokuApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH "/home/marc-antoine/Libraries/Qt/6.7.2/gcc_64")  
set(CMAKE_PREFIX_PATH "/home/marc-antoine/Libraries/or-tools")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(ortools REQUIRED CONFIG)

set(CMAKE_AUTOMOC ON)

add_executable(SudokuApp main.cpp sudokuboard.cpp sudokucell.cpp mainmenu.cpp)

target_include_directories(SudokuApp PRIVATE ${Qt6Widgets_INCLUDE_DIRS})


target_link_libraries(SudokuApp Qt6::Widgets ortools::ortools)


#cmake --build build
