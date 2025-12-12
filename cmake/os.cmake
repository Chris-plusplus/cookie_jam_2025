# OS handling file

include_guard()

# Check for OS
if(WIN32)
    set(${PROJECT_NAME}_WINDOWS TRUE)
    set(${PROJECT_NAME}_LINUX FALSE)

    message(STATUS "OS: Windows")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(${PROJECT_NAME}_WINDOWS FALSE)
    set(${PROJECT_NAME}_LINUX TRUE)

    message(STATUS "OS: Linux")
endif()
