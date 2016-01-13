forte_add_module(OPC_UA "OCP Unified Architecture Function Blocks")

#############################################################################
# OPC UA FB
#############################################################################
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_sourcefile_hcpp(OPC_UA_READ)
forte_add_sourcefile_hcpp(OPC_UA_WRITE)
forte_add_sourcefile_hcpp(OPC_UA_PUBLISH)
forte_add_sourcefile_hcpp(OPC_UA_SUBSCRIBE)



if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
    	forte_add_link_library( libmodbus.so )
    	forte_add_include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
    	forte_add_link_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
elseif("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
	forte_add_link_library( libopen62541.dll )
    	forte_add_include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
    	forte_add_link_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
endif()