// #declare CHEAP_MUSIC=1;

#include "colors.inc"
#include "drums.inc"
#include "cymbals.inc"

global_settings {
//  ambient_light rgb 0.08
// ambient_light 0.0 radiosity { adc_bailout 0.01/3 error_bound 1.0 }
  max_trace_level 100
}

//light_source { <70, 70, -70> White/2 }
light_source { <0, 7, 4> White }

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

//  camera {location <0.9, 1, -1.0> look_at <0, 0.4, 0>}
//camera {location <0.7, 0.3, -0.8> look_at <0, 0.4, 0>}
// object {bassdrum}

// camera {location <0, 0.5, -0.7> look_at <0, 0.2, 0>}
// object {tom(0.203, 0.202) translate <0.03, 0.35, -0.4>}
// object {tom(0.305, 0.355) translate <-0.2, 0.25, 0>}

// From the drummer's point of view!
// camera {location <0, 2.5, -0.1> look_at <0, 0.5, 0.2>}
camera {location 16*z rotate <-45, -30, 0> look_at <0.3, 0.6, 0> angle 17}
// camera {location 2*z rotate <-25, -60, 0> look_at <0, 0.8, -1>} // collisions
// camera {location <0, 1, -6> look_at <0, 0.6, 0.2> angle 35} // dscn1751

nick()
