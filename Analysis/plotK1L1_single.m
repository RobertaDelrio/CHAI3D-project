%% Load variables file
clear all , close all;
ntrial = 1:120;
filename = 'dataLogs/S5/Experiment/data/Yossi';
n = 10;
m = 6;
%% Plot 3d for every K1,L1 plot the 6 K2

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
    
    
    jump(i) = L1(1,1);
end

last = 0;
L = unique(jump, 'stable');
for j=1:length(L)
    i = 0;
    k = 1;
    h = 1;
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
        
        figure(j);
        
        if L(j) == L1(1,1)
            
            if (i <= 60)
                f_1 = subplot(1,2,1)
                k_2{k} = num2str(K2(1,1));
                %p1(k) = plot3(y_cursor,z_cursor,x_cursor, 'Linewidth',3);
                p1(k) = plot(y_cursor,x_cursor, 'Linewidth',3);
                hold on;
                %xlabel ('x [m]'),ylabel('y [m]'),zlabel('z [m]'),axis equal;
                xlabel ('x [m]'),ylabel('z [m]'),axis equal;
                title(['Cursor Position',' K1: ', num2str(K1(1)),' L1: ',num2str(L(j))]);
                hold on;
                leg = legend(p1(1:k),k_2(1:k));
                title(leg,'K2 value');
                k = k+1;
            else
                f_2 = subplot(1,2,2)
                    k_2{h} = num2str(K2(1,1));
                    %p2(h) = plot3(y_cursor,z_cursor,x_cursor, 'Linewidth',3);
                    p2(h) = plot(y_cursor,x_cursor, 'Linewidth',3);
                    hold on;
                    %xlabel ('x [m]'),ylabel('y [m]'),zlabel('z [m]'),axisequal;
                    xlabel ('x [m]'),ylabel('z [m]'),axis equal;
                    title(['Cursor Position',' K1: ', num2str(K1(1)),' L1: ',num2str(L(j))]);
                    hold on;
                    legi = legend(p2(1:h),k_2(1:h));
                    title(legi,'K2 value');
                    h = h+1;

            end
        end
    
        
    end
    linkaxes([f_1,f_2],'xy');
    

end
