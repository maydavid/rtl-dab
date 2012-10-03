add_executable(rtldab rtldab.c)
target_link_libraries(rtldab rtlsdr dab_demod dab_helper_functions)


add_subdirectory(dab_demod)
add_subdirectory(dab_helper)
