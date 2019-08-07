

%Definition of K1 & experiment numbers
array = [70,120]
[~,N] = size(array)
m = 5;
n = 10;
p = 18;
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
    gap = (1/p)*new(i)
    val = new(i);
    for j= 1:m
        val = val-gap;
        k(j) = val
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
jump = [0.25, 0.30, 0.35, 0.4, 0.45, 0.47, 0.5, 0.55, 0.57, 0.6];
fileL1= fopen('L1.txt','w');
for i = 1:m*N
    fileL1= fopen('L1.txt','a');
    index = randperm(10,10);
    j =jump(index);
    fprintf(fileL1, '%f\n', j);
end
fclose(fileL1);

