find_package(GTest REQUIRED)

include_directories(
	../${DIR_INCLUDES}
	${GTEST_INCLUDE_DIRS}
)

set( TEST_LIBRARIES
	${GTEST_LIBRARIES}
	cutils
)

# Build tests
file(GLOB testFiles *_test.cpp *_test.c)
foreach(testFile ${testFiles})
	file(RELATIVE_PATH test_file ${CMAKE_CURRENT_SOURCE_DIR} ${testFile})
	string(REGEX REPLACE "^([0-9a-zA-Z_\-]+)_test\.c.*$" "\\1" test "${test_file}")
	add_executable( test_${test} ${test_file} )
	target_link_libraries ( test_${test} ${TEST_LIBRARIES} )
	add_test(
		NAME test_${test}
		COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_${test}
	)
endforeach(testFile)

#foreach( t IN LISTS TESTS )
#add_executable( test_${t} test_${t}.cpp )
#target_link_libraries ( test_${t} ${LIBRARIES} )
#add_test(
   #NAME test_${t}
   #COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_${t}
#)
#endforeach(t)

# Build benchmarks
if ( BENCH )
set( BENCH_LIBRARIES
        #benchmark
)

if ( SOURCES_TEST )
#       add_library( test${PROJECT} STATIC ${SOURCES_TEST} )
	list( APPEND BENCH_LIBRARIES test${PROJECT} )
endif()

file(GLOB testFiles *_bench.cpp *_bench.c)
foreach(testFile ${testFiles})
	file(RELATIVE_PATH test_file ${CMAKE_CURRENT_SOURCE_DIR} ${testFile})
	string(REGEX REPLACE "^([0-9a-zA-Z_\-]+)_bench\.c.*$" "\\1" test "${test_file}")
	add_executable( bench_${test} ${test_file} ${SOURCES_TEST} )
	target_link_libraries ( bench_${test} ${BENCH_LIBRARIES} )
	add_bench(
		bench_${test}
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bench_${test}
	)
endforeach(testFile)
endif() # BENCH
