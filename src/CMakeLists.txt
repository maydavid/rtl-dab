add_executable(rtldab rtldab.c)
add_executable(rtldab_test rtldab_test.c)
target_link_libraries(rtldab rtlsdr dab_demod dab_helper_functions dab_fic_parser)
target_link_libraries(rtldab_test rtlsdr dab_demod dab_helper_functions dab_fic_parser)

add_subdirectory(dab_demod)
add_subdirectory(dab_helper)
add_subdirectory(dab_fic_parser)
