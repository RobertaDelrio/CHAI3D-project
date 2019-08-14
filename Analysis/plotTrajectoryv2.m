%% Load variables file
clear all , close all;
ntrial = 1:100;
filename = 'Roberta';
n = 10;
m = 5;
%%
L = [];
last = 0;
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
    
    L1_d = load('L1.txt');
    j = L1_d(i);
    L(i)= j; 
    l = string(L);
    %figure,plot(x_cursor,y_cursor);
    p1(i) = plot3(y_cursor,z_cursor,x_cursor, 'Linewidth',3);
    hold on;
    xlabel ('x [m]');
    ylabel('y [m]');
    zlabel('z [m]');
    axis equal;
    title(['Cursor Position',' K1: ', num2str(K1(1)),' K2: ',num2str(K2(1))]);
    hold on;
    [x ,y] = meshgrid(-0.06:0.01:0.06); % Generate x and y data
    z = -j*ones(size(x, 1)); % Generate z data
    %surf(x, y, z); % Plot the surface
    alpha 0.5;
    if mod(i,n) == 0
        
        leg = legend(p1(last+1:i),l(last+1:i));
        title(leg,'Jump position'); 
        last = i;
        figure(last);
    end
    
    startpt = plot3(-0.05,-0.0,0.0,'og');
    endpt = plot3(0.05,-0.0,0.0,'or');

end


  % plot3(0.05,-0.0,0.0,'or');
    