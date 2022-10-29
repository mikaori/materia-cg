project(dogsimulate)
add_executable(${PROJECT_NAME} main.cpp window.cpp balls.cpp
                               dog.cpp flowers.cpp)
enable_abcg(${PROJECT_NAME})
