clear all;
%1.read data files
controlPts=csvread('controlPoints.txt');
imgPts=controlPts(:,1:2);%image points
gPts=controlPts(:,3:5);%ground points

interOrient=csvread('interiorOrientation.txt');
scale=csvread('scale.txt');
fk=interOrient(1);%focus
x0=interOrient(2);%x0
y0=interOrient(3);%y0
height=scale*fk;%flight height

%to correct imgPts
imgPts(:,1)=imgPts(:,1)-x0;
imgPts(:,2)=imgPts(:,2)-y0;

%2.initialization elements of exterior orientation
Zs=height;
Xs=mean(gPts(:,1));
Ys=mean(gPts(:,2));
Phi=0;
Omega=0;
Kappa=0;

%3.calculate rotation matrix R in Chinese Standard
R=zeros(3,3);
R(1,1)=cos(Phi)*cos(Kappa)-sin(Phi)*sin(Omega)*sin(Kappa);
R(1,2)=-cos(Phi)*sin(Kappa)-sin(Phi)*sin(Omega)*cos(Kappa);
R(1,3)=-sin(Phi)*cos(Omega);
R(2,1)=cos(Omega)*sin(Kappa);
R(2,2)=cos(Omega)*cos(Kappa);
R(2,3)=-sin(Omega);
R(3,1)=sin(Phi)*cos(Kappa)+cos(Phi)*sin(Omega)*sin(Kappa);
R(3,2)=-sin(Phi)*sin(Kappa)+cos(Phi)*sin(Omega)*cos(Kappa);
R(3,3)=cos(Phi)*cos(Omega);

%4.calculate Lx Ly (error values)

%4.1 calculate A matrix.
A1 = getMatrixA(gPts(1,1),gPts(1,2),gPts(1,3),imgPts(1,1),imgPts(1,2),Xs,Ys,Zs,R,fk,x0,y0,Phi,Omega,Kappa);














