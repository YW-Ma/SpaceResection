clear all;
%1.read data files
controlPts=csvread('controlPoints.txt');
imgPts=controlPts(:,1:2);%image points
gPts=controlPts(:,3:5);%ground points
gPts=gPts.*1000;%use (mm) as unit
nPts=size(gPts);
nPts=nPts(1);

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
MARK=zeros(10000,6);
X0=[Xs,Ys,Zs,Phi,Omega,Kappa]'; % not Delta value!
loop=1;
warning off all
while loop<=10000
    Phi=X0(4);
    Omega=X0(5);
    Kappa=X0(6);
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

    %4. calculate Lx Ly (error values) 

    A=zeros(2*nPts,6);
    L=zeros(2*nPts,1);
    for i=1:nPts
        %4.1 calculate A matrix.
        [A(2*i-1:2*i,:),XYZ_] = getMatrixA(gPts(i,1),gPts(i,2),gPts(i,3),imgPts(i,1),imgPts(i,2),R,fk,x0,y0,X0);
        X_=XYZ_(1);
        Y_=XYZ_(2);
        Z_=XYZ_(3);
        %4.2 V=Ax-(L-L0);L0=A*X0+d
        %L0=[xl,yl]'=[-f*X_/Z_,-f*Y_/Z_]';
        L0=[-fk*X_/Z_,-fk*Y_/Z_]';
        L(2*i-1:2*i,:)=imgPts(i,:)'-L0; % strange, why so big?
    end
    %5. V=AX-L; X=inv(A'A)A'L

    X=(A'*A)\A'*L;
    MARK(loop,:)=X;
    X0=X0+X;
%     if(abs(L)<10)
%         break;
%     end
    loop=loop+1;
end









