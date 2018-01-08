cmake_minimum_required (VERSION 2.8)

# Get the library suffix for lib or lib64.
get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)       
if(LIB64)
   set(LIBSUFFIX 64)
else()
   set(LIBSUFFIX "")
endif()

set(requiredLibs ${requiredLibs} ossim_msp_plugin  )
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")

add_executable(msp-plugin-test msp-plugin-test.cpp )
set_target_properties(msp-plugin-test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
target_link_libraries( msp-plugin-test ${requiredLibs} )

add_executable(msp-test msp-test.cpp )
set_target_properties(msp-test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
target_link_libraries( msp-test ${requiredLibs} )

add_executable(msp-ss-test msp-ss-test.cpp )
set_target_properties(msp-ss-test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
target_link_libraries( msp-ss-test ${requiredLibs} )

add_executable(photoblock-json-test photoblock-json-test.cpp )
set_target_properties(photoblock-json-test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
target_link_libraries( photoblock-json-test ${requiredLibs} )

