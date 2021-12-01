cmake_minimum_required(VERSION 3.10.0)

project(glDemo)

add_executable(glDemo main.cpp ../shader.cpp)

if(UNIX)
  target_link_libraries(glDemo stdc++ GL GLU glut GLEW)
elseif(WIN32)
  target_link_libraries(glDemo freeglut glew32 opengl32)
endif()