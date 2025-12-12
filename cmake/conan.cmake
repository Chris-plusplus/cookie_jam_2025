# Conan handling

include_guard()

set(${PROJECT_NAME}_CONAN_FILES "${PROJECT_SOURCE_DIR}/conan_files/${${PROJECT_NAME}_BUILD_TYPE}/build/generators")

set(${PROJECT_NAME}_CONANFILE "${PROJECT_SOURCE_DIR}/conanfile.py")
set(${PROJECT_NAME}_CONANFILE_HASH "${${PROJECT_NAME}_CONAN_FILES}/${${PROJECT_NAME}_BUILD_TYPE}/conanfile.hash")

# Check for changes in conanfile
file(SHA256 "${${PROJECT_NAME}_CONANFILE}" ${PROJECT_NAME}_CONANFILE_HASH_NEW_VALUE) # compute hash
if(EXISTS ${${PROJECT_NAME}_CONANFILE_HASH})
	set(${PROJECT_NAME}_CONAN_INSTALLED TRUE)

	# get hash value from file
	file(STRINGS ${${PROJECT_NAME}_CONANFILE_HASH} ${PROJECT_NAME}_CONAN_INSTALL_HASH_CONTENTS LIMIT_COUNT 1)
	list(GET ${PROJECT_NAME}_CONAN_INSTALL_HASH_CONTENTS 0 ${PROJECT_NAME}_CONANFILE_HASH_OLD_VALUE)

	if(${${PROJECT_NAME}_CONANFILE_HASH_OLD_VALUE} STREQUAL ${${PROJECT_NAME}_CONANFILE_HASH_NEW_VALUE})
		set(${PROJECT_NAME}_CONANFILE_HASH_EQ TRUE)
		set(${PROJECT_NAME}_CONAN_INSTALL FALSE)
	else()
		set(${PROJECT_NAME}_CONANFILE_HASH_EQ FALSE)
		set(${PROJECT_NAME}_CONAN_INSTALL TRUE)
	endif()
else() # conanfile was not installed
	set(${PROJECT_NAME}_CONAN_INSTALLED FALSE)
	set(${PROJECT_NAME}_CONAN_INSTALL TRUE)
endif()
if(${PROJECT_NAME}_FORCE_CONAN_INSTALL)
	set(${PROJECT_NAME}_CONAN_INSTALL TRUE)
endif()

message("Conan installation status:")
if(${PROJECT_NAME}_CONAN_INSTALL)
	if(${PROJECT_NAME}_FORCE_CONAN_INSTALL)
		message("    forced")
	elseif(NOT ${PROJECT_NAME}_CONANFILE_HASH_EQ)
		message("    conanfile changed")
	elseif(NOT ${PROJECT_NAME}_CONAN_INSTALLED)
		message("    not installed")
	endif()

	# check for conan
	execute_process(COMMAND
		conan version
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_VARIABLE ${PROJECT_NAME}_CONAN_OUTPUT
	)
	# match with expected output
	if(NOT ${PROJECT_NAME}_CONAN_OUTPUT MATCHES "^version:[^\n]*\nconan_path:[^\n]*")
		message(FATAL_ERROR "Conan not found")
	else()
		message(STATUS "Conan found")
	endif()

	# set environment variables
	#set(ENV{CC} ${CMAKE_C_COMPILER})
	#set(ENV{CXX} ${CMAKE_CXX_COMPILER})

	# detect conan profile
	#execute_process(
	#	COMMAND conan profile detect --force
	#	COMMAND_ERROR_IS_FATAL ANY
	#)

	file(MAKE_DIRECTORY ${${PROJECT_NAME}_CONAN_FILES})

	# MSVC-only setting
	if(MSVC)
		set(${PROJECT_NAME}_CONAN_COMPILER_RUNTIME_TYPE "-s:b compiler.runtime_type=${CMAKE_BUILD_TYPE} -s:h compiler.runtime_type=${CMAKE_BUILD_TYPE}")
	endif()

	# Set package manager settings for Linux
	if(${${PROJECT_NAME}_LINUX})
		set(${PROJECT_NAME}_CONAN_TOOLS_SYSTEM_PACKAGE_MANAGER_MODE "-c tools.system.package_manager:mode=install")
		set(${PROJECT_NAME}_CONAN_TOOLS_SYSTEM_PACKAGE_MANAGER_SUDO "-c tools.system.package_manager:sudo=True")
	endif()

	# Parse additional conan args
	separate_arguments(${PROJECT_NAME}_CONAN_ARGS_LIST UNIX_COMMAND "${${PROJECT_NAME}_CONAN_ARGS}")

	# run the install command
	execute_process(
		COMMAND conan install ./conanfile.py -s:b build_type=${${PROJECT_NAME}_BUILD_TYPE} -s:b compiler.cppstd=${CMAKE_CXX_STANDARD} -s:h build_type=${${PROJECT_NAME}_BUILD_TYPE} -s:h compiler.cppstd=${CMAKE_CXX_STANDARD} ${${PROJECT_NAME}_CONAN_INSTALL_RUNTIME_TYPE} ${${PROJECT_NAME}_CONAN_TOOLS_SYSTEM_PACKAGE_MANAGER_SUDO} ${${PROJECT_NAME}_CONAN_TOOLS_SYSTEM_PACKAGE_MANAGER_MODE} -of=conan_files/${${PROJECT_NAME}_BUILD_TYPE} --build=missing -pr default
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_VARIABLE ARCHIEMDES_CONAN_INSTALL_OUTPUT
		RESULT_VARIABLE ARCHIEMDES_CONAN_INSTALL_RESULT
		COMMAND_ECHO STDOUT
	)

	# check installation results
	if(NOT ${ARCHIEMDES_CONAN_INSTALL_RESULT} EQUAL 0)
		file(REMOVE_RECURSE ${${PROJECT_NAME}_CONAN_FILES})
		message(FATAL_ERROR "Conan installation failed")
	else()
		file(WRITE ${${PROJECT_NAME}_CONANFILE_HASH} ${${PROJECT_NAME}_CONANFILE_HASH_NEW_VALUE})
		message("Conan installation completed")
	endif()
else()
	message("\tfound files for ${CMAKE_BUILD_TYPE}")
endif()

# find & include toolchain & conandeps
file(GLOB_RECURSE ${PROJECT_NAME}_TOOLCHAIN_CONANDEPS "${PROJECT_SOURCE_DIR}/conan_files/${${PROJECT_NAME}_BUILD_TYPE}/*/conan_toolchain.cmake" "${PROJECT_SOURCE_DIR}/conan_files/${${PROJECT_NAME}_BUILD_TYPE}/*/conandeps_legacy.cmake")
foreach(${PROJECT_NAME}_TOOLCHAIN_CONANDEPS_FILE ${${PROJECT_NAME}_TOOLCHAIN_CONANDEPS})
	include(${${PROJECT_NAME}_TOOLCHAIN_CONANDEPS_FILE})
endforeach()

# Add conan libraries to archimedes
list(APPEND ${PROJECT_NAME}_LIBRARIES ${CONANDEPS_LEGACY})
