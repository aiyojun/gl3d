cmake_minimum_required(VERSION 3.10.0)

project(gl3d)

message(STATUS "--------------------------------")
message(STATUS "[subproject] Initialize : glDemo")
add_subdirectory(glDemo)
message(STATUS "--------------------------------")
message(STATUS "[subproject] Initialize : moDemo")
add_subdirectory(moDemo)
message(STATUS "--------------------------------")
message(STATUS "[subproject] Initialize : imDemo")
add_subdirectory(imDemo)
message(STATUS "--------------------------------")
message(STATUS "[subproject] Initialize : skyDemo")
add_subdirectory(skyDemo)

#if(UNIX)
#  message(STATUS "Using Unix")
#  set(IMGUI_ROOT /opt/jpro/imgui)
#  set(SYSTEM_INCLUDE /usr/include /usr/local/include)
#  set(EXTRA_INCLUDE ${IMGUI_ROOT} ${IMGUI_ROOT}/backends)
#  message(STATUS "include directories: ${SYSTEM_INCLUDE};${EXTRA_INCLUDE}")
#elseif(WIN32)
#  message(STATUS "Using Win32")
#  set(IMGUI_ROOT "H:/opt/jpro/imgui")
#  set(SYSTEM_INCLUDE )
#  set(EXTRA_INCLUDE
#    ${IMGUI_ROOT}
#    ${IMGUI_ROOT}/backends
#    "F:/glm.0.9.9.800/build/native/include"
#    "F:/Program Files/assimp-5.0.1/include"
#    "F:/Program Files/glew-2.1.0-win32/glew-2.1.0/include"
#    "F:/freeglut.2.8.1.15/build/native/include")
#else()
#  message(FATAL_ERROR "Just support [Win32, Unix]!")
#endif()

#set(SOURCE_FILES
#  main.cpp
#  ${IMGUI_ROOT}/imgui.cpp
#  ${IMGUI_ROOT}/imgui_demo.cpp
#  ${IMGUI_ROOT}/imgui_draw.cpp
#  ${IMGUI_ROOT}/imgui_tables.cpp
#  ${IMGUI_ROOT}/imgui_widgets.cpp
#  ${IMGUI_ROOT}/backends/imgui_impl_glut.cpp
#  ${IMGUI_ROOT}/backends/imgui_impl_opengl2.cpp)
#
#include_directories(${SYSTEM_INCLUDE} ${EXTRA_INCLUDE})
#
#if(UNIX)
#  add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#  target_link_libraries(${PROJECT_NAME} stdc++ assimp GL GLU glut GLEW)
#elseif(WIN32)
#  add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
#  target_link_libraries(${PROJECT_NAME} freeglut glew32 opengl32 assimp-vc141-mtd)
#endif()