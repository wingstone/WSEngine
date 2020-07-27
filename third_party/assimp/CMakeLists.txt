cmake_minimum_required(VERSION 3.11...3.16)

target_include_directories(${TARGET} PUBLIC include)
target_link_directories(${TARGET} PUBLIC lib)
target_link_libraries(${TARGET} assimp-vc141-mt.lib)

# dll
install(FILES bin/assimp-vc141-mt.dll DESTINATION WSEngine/bin)