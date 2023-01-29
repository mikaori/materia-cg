project(backtotheforest)
add_executable(${PROJECT_NAME} camera.cpp ground.cpp skull.cpp forest.cpp main.cpp window.cpp moon.cpp model.cpp material.cpp light.cpp)
enable_abcg(${PROJECT_NAME})
