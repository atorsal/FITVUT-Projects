[s, Fs] = audioread ('xbolva00.wav');

% 1
Fs % Vzorkovacia
N = length(s) % Pocet vzorkov
N/Fs % Dlzka v sekundach

% 2
m = abs(fft(s));
G = 10 * log10(1/N * m.^2);
f = (0:N/2-1)/N * Fs; 
G = G(1:Fs/2);
plot (f,G); 
xlabel('f [Hz]'); 
ylabel ('PSD [dB]');

% 3
[mmx, index] = max(G);
mmx
index

% 4
b = [0.2324 -0.4112 0.2324]; a = [1 0.2289 0.4662];
zplane (b,a);
p = roots(a); 
if (isempty(p) | abs(p) < 1) 
  disp('Stabilny')
else
  disp('Nestabilny')
end

% 5
H = freqz(b,a,Fs/2);
plot (f,abs(H)); 
xlabel('f [Hz]');
ylabel('|H(f)|')

% 5
sf = filter(b,a, s);
f = (0:N/2-1)/N * Fs;
m = abs(fft(sf));
G = 10 * log10(1/N * m.^2);
f = (0:N/2-1)/N * Fs; 
G = G(1:Fs/2);
plot (f,G); 
xlabel('f [Hz]'); 
ylabel ('PSD [dB]');

% 7
[mmx, index] = max(G);
mmx
index

% 8
a = [1 1 1 1 -1 -1 -1 -1];
a = repmat(a, 1,40);
[rcoef, lags] = xcorr(s,a);

[max2, pos] = max(rcoef);
r = lags(pos) 
r/Fs
t = s(r)

% 9
S=50
Rv=xcorr(s,'biased');
k= -S:S;
plot(k, Rv(N-S:N+S));
xlabel('k'); 
ylabel('R[k]');

% 10
R10 = Rv(N+10)

% pre ulohy 11, 12, 13
Y = s;
xmin = min(Y);
xmax = max(Y);
N = 50;
x = linspace(xmin,xmax,N);
L = length(x);
N = length(Y);
h = zeros(L,L);
xcol = x(:);
bigx = repmat(xcol,1,N);
yr = Y(:)';
bigy = repmat(Y,1,L)';
[d,ind1] = min(abs(bigy - bigx));

ind2 = ind1(11:N);

for ii=1:N-10,  
    d1 = ind1(ii);
    d2 = ind2(ii);
    h(d1,d2) = h(d1,d2) + 1;
end

surf = ((x(2)-x(1))^2);
p = h / N / surf;
x = x(:); X1 = repmat(x,1,L);
x = x'; X2 = repmat(x,L,1);
r = sum(sum (X1 .* X2 .* p)) * surf;
check = sum(sum (p)) * surf;

% 11
bar3(p);
set(gca,'XTickLabel',{'-0.6';'-0.2';'0.2';'0.6';'1'})
set(gca,'YTickLabel',{'-1';'-0.6';'-0.2';'0.2';'0.6';'1'})

% 12 integral
check

% 13
r % r10