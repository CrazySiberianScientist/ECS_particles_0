project("ECS_particles")

cmake_minimum_required(VERSION 3.16)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
	message(STATUS 'OPENGL_FOUND-is-true')
else()
	message('OPENGL_FOUND-is-false')
endif(OPENGL_FOUND)	

if (MSVC)
	
	set(GLFW_PATH "$ENV{GLFW_DIR}/")
	
	if(NOT DEFINED $ENV{GLFW_DIR} OR $ENV{GLFW_DIR} EQUAL "")
		#set(GLFW_PATH "D:/GLFW/glfw-3.3.3.bin.WIN64/glfw-3.3.3.bin.WIN64")
	endif()

	if (EXISTS ${GLFW_PATH})
		set(GLFW_INCLUDE_DIR 
			${GLFW_PATH}
			${GLFW_PATH}/include
			)
		
		set(GLFW_LIBRARY_DIR ${GLFW_PATH}/lib-vc2019)

		message(STATUS "GLFW_DIR found here ->" ${GLFW_PATH})
	else ()
		message(FATAL_ERROR "GLFW_DIR path is not found")
	endif ()

endif (MSVC)

if (MSVC)
	
	set(GLM_PATH $ENV{GLM_DIR})
	
	if(NOT DEFINED GLM_PATH OR GLM_PATH EQUAL "")
		#set(GLM_PATH "D:/GLM/glm-master/glm-master")
	endif()

	if (EXISTS ${GLM_PATH})
		set(GLM_INCLUDE_DIR
			${GLM_PATH}
			)
				
		message(STATUS "GLM_DIR found here ->" ${GLM_PATH})
	else ()
		message(FATAL_ERROR "GLM_DIR path is not found")
	endif ()

endif (MSVC)

if (MSVC)
	
	set(OPENGL_EXT_PATH $ENV{OPENGL_EXT_DIR})
	
	if(NOT DEFINED OPENGL_EXT_PATH OR OPENGL_EXT_PATH EQUAL "")
		#set(GLM_PATH "D:/GLM/glm-master/glm-master")
	endif()

	if (EXISTS ${OPENGL_EXT_PATH}) 
		message(STATUS "OPENGL_EXT_DIR found here ->" ${OPENGL_EXT_PATH})
	else ()
		message(FATAL_ERROR "OPENGL_EXT_DIR path is not found")
	endif ()

endif (MSVC)

if (MSVC)
	add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
endif (MSVC)

add_subdirectory(./source)