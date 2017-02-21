// This looks like an early idea for a cymbal.
// No longer useful.

#include "colors.inc"

light_source { <50, 70, -100> White }
  
plane {
  y, 0
  texture {
    pigment { checker colour Green colour Yellow}
  }
}
sky_sphere {
  //      pigment { checker colour Green colour Yellow}
  pigment { gradient y color_map {[0, 1  color LightBlue/2 color LightBlue]}}
}

// camera {location <-0.1, 0.9, -1> look_at 0.4*y} // whole object
camera {location <-0.1, 1.1, -0.30> look_at y} // top
//  camera {location <0.1, 0.8, -0.35> look_at 0.7*y} light_source { <50, 0.5, -70> White } // underside
//  camera {location <0.1, 0.4, -0.35> look_at 0.12*y} // legs
//  camera {location <0.1, 0.3, -0.45> look_at 0*y} // feet

#declare shape = blob {
  threshold 0.0460061516
  sphere {-1.32320167*y, 1.3401958608, 72.4462742556 pigment {Blue}}
  sphere {(-0.12748766+0.012)*y, .1438442195, 1 pigment {Red}}
};

intersection {
  object {shape}
  object {shape translate -0.001*y inverse}
  cylinder {-1*y, y, 0.18}
    
  translate y
  
}
