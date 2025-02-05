function [f,df_dr1,df_dr2,g,dg_dr1,dg_dr2] = f_eval_2eq(r1,r2,r3,z1,z2,z3,n1,n2,n3)

f      = (r1)./sqrt(r1.^2 + z1.^2) - (n2/n1)*(r2-r1)./sqrt((r2-r1).^2 + z2.^2);

df_dr1 = 1./sqrt(r1.^2 + z1.^2) - r1.^2./(r1.^2 + z1.^2).^(3/2) + (n2/n1)./sqrt(z2.^2 + (r1-r2).^2)...
    - (n2/n1).*(r1-r2).*(2*r1-2*r2)./(2*((r1-r2).^2 + z2.^2).^(3/2));

df_dr2 = (n2/n1).*(r1-r2).*(2*r1-2*r2)./(2*((r1-r2).^2 + z2.^2).^(3/2)) - (n2/n1)./sqrt(z2.^2 + (r1-r2).^2);


g      = (r2-r1)./sqrt((r2-r1).^2 + z2.^2) - (n3/n2)*(r3-r2)./sqrt((r3-r2).^2 + z3.^2);

dg_dr1 = (r1-r2).*(2*r1-2*r2)./(2*((r1-r2).^2 + z2.^2).^(3/2)) - 1./sqrt(z2.^2 + (r1-r2).^2);

dg_dr2 = 1./sqrt((r1-r2).^2 + z2.^2) + (n3/n2)./sqrt(z3.^2 + (r2-r3).^2)...
    - (r1-r2).*(2*r1-2*r2)./(2*((r1-r2).^2 + z2.^2).^(3/2))...
    - (n3/n2).*(r2-r3).*(2*r2-2*r3)./(2*((r2-r3).^2 + z3.^2).^(3/2));

end


