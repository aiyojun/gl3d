cmake_minimum_required(VERSION 3.10.0)

project(sphereDemo)

if(UNIX)
  add_executable(sphereDemo main.cpp)
  target_link_libraries(sphereDemo stdc++ m)
elseif(WIN32)
  set(HOME_LIB       "E:/Windows Kits/10/Lib/10.0.17763.0/um/x86")
  add_executable(sphereDemo WIN32 main.cpp)
  target_link_libraries(sphereDemo)
endif()