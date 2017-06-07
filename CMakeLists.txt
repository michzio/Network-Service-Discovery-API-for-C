cmake_minimum_required(VERSION 3.7)
project(Network_Service_Discovery)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c99")
set(SOURCE_FILES network_service_discovery.h network_service_discovery.c unix/event_loop.h unix/event_loop.c apple/event_loop.h apple/event_loop.c windows/event_loop.h windows/event_loop.c)
set(TEST_SOURCE_FILES )

if(WIN32)
    # windows native
    message("building for Windows")
elseif(CYGWIN)
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

add_executable(Network_Service_Discovery main.c ${TEST_SOURCE_FILES})
set_target_properties(Network_Service_Discovery PROPERTIES OUTPUT_NAME nsd)
target_link_libraries(Network_Service_Discovery Network_Service_Discovery_Lib_Static)

# install targets (executables, shared libraries, static libraries) in current project source directory
add_custom_target(Install_${PROJECT_NAME}
                 $(MAKE) install
                  DEPENDS ${PROJECT_NAME}
                 COMMENT "Installing ${PROJECT_NAME}")

if(WIN32)
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