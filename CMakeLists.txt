cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

if(WIN32)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeBuild")

    project(PhysicalModels LANGUAGES CXX)

    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
    endif()

    add_definitions(-DUNICODE -D_UNICODE)
    add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
    add_compile_definitions(PROJET_DIR="${PROJECT_SOURCE_DIR}/")

    file(GLOB_RECURSE IMGUI_FILE CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/ThirdPartys/Imgui/*.h"
        "${PROJECT_SOURCE_DIR}/ThirdPartys/Imgui/*.cpp")

    file(GLOB_RECURSE IMPLOT_FILE CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/ThirdPartys/Implot/*.h"
        "${PROJECT_SOURCE_DIR}/ThirdPartys/Implot/*.cpp")

    file(GLOB_RECURSE JSON_FILE CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/ThirdPartys/Json/*.h"
        "${PROJECT_SOURCE_DIR}/ThirdPartys/Json/*.cpp")

    file(GLOB_RECURSE MATH_FILE CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/Math/*.h" 
        "${PROJECT_SOURCE_DIR}/Math/*.inl" 
        "${PROJECT_SOURCE_DIR}/Math/*.cpp")

    file(GLOB_RECURSE SCRIPTS_FILE CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/Scripts/*.h" 
        "${PROJECT_SOURCE_DIR}/Scripts/*.cpp")

    file(GLOB_RECURSE PLOTGUI_FILE CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/PlotGUI/*.h" 
        "${PROJECT_SOURCE_DIR}/PlotGUI/*.cpp")

    source_group(TREE ${PROJECT_SOURCE_DIR}/ThirdPartys/Imgui FILES ${IMGUI_FILE})
    source_group(TREE ${PROJECT_SOURCE_DIR}/ThirdPartys/Implot FILES ${IMPLOT_FILE})
    source_group(TREE ${PROJECT_SOURCE_DIR}/ThirdPartys/Json FILES ${JSON_FILE})
    source_group(TREE ${PROJECT_SOURCE_DIR}/Math FILES ${MATH_FILE})
    source_group(TREE ${PROJECT_SOURCE_DIR}/Scripts FILES ${SCRIPTS_FILE})
    source_group(TREE ${PROJECT_SOURCE_DIR}/PlotGUI FILES ${PLOTGUI_FILE})

    add_library(Imgui STATIC ${IMGUI_FILE})
    target_include_directories(Imgui PUBLIC ${PROJECT_SOURCE_DIR}/ThirdPartys/Imgui)
    target_link_libraries(Imgui PUBLIC dwmapi d3dcompiler)
    set_target_properties(Imgui PROPERTIES FOLDER "ThirdPartys")

    add_library(Implot STATIC ${IMPLOT_FILE})
    target_include_directories(Implot PUBLIC ${PROJECT_SOURCE_DIR}/ThirdPartys/Imgui ${PROJECT_SOURCE_DIR}/ThirdPartys/Implot)
    set_target_properties(Implot PROPERTIES FOLDER "ThirdPartys")

    add_library(Json STATIC ${JSON_FILE})
    target_include_directories(Json PUBLIC ${PROJECT_SOURCE_DIR}/ThirdPartys/Json)
    set_target_properties(Json PROPERTIES FOLDER "ThirdPartys")

    add_library(Math STATIC ${MATH_FILE})
    target_include_directories(Math PUBLIC ${PROJECT_SOURCE_DIR}/Math)
    set_target_properties(Math PROPERTIES FOLDER "Math")

    add_executable(PlotGUI ${PLOTGUI_FILE})
    target_link_libraries(PlotGUI PUBLIC d3d11 d3dcompiler dxgi Imgui Implot Json Math)
    target_include_directories(PlotGUI PUBLIC
        ${PROJECT_SOURCE_DIR}/ThirdPartys/Imgui  
        ${PROJECT_SOURCE_DIR}/ThirdPartys/Implot
        ${PROJECT_SOURCE_DIR}/ThirdPartys/Json
        ${PROJECT_SOURCE_DIR}/Math 
        ${PROJECT_SOURCE_DIR}/Scripts
        ${PROJECT_SOURCE_DIR}/PlotGUI
	    ${PROJECT_SOURCE_DIR})
    set_target_properties(PlotGUI PROPERTIES FOLDER "PlotGUI")

    # VS startup project
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT PlotGUI)

elseif(APPLE)
    message("Unsupport Apple Platform.")

elseif(UNIX)
    message("Unsupport Linux Platform.")

endif()
