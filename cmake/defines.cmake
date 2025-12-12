# Macro definiions

include_guard()

add_compile_definitions(SPDLOG_USE_STD_FORMAT=1)

# Set version
add_compile_definitions(${PROJECT_NAME}_VERSION_MAJOR=${${PROJECT_NAME}_VERSION_MAJOR})
add_compile_definitions(${PROJECT_NAME}_VERSION_MINOR=${${PROJECT_NAME}_VERSION_MINOR})
add_compile_definitions(${PROJECT_NAME}_VERSION_PATCH=${${PROJECT_NAME}_VERSION_PATCH})

# Add macros
if(${PROJECT_NAME}_DEBUG)
	add_compile_definitions(
		${PROJECT_NAME}_DEBUG=1
		${PROJECT_NAME}_RELEASE=0
	)
else()
	add_compile_definitions(
		${PROJECT_NAME}_DEBUG=0
		${PROJECT_NAME}_RELEASE=1
	)
endif()

# Add macros
if(${PROJECT_NAME}_WINDOWS)
	add_compile_definitions(
		${PROJECT_NAME}_WINDOWS=1
		${PROJECT_NAME}_LINUX=0
		${PROJECT_NAME}_UNIX=0
	)
elseif(${PROJECT_NAME}_LINUX)
	add_compile_definitions(
		${PROJECT_NAME}_WINDOWS=0
		${PROJECT_NAME}_LINUX=1
		${PROJECT_NAME}_UNIX=1
	)
endif()