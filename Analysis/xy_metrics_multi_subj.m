%% Load variables file
clear all , close all;
ntrial = 1:120;
filenames = {'dataLogs/S1/Experiment/data/Inbali','dataLogs/S2/Experiment/data/Eilav',...
   'dataLogs/S4/Experiment/data/Max','dataLogs/S5/Experiment/data/Yossi','dataLogs/S6/Experiment/data/Shir',...
   'dataLogs/S7/Experiment/data/Meron','dataLogs/S8/Experiment/data/Dana','dataLogs/S9/Experiment/data/Meitav'};
%filenames = { 'dataLogs/S1/Experiment/data/Inbali'};
numsub = length(filenames);
n = 10;
m = 6;
%%

for k=1:numsub
    filename = filenames{k};
    last = 0;
    j = 1;
for i=1:length(ntrial)
    trial=ntrial(i);
    data=importdata([filename '_trial_' num2str(trial) '.txt']);
    
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
K2_val_u_s = sort(K2_val_u);
%TO FIX
K1(1,1)
if K1(1,1) == 110
    
    K2_val_u1 = K2_val_u(1:n/2);
    K2_val_u2 = K2_val_u((n/2)+1:n);
    
    MAX_e_1seg_u1 = MAX_e_1seg(1:n/2);
    MAX_e_1seg_u2 = MAX_e_1seg((n/2)+1:n);
    
    MAX_e_2seg_u1 = MAX_e_2seg(1:n/2);
    MAX_e_2seg_u2 = MAX_e_2seg((n/2)+1:n);
    
    AVG_e_1seg_u1 = AVG_e_1seg(1:n/2);
    AVG_e_1seg_u2 = AVG_e_1seg((n/2)+1:n);
    
    AVG_e_2seg_u1 = AVG_e_2seg(1:n/2);
    AVG_e_2seg_u2 = AVG_e_2seg((n/2)+1:n);
else
    K2_val_u1 = K2_val_u((n/2)+1:n);
    K2_val_u2 = K2_val_u(1:n/2);
    
    MAX_e_1seg_u1 = MAX_e_1seg((n/2)+1:n);
    MAX_e_1seg_u2 = MAX_e_1seg(1:n/2);
    
    MAX_e_2seg_u1 = MAX_e_2seg((n/2)+1:n);
    MAX_e_2seg_u2 = MAX_e_2seg(1:n/2);
    
    AVG_e_1seg_u1 = AVG_e_1seg((n/2)+1:n);
    AVG_e_1seg_u2 = AVG_e_1seg(1:n/2);
    
    AVG_e_2seg_u1 = AVG_e_2seg((n/2)+1:n);
    AVG_e_2seg_u2 = AVG_e_2seg(1:n/2);
    
    
end
   K2_val_u1_s = sort(K2_val_u1);
   K2_val_u2_s = sort(K2_val_u2); 
    
%Save on a matrix the following data for every subject
%name of the subject
%k2 values for first K1
%k2 values for second K1
%Max error first segment for both K1
%Max error second segment for both K1
%Average error first segment for both K1
%Average error second segment for both K1

%Max error first segment for both K1
figure(1),title('Max error first segment K1: 70 N/m');
%f_1(k) = subplot(1,numsub,k);
plot(K2_val_u1, MAX_e_1seg_u1, 'o');
xlabel('K2 [N/m]');
ylabel('Error [m]');
xticks(K2_val_u1_s);
%linkaxes(f_1,'xy'); 
hold on;
figure(2),title('Max error first segment K1: 110 N/m');
% f_2(k) = subplot(1,numsub,k)
plot(K2_val_u2, MAX_e_1seg_u2,'*');
xlabel('K2[N/m]');
ylabel('Error [m]');
xticks(K2_val_u2_s);
hold on;
% linkaxes(f_2,'xy'); 
% %Max error second segment for both K1
figure(3),title('Max error second segment K1: 70 N/m');
% f_3(k) = subplot(1,numsub,k)
plot(K2_val_u1, MAX_e_2seg_u1,'+');
 xlabel('K2 [N/m]');
 ylabel('Error [m]');
 xticks(K2_val_u1_s);
 hold on;
% linkaxes(f_3,'xy'); 
 figure(4),title('Max error second segment K1: 110 N/m');
% f_4(k) = subplot(1,numsub,k);
plot(K2_val_u2, MAX_e_2seg_u2,'d');
 xlabel('K2[N/m]');
 ylabel('Error [m]');
 xticks(K2_val_u2_s);
 hold on;
% linkaxes(f_4,'xy');
% %Average error first segment for both K1
 figure(5),title('Mean error first segment K1: 70 N/m');
% f_5(k) = subplot(1,numsub,k)
plot(K2_val_u1,  AVG_e_1seg_u1,'s');
xlabel('K2 [N/m]');
ylabel('Error [m]');
xticks(K2_val_u1_s);
% linkaxes(f_5,'xy');
hold on;
figure(6),title('Mean error first segment K1: 110 N/m');
% f_6(k) = subplot(1,numsub,k)
plot(K2_val_u2,  AVG_e_1seg_u2,'x');
xlabel('K2[N/m]');
ylabel('Error [m]');
xticks(K2_val_u2_s);
% linkaxes(f_6,'xy');
hold on;
% %Average error second segment for both K1
figure(7),title('Mean error second segment K1: 70 N/m');
% f_7(k) = subplot(1,numsub,k)
plot(K2_val_u1,  AVG_e_2seg_u1,'p');
xlabel('K2 [N/m]');
ylabel('Error [m]');
xticks(K2_val_u1_s);
hold on;
% linkaxes(f_7,'xy');
figure(8),title('Mean error second segment K1: 110 N/m');
% f_8(k) = subplot(1,numsub,k)
plot(K2_val_u2,  AVG_e_2seg_u2,'h');
xlabel('K2[N/m]');
ylabel('Error [m]');
xticks(K2_val_u2_s);
hold on;
% linkaxes(f_8,'xy');
end