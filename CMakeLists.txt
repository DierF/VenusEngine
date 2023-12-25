cmake_minimum_required(VERSION 3.5)

project(VenusEngine)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR})

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed.(build in another folder)"
  )
endif()

file(GLOB_RECURSE CoreFiles "Core/*")
file(GLOB_RECURSE RenderFiles "Render/*")
file(GLOB_RECURSE EditorFiles "Editor/*")
file(GLOB gladc "Library/glad/src/glad.c")

source_group("Core" FILES ${CoreFiles})
source_group("Render" FILES ${RenderFiles})
source_group("Editor" FILES ${EditorFiles})

include_directories(".")

add_executable(VenusEngine main.cpp ${CoreFiles} ${RenderFiles} ${EditorFiles} ${gladc})

target_link_libraries(VenusEngine PUBLIC glfw glm)

target_include_directories(VenusEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Library/glad/include)

add_subdirectory(Library)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build)