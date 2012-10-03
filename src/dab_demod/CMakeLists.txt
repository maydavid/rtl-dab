add_library(dab_demod dab_demod.c dab_sync.c)
target_link_libraries(dab_demod fftw3)
