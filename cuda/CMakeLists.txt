cmake_minimum_required(VERSION 3.21)

project(lab_09 LANGUAGES CXX CUDA)
set(SOURCE_FILES 
    src/main.cu
)
add_executable(example src/main.cu)