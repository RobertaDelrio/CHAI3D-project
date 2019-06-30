%% Load txt file
clear all , close all;
ntrial = 1:4;
filename = 'ro';
%% filter
Fs = 200;  % Sampling Frequency
 
N      = 2;   % Order
Fpass1 = 0.2;  % First Passband Frequency
Fpass2 = 4;    % Second Passband Frequency
Apass  = 1;    % Passband Ripple (dB)
 
% Construct an FDESIGN object and call its CHEBY1 method.
h  = fdesign.bandpass('N,Fp1,Fp2,Ap', N, Fpass1, Fpass2, Apass, Fs);
Hd = design(h, 'cheby1');
L = [];
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
    %figure,plot(x_cursor,y_cursor);
    plot3(y_cursor,z_cursor,x_cursor, 'Linewidth',3);
    hold on;
%     A = 1;
%     B = 1;
%     C = 1;
%     [x1, y1] = meshgrid(-0.09:0.03:0.09); % LEV 1 plane
%     z1 = -1/C*(A*x1+B*y1+1); % Generate z data
%     [x2, y2] = meshgrid(-0.09:0.03:0.09); % LEV 1 plane
%     z2 = zeros(size(x2, 1)); % Generate z data
%     mesh(x2,y2,z2);
%     %surf(x2, y2, z2, 'EdgeColor', 'none');
%     hold on;
%     mesh(x1,y1,z1);
%     %surf(x1, y1, z1);
    xlabel ('x');
    ylabel('y');
    zlabel('z');
    
    title(['Cursor Position',' K1: ', num2str(K1(1)),' K2: ',num2str(K2(1))]);
   
    L1_d = load('L1.txt');
    j = L1_d(i);
    L(i)= j; 
    l = string(L);
    leg = legend(l);
    title(leg,'Jump position'); 
    hold on;
end

   
    