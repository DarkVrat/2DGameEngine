cmake_minimum_required (VERSION 3.24 FATAL_ERROR)

set(PROJECT 2DGameEngine)
project(${PROJECT})

add_executable(${PROJECT} 
	src/main.cpp
	src/Renderer/ShaderProgram.cpp
	src/Renderer/ShaderProgram.h
	src/Renderer/Texture2D.cpp
	src/Renderer/Texture2D.h
	src/Renderer/Sprite.cpp
	src/Renderer/Sprite.h
	src/Renderer/StateAnimation.cpp
	src/Renderer/StateAnimation.h
	src/Renderer/VertexBuffer.cpp
	src/Renderer/VertexBuffer.h
	src/Renderer/VertexBufferLayout.cpp
	src/Renderer/VertexBufferLayout.h
	src/Renderer/VertexArray.cpp
	src/Renderer/VertexArray.h
	src/Renderer/IndexBuffer.cpp
	src/Renderer/IndexBuffer.h
	src/Renderer/PrintText.cpp
	src/Renderer/PrintText.h
	src/Renderer/RenderEngine.cpp
	src/Renderer/RenderEngine.h
	
	src/Managers/ResourceManager.cpp
	src/Managers/ResourceManager.h
	src/Managers/stb_image.h 
	src/Managers/SoundManager.h
	src/Managers/SoundManager.cpp
	src/Managers/ConfigManager.h
	src/Managers/ConfigManager.cpp
	
	src/Audio/SoundEffectsPlayer.cpp
	src/Audio/SoundEffectsPlayer.h
	src/Audio/SoundDevice.cpp
	src/Audio/SoundDevice.h
	src/Audio/SampleSourse.h
	src/Audio/SoundEffectsLibrary.cpp
	src/Audio/SoundEffectsLibrary.h
	src/Audio/FileOfSound.cpp
	src/Audio/FileOfSound.h
	
	src/Control/KeyboardControl.cpp
	src/Control/KeyboardControl.h
	src/Control/MouseControl.cpp
	src/Control/MouseControl.h
	
	src/GameClass/MainGameClass.cpp
	src/GameClass/MainGameClass.h
	src/GameClass/GameObject.cpp
	src/GameClass/GameObject.h
	src/GameClass/StateController.cpp
	src/GameClass/StateController.h
)

target_compile_features(${PROJECT} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT} glad)

include_directories(external/glm)

include_directories(external/rapidjson/include)

add_subdirectory(external/freetype)
target_link_libraries(${PROJECT} freetype)

add_subdirectory(external/openal-soft)
include_directories(external/openal-soft/include)
target_link_libraries(${PROJECT} OpenAL)


add_subdirectory(external/libsndfile)
include_directories(external/libsndfile/include)
target_link_libraries(${PROJECT} sndfile)

set_target_properties(${PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT}>/res)