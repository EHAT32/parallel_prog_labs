cmake_minimum_required(VERSION 3.21)
project(lab_04)
set(SOURCE_FILES 
    src/barbershop.cpp
    src/philosphers.cpp
)
add_executable(barbershop src/barbershop.cpp)
add_executable(philosphers src/philosophers.cpp)
add_executable(new_philosphers src/new_philosopher.cpp)
set_target_properties(new_philosphers PROPERTIES CXX_STANDARD 17)