cmake_minimum_required(VERSION 2.8)
project(GPSymbReg)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(SOURCE_FILES
    main.cpp
    SymbRegEvalOp.cpp
    SymbRegEvalOp.h Lexicase.h Lexicase.cpp LexiCaseFitnessMin.cpp LexiCaseFitnessMin.h
        LexiCaseSelBestOp.h LexiCaseSelBestOp.cpp LexiCaseSelOp.h LexiCaseSelOp.cpp)

include_directories(/home/kristijan/Documents/Seminar/ECF/trunk/)
add_executable(GPSymbReg ${SOURCE_FILES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(GPSymbReg ${CMAKE_CURRENT_SOURCE_DIR}/../../debug/libECF.a)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(GPSymbReg ${CMAKE_CURRENT_SOURCE_DIR}/../../release/libECF.a)
endif()