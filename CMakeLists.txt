cmake_minimum_required( VERSION 3.5 )

# Options can be turned on and off by providing cmake with a -DOPTIONNAME=ON (or OFF) argument.
# For example, to turn off benchmarks, and use sanitize flags in a DEBUG build:
#    cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBENCH=OFF -DSANITIZE=ON

# Add test files with filenames <NAME>_test.cpp or <NAME>_test.c into test dir
# Add bench files with filenames <NAME>_bench.cpp or <NAME>_bench.c into test dir

# Override install prefix with CMAKE_INSTALL_PREFIX
# Override bin dir with BINDIR
# Override lib dir with LIBDIR

option(TEST      "Build tests" ON)
option(BENCH     "Build benchmarks" OFF)
option(ASAN      "Adds sanitize flags" OFF)
option(TSAN      "Adds thread sanitize flags" OFF)

set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}
    CACHE STRING "Choose the type of build: None Debug Release Coverage ASan ASanDbg MemSan MemSanDbg TSan TSanDbg"
    FORCE)

if(SANITIZE)
  set(TSANITIZE OFF)
endif()

if(BENCH)
  set(TEST ON)
endif()

project( test )

set( DIR_SOURCES src )
set( DIR_INCLUDES include )
set( DIR_TESTS test )
set( DIR_SCRIPT cmake )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB cmakeFiles ${DIR_SCRIPT}/*.cmake)
foreach(cmakeFile ${cmakeFiles})
    message("INCLUDE ${cmakeFile}")
    INCLUDE(${cmakeFile})
endforeach(cmakeFile)

include_directories( ${DIR_INCLUDES} )

# Be nice to visual studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Be nice and export compile commands by default, this is handy for clang-tidy and for other tools.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories( ${DIR_INCLUDES} )

#Scan dir for standart source files
aux_source_directory( ${DIR_SOURCES} SOURCES )
add_library( cutils ${SOURCES} )

aux_source_directory( ${DIR_SOURCES}/httputils SOURCES_HTTPUTILS )
add_library( httputils ${SOURCES_HTTPUTILS} )

aux_source_directory( ${DIR_SOURCES}/netutils SOURCES_NETUTILS )
add_library( netutils ${SOURCES_NETUTILS} )

aux_source_directory( ${DIR_SOURCES}/pthreadutils SOURCES_PTHREADUTILS )
add_library( pthreadutils ${SOURCES_PTHREADUTILS} )

#if ( DEFINED DIR_TESTS AND BUILD_TESTING )
if ( DEFINED DIR_TESTS )
    #set enable testing
    enable_testing()
    foreach ( dir IN LISTS DIR_TESTS )
        if ( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${dir} )
            message("test dir ${dir}")
            add_subdirectory( ${dir} )
        endif()
    endforeach()
endif()
