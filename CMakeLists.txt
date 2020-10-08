cmake_minimum_required(VERSION 3.11)
project(Glide3D)

find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

add_executable(main ./main.cpp
	./Dependencies/imgui/imgui.cpp
	./Dependencies/imgui/imgui_demo.cpp
	./Dependencies/imgui/imgui_draw.cpp
	./Dependencies/imgui/imgui_impl_glfw.cpp
	./Dependencies/imgui/imgui_impl_opengl3.cpp
	./Dependencies/imgui/imgui_widgets.cpp
	./Core/Camera.cpp
	./Core/FpsCamera.cpp
	./Core/GL_Classes/CubeTextureMap.cpp
	./Core/GL_Classes/IndexBuffer.cpp
	./Core/GL_Classes/Random.cpp
	./Core/GL_Classes/SSBO.cpp
	./Core/GL_Classes/Texture.cpp
	./Core/GL_Classes/stb_image.cpp
	./Core/GL_Classes/Shader.cpp
	./Core/GL_Classes/UBO.cpp
	./Core/GL_Classes/Fps.cpp
	./Core/GL_Classes/VertexArray.cpp
	./Core/GL_Classes/Framebuffer.cpp
	./Core/GL_Classes/VertexBuffer.cpp
	./Core/Editor/Editor.cpp
	./Core/Model\ Loader/OBJFileLoader.cpp
	./Core/Skybox.cpp
	./Core/TextureAtlas.cpp
	./Core/OrthographicCamera.cpp
	./Core/Renderer/CubeRenderer.cpp
	./Core/Renderer/Renderer.cpp
	./Core/Transform/Transform.cpp
	./Core/Entity/Entity.cpp
	./Core/Application/Logger.cpp
	./Core/Application/Application.cpp
	./Core/Object/Object.cpp
	./Core/Post\ Processor/PostProcessor.cpp)

add_library(glad Dependencies/glad/src/glad.c)
target_include_directories(glad PUBLIC Dependencies/glad/include)
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})

target_link_libraries(main PRIVATE glad glfw assimp)
target_compile_features(main PRIVATE cxx_std_17)
target_include_directories(main PRIVATE Dependencies/imgui)
target_compile_definitions(main PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

file(COPY Core/Shaders DESTINATION ${CMAKE_BINARY_DIR}/Core)

