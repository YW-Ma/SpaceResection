load good_for_station2_real
input = [12.262,-8.543,1.085]
Xi = -input(2);
Yi = input(3);
Zi = -input(1);
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
%draw several point in the photo.
img = imread('Occlusion040.JPG');
x = xi;
y = yi;

%convert
x = floor(x+1544);
y = floor(1028-y);
color = [255,0,0];
length = 25;
width = 2;
for c = 1:3
    for w = -width:width
        img(y-length:y+length, x+w, c) = color(c);
        img(y+w, x-length:x+length, c) = color(c);
    end
end
imshow(img);