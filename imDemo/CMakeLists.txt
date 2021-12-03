cmake_minimum_required(VERSION 3.10.0)

project(imDemo)

if(UNIX)
  message(STATUS "Using Unix")
  set(IMGUI_ROOT /opt/jpro/imgui)
  set(SYSTEM_INCLUDE /usr/include /usr/local/include)
  set(EXTRA_INCLUDE ${IMGUI_ROOT} ${IMGUI_ROOT}/backends)
  message(STATUS "include directories: ${SYSTEM_INCLUDE};${EXTRA_INCLUDE}")
elseif(WIN32)
  message(STATUS "Using Win32")
  set(IMGUI_ROOT "H:/opt/jpro/imgui")
  set(SYSTEM_INCLUDE )
  set(EXTRA_INCLUDE
    ${IMGUI_ROOT}
    ${IMGUI_ROOT}/backends
    "F:/glm.0.9.9.800/build/native/include"
    "F:/Program Files/assimp-5.0.1/include"
    "F:/Program Files/glew-2.1.0-win32/glew-2.1.0/include"
    "F:/freeglut.2.8.1.15/build/native/include")
else()
  message(FATAL_ERROR "Just support [Win32, Unix]!")
endif()

set(SOURCE_FILES
  main.cpp
  ${IMGUI_ROOT}/imgui.cpp
  ${IMGUI_ROOT}/imgui_demo.cpp
  ${IMGUI_ROOT}/imgui_draw.cpp
  ${IMGUI_ROOT}/imgui_tables.cpp
  ${IMGUI_ROOT}/imgui_widgets.cpp
  ${IMGUI_ROOT}/backends/imgui_impl_glut.cpp
  ${IMGUI_ROOT}/backends/imgui_impl_opengl2.cpp)

include_directories(${SYSTEM_INCLUDE} ${EXTRA_INCLUDE})

if(UNIX)
  add_executable(imDemo ${SOURCE_FILES})
  target_link_libraries(imDemo stdc++ GL GLU glut GLEW assimp)
elseif(WIN32)
  set(HOME_LIB       "E:/Windows Kits/10/Lib/10.0.17763.0/um/x86")
  set(HOME_GLM       "F:/glm.0.9.9.800/build/native")
  set(HOME_GLEW      "F:/Program Files/glew-2.1.0-win32/glew-2.1.0")
  set(HOME_FREE_GLUT "F:/freeglut.2.8.1.15/build/native")
  include_directories("${HOME_GLM}/include" "${HOME_GLEW}/include" "${HOME_FREE_GLUT}/include")
  link_directories("${HOME_GLEW}/lib/Release/Win32" "${HOME_FREE_GLUT}/lib/v110/Win32/Release/dynamic")
  add_executable(imDemo WIN32 ${SOURCE_FILES})
  target_link_libraries(imDemo freeglut glew32 opengl32 assimp-vc141-mtd)
endif()