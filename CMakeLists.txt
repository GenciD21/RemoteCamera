cmake_minimum_required(VERSION 3.15)
project(my_app)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

set(MYSQLX_INCLUDE_DIR "/home/genci21/Downloads/mysql-connector-c++-9.4.0-linux-glibc2.28-x86-64bit/include/mysqlx")
set(MYSQL_INCLUDE_DIR "/home/genci21/Downloads/mysql-connector-c++-9.4.0-linux-glibc2.28-x86-64bit/include/mysqlx")
set(MYSQLX_LIBRARY "/home/genci21/Downloads/mysql-connector-c++-9.4.0-linux-glibc2.28-x86-64bit/lib64/libmysqlcppconnx.so")
set(MYSQL_LIBRARY "/home/genci21/Downloads/mysql-connector-c++-9.4.0-linux-glibc2.28-x86-64bit/lib64/libmysqlcppconn.so")
add_executable(my_app
        main.cpp
        ./extra_executables/check_images.cpp
)

target_include_directories(my_app PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        ${MYSQLX_INCLUDE_DIR}
        ${MYSQL_INCLUDE_DIR}
        ./extra_headers
)

target_link_libraries(my_app PRIVATE
        ${MYSQLX_LIBRARY}
        ${MYSQL_LIBRARY}
        ${OpenCV_LIBS}
)
