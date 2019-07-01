# Content
Space Resection (surveying&amp;photogrammetry) algorithm (in both c++ and MATLAB version)
Also has a Matrix class,which contains transposition\inversion\multiplication.
# Data format
In controlPoints.txt, each row is:
x_img(mm),y_img(mm),X(m),Y(m),Z(m)  
[control points' img coordinate values and geographical coordinate values]

The file interiorOrientation.txt record interior orientation elements.
they are:
focus(mm)
x0(mm)
y0(mm)

# Notice
Notice: elements of exterior orientation is coordinate to the Chinese standard(Phi,Omega,Kappa), NOT WESTERN STANDARD(Omega,Phi,Kappa)

Additionally, a MATLAB Program is available as a test.
