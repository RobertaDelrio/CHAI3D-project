%% Load variables file
clear all , close all;
ntrial = 1:100;
filenames =  {'Asya', 'Roberta'};
n = 10;
m = 5;
%%
for k=1:length(filenames)
    filename=filenames{k}
    last = 0;
    last = 0;
    j=1;
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
    
    %L1_d = load('L1.txt');
    %j = L1_d(i);
    %L(i)= j; 
    %l = string(L);
    l{i} = num2str(L1(1,1));
    figure(k),subplot(2,5,j)
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
    %z = -j*ones(size(x, 1)); % Generate z data
    %surf(x, y, z); % Plot the surface
    alpha 0.5;
    if mod(i,n) == 0
         
        leg = legend(p1(last+1:i),l(last+1:i));
        title(leg,'Jump position');
       
        last = i;
        
        %figure(last);
       
        j=j+1;
%         fi = figure(last);
%         string = strcat(num2str(i),'fig');
%         saveas(fi, string);
    end
    
    %startpt = plot3(-0.05,-0.0,0.0,'og');
    %endpt = plot3(0.05,-0.0,0.0,'or');

end
end





%subplot(2,5,2), imshow('20fig.fig');
  % plot3(0.05,-0.0,0.0,'or');
    