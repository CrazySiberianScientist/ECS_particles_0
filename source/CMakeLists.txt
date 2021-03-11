project("ECS_particles_0")

cmake_minimum_required(VERSION 3.16)

set(UTILS 
	./utils/Utils.h
	./utils/StructOfVectors.h
	)	
source_group("utils" FILES ${UTILS})

set(ENGINE 
	./engine/AppSystem.h
	./engine/AppSystem.cpp
	./engine/BaseSystem.h
	./engine/CameraSystem.h
	./engine/CameraSystem.cpp
	./engine/Components.h
	./engine/Engine.h
	./engine/EngineComponents.h
	./engine/EngineSystems.h
	./engine/EngineSystemsOrders.h
	./engine/ShadersSystem.h
	./engine/ShadersSystem.cpp
	./engine/Systems.h
	./engine/SystemsOrders.h
	)
source_group("engine" FILES ${ENGINE})

set(GLAD 
	./glad/gl.h
	./glad/glad_gl.c
	./glad/khrplatform.h
	./glad/vk_platform.h
	./glad/vulkan.h
	)
source_group("glad" FILES ${GLAD})

set(ECS 
	./ecs/ECS.h
	)
source_group("ecs" FILES ${ECS})

set(USER_LOGIC 
	./user_logic/SceneLogicSystem.h
	./user_logic/SceneLogicSystem.cpp
	./user_logic/TestLogicSystem.h
	./user_logic/TestLogicSystem.cpp
	./user_logic/UserComponents.h
	./user_logic/UserSystems.h
	./user_logic/UserSystemsOrders.h
	)
source_group("user_logic" FILES ${USER_LOGIC})

add_executable( ${PROJECT_NAME}
	${UTILS}
	${ENGINE}
	${GLAD}
	${ECS}
	${USER_LOGIC}
	./main.cpp
	)
	
target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/data/shaders
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/ecs
	${CMAKE_CURRENT_SOURCE_DIR}/engine
	${CMAKE_CURRENT_SOURCE_DIR}/glad
	${CMAKE_CURRENT_SOURCE_DIR}/user_logic
	${CMAKE_CURRENT_SOURCE_DIR}/utils
	${OPENGL_INCLUDE_DIR}
	${GLFW_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
	${OPENGL_EXT_PATH}
	)

target_link_directories(${PROJECT_NAME} PUBLIC ${GLFW_LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES} glfw3.lib )

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)