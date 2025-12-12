include_guard()

file(GLOB_RECURSE ${PROJECT_NAME}_TARGETS_CMAKE cmake/targets/*.cmake)
foreach (${PROJECT_NAME}_TARGET_CMAKE ${${PROJECT_NAME}_TARGETS_CMAKE})
	include(${${PROJECT_NAME}_TARGET_CMAKE})
endforeach ()

LaunchMake()