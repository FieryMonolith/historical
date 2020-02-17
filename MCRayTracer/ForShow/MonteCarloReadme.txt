Original raytracer was given to me from Sherri Goings.
The primary modifications took place in the following files:
CChildView.h
CChildView.cpp
MyRaytraceRenderer.h
MyRaytraceRenderer.cpp
and the resources to add a dialog for the number of sample rays

*Also, I modified and overloaded the AddLight functions.  If you want 
to modify the Monte Carlo settings for a light, the more complicated 
AddLight functions must be used, which can't be used with the Opengl 
renderer.

All of my code should be outlined by ifdefs of MONTE_CARLO.  I hope 
this easies the burden of incorporating my code.  The following is an 
overview of the algorithms and design of this feature.

  This is the information pertaining to the Monte Carlo raytracing used 
for CSE 891 Advance Computer Graphics, implemented by Jason Stredwick.  
Monte Carlo raytracing is the use of volumetric lights that have 3D 
surface.  Using a stochastic sampling of surface points on a light, a 
more realistic blended lighting occurs.
  For this project, I used a simplied algorithm which I feel gives 
very similar results.  All lights that can be used with the Monte 
Carlo technique must be spherical, or cones of light defined in terms of 
a sphere, light direction, and breadth angle.  The simplification occurs 
in that I use a disc centered at a sphere's origin with a radius equal to
the sphere radius, to represent the sphere.  The disc is orientated such 
that the normal to the disc is the vector from the center to the surface point 
being lit.  A random sampling from the surface of the disc is used to 
determine approximately how much, if any, of the light is blocked from the 
surface point by other objects.
  The sampling functions were drawn from the website 
http://mathworld.wolfram.com/DiskPointPicking.html   
According to that website, the proper sampling formulas are 

x = sqrt(radius) * cos(theta), y = sqrt(radius) * sin(theta)

where r is a random number from 0 to 1, and theta is a random number from 
zero to two pi.  The reason given was that the sqrt(radius) relates to 
area of a circle which is where the samples originate from.  Pictures 
illustrating the results showed that just using r, the samples clusted 
near the center of the circle.
  Once the samples were determined, they needed to be converted to 3D 
points on the disc.  A function was determined to generate a random "xy" 
coordinate system relative to the disc.  Using these coordinate vectors,
I was able to convert the 2D disc points to 3D disc points.
  For each sample on the disc, a ray was cast from the surface point to 
the sample point.  If the ray makes it to the sample point without 
intersecting something else along the way, it is given a value of 1, 
otherwise it is given a value of 0.  A vector of these ones and zeros 
along with the light direction for each of the sampled points to the 
surface was recorded.  For diffuse lighting, the dot product of each 
light ray by the surface normal was multiplied by each of the 0 or 1 
associated with that ray.  Summing all these values and dividing by the 
number of rays gives an approximation for the lighting of the surface 
point.
  For directional lights, the same process occurs with one additional 
feature.  When the sample is taken and the sampled light vector is 
computed.  If the dot product of that vector with the light orientation 
is less than the cos of the sweep angle of the cone, then this vector 
is outside the cone of light and the contribution is set to zero.  
However, this is not fully functioning.  I originally had a problem 
with the non-directional lights which was fixed by transforming the 
lights by transformation defined by the LookAt method.  However, this 
did not solve my problem for directional lights, which I suspect is it 
is not properly transforming the orientation of the light.  It does 
work, just the circle of light moves with viewer to some extent.
  To really see the effects, you will need to change the light radius, 
and move the lights around.  I currently have simple lighting that 
shows the basic features.
