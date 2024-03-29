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
	src/Renderer/VertexArray.cpp
	src/Renderer/VertexArray.h
	src/Renderer/PrintText.cpp
	src/Renderer/PrintText.h
	src/Renderer/RenderEngine.cpp
	src/Renderer/RenderEngine.h
	src/Renderer/MapRender.cpp
	src/Renderer/MapRender.h
	
	src/Managers/ResourceManager.cpp
	src/Managers/ResourceManager.h
	src/Managers/stb_image.h 
	src/Managers/SoundManager.h
	src/Managers/SoundManager.cpp
	src/Managers/ConfigManager.h
	src/Managers/ConfigManager.cpp
	src/Managers/JSONtoType.h
	
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
	
	src/GameClass/Animation/StateAnimationController.cpp
	src/GameClass/Animation/StateAnimationController.h
	
	src/GameClass/UIGame/Menu.cpp
	src/GameClass/UIGame/Menu.h
	src/GameClass/UIGame/staticAnimatedSprite.cpp
	src/GameClass/UIGame/staticAnimatedSprite.h
	src/GameClass/UIGame/staticSprite.cpp
	src/GameClass/UIGame/staticSprite.h
	src/GameClass/UIGame/staticTextForUI.cpp
	src/GameClass/UIGame/staticTextForUI.h
	src/GameClass/UIGame/functions.h
	
	src/GameEngine/GameScene.cpp
	src/GameEngine/GameScene.h
	src/GameEngine/GameSceneResources.cpp
	src/GameEngine/GameSceneResources.h
	
	src/GameEngine/Objects/Camera.h
	src/GameEngine/Objects/Camera.cpp
	src/GameEngine/Objects/Collider.h
	src/GameEngine/Objects/Collider.cpp
	src/GameEngine/Objects/Entity.h
	src/GameEngine/Objects/Entity.cpp
	src/GameEngine/Objects/Object.h
	src/GameEngine/Objects/Object.cpp
	src/GameEngine/Objects/Trigger.h
	src/GameEngine/Objects/Trigger.cpp
	src/GameEngine/Objects/Character.h
	src/GameEngine/Objects/Character.cpp
	src/GameEngine/Objects/Projectile.h
	src/GameEngine/Objects/Projectile.cpp
	
	src/GameEngine/PhysicsAndLogic/Collision.h
	src/GameEngine/PhysicsAndLogic/Collision.cpp
	src/GameEngine/PhysicsAndLogic/DebugRender.h
	src/GameEngine/PhysicsAndLogic/DebugRender.cpp
	src/GameEngine/PhysicsAndLogic/Shape.h
	src/GameEngine/PhysicsAndLogic/Shape.cpp
	src/GameEngine/PhysicsAndLogic/GameSpaceTree.h
	src/GameEngine/PhysicsAndLogic/GameSpaceTree.cpp
	src/GameEngine/PhysicsAndLogic/WaySearch.h
	src/GameEngine/PhysicsAndLogic/WaySearch.cpp
	src/GameEngine/PhysicsAndLogic/WayControlBlock.h
	src/GameEngine/PhysicsAndLogic/WayControlBlock.cpp
	src/GameEngine/PhysicsAndLogic/CollisionController.h
	src/GameEngine/PhysicsAndLogic/CollisionController.cpp
	src/GameEngine/PhysicsAndLogic/Timer.h
	src/GameEngine/PhysicsAndLogic/Timer.cpp
	src/GameEngine/PhysicsAndLogic/ProjectileMoveController.h
	src/GameEngine/PhysicsAndLogic/ProjectileMoveController.cpp
	src/GameEngine/PhysicsAndLogic/ProjectileSpawner.h
	src/GameEngine/PhysicsAndLogic/ProjectileSpawner.cpp
	
	src/UI/Button.cpp
	src/UI/Button.h
	src/UI/ListParameter.h
	src/UI/Slider.cpp
	src/UI/Slider.h
	src/UI/SwitchBool.cpp
	src/UI/SwitchBool.h
	src/UI/Translater.cpp
	src/UI/Translater.h
	src/UI/InputField.cpp
	src/UI/InputField.h
	src/UI/IElementUI.h
	src/UI/valuesControl.h
	
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