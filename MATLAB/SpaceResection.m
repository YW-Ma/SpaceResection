load SpaceResection;
% For the format of the following two variables, please load
% SpaceResection.mat.
controlPts = c2;
ImagePts = station2;

%1. INITIALIZE
f=3940;
Xs=100;
Ys=0;
Zs=0;
Phi=0;
Omega=0;
Kappa=0;
x0=0;
y0=0;

elementNumber=9;% no k1 k2 p1 p2
equationNum = 2*size(ImagePts,1);
V = zeros(elementNumber,1);
A = zeros(equationNum, elementNumber);
X = zeros(equationNum, 1);
L = zeros(equationNum, 1);
R = zeros(3, 3);
itrNum = 0;

%2. space resection

%update RAL
%caculating R?R=R1 x R2 x R3;
R(1, 1) = cos(Phi)*cos(Kappa) - sin(Phi)*sin(Omega)*sin(Kappa);
R(1, 2) = cos(Omega)*sin(Kappa);
R(1, 3) = sin(Phi)*cos(Kappa) + cos(Phi)*sin(Omega)*sin(Kappa);
R(2, 1) = -cos(Phi)*sin(Kappa) - sin(Phi)*sin(Omega)*cos(Kappa);
R(2, 2) = cos(Omega)*cos(Kappa);
R(2, 3) = -sin(Phi)*sin(Kappa) + cos(Phi)*sin(Omega)*cos(Kappa);
R(3, 1) = -sin(Phi)*cos(Omega);
R(3, 2) = -sin(Omega);
R(3, 3) = cos(Phi)*cos(Omega);
%error equation
for i = 1:size(ImagePts,1)

    x = ImagePts.x(i);
    y = ImagePts.y(i);
    Xi = controlPts.X(i);
    Yi = controlPts.Y(i);
    Zi = controlPts.Z(i);

    XR = R(1, 1)*(Xi - Xs) + R(1, 2)*(Yi - Ys) + R(1, 3)*(Zi - Zs);
    YR = R(2, 1)*(Xi - Xs) + R(2, 2)*(Yi - Ys) + R(2, 3)*(Zi - Zs);
    ZR = R(3, 1)*(Xi - Xs) + R(3, 2)*(Yi - Ys) + R(3, 3)*(Zi - Zs);
    xi = -f * XR / ZR + x0;
    yi = -f * YR / ZR + y0;
    r = sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0));
    
    L(2 * i - 1, 1) = x - xi;
    A(2 * i - 1, 1) = (R(1, 1)*f + R(3, 1)*(x - x0)) / ZR;
    A(2 * i - 1, 2) = (R(1, 2)*f + R(3, 2)*(x - x0)) / ZR;
    A(2 * i - 1, 3) = (R(1, 3)*f + R(3, 3)*(x - x0)) / ZR;
    A(2 * i - 1, 4) = (y - y0)*sin(Omega) - (((x - x0) / f)*((x - x0)*cos(Kappa) - (y - y0)*sin(Kappa)) + f * cos(Kappa))*cos(Omega);
    A(2 * i - 1, 5) = -1 * f*sin(Kappa) - ((x - x0) / f)*((x - x0)*sin(Kappa) + (y - y0)*cos(Kappa));
    A(2 * i - 1, 6) = y - y0;
    A(2 * i - 1, 7) = (x - x0) / f;
    A(2 * i - 1, 8) = 1;
    A(2 * i - 1, 9) = 0;
    
    L(2 * i, 1) = y - yi;
    
    A(2 * i, 1) = (R(2, 1)*f + R(3, 1)*(y - y0)) / ZR;
    A(2 * i, 2) = (R(2, 2)*f + R(3, 2)*(y - y0)) / ZR;
    A(2 * i, 3) = (R(2, 3)*f + R(3, 3)*(y - y0)) / ZR;
    A(2 * i, 4) = -1 * (x - x0)*sin(Omega) - (((y - y0) / f)*((x - x0)*cos(Kappa) - (y - y0)*sin(Kappa)) - f * sin(Kappa))*cos(Omega);
    A(2 * i, 5) = -1 * f*cos(Kappa) - ((y - y0) / f)*((x - x0)*sin(Kappa) + (y - y0)*cos(Kappa));
    A(2 * i, 6) = -1 * (x - x0);
    A(2 * i, 7) = (y - y0) / f;
    A(2 * i, 8) = 0;
    A(2 * i, 9) = 1;
end
%update X
X = (A'*A)\A'*L;
Xs = Xs + X(1, 1);
Ys = Ys + X(2, 1);
Zs = Zs + X(3, 1);
Phi = Phi + X(4, 1);
Omega = Omega + X(5, 1);
Kappa = Kappa + X(6, 1);
f = f + X(7, 1);
x0 = x0 + X(8, 1);
y0 = y0 + X(9, 1);
%not converge
notConverge = ((abs(X(4, 1)) > 1e-5 )|| (abs(X(5, 1)) > 1e-5) || (abs(X(6, 1)) > 1e-5));
while notConverge
%update RAL
%caculating R?R=R1 x R2 x R3;
R(1, 1) = cos(Phi)*cos(Kappa) - sin(Phi)*sin(Omega)*sin(Kappa);
R(1, 2) = cos(Omega)*sin(Kappa);
R(1, 3) = sin(Phi)*cos(Kappa) + cos(Phi)*sin(Omega)*sin(Kappa);
R(2, 1) = -cos(Phi)*sin(Kappa) - sin(Phi)*sin(Omega)*cos(Kappa);
R(2, 2) = cos(Omega)*cos(Kappa);
R(2, 3) = -sin(Phi)*sin(Kappa) + cos(Phi)*sin(Omega)*cos(Kappa);
R(3, 1) = -sin(Phi)*cos(Omega);
R(3, 2) = -sin(Omega);
R(3, 3) = cos(Phi)*cos(Omega);
%error equation
for i = 1:size(ImagePts,1)

    x = ImagePts.x(i);
    y = ImagePts.y(i);
    Xi = controlPts.X(i);
    Yi = controlPts.Y(i);
    Zi = controlPts.Z(i);

    XR = R(1, 1)*(Xi - Xs) + R(1, 2)*(Yi - Ys) + R(1, 3)*(Zi - Zs);
    YR = R(2, 1)*(Xi - Xs) + R(2, 2)*(Yi - Ys) + R(2, 3)*(Zi - Zs);
    ZR = R(3, 1)*(Xi - Xs) + R(3, 2)*(Yi - Ys) + R(3, 3)*(Zi - Zs);
    xi = -f * XR / ZR + x0;
    yi = -f * YR / ZR + y0;
    r = sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0));
    
    L(2 * i - 1, 1) = x - xi;
    A(2 * i - 1, 1) = (R(1, 1)*f + R(3, 1)*(x - x0)) / ZR;
    A(2 * i - 1, 2) = (R(1, 2)*f + R(3, 2)*(x - x0)) / ZR;
    A(2 * i - 1, 3) = (R(1, 3)*f + R(3, 3)*(x - x0)) / ZR;
    A(2 * i - 1, 4) = (y - y0)*sin(Omega) - (((x - x0) / f)*((x - x0)*cos(Kappa) - (y - y0)*sin(Kappa)) + f * cos(Kappa))*cos(Omega);
    A(2 * i - 1, 5) = -1 * f*sin(Kappa) - ((x - x0) / f)*((x - x0)*sin(Kappa) + (y - y0)*cos(Kappa));
    A(2 * i - 1, 6) = y - y0;
    A(2 * i - 1, 7) = (x - x0) / f;
    A(2 * i - 1, 8) = 1;
    A(2 * i - 1, 9) = 0;
    
    L(2 * i, 1) = y - yi;
    
    A(2 * i, 1) = (R(2, 1)*f + R(3, 1)*(y - y0)) / ZR;
    A(2 * i, 2) = (R(2, 2)*f + R(3, 2)*(y - y0)) / ZR;
    A(2 * i, 3) = (R(2, 3)*f + R(3, 3)*(y - y0)) / ZR;
    A(2 * i, 4) = -1 * (x - x0)*sin(Omega) - (((y - y0) / f)*((x - x0)*cos(Kappa) - (y - y0)*sin(Kappa)) - f * sin(Kappa))*cos(Omega);
    A(2 * i, 5) = -1 * f*cos(Kappa) - ((y - y0) / f)*((x - x0)*sin(Kappa) + (y - y0)*cos(Kappa));
    A(2 * i, 6) = -1 * (x - x0);
    A(2 * i, 7) = (y - y0) / f;
    A(2 * i, 8) = 0;
    A(2 * i, 9) = 1;
end
%update X
X = (A'*A)\A'*L; 
Xs = Xs + X(1, 1);
Ys = Ys + X(2, 1);
Zs = Zs + X(3, 1);
Phi = Phi + X(4, 1);
Omega = Omega + X(5, 1);
Kappa = Kappa + X(6, 1);
f = f + X(7, 1);
x0 = x0 + X(8, 1);
y0 = y0 + X(9, 1);
%not converge
notConverge = ((abs(X(4, 1)) > 1e-3 )|| (abs(X(5, 1)) > 1e-3) || (abs(X(6, 1)) > 1e-3));
itrNum=itrNum+1;
end
%loop - update RAL - X - Not converge