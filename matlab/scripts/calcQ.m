% This function compares two voxel intensity volumes to determine reconstruction quality Q
% as defined by Elsinga et al 2006

% Written by Leah Mendelson 4/3/13
% 
% direc1 = 'I:\Leah\Processed_7_30\refocusing_data\to_refocus\LD2_8psi_Run1_preprocessed\img0020\refocused';
% direc2 = 'I:\Leah\Processed_7_30\refocusing_data\orig_compare\19';
% 
% imnames1 = dir([direc1 '/*.tif']);
% imnames1 = {imnames1.name};
% 
% imnames1 = sort_nat(imnames1);
% 
% imnames2 = dir([direc2 '/*.tif']);
% imnames2 = {imnames2.name};
% 
% imnames2 = sort_nat(imnames2);
% 
% im1 = zeros(800,1280,length(imnames1));
% im2 = im1;
% 
% for i = 1:length(imnames2);
%     itemp = imread([direc1 '/' imnames1{i}]);
%     itemp = im2double(itemp);
%     im1(:,:,i)=itemp;
% end
% 
% for i = 1:length(imnames2);
%     itemp = imread([direc2 '/' imnames2{i}]);
%     itemp = im2double(itemp);
%     im2(:,:,i)=itemp;
% end

function Q = calcQ(Ivol1,Ivol2)

Qn_tot = 0;
Qd1_tot = 0;
Qd2_tot = 0;

for i = 1:length(imnames1)
    Qnum = im1(:,:,i).*im2(:,:,i);
    Qnum = sum(sum(Qnum));
    Qn_tot = Qn_tot+Qnum;
    
    Qd1 = im1(:,:,i).^2;
    Qd1 = sum(sum(Qd1));
    Qd1_tot = Qd1_tot+Qd1;
    
    Qd2 = im2(:,:,i).^2;
    Qd2 = sum(sum(Qd2));
    Qd2_tot = Qd2_tot+Qd2;
      
end

Qd = sqrt(Qd1_tot*Qd2_tot);
Q = Qn_tot/Qd;

end