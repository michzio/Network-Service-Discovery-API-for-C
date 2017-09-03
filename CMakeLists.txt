cmake_minimum_required(VERSION 3.7)
project(Network_Service_Discovery)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c99")
set(SOURCE_FILES include/network_service_discovery.h src/network_service_discovery.c unix/event_loop.h unix/event_loop.c apple/event_loop.h apple/event_loop.c windows/event_loop.h windows/event_loop.c)
set(TEST_SOURCE_FILES )

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs)
if(WIN32)
    # windows native
    message("building for Windows")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/lib/windows)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/lib/windows)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/bin/windows)
elseif(CYGWIN)
    # windows cygwin
    message("building for Cygwin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/lib/cygwin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/lib/cygwin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/bin/cygwin)
elseif(APPLE)
    # apple
    message("building for Apple")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/lib/apple)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/lib/apple)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/bin/apple)
else()
    # unix
    message("building for Unix")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/lib/unix)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/lib/unix)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/bin/unix)
endif()

### MAKING LIBRARIES ###
# include current project headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(APPLE)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
endif()

# libraries paths
# Bonjour SDK (DNS SD) library
if(WIN32)
    find_path(BONJOUR_PATH dns_sd.h
              PATHS $ENV{BONJOUR_SDK_HOME}/include)
    include_directories(BONJOUR_PATH)

    find_library(BONJOUR_LIBRARY
            NAMES dnssd.lib
            PATHS "$ENV{BONJOUR_SDK_HOME}/Lib/x64")

    find_library(WSOCK32_LIBRARY wsock32)
    find_library(WS2_32_LIBRARY ws2_32)
elseif(CYGWIN)
    find_path(BONJOUR_PATH dns_sd.h
            PATHS $ENV{BONJOUR_SDK_HOME}/include)
    include_directories(BONJOUR_PATH)

    find_library(BONJOUR_LIBRARY
            NAMES libdns_sd.dll.a
            PATHS "c:/cygwin64/lib")
endif()

# current project shared and static libraries creation
# SHARED
add_library(Network_Service_Discovery_Lib SHARED ${SOURCE_FILES})
set_target_properties(Network_Service_Discovery_Lib PROPERTIES OUTPUT_NAME nsd)
if(APPLE)
    target_link_libraries(Network_Service_Discovery_Lib ${COREFOUNDATION_LIBRARY})
endif()
if(WIN32)
    target_link_libraries(Network_Service_Discovery_Lib ${BONJOUR_LIBRARY})
    target_link_libraries(Network_Service_Discovery_Lib ${WSOCK32_LIBRARY})
    target_link_libraries(Network_Service_Discovery_Lib ${WS2_32_LIBRARY})
endif()
if(CYGWIN)
    target_link_libraries(Network_Service_Discovery_Lib ${BONJOUR_LIBRARY})
endif()

# STATIC
add_library(Network_Service_Discovery_Lib_Static ${SOURCE_FILES})
set_target_properties(Network_Service_Discovery_Lib_Static PROPERTIES OUTPUT_NAME nsd)
if(APPLE)
    target_link_libraries(Network_Service_Discovery_Lib_Static ${COREFOUNDATION_LIBRARY})
endif()
if(WIN32)
    target_link_libraries(Network_Service_Discovery_Lib_Static ${BONJOUR_LIBRARY})
    target_link_libraries(Network_Service_Discovery_Lib_Static ${WSOCK32_LIBRARY})
    target_link_libraries(Network_Service_Discovery_Lib_Static ${WS2_32_LIBRARY})
endif()
if(CYGWIN)
    target_link_libraries(Network_Service_Discovery_Lib_Static ${BONJOUR_LIBRARY})
endif()

### MAKING EXECUTABLE ###
# current project executable creation
add_executable(Network_Service_Discovery src/main.c ${TEST_SOURCE_FILES})
set_target_properties(Network_Service_Discovery PROPERTIES OUTPUT_NAME nsd)
target_link_libraries(Network_Service_Discovery Network_Service_Discovery_Lib)