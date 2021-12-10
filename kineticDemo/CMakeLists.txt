cmake_minimum_required(VERSION 3.10.0)

project(kineticDemo)

if(UNIX)
  add_executable(kineticDemo main.cpp)
  target_link_libraries(kineticDemo stdc++ m GL GLU glut GLEW)
elseif(WIN32)
  set(HOME_LIB       "E:/Windows Kits/10/Lib/10.0.17763.0/um/x86")
  add_executable(kineticDemo WIN32 main.cpp)
  target_link_libraries(kineticDemo freeglut glew32 opengl32)
endif()