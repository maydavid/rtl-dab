load zPRS.mat
load prs_opendab.mat
a = fopen('../222055_dump.dump');
tshift = 0;
for i = 1:100

b = fread(a,196608*2+tshift*2,'uint8');



%b = fread(a,196608*4,'uint8');
%b = fread(a,196608*4,'uint8');
%b = fread(a,196608*4,'uint8');
%b = fread(a,196608*4,'uint8');
%b = fread(a,196608*4,'uint8');
%b = fread(a,196608*4,'uint8');

c = complex(b(1:2:end)-127,b(2:2:end)-127);
prs_rec = c(95700:95701+2552);
prs_rec = prs_rec(505:505+2047);
%plot(real(prs_rec))


prs_rec_fft = (fft(prs_rec));
%plot(abs(prs_rec_fft))
%plot(real(prs_opendab))

%% opendab prs
%prs_rec_fft = circshift(prs_rec_fft,-1);
cc = ifft((prs_rec_fft).*(prs_opendab));
%plot(abs(cc))

%% zPRS 
sym = prs_rec_fft;
shift  = 0;
sym = [sym(size(sym,1)-(1536/2-1)+shift:size(sym,1),:) ; sym(2:1536/2+1+shift,:) ];
bla =(abs(ifftshift(ifft(sym.*conj(zPRS)))));
%hold on
[d e] = max(bla(568:968));
tshift = -(768-(568+e))
plot(bla)

pause(.1)
end
fclose(a);
