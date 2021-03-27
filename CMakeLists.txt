# requiered CMAKE version to build the project
cmake_minimum_required (VERSION 3.8)

# current project
project ("EventHandler")

# set comiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# add external include directories
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/glm")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.3/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/glew-2.1.0/include")

# add external library 
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/glm/glm/lib")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.3/lib")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/glew-2.1.0/lib/Release/x64")

# link event handler library
add_library(gl_shader STATIC 
			"${CMAKE_CURRENT_SOURCE_DIR}/src/shader_load_src.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/src/shader_other_src.cpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/src/shader_uniform_src.cpp") 

# compile and link final executable
add_executable(gl_shader_test 
			   "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

# link external libraries to the executable
target_link_libraries(gl_shader_test
					  "gl_shader"
					  "-lglm_static"
					  "-lglfw3"
					  "-lglew32s"
					  "-lopengl32"
					  "-lglu32"
					  "-lgdi32")

# additional work
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
