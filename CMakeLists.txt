cmake_minimum_required(VERSION 3.11...3.16)

#------------------------
# project setting
#------------------------
project(WSEngine)
set(TARGET WSEngine)

#------------------------
# c++ standard setting
#------------------------
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)


#------------------------
# file setting
#------------------------
set(EXTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
set(INCLUDE_DIR include)
set(SRC_DIR src)

file(GLOB INCLUDE_FILES ${INCLUDE_DIR}/*.h)
file(GLOB SOURCE_FILES ${SRC_DIR}/*.cpp)

#------------------------
# building
#------------------------
# add_subdirectory(${EXTERNAL}/bullet/wse)
# add_subdirectory(${EXTERNAL}/glfw/wse)
# add_subdirectory(${EXTERNAL}/assimp/wse)
# add_subdirectory(${EXTERNAL}/std/wse)
# add_subdirectory(${EXTERNAL}/imgui/wse)
# add_subdirectory(${EXTERNAL}/tinyexr/wse)

add_executable(${TARGET} ${INCLUDE_FILES} ${SOURCE_FILES})
target_include_directories(${TARGET} PUBLIC ${INCLUDE_DIR})

#------------------------
# installing
#------------------------
install(TARGETS ${TARGET} DESTINATION bin) # binary

install(TARGETS ${TARGET} LIBRARY DESTINATION lib) # Library

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include) # Header files