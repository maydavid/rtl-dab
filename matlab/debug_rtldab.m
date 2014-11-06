clear all
load zPRS.mat
n= 2;
m = 3;
a = importdata('../prs_received.dat');
prs_time = complex(a(1:2:end),a(2:2:end));
subplot(n,m,1)
plot(abs((prs_time)))


a = importdata('../prs_received_fft.dat');
prs_freq = complex(a(1:2:end),a(2:2:end));
subplot(n,m,2)
plot(abs((prs_freq)))


a = importdata('../prs_star.dat');
prs_star = complex(a(1:2:end),a(2:2:end));
subplot(n,m,3)
plot(((prs_star)))

subplot(n,m,4)
plot(((zPRS)))


a = importdata('../prs_rec_shift.dat');
prs_rec_shift = complex(a(1:2:end),a(2:2:end));
subplot(n,m,5)
plot((abs(prs_rec_shift)))

a = importdata('../convoluted_prs_time.dat');
convoluted_prs_time = complex(a(1:2:end),a(2:2:end));
subplot(n,m,6)
plot((abs(convoluted_prs_time)))
