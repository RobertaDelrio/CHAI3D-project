%% Load variables file
clear all , close all;
ntrial = 1:120;
trialz = 120;
filenames = {'dataLogs/S1/Experiment/data/Inbali','dataLogs/S2/Experiment/data/Eilav','dataLogs/S3/Experiment/data/Anna',...
  'dataLogs/S4/Experiment/data/Max','dataLogs/S5/Experiment/data/Yossi','dataLogs/S6/Experiment/data/Shir',...
  'dataLogs/S7/Experiment/data/Meron','dataLogs/S8/Experiment/data/Dana','dataLogs/S9/Experiment/data/Meitav',...
  'dataLogs/S10/Experiment/data/Yael','dataLogs/S11/Experiment/data/Yogev'};
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
    
    F(i) = K2(1,1)*L1(1,1);
   
    if find ( F(i) == F(1:i-1))
        k(j) = find ( F(i) == F(1:i-1));
        I(j) = i;
        j=j+1;
    end
    
    e_1seg(i) = abs(min(x_cursor_1seg) + L1(1,1));
    %compute error for every trial of a K2 on the second segment
    e_2seg(i) = abs(min(x_cursor_2seg) + L1(1,1));
    
    end
    
    if K1(1,1) == 110
        F_u1 = F(1:trialz/2);
        F_u2 = F((trialz/2)+1:trialz);
        
        e_1seg_u1 = e_1seg(1:trialz/2);
        e_1seg_u2 = e_1seg((trialz/2)+1:trialz);
        
        e_2seg_u1 = e_2seg(1:trialz/2);
        e_2seg_u2 = e_2seg((trialz/2)+1:trialz);
    else
        F_u1 = F((trialz/2)+1:trialz);
        F_u2 = F(1:trialz/2);
        
        e_1seg_u1 = e_1seg((trialz/2)+1:trialz);
        e_1seg_u2 = e_1seg(1:trialz/2);
        
        e_2seg_u1 = e_2seg((trialz/2)+1:trialz);
        e_2seg_u2 = e_2seg(1:trialz/2);
    end
    
    
    [f_u1, i_u1] = sort(F_u1);
    [f_u2, i_u2] = sort(F_u2);
    
    M_1(k,:) = e_1seg_u1(i_u1);
    M_2(k,:) = e_1seg_u2(i_u2);
    M_3(k,:) = e_2seg_u1(i_u1);
    M_4(k,:) = e_2seg_u2(i_u2);
    
    
    
end

    m_1 = mean(M_1);
    m_2 = mean(M_2);
    m_3 = mean(M_3);
    m_4 = mean(M_4);
    
 %%
 x1 = unique(f_u1);
 x2 = unique(f_u2);
figure(1),plot(f_u1, m_1);
title('Error first segment K1: 70 N/m across subject');
xlabel('Fz [N]'),ylabel('Error [m]'),xticks(x1);
axis([0 5 0 0.1])
figure(2),plot(f_u2, m_2);
title('Error first segment K1: 110 N/m across subject');
xlabel('Fz [N]'),ylabel('Error [m]'),xticks(x2);
axis([0 5 0 0.1])
% %Max error second segment for both K1
figure(3),plot(f_u1, m_3);
title('Error second segment K1: 70 N/m across subject');
xlabel('Fz [N]'),ylabel('Error [m]'),xticks(x1);
axis([0 5 0 0.1])
figure(4),plot(f_u2, m_4);
title('Error second segment K1: 110 N/m across subject');
xlabel('Fz [N]'),ylabel('Error [m]'),xticks(x2);
axis([0 5 0 0.1])


