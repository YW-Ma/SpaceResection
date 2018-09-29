# SpaceResection
1.Matrix class(including inverse\multi\add\init\...)
2.Space Resection(in both c++ and MATLAB version)

Space Resection (surveying&amp;photogrammetry) algorithm

Also has a Matrix class,which contains transposition\inversion\multiplication.

In controlPoints.txt, each row is:
x_img(mm),y_img(mm),X(m),Y(m),Z(m)  
[control points' img coordinate values and geographical coordinate values]

The file interiorOrientation.txt record interior orientation elements.
they are:
focus(mm)
x0(mm)
y0(mm)

Additionally, a MATLAB Program is available as a test.
Notice: elements of exterior orientation is coordinate to the Chinese standard(Phi,Omega,Kappa), NOT WESTERN STANDARD(Omega,Phi,Kappa)!!!