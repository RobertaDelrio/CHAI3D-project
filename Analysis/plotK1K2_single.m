%% Load variables file
clear all , close all;
ntrial = 1:120;
filename = 'dataLogs/S1/Experiment/data/Inbali';
n = 10;
m = 6;

%% Plot 3d for every K1,K2 plot the 10 jump
last = 0;
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
    
    
    l{i} = num2str(L1(1,1));
    p1(i) = plot3(y_cursor,z_cursor,x_cursor, 'Linewidth',3);
    hold on;
    xlabel ('x [m]'),ylabel('y [m]'),zlabel('z [m]'),axis equal;
    title(['Cursor Position',' K1: ', num2str(K1(1)),' K2: ',num2str(K2(1))]);
    hold on;
    if mod(i,n) == 0
        leg = legend(p1(last+1:i),l(last+1:i));
        title(leg,'Jump position');
        last = i;
        figure(last);

    end
    
    
    

end





%subplot(2,5,2), imshow('20fig.fig');
  % plot3(0.05,-0.0,0.0,'or');