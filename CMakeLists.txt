cmake_minimum_required(VERSION 3.30)
project(GroundWaterMeasurement)

set(CMAKE_CXX_STANDARD 20)

# Boost Bibliothek einbinden
find_package(Boost REQUIRED COMPONENTS system)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# Erstelle das Server-Programm
add_executable(server src/server.cpp)
target_link_libraries(server PRIVATE Boost::boost Boost::system)

# Erstelle das Client-Programm
add_executable(client src/client.cpp)
target_link_libraries(client PRIVATE Boost::boost Boost::system)
