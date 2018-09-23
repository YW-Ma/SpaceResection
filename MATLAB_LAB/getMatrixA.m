function [A] = getMatrixA(X,Y,Z,x,y,R,fk,x0,y0,X0)
%(X,Y,Z)->gPts;(x,y)->imgPts;
%1.initialize assistant values X_,Y_ Z_
Xs=X0(1);
Ys=X0(2);
Zs=X0(3);
Phi=X0(4);
Omega=X0(5);
Kappa=X0(6);

temp=R\[X-Xs,Y-Ys,Z-Zs]';
X_=temp(1);
Y_=temp(2);
Z_=temp(3);
%2.calculate Marix A [only exterior orientation elements is needed. 
%                     So size of A is (2,6) instead of (2,9)]
A=zeros(2,6);
A(1,1)=1/Z_*(R(1,1)*fk+R(1,3)*(x-x0));
A(1,2)=1/Z_*(R(2,1)*fk+R(2,3)*(x-x0));
A(1,3)=1/Z_*(R(3,1)*fk+R(3,3)*(x-x0));
A(1,4)=(y-y0)*sin(Omega)-((x-x0)/fk*((x-x0)*cos(Kappa)-(y-y0)*sin(Kappa))+fk*cos(Kappa))*cos(Omega);
A(1,5)=-fk*sin(Kappa)-(x-x0)/fk*((x-x0)*sin(Kappa)+(y-y0)*cos(Kappa));
A(1,6)=y-y0;

A(2,1)=1/Z_*(R(1,2)*fk+R(1,3)*(y-y0));
A(2,2)=1/Z_*(R(2,2)*fk+R(2,3)*(y-y0));
A(2,3)=1/Z_*(R(3,2)*fk+R(3,3)*(y-y0));
A(2,4)=(x-x0)*sin(Omega)-((y-y0)/fk*((x-x0)*cos(Kappa)-(y-y0)*sin(Kappa))+fk*sin(Kappa))*cos(Omega);
A(2,5)=-fk*cos(Kappa)-(y-y0)/fk*((x-x0)*sin(Kappa)+(y-y0)*cos(Kappa));
A(2,6)=-(x-x0);

end