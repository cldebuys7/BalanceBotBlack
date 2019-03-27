#this is a comment
cmake_minimum_required(VERSION 2.8.9)
project(balancebotblack)
 
#Bring the headers into the project
include_directories(include)
link_directories(lib)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")
 
add_executable(balancebotEXECUTE ${SOURCES})

TARGET_LINK_LIBRARIES(balancebotEXECUTE Kalman)
