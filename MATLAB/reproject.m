Xi = 7.243

Yi = -1.596

Zi = -8.998


Xs = 0.247573

Ys = -0.090334

Zs = -0.062171

Phi = 0.525132

Omega = 0.076066

Kappa = 0.031935

f = 3797.414699

x0 = 85.075731

y0 = -42.676485


R = zeros(3, 3);

R(1, 1) = cos(Phi)*cos(Kappa) - sin(Phi)*sin(Omega)*sin(Kappa);
R(1, 2) = cos(Omega)*sin(Kappa);
R(1, 3) = sin(Phi)*cos(Kappa) + cos(Phi)*sin(Omega)*sin(Kappa);
R(2, 1) = -cos(Phi)*sin(Kappa) - sin(Phi)*sin(Omega)*cos(Kappa);
R(2, 2) = cos(Omega)*cos(Kappa);
R(2, 3) = -sin(Phi)*sin(Kappa) + cos(Phi)*sin(Omega)*cos(Kappa);
R(3, 1) = -sin(Phi)*cos(Omega);
R(3, 2) = -sin(Omega);
R(3, 3) = cos(Phi)*cos(Omega);

XR = R(1, 1)*(Xi - Xs) + R(1, 2)*(Yi - Ys) + R(1, 3)*(Zi - Zs);
YR = R(2, 1)*(Xi - Xs) + R(2, 2)*(Yi - Ys) + R(2, 3)*(Zi - Zs);
ZR = R(3, 1)*(Xi - Xs) + R(3, 2)*(Yi - Ys) + R(3, 3)*(Zi - Zs);
xi = -f * XR / ZR + x0;
yi = -f * YR / ZR + y0;