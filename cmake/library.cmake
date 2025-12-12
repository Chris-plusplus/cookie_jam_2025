include_guard()

add_library(${PROJECT_NAME}_lib)

file(GLOB_RECURSE ${PROJECT_NAME}_LIB_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

if(NOT MSVC)
	list(REMOVE_ITEM ${PROJECT_NAME}_LIB_SOURCES "${PROJECT_SOURCE_DIR}/src/pch.cpp")
endif ()

target_sources(${PROJECT_NAME}_lib PRIVATE ${${PROJECT_NAME}_LIB_SOURCES})

target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_INCLUDE_PATH} ${PROJECT_SOURCE_DIR}/include)
target_precompile_headers(${PROJECT_NAME}_lib PUBLIC "${PROJECT_SOURCE_DIR}/include/pch.h")
target_link_libraries(${PROJECT_NAME}_lib PUBLIC nvrhi_vk archimedes::archimedes)
set_property(TARGET ${PROJECT_NAME}_lib PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
