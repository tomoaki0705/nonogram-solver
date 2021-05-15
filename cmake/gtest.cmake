
include(ExternalProject)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/third-party/include)

ExternalProject_Add(gtest_ext
    URL "https://github.com/google/googletest/archive/release-1.8.1.zip"
    BINARY_DIR "${CMAKE_BINARY_DIR}/third-party/gtest-build"
    SOURCE_DIR "${CMAKE_BINARY_DIR}/third-party/gtest-src"
    INSTALL_COMMAND "${CMAKE_COMMAND}"
        --build . 
        --target install
    CMAKE_ARGS "${gtest_cmake_args}"
        "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
        "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/third-party/"
        "-Dgtest_force_shared_crt=ON"
)
set(GTEST_INCLUDE_DIRS
    "${CMAKE_BINARY_DIR}/third-party/gtest-src/include"
)
add_library(gtest STATIC IMPORTED)
if(WIN32)
    set(LIB_EXTENTION "lib")
else(WIN32)
    set(LIB_EXTENTION "a")
endif()


#set_property(TARGET gtest APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(gtest PROPERTIES
    IMPORTED_LOCATION             "${CMAKE_BINARY_DIR}/third-party/lib/gtestd.${LIB_EXTENTION}"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/third-party/include"
)

#list(APPEND _IMPORT_CHECK_TARGETS gtest )
#list(APPEND _IMPORT_CHECK_FILES_FOR_gtest "${CMAKE_BINARY_DIR}/third-party/lib/gtestd.lib" )


#set_target_properties(gtest PROPERTIES
#    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
#    IMPORTED_IMPLIB_RELEASE "${CMAKE_BINARY_DIR}/third-party/lib/<gtest_release_importlib>.${LIB_EXTENTION}"
#    IMPORTED_IMPLIB_DEBUG   "${CMAKE_BINARY_DIR}/third-party/lib/<gtest_debug_importlib>.${LIB_EXTENTION}"
#    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/third-party/include"
#)
add_dependencies(gtest gtest_ext)


enable_testing()

find_package(Threads)

function(cxx_test name sources)
    add_executable(${name} ${sources})
    target_link_libraries(${name} gtest ${ARGN} ${CMAKE_THREAD_LIBS_INIT})
    set_property(TARGET ${name} APPEND PROPERTY INCLUDE_DIRECTORIES
                 ${GTEST_INCLUDE_DIRS}
                 )
    # Working directory: where the dlls are installed.
    add_test(NAME ${name} 
             COMMAND ${name} "--gtest_break_on_failure")
endfunction()

