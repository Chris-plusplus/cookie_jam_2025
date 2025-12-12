include_guard()

add_executable(main "${PROJECT_SOURCE_DIR}/targets/Main.cpp")
target_link_libraries(main PRIVATE ${PROJECT_NAME}_lib)
target_precompile_headers(main REUSE_FROM ${PROJECT_NAME}_lib)
#target_include_directories(main PRIVATE ${CMAKE_INCLUDE_PATH})
set_property(TARGET main PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

LaunchAddTarget(main)