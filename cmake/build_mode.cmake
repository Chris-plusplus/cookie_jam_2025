# Build type handling

include_guard()

# Check build type
if(
    "${CMAKE_CONFIGURATION_TYPES}" STREQUAL Debug OR
    "${CMAKE_BUILD_TYPE}" STREQUAL Debug
)
    set(${PROJECT_NAME}_DEBUG TRUE)
    set(${PROJECT_NAME}_RELEASE FALSE)

    set(${PROJECT_NAME}_BUILD_TYPE "Debug")
elseif(
    "${CMAKE_CONFIGURATION_TYPES}" STREQUAL "RelWithDebInfo" OR
    "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo"
)
    set(${PROJECT_NAME}_DEBUG FALSE)
    set(${PROJECT_NAME}_RELEASE TRUE)
    
    set(${PROJECT_NAME}_BUILD_TYPE "RelWithDebInfo")
else(
	# Release or MinSizeRel
)  
    set(${PROJECT_NAME}_DEBUG FALSE)
    set(${PROJECT_NAME}_RELEASE TRUE)
    
    set(${PROJECT_NAME}_BUILD_TYPE "Release")
endif()

message(STATUS "Build mode: ${${PROJECT_NAME}_BUILD_TYPE}")

set(CMAKE_BUILD_TYPE ${${PROJECT_NAME}_BUILD_TYPE})
