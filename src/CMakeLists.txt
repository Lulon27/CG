set(FILES_CPP	"main.cpp"
				"GLSLProgram.cpp" "ShaderManager.cpp" "MeshGLInfo.cpp" "Object.cpp" "Scene.cpp" "GeometryUtil.cpp" "Window.cpp" "VertexArrayObject.cpp" "OBJFile.cpp")

include_directories(CG PUBLIC	"${CMAKE_SOURCE_DIR}/include"
								"${CMAKE_SOURCE_DIR}/libs/glfw/include"
								"${CMAKE_SOURCE_DIR}/libs/glad/include"
								"${CMAKE_SOURCE_DIR}/libs/glm")
												
add_executable (CG ${FILES_CPP})
target_compile_definitions(CG PUBLIC GLFW_INCLUDE_NONE)

target_link_libraries (CG glfw glad)

configure_file("${CMAKE_SOURCE_DIR}/shader/simple.frag" "shader/simple.frag" COPYONLY)
configure_file("${CMAKE_SOURCE_DIR}/shader/simple.vert" "shader/simple.vert" COPYONLY)

configure_file("${CMAKE_SOURCE_DIR}/shader/shaded.frag" "shader/shaded.frag" COPYONLY)
configure_file("${CMAKE_SOURCE_DIR}/shader/shaded.vert" "shader/shaded.vert" COPYONLY)

configure_file("${CMAKE_SOURCE_DIR}/shader/shadedPhong.frag" "shader/shadedPhong.frag" COPYONLY)
configure_file("${CMAKE_SOURCE_DIR}/shader/shadedPhong.vert" "shader/shadedPhong.vert" COPYONLY)

configure_file("${CMAKE_SOURCE_DIR}/shader/shadedGouraud.frag" "shader/shadedGouraud.frag" COPYONLY)
configure_file("${CMAKE_SOURCE_DIR}/shader/shadedGouraud.vert" "shader/shadedGouraud.vert" COPYONLY)


configure_file("${CMAKE_SOURCE_DIR}/Testobjs/bigguy.obj"					"Testobjs/bigguy.obj" COPYONLY)
configure_file("${CMAKE_SOURCE_DIR}/Testobjs/chess_king.obj"				"Testobjs/chess_king.obj" COPYONLY)
configure_file("${CMAKE_SOURCE_DIR}/Testobjs/kship3.obj"					"Testobjs/kship3.obj" COPYONLY)
configure_file("${CMAKE_SOURCE_DIR}/Testobjs/kship3_mitNormalenTest.obj"	"Testobjs/kship3_mitNormalenTest.obj" COPYONLY)
configure_file("${CMAKE_SOURCE_DIR}/Testobjs/sonne.obj"						"Testobjs/sonne.obj" COPYONLY)
configure_file("${CMAKE_SOURCE_DIR}/Testobjs/stanford_bunny_closed_withnormals.obj" "Testobjs/stanford_bunny_closed_withnormals.obj" COPYONLY)
configure_file("${CMAKE_SOURCE_DIR}/Testobjs/suzanne.obj"					"Testobjs/suzanne.obj" COPYONLY)
