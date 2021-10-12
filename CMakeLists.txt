cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME "onepiece_bot")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(OpenSSL REQUIRED)

find_package(Boost 1.74 REQUIRED COMPONENTS system)
IF(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
ELSE()
    message(FATAL "${CMAKE_SOURCE_DIR}/../include")
ENDIF(Boost_FOUND)

set(include_dir "${CMAKE_SOURCE_DIR}/dep/libtelegram/include"
                "${CMAKE_SOURCE_DIR}/src" 
                "${CMAKE_SOURCE_DIR}/dep/HTTPRequest/include")
                
message(STATUS "include_dir: ${include_dir}")
include_directories (${include_dir})

set(link_libs 
    Threads::Threads
    OpenSSL::SSL
    Boost::system)

add_executable(${PROJECT_NAME} src/main.cpp src/bot.cpp src/database.cpp src/fetcher.cpp)
target_link_libraries(${PROJECT_NAME} ${link_libs})