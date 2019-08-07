%% Load variables file
clear all , close all;
ntrial = 1:10;
filename = 'Asya';
n = 5;
m = 5;
%%
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
    e_1seg(i-last) = max(x_cursor_1seg) - L1(i,1);
    %compute error for every trial of a K2 on the second segment
    e_2seg(i-last) = max(x_cursor_2seg) - L1(i,1);
    
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