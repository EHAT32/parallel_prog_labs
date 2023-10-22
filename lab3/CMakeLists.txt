cmake_minimum_required(VERSION 3.21)
project(lab_03)
set(SOURCE_FILES 
    src/producer_consumer.cpp
    src/writers_readers.cpp
)
add_executable(producer_consumer src/producer_consumer.cpp)
add_executable(writers_readers src/writers_readers.cpp)