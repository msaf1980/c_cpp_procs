project(tests)

#link_directories( ${PATH_BUILD} )

set( test_procspawn_source test_procspawn.c )
add_executable(test_procspawn ${test_procspawn_source})
target_link_libraries(test_procspawn ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libc_procs.a )

add_test( NAME test_procspawn COMMAND test_procspawn ${CMAKE_CURRENT_SOURCE_DIR}/test_procspawn.pl )

