add_library(dab_demod dab_demod.c dab_sync.c opendab_prsgen.c dab_fic_descramble.c)
target_link_libraries(dab_demod fftw3 m)
