cmake_minimum_required(VERSION 3.11...3.16)

set(IMGUI_INCLUDE_DIR /)

set(IMGUI_INCLUDE_FILES
    imconfig.h
    imgui.h
    imgui_internal.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
)

set(IMGUI_SRCS_FILES
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
)

add_library(imgui ${IMGUI_INCLUDE_FILES} ${IMGUI_SRCS_FILES})

# main target
target_include_directories(${TARGET} PUBLIC ${IMGUI_INCLUDE_DIR})
target_link_libraries(${TARGET} imgui)