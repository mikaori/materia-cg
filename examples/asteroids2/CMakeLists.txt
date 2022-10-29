project(asteroids2)
add_executable(${PROJECT_NAME} main.cpp window.cpp balls.cpp
                               dog.cpp starlayers.cpp)
enable_abcg(${PROJECT_NAME})
