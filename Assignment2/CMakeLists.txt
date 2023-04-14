project(Assignment2)

set(CMAKE_CXX_FLAGS -pthread)

set(ASS21 mainAss2.cpp)
set(ASS22 mainAss22.cpp)
set(ASS23 mainAss2Asych.cpp)

add_executable(Assignment21 ${ASS21})
add_executable(Assignment22 ${ASS22})
add_executable(Assignment23 ${ASS23})


