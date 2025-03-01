cmake_minimum_required(VERSION 3.15)
project(Minigin)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable solution folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Find VLD using environment variable
if(NOT DEFINED ENV{VLD_DIR})
    message(WARNING "VLD_DIR environment variable is not set. Memory leak detection will be disabled.")
else()
    set(VLD_INCLUDE_DIR "$ENV{VLD_DIR}/include")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(VLD_LIBRARY "$ENV{VLD_DIR}/lib/Win64/vld.lib")
    else()
        set(VLD_LIBRARY "$ENV{VLD_DIR}/lib/Win32/vld.lib")
    endif()
endif()

# Add the compile definition before adding source files.
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

# Define ImGui source files
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/3rdParty/imgui-1.91.8")
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Define ImGui headers
set(IMGUI_HEADERS
    ${IMGUI_DIR}/imconfig.h
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/imgui_internal.h
    ${IMGUI_DIR}/imstb_rectpack.h
    ${IMGUI_DIR}/imstb_textedit.h
    ${IMGUI_DIR}/imstb_truetype.h
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.h
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
)

# Define source files for Minigin
set(MINIGIN_SOURCES
    Minigin/BaseComponent.cpp
    
    Minigin/Font.cpp
    Minigin/FPSComponent.cpp
    Minigin/GameObject.cpp
    Minigin/InputManager.cpp
    Minigin/Main.cpp
    Minigin/Minigin.cpp
    Minigin/Renderer.cpp
    Minigin/ResourceManager.cpp
    Minigin/Scene.cpp
    Minigin/SceneManager.cpp
    Minigin/TextComponent.cpp
    
    Minigin/Texture2D.cpp
    Minigin/TextureComponent.cpp
    
    Minigin/TransformComponent.cpp
    Minigin/OrbitComponent.cpp)

set(MINIGIN_HEADERS
    Minigin/BaseComponent.h
    
    Minigin/Font.h
    Minigin/FPSComponent.h
    Minigin/GameObject.h
    Minigin/InputManager.h
    Minigin/Minigin.h
    Minigin/Renderer.h
    Minigin/ResourceManager.h
    Minigin/Scene.h
    Minigin/SceneManager.h
    Minigin/Singleton.h
    Minigin/TextComponent.h
    
    Minigin/Texture2D.h
    Minigin/TextureComponent.h
    
    Minigin/TransformComponent.h
    Minigin/OrbitComponent.h)

# Create executable
add_executable(Minigin ${MINIGIN_SOURCES} ${MINIGIN_HEADERS} ${IMGUI_SOURCES} ${IMGUI_HEADERS})

# Use target_compile_features for C++20.
target_compile_features(Minigin PRIVATE cxx_std_20)
# Set warning level 4 and treat warnings as errors
target_compile_options(Minigin PRIVATE /W4 /WX)

# Set up third-party libraries
set(SDL2_DIR "${CMAKE_SOURCE_DIR}/3rdParty/SDL2")
set(SDL2_IMAGE_DIR "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image")
set(SDL2_TTF_DIR "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf")
set(GLM_DIR "${CMAKE_SOURCE_DIR}/3rdParty/glm")

# Include directories
target_include_directories(Minigin PRIVATE
    ${SDL2_DIR}/include
    ${SDL2_IMAGE_DIR}/include
    ${SDL2_TTF_DIR}/include
    ${GLM_DIR}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Add VLD include directory if available
if(DEFINED ENV{VLD_DIR})
    target_include_directories(Minigin PRIVATE ${VLD_INCLUDE_DIR})
endif()

# Link directories based on architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64-bit
    target_link_directories(Minigin PRIVATE
        ${SDL2_DIR}/lib/x64
        ${SDL2_IMAGE_DIR}/lib/x64
        ${SDL2_TTF_DIR}/lib/x64
    )
else()
    # 32-bit
    target_link_directories(Minigin PRIVATE
        ${SDL2_DIR}/lib/x86
        ${SDL2_IMAGE_DIR}/lib/x86
        ${SDL2_TTF_DIR}/lib/x86
    )
endif()

# Link against the libraries
target_link_libraries(Minigin PRIVATE
    SDL2.lib
    SDL2main.lib
    SDL2_image.lib
    SDL2_ttf.lib
    opengl32.lib
)

# Add VLD library if available
if(DEFINED ENV{VLD_DIR})
    target_link_libraries(Minigin PRIVATE ${VLD_LIBRARY})
    target_compile_definitions(Minigin PRIVATE $<$<CONFIG:Debug>:USE_VLD>)
endif()

# Copy SDL2 DLLs to output directory
add_custom_command(TARGET Minigin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>:${SDL2_DIR}/lib/x64/SDL2.dll>
        $<$<EQUAL:${CMAKE_SIZEOF_VOID_P},4>:${SDL2_DIR}/lib/x86/SDL2.dll>
        $<TARGET_FILE_DIR:Minigin>
)

add_custom_command(TARGET Minigin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>:${SDL2_IMAGE_DIR}/lib/x64/SDL2_image.dll>
        $<$<EQUAL:${CMAKE_SIZEOF_VOID_P},4>:${SDL2_IMAGE_DIR}/lib/x86/SDL2_image.dll>
        $<TARGET_FILE_DIR:Minigin>
)

add_custom_command(TARGET Minigin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>:${SDL2_TTF_DIR}/lib/x64/SDL2_ttf.dll>
        $<$<EQUAL:${CMAKE_SIZEOF_VOID_P},4>:${SDL2_TTF_DIR}/lib/x86/SDL2_ttf.dll>
        $<TARGET_FILE_DIR:Minigin>
)

# Copy data folder to output directory
add_custom_command(TARGET Minigin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Data
        $<TARGET_FILE_DIR:Minigin>/Data
)

# Create source groups for better organization in IDEs (similar to Visual Studio filters)
source_group("ImGui" FILES ${IMGUI_SOURCES} ${IMGUI_HEADERS})