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

set(INCLUDE_DIR include)
set(SRC_DIR src)

file(GLOB INCLUDE_FILES ${SRC_DIR}/*.h)
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

add_executable(${TARGET} ${INCLUDE_FILES} ${SOURCE_FILES} ${EXTERN_SRC})
target_link_libraries(${TARGET} opengl32.lib)

#------------------------
# third setting
#------------------------\
set(EXTERNAL third_party)
add_subdirectory(${EXTERNAL})

#------------------------
# installing
#------------------------
install(TARGETS ${TARGET} DESTINATION WSEngine/bin) # binary

install(TARGETS ${TARGET} LIBRARY DESTINATION WSEngine/lib) # Library

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION WSEngine/include) # Header files