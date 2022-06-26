set(FILES_CPP	"main.cpp"
				"GLSLProgram.cpp" "ShaderManager.cpp" "MeshGLInfo.cpp" "Object.cpp" "Scene.cpp" "GeometryUtil.cpp" "Window.cpp" "VertexArrayObject.cpp")

include_directories(CG PUBLIC	"${CMAKE_SOURCE_DIR}/include"
								"${CMAKE_SOURCE_DIR}/libs/glfw/include"
								"${CMAKE_SOURCE_DIR}/libs/glad/include"
								"${CMAKE_SOURCE_DIR}/libs/glm")
												
add_executable (CG ${FILES_CPP})
target_compile_definitions(CG PUBLIC GLFW_INCLUDE_NONE)

target_link_libraries (CG glfw glad)

configure_file("${CMAKE_SOURCE_DIR}/shader/simple.frag" "shader/simple.frag" COPYONLY)
configure_file("${CMAKE_SOURCE_DIR}/shader/simple.vert" "shader/simple.vert" COPYONLY)
