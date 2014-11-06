add_library(dab_demod dab_demod.c dab_sync.c dab_fic_descramble.c viterbi.c dab_fic_depuncture.c)
target_link_libraries(dab_demod fftw3 m)
