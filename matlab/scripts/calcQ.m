% This function compares two voxel intensity volumes to determine reconstruction quality Q
% as defined by Elsinga et al 2006

% Written by Leah Mendelson 4/3/13
% Updated 6/2022

function Q = calcQ(Ivol1,Ivol2)

    numprod = Ivol1.*Ivol2;
    Qn_tot = sum(numprod(:));
    
    denom1 = Ivol1.^2;
    denom2 = Ivol2.^2;
    Qd = sqrt(sum(denom1(:))*sum(denom2(:)));
    Q = Qn_tot/Qd;

end