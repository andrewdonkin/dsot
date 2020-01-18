#version 3.7;
#declare DALEK_CHEAP_TEXTURES=1;
#include "dalek.inc"
  #include "dsot.inc"
  #include "colors.inc"
  
  global_settings {
    ambient_light rgb 0.08
    //ambient_light 0.0 radiosity { adc_bailout 0.01/3 error_bound 1.0 }    max_trace_level 30
  }
  

  plane {
    y, 0
    texture {
      pigment { rgb 1}
    }
  }

//light_source { <7, 7, -10> White media_interaction off}
  light_source { <-6, 6, -20> White spotlight radius 1 falloff 12 point_at y media_interaction off}
//light_source { <-6, 20, 30> White }
//light_source { <-1, 0.2, -1> White }

  plane { -z, -3 pigment {rgb 1}}
  
  object {
    #if (clock_on)
      dalek(Green, Blue, 1 + clock*360, 1 + clock*360*2, 0, -30 + clock*60)
//      dalek(Green, Blue, 0, 0, 0, -90 + 180*clock)
    #else
      dalek(Green, Blue, 0, 0, 0, 0)
    #end
    rotate 180*y
    translate y/2
  }

  #if(0)box {<-2, 0, -1.5>, <2, 2, 3> hollow
      interior { ior 1
	media {
	  scattering {2, 0.4 extinction 0}
	  samples 10 intervals 1 method 3
	}
      }
      texture {	pigment {rgbt 1}}
    }
  #end
  
//  media { scattering { 2, rgb 1/5 extinction 0} }

    camera {
//  location <-0.6, 0.8, 0.8> look_at <0, 0.3, 0.2> // mount
//  location <0, 0.9, 0.9> look_at <0, 0.3, 0.3> // mount, close
//  location <0, 0.2, -3> look_at <0, 0.8, 0>  // matches dscn1746.jpg
  // for checking lights.  Use 135 degree rotation
//  location <0.6, 3, -3.5> look_at <1.5, 0, -1.5>
//    location <0.3, 1.2, -1.5> look_at <0.5, 0.9, 0> // Angle at -55
//    location <0, 0.8, -1.2> look_at <0, 0.2, 0>  // Checking plinth and cables
//    location <0, 0.8, -1.2> look_at <0, 0.55, 0>  angle 70 // Checking disc textures
//    location <0, 1, -1.5> look_at <0, 1, 0>  // right down the barrel
//    location <0.3, 1.2, -1.5> look_at <0.41, 1, 0> angle 30 // checking blur
//    location <0.3, 1.2, -1.5> look_at <0.3, 0.95, 0> angle 20 // checking rounding
//  location <-0.1, 0.3, -0.4> look_at <0, 0.2, 0> // check cables
//  location <0.5, 0.6, -1> look_at <0, 0.5, 0> // check cables with rotation 130*y
    location <0, 0.6, -1.2> look_at <0, 0.25, 0>  angle 60 // Checking lower cables (rotate 180)
}


