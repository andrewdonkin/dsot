#include "dsot.inc"
#include "stage.inc"
#include "vl2c.inc"
#include "varidisc.inc"
#include "dalek.inc"

#include "drums.inc"
#include "guitars.inc"
#include "keys.inc"		// Needs guitars.inc

#local StageHeight = 3;
#local DiscRadius = 4.9;	// 32' diameter.

global_settings {
  //  ambient_light rgb 0.08
  // ambient_light 1
  ambient_light 0 radiosity { count 100 brightness 0.2 }
  max_trace_level 200
  assumed_gamma 1
}

#if (clock_on)
  camera {location <-12 + 24*softly(clock), 8, -4> look_at <-8+16*clock, 5, 2>}
#else
// camera {location <-1, 8, -4> look_at <-2.5, 3.5, 8> angle 20 } // checking Dick
// camera {location <-5, 8, -4> look_at <-4, 3, 2>} // checking grilles SR
// camera {location <6, 8, -4> look_at <4.5, 3, 2>} // checking grilles SL
// camera {location <-1, 7, 2> look_at <4, 2, 2>} // checking keys
// camera {location <0, 9, -20> look_at <0, 7, 4>} // straight on
 camera {location <-12, 5, -2> look_at <0, 2, 4>  } // wawa
// camera {location <0.8, 4.2, 5> look_at <-1.5, 3.8, 3.8> angle 20} // Dalek #2, angle it at -45
// camera {location <-10, 4, -2> look_at <-8, 2, 4> angle 30} light_source{<-12, 5, -2> White} // Wawa bench
//  camera {location <-10, 6, 18> look_at <-7, 2, 5>} light_source{ <-9, 6, 18> White}
// camera {orthographic location <0, 15, 0> look_at z}
// camera {location <12, 6, -4> look_at <10, 3, 0>} // audience right
// camera {location <-8, 5, 5> look_at <-12, 2, 0> }  light_source { <-8, 5, 6> White} // platform widgets
// camera {location <-1.5, 2, -10> look_at <2, 5, 0> angle 45} // Up past Rick to screen
// camera {location <0.5, 3.2, 0> look_at <0, 4, 2> angle 60} // Up Nick, at screen
#end

//light_source { <30, 70, -70> White  media_interaction off spotlight point_at 0}
//plane {y, 0 texture {pigment { checker colour Green colour Yellow}}}
plane {y, 0 texture {pigment { rgb 0}}}
  sky_sphere { test_sky_sphere_sesame }
/*sky_sphere {
  //      pigment { checker colour Green colour Yellow}
  pigment { gradient y color_map {[0, 1  color Blue/10 color LightBlue/2]}}
}
*/

#if (0)
  box {<-12, 0, -10>, <12, 26, 12>
  hollow
  interior {
    ior 1
    media {
      scattering {4, 0.5 extinction 0.1}
      samples 30,30
      intervals 1
      method 3
    }
  }
  texture {
    pigment {rgbt 1}
    finish {ambient 1 diffuse 0}
  }
}
#end

object {
  circlescreen(DiscRadius)
  translate <0, (StageHeight+DiscRadius+4.5), 10>
}

#local nick_v = <0, 0, 3>;
#local guy_v  = <nick_v.x + 2.1, 0, nick_v.z-0.7>;
#local dave_v = <nick_v.x - 3, 0, 1.5>;
#local rick_v = <3, 0, 0.5>;
union {
  stage(StageHeight)
  nick(nick_v)
  guy(guy_v)
  dave(dave_v)
  rick(rick_v)
// Gary and Jon are added by stage()
  translate StageHeight*y
}

//light_source {<-11, 10, 1> White/2 spotlight point_at nick_v + StageHeight*y radius 10 falloff 11
//  media_interaction off
//  #if(! NO_AREA_LIGHTS) area_light 0.2*x 0.2*y 5 5 circular orient adaptive 1  #end
//}
light_source {<-11, 10, -5> White/2 spotlight point_at nick_v + StageHeight*y radius 5 falloff 6
  media_interaction off
  #if(! NO_AREA_LIGHTS) area_light 0.2*x 0.2*y 5 5 circular orient adaptive 1  #end
}
/* light_source {<-9, 10, 8> White/2 spotlight point_at <6, StageHeight, 1> radius 10 falloff 11
  media_interaction off
  #if(! NO_AREA_LIGHTS) area_light 0.2*x 0.2*y 5 5 circular orient adaptive 1 #end }
*/

