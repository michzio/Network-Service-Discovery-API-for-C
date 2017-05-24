cmake_minimum_required(VERSION 3.7)
project(Network_Service_Discovery)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c99")
set(SOURCE_FILES network_service_discovery.h network_service_discovery.c unix/event_loop.h unix/event_loop.c apple/event_loop.h apple/event_loop.c windows/event_loop.h windows/event_loop.c)
set(TEST_SOURCE_FILES )

if(CYGWIN)
    # windows cygwin
    message("building for Cygwin")
else()
    # unix
    message("building for Unix")
endif()

if(APPLE)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
endif()

# libraries paths
set(UNIT_TESTS_PATH ${CMAKE_SOURCE_DIR}/../unit_tests)
if(CYGWIN)
    # windows cygwin
    set(UNIT_TESTS_LIB_PATH ${UNIT_TESTS_PATH}/output/windows/lib/static/libunit_tests.a)
else()
    # unix
    set(UNIT_TESTS_LIB_PATH ${UNIT_TESTS_PATH}/output/unix/lib/static/libunit_tests.a)
endif()

add_library(Network_Service_Discovery_Lib SHARED ${SOURCE_FILES})
set_target_properties(Network_Service_Discovery_Lib PROPERTIES OUTPUT_NAME nsd_shared)
if(APPLE)
    target_link_libraries(Network_Service_Discovery_Lib ${COREFOUNDATION_LIBRARY})
endif()

add_library(Network_Service_Discovery_Lib_Static ${SOURCE_FILES})
set_target_properties(Network_Service_Discovery_Lib_Static PROPERTIES OUTPUT_NAME nsd)
if(APPLE)
    target_link_libraries(Network_Service_Discovery_Lib_Static ${COREFOUNDATION_LIBRARY})
endif()

add_executable(Network_Service_Discovery main.c ${TEST_SOURCE_FILES})
set_target_properties(Network_Service_Discovery PROPERTIES OUTPUT_NAME nsd)
target_link_libraries(Network_Service_Discovery Network_Service_Discovery_Lib_Static)
target_link_libraries(Network_Service_Discovery ${UNIT_TESTS_LIB_PATH})

# install targets (executables, shared libraries, static libraries) in current project source directory
add_custom_target(Install_${PROJECT_NAME}
                 $(MAKE) install
                  DEPENDS ${PROJECT_NAME}
                 COMMENT "Installing ${PROJECT_NAME}")

if(CYGWIN)
    # windows cygwin
    set(RUNTIME_DEST_DIR ${CMAKE_SOURCE_DIR}/output/windows/bin)
    set(LIBRARY_DEST_DIR ${CMAKE_SOURCE_DIR}/output/windows/lib)
    set(ARCHIVE_DEST_DIR ${CMAKE_SOURCE_DIR}/output/windows/lib/static)
else()
    #unix
    set(RUNTIME_DEST_DIR ${CMAKE_SOURCE_DIR}/output/unix/bin)
    set(LIBRARY_DEST_DIR ${CMAKE_SOURCE_DIR}/output/unix/lib)
    set(ARCHIVE_DEST_DIR ${CMAKE_SOURCE_DIR}/output/unix/lib/static)
endif()

install(TARGETS Network_Service_Discovery Network_Service_Discovery_Lib Network_Service_Discovery_Lib_Static
        RUNTIME DESTINATION ${RUNTIME_DEST_DIR}
        LIBRARY DESTINATION ${LIBRARY_DEST_DIR}
        ARCHIVE DESTINATION ${ARCHIVE_DEST_DIR})