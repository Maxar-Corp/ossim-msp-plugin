cmake_minimum_required (VERSION 2.8)

# Get the library suffix for lib or lib64.
get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)       
if(LIB64)
   set(LIBSUFFIX 64)
else()
   set(LIBSUFFIX "")
endif()

SET(LIB_NAME ossim_msp_plugin)

FILE(GLOB OSSIM_MSP_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}  
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/services/*.h"    
     "${CMAKE_CURRENT_SOURCE_DIR}/common/*.h"    
     "${CMAKE_CURRENT_SOURCE_DIR}/plugin/*.h")     
     
       
FILE(GLOB OSSIM_MSP_SRCS  RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} 
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/services/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/plugin/*.cpp")     

IF(APPLE)
  FOREACH(f  ${OSSIM_MSP_HEADERS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers )
  ENDFOREACH(f)
ENDIF(APPLE)

# Adjust the install library path:
if(OSSIM_INSTALL_PLUGINS_WITH_VERSION)
   set(INSTALL_LIBRARY_DIR   "${INSTALL_LIBRARY_DIR}/ossim-${OSSIM_VERSION}/plugins")
else(OSSIM_INSTALL_PLUGINS_WITH_VERSION)
   set(INSTALL_LIBRARY_DIR   "${INSTALL_LIBRARY_DIR}/ossim/plugins")
endif(OSSIM_INSTALL_PLUGINS_WITH_VERSION)

OSSIM_LINK_LIBRARY(${LIB_NAME}
                   COMPONENT_NAME ossim TYPE "${OSSIM_PLUGIN_LINK_TYPE}"
                   LIBRARIES ${requiredLibs}
                   HEADERS "${OSSIM_MSP_HEADERS}" 
                   SOURCE_FILES "${OSSIM_MSP_SRCS}"
                   INSTALL_LIB)

