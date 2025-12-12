# MSVC specific behavior

include_guard()

if(MSVC)
    # Conform to C++ standard
    add_compile_options("/permissive-")
    # Conform to C++ standard (preprocessor)
    add_compile_options("/Zc:preprocessor")
    # Set the __cplusplus macro
    add_compile_options("/Zc:__cplusplus")
	
	# Enable more aggressive inlining in RelWithDebInfo, the /Ob1 setting is legacy yet still the default
	string(REPLACE "/Ob1" "/Ob2" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()
