cmake_minimum_required(VERSION 3.16)
project(VisualizedPathfinding VERSION 0.1.0)

#Bad style: Now we can only have Source/Header files in one-folder-subdirs of src. But i can't be bothered to find out how to do it properly
file(GLOB allFiles
     "src/*/*.hpp"
     "src/*/*.cpp"
     "src/*/*.h"
     "src/*/*.c"
)
#find_package(SFML 2 REQUIRED graphics window system)

include_directories(${PROJECT_SOURCE_DIR}/src/Rendering ${PROJECT_SOURCE_DIR}/src/World ${PROJECT_SOURCE_DIR}/src/Algorithm)

set(SFML_STATIC_LIBRARIES TRUE)
#This project includes build versions of the SFML library for common compilers for convenience. 
if (BUILD_32)
    if(WIN32)#Windows x86
        set(SFML_DIR "${PROJECT_SOURCE_DIR}/SFML/SFML_Windows_x86/lib/cmake/SFML")
    endif()
else ()
    if(WIN32) #Windows #64
        set(SFML_DIR "${PROJECT_SOURCE_DIR}/SFML/SFML_Windows_x64/lib/cmake/SFML")
    endif()
    if(MINGW) #Windows MingW64
        set(SFML_DIR "${PROJECT_SOURCE_DIR}/SFML/SFML_MingW_x64/lib/cmake/SFML")
    endif()
endif ()

if(UNIX) #Unix GCC x64
    set(SFML_DIR "${PROJECT_SOURCE_DIR}/SFML/SFML_Unix_x64/lib/cmake/SFML")
endif()

find_package(SFML 2.5.1 REQUIRED COMPONENTS graphics window system) #find SFML
include_directories(${SFML_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/SFML_Win32/bin) #include SFML and dlls

add_executable(VisualizedPathfinding ${allFiles})
target_link_libraries (VisualizedPathfinding sfml-graphics sfml-window sfml-system -pthread)

