cmake_minimum_required(VERSION 3.11...3.16)

include_directories(include)
file(GLOB GLAD_SRC ./src/*.c ./src/*.cpp)

add_library(glad ${GLAD_INCLUDE} ${GLAD_SRC})

# main target
target_include_directories(${TARGET} PUBLIC include)
target_link_libraries(${TARGET} glad)