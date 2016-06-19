cmake_minimum_required(VERSION 2.8)
project(fingers)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS system REQUIRED)

include_directories(".")
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -march=native -Wall -O2")
set(SRC_LIST app.cpp main.cpp checker.cpp configs.cpp log.cpp tcpclient.cpp notify.cpp database.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} -lcurl -lpthread -lmysqlclient)

install (FILES build/fingers DESTINATION /usr/share/fingers/)
install (FILES fingers.cfg DESTINATION /etc/)
