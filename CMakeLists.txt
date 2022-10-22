cmake_minimum_required(VERSION 3.23)
include(GenerateExportHeader)
#Main Project
project(Glass)

#Set C++ Version   // -O3 -Ofast
set(CMAKE_CXX_STANDARD 20)

add_library(Glass SHARED
        external/glad/glad.c external/lodepng/lodepng.cpp
        lib/Base/Glass.cc
        lib/Components/IComponent.h lib/Components/Mesh.h
        lib/Components/Transform.h lib/Base/Logging.h
        lib/Managers/ComponentManager.h lib/Managers/EntityManager.cc
        lib/Managers/EntityManager.h lib/Loaders/MeshLoader.cc lib/Loaders/MeshLoader.h
        lib/Rendering/RenderSystem.cc lib/Rendering/RenderSystem.h lib/Components/Shader.h
        lib/Base/Window.h lib/Base/Result.h lib/Base/Window.cc lib/Loaders/TextureLoader.cc lib/Loaders/TextureLoader.h
        lib/Components/Camera.h lib/Components/Camera.cpp lib/Base/Reflectable.h lib/Base/Utils.h lib/Filesystem/Vfs.cc lib/Filesystem/Vfs.h lib/Components/IComponentFactory/IComponentFactory.h lib/Components/Mesh.cc lib/Components/Shader.cc lib/Components/Texture.cc lib/Components/Transform.cc lib/Components/IComponentFactory/IComponentFactory.cc lib/Components/IComponentFactory/DerivedRegister.h lib/Loaders/ShaderLoader.cc lib/Loaders/ShaderLoader.h)

add_subdirectory(external/refl-cpp-master/)

target_include_directories(Glass PRIVATE
        lib/
        include/
        include/lodepng/
        external/refl-cpp-master/include/
        ${PROJECT_BINARY_DIR}/exports/
        )



target_compile_definitions(Glass PRIVATE Glass_EXPORTS)

generate_export_header(Glass EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/glass_export.h)

target_link_libraries(Glass opengl32.dll ${CMAKE_SOURCE_DIR}/external/glfw/glfw3dll.lib)


#Project for testing
project(Test)

add_executable(Test tests/main.cc)


target_link_libraries(Test ${CMAKE_BINARY_DIR}/Glass.lib)

target_include_directories(Test PRIVATE
        lib
        include
        include/lodepng/
        external/refl-cpp-master/include/
        ${CMAKE_BINARY_DIR}/exports/
        )