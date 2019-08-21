

%Definition of K1 & experiment numbers
array = [70,110]
[~,N] = size(array)
m = 6;
n = 10;
p = 15;
% Randomization of the blocks 
index = randperm(N,N);
new = array(index);
ne = repelem(new,m*n)

%Creation of the K1.txt file
fileK1= fopen('K1.txt','w');
fprintf(fileK1, '%d\n', ne);
fclose(fileK1);

f = [];
for i = 1:N
    gap = 0;
    val = new(i);
    for j= 1:m
        val = val-gap;
        k(j) = val
        gap = (p/100)*new(i);
    end
    index = randperm(m,m);
    newk = k(index);
    f = [f,newk];
    size(f)
    
end
fe = repelem(f,n)
%Creation of the K2.txt file
fileK2= fopen('K2.txt','w');
fprintf(fileK2, '%f\n', fe);
fclose(fileK2);

%% Creation of the L1.txt file
%l = randi([25 55],N*m*n,1)/1000;
jump = [0.025, 0.030, 0.035, 0.04, 0.045, 0.047, 0.05, 0.055, 0.057, 0.06];
fileL1= fopen('L1.txt','w');
J=[];
for i = 1:m*N
    fileL1= fopen('L1.txt','a');
    index = randperm(10,10);
    j =jump(index);
    fprintf(fileL1, '%f\n', j);
    J = [J,jump];
end

fclose(fileL1);

F = fe.*J;
[C,IA,IC] = unique(F(1:60))
T = table(ne',fe',J',F','VariableNames', {'K1','K2','L1','Force'});
save('protocol_14.mat', 'T')