cmake_minimum_required(VERSION 3.0.0)
project(VisualizedPathfinding VERSION 0.1.0)

file(COPY myRecources DESTINATION .)

#Bad style: Now we can only have Source/Header files in one-folder-subdirs of src. But i can't be bothered to find out how to do it properly
file(GLOB allFiles
     "src/*/*.hpp"
     "src/*/*.cpp"
     "src/*/*.h"
     "src/*/*.c"
)

add_executable(VisualizedPathfinding ${allFiles})
target_include_directories(VisualizedPathfinding PUBLIC ${PROJECT_SOURCE_DIR}/src/Rendering ${PROJECT_SOURCE_DIR}/src/World)

find_package(SFML 2 REQUIRED graphics window system)

include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(VisualizedPathfinding sfml-graphics sfml-window sfml-system)

target_link_libraries(VisualizedPathfinding -pthread)