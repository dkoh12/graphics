
Initially implemented using OpenCV which outputs jpg images. But the school instructional machines did not have OpenCV installed and since students do not have sudo access to download software on instructional machines, we used CImg which outputs ppm images.


****************************************************
Files
****************************************************
    c++
        main.cpp
        objReader.cpp
        Matrices.cpp
        
    headers
        Boundary.h
        Camera.h
        Colors.h
        Light.h
        Vectors.h
        Shape.h
        Material.h
        Matrices.h

****************************************************
Build Instructions
****************************************************
- to run the current sample main, go to project directory
	1) rm -r build
	2) mkdir build
	3) cd build
	4) cmake ..
	5) make 
	6) ./as2 ../input/input.txt


****************************************************
 Flow
****************************************************
- The Scene class has a rendering loop that asks the Sampler class samples for all the pixels in the screen.
- For each sample, Scene asks the camera class to create eye ray. 
- Then Scene asks the Raytracer class to compute the color for that ray.
- Finally, Scene will tell Film class that the sample has the color value. Film then output the image to the file.

[Raytracer -> Primitive -> Shape -> Primitive]
Inside Raytracer class, it asks Primitive to compute the nearest ray-object intersection. Then it shades that pixel, or recursively create mirror reflection ray and call itself.
Then it returns the resulting color to scene. Primitive will transform the ray from world space to object space and hand it to Shape to do intersection in object space.
Shape then computes the intersection and returns the result to Primitive.

****************************************************
Images
****************************************************
![alt text][reflection]

[reflection]: https://github.com/dkoh12/graphics/blob/master/ray%20tracer/output/input1_output.jpg "Reflection"

![alt text][spheres]

[spheres]: https://github.com/dkoh12/graphics/blob/master/ray%20tracer/output/lineuptest_output.jpg "Line Up Test"

****************************************************
 Source Credit: 
****************************************************
Raytracer concept:
- [Fall 2009 guide] 
	http://inst.eecs.berkeley.edu/~cs184/fa09/resources/raytracing.htm
- [Fall 2009 Journal]
	http://inst.eecs.berkeley.edu/~cs184/fa09/raytrace_journal.php
Github source:
https://github.com/marczych/RayTracer/tree/master/src
