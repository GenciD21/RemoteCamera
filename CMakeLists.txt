cmake_minimum_required(VERSION 3.15)
project(my_app)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(mysql-concpp  REQUIRED)

set(EXTRA_FILES
    extra_includes/image_recieve.cpp
)


add_executable(my_app main.cpp ${EXTRA_FILES})

target_include_directories(my_app PRIVATE "./extra_includes")

target_link_libraries(my_app PRIVATE ${OpenCV_LIBS} mysql::concpp)


