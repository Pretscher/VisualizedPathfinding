cmake_minimum_required(VERSION 3.0.0)
project(VisualizedPathfinding VERSION 0.1.0)

#Bad style: Now we can only have Source/Header files in one-folder-subdirs of src. But i can't be bothered to find out how to do it properly
file(GLOB allFiles
     "src/*/*.hpp"
     "src/*/*.cpp"
     "src/*/*.h"
     "src/*/*.c"
)
#find_package(SFML 2 REQUIRED graphics window system)

link_directories(${PROJECT_SOURCE_DIR}/SFML/lib)
include_directories(${PROJECT_SOURCE_DIR}/SFML/include ${PROJECT_SOURCE_DIR}/src/Rendering ${PROJECT_SOURCE_DIR}/src/World ${PROJECT_SOURCE_DIR}/src/Algorithm)

add_executable(VisualizedPathfinding ${allFiles})
target_link_libraries(VisualizedPathfinding sfml-graphics sfml-window sfml-system -pthread)
