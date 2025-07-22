cmake_minimum_required(VERSION 3.15)
project(my_app)

set(CMAKE_CXX_STANDARD 17)


#set(OpenCV_DIR "/home/genci21/opencv/opencv-4.x/cmake")
#set(mysql_concpp_DIR )
#find_package(OpenCV REQUIRED)


find_package(OpenCV REQUIRED)

# set(EXTRA_FILES
#     extra_includes/image_recieve.cpp
# )


add_executable(my_app main.cpp
        esp_stuff/main/extra_executables/wifi.c
        esp_stuff/main/extra_includes/wifi.h
        esp_stuff/main/extra_executables/http_requests.c
        esp_stuff/main/extra_includes/http_requests.h)

# target_include_directories(my_app PRIVATE "./extra_includes")

target_link_libraries(my_app mysqlcppconn)
target_link_libraries(my_app ${OpenCV_LIBS})
target_include_directories(my_app PRIVATE ${OpenCV_INCLUDE_DIRS})
#PRIVATE ${OpenCV_LIBS}

