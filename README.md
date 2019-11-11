SpaceResection
========
Determine the positional and rotational elements of a photograph, or of a camera, by utilizing fixed object space coordinates(Contorl Points).

Input and Output
-----
* Input:
    * `Space coordinates` of fixed objects.
    * `Image coordinates` of fixed objects.
    * `Initial value` of positional and rotational elements of a camera.
* Output:
    * positional and rotational elements (orientation elements) `focus`, `x0`, `y0`, `Xc`, `Yc`, `Zc`, `Phi`, `Omega`, and `Kappa`
* Method:
    * Taylor Series Linearization [(TSL)](https://en.wikipedia.org/wiki/Linearization)
    * Weighted least squares [(WLS)](https://en.wikipedia.org/wiki/Weighted_least_squares) 

Install & Usage
-----
* MATLAB
    * Load data from `SpaceResection.mat`.
    * Enjoy.
* C++
    * Pre-requisite: `OpenCV 4.1.0`
    * Only contains source code, you can reuse the class `RESECTION`
    * If you want to reproduce the results I got, refer to Visual Studio Project or compile the source code by yourself. XD
* Visual Studio Project
    * `OpenCV 4.1.0` already embedded.
    * Step to use:
        * Unzip the `opencv.zip` follow the `README` file of `VS Project`
        * Change the version of VS in `SpaceResection.vcxproj` if necessary
        * Open the project in your VS, rebuild the solution and project.
        * Run the source code.

Examples
----
* To describe the process by which directional elements converge in an iteration, I made a GIF map.
* After each iteration, a set of updated orientation elements is obtained.
* Using the [Collinearity equation](https://en.wikipedia.org/wiki/Collinearity_equation), we can compute the image coordinates of the space control points when the camera takes this set of orientation elements.
*  Using the red crosses to mark the image coordinates we computed, we will find that they step closer to the true positions of the control points on the image after each iteration.
* In the end, there is a set of orientation elements that enale the positions of these red crosses to be very close to the real position and no longer change, which means the orientation elements converge.

[Click me to watch the GIF](https://github.com/YW-Ma/SpaceResection/blob/master/spaceResection-iteration.gif)









