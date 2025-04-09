cmake_minimum_required(VERSION 3.30)
project(MultithreadedParticleSimulation)

set(CMAKE_CXX_STANDARD 20)

if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()

include(FetchContent)

function(include_dependency libName gitURL gitTag)
    FetchContent_Declare(${libName}
            GIT_REPOSITORY ${gitURL}
            GIT_TAG    ${gitTag}
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(${libName})
endfunction()

set(LIB1 raylib)
find_package(${LIB1} QUIET)
if (NOT ${LIB1}_FOUND)
    include_dependency(${LIB1} http://github.com/raysan5/raylib.git 5.0)
endif ()

add_executable(MultithreadedParticleSimulation main.cpp
        Particles.cpp
        Particles.h
        ThreadedSolver.cpp
        ThreadedSolver.h
        Renderer.cpp
        Renderer.h
        Benchmark.h
        Benchmark.cpp
)

target_include_directories(MultithreadedParticleSimulation PRIVATE ${raylib_INCLUDE_DIRS})

target_link_libraries(MultithreadedParticleSimulation PRIVATE ${LIB1})