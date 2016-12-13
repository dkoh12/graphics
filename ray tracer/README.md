****************************************************
 Worklog
****************************************************
Tinnei 10/17:
	File editted: main.cpp, scene.cpp, film.cpp, MakeFile, README
	main.cpp: initialize scene
	scene.cpp: loop through pixels, display and output result
	film: display and output result (honestly this is so unneccessary for the moment)

David 10/17:
    main.cpp takes in input text file from ../input/input.txt and parses the inputs
    
    Everything compiles now. Added Intersection.h, Boundary.h, Intersection.h, and Light.h and fixed Shape.h
    
    Vectors.h contains both vectors and rays
    Shape.h contains shapes sphere and triangle
    
****************************************************
Files
****************************************************
    c++
        camera.cpp
		film.cpp
        main.cpp
		Matrices.cpp
		primitive.cpp
		raytracer.cpp
		sampler.cpp
		scene.cpp
        
    headers
        Boundary.h
        Light.h
        Vectors.h
        Shape.h
        Material.h
        Matrices.h
        Intersection.h
        Colors.h

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
 Source Credit: 
****************************************************
Raytracer concept:
- [Fall 2009 guide] 
	http://inst.eecs.berkeley.edu/~cs184/fa09/resources/raytracing.htm
- [Fall 2009 Journal]
	http://inst.eecs.berkeley.edu/~cs184/fa09/raytrace_journal.php
Github source:
https://github.com/marczych/RayTracer/tree/master/src
