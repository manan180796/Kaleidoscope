cmake_minimum_required(VERSION 3.0.0)
project(Kaleidoscope VERSION 0.1.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
# set(CMAKE_CXX_COMPILER clang++)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_subdirectory(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS")
add_executable(Kaleidoscope main.cpp)
target_include_directories(Kaleidoscope PUBLIC include)

llvm_map_components_to_libnames(llvm_libs
    all
    )

target_link_libraries(Kaleidoscope 
# ${llvm_libs}
    LLVM-10
    Driver
    # LLVM 
)
get_target_property(OUT Kaleidoscope LINK_LIBRARIES)
message("${llvm_libs}")
install(TARGETS Kaleidoscope
        # CONFIGURATIONS Debug
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
