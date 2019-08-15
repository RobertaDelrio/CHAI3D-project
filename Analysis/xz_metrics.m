%% Load variables file
clear all , close all;
ntrial = 1:100;
filenames = {'Zack'};
numsub = length(filenames);
n = 10;
m = 5;
%%

for k=1:numsub
    filename=filenames{k}
    last = 0;
    j = 1;
for i=1:length(ntrial)
    trial=ntrial(i);
    data=importdata([filename '_trial_' num2str(trial) '.txt']);
    %data = importdata('a_trial_3.txt');
    time = data.data(:,1);
    x_cursor = smoothdata(data.data(:,2));
    y_cursor = smoothdata(data.data(:,3));
    z_cursor = smoothdata(data.data(:,4));
    K1 = data.data(:,5);
    K2 = data.data(:,6);
    L1 = data.data(:,7);
    K2_val(i) = K2(1,1);
    index_1seg = y_cursor < -0.03;
    x_cursor_1seg = x_cursor(index_1seg);
    z_cursor_1seg = z_cursor(index_1seg);
    y_cursor_1seg = y_cursor(index_1seg);
    
    index_2seg = y_cursor > -0.03;
    x_cursor_2seg = x_cursor(index_2seg);
    z_cursor_2seg = z_cursor(index_2seg);
    y_cursor_2seg = y_cursor(index_2seg);
    
    %% Compute error in the first segment and second segment
    
    %figure,plot3(y_cursor_1seg,z_cursor_1seg,x_cursor_1seg);
    %axis equal
    %compute error for every trial of a K2 on the first segment
    e_1seg(i-last) = abs(min(x_cursor_1seg) + L1(1,1));
    %compute error for every trial of a K2 on the second segment
    e_2seg(i-last) = abs(min(x_cursor_2seg) + L1(1,1));
    
    if mod(i,n) == 0
       %firsts segment
       avg_e_1seg = mean(e_1seg);
       max_e_1seg = max(e_1seg);
       %mean error and average error for every K2
       AVG_e_1seg(j) = avg_e_1seg;
       MAX_e_1seg(j) = max_e_1seg;
       
       %second segment
       avg_e_2seg = mean(e_2seg);
       max_e_2seg = max(e_2seg);
       %mean error and average error for every K2
       AVG_e_2seg(j) = avg_e_2seg;
       MAX_e_2seg(j) = max_e_2seg;
       
       e_1seg = [];
       e_2seg = [];
       j = j +1;
       last = i;
    end
    
    
    
    
end
K2_val_u = unique(K2_val, 'stable');
[~,n] = size(K2_val_u);

%TO FIX
if K1(1,1) == 70
    
    K2_val_u1 = K2_val_u(1:n/2);
    K2_val_u2 = K2_val_u((n/2)+1:n);

else
    
end
    
    
%Save on a matrix the following data for every subject
%name of the subject
%k2 values for first K1
%k2 values for second K1
%Max error first segment for both K1
%Max error second segment for both K1
%Average error first segment for both K1
%Average error second segment for both K1

%Max error first segment for both K1
figure(1),subplot(1,numsub,k)
bar(K2_val_u1, MAX_e_1seg(1:n/2));
title('Max error first segment K1: 70 N/m');
xlabel('K2 [N/m]');
ylabel('Error [m]');
figure(2),subplot(1,numsub,k)
bar(K2_val_u2, MAX_e_1seg((n/2)+1:n));
title('Max error first segment K1: 110 N/m');
xlabel('K2[N/m]');
ylabel('Error [m]');
%Max error second segment for both K1
figure(3),subplot(1,numsub,k)
bar(K2_val_u1, MAX_e_2seg(1:n/2));
title('Max error second segment K1: 70 N/m');
xlabel('K2 [N/m]');
ylabel('Error [m]');
figure(4),subplot(1,numsub,k)
bar(K2_val_u2, MAX_e_2seg((n/2)+1:n));
title('Max error second segment K1: 110 N/m');
xlabel('K2[N/m]');
ylabel('Error [m]');
%Average error first segment for both K1
figure(5),subplot(1,numsub,k)
bar(K2_val_u1, AVG_e_1seg(1:n/2));
title('Mean error first segment K1: 70 N/m');
xlabel('K2 [N/m]');
ylabel('Error [m]');
figure(6),subplot(1,numsub,k)
bar(K2_val_u2, AVG_e_1seg((n/2)+1:n));
title('Mean error first segment K1: 110 N/m');
xlabel('K2[N/m]');
ylabel('Error [m]');
%Average error second segment for both K1
figure(7),subplot(1,numsub,k)
bar(K2_val_u1, AVG_e_2seg(1:n/2));
title('Mean error second segment K1: 70 N/m');
xlabel('K2 [N/m]');
ylabel('Error [m]');
figure(8),subplot(1,numsub,k)
bar(K2_val_u2, AVG_e_2seg((n/2)+1:n));
title('Mean error second segment K1: 110 N/m');
xlabel('K2[N/m]');
ylabel('Error [m]');
end