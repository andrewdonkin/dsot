#include "colors.inc"
#include "textures.inc"

#declare origin = <0,0,0>;

#include "varidisc.pov"
// #include "gantry.pov"
#include "stage.pov"

#declare lightball = sphere {0, 1
  texture {
    pigment {color rgb <1, 1, 1>}
    finish {ambient .8 diffuse .6}
  }
}

// Distance
// camera {location <0,1,-50> look_at <0,-DiscRadius/2,0>}
// Left varilights.
// camera {location <-SpotCircleRad-5, 4, -4> look_at <0, -SpotCircleRad/3, 1>}
 camera {location <-SpotCircleRad-3, 4, -2.5> look_at <0, -SpotCircleRad/3, 1>}
// Right varilight.
// camera {location <SpotCircleRad * 0.8, 4, -4> look_at <SpotCircleRad, -SpotCircleRad/3, 1>}
// For a gantry
// camera {location <0, 3, -55> look_at <0, 0, 0>}
// light_source {<10, -10, -10> color rgb <0.3,0.3,0.3> looks_like {lightball}}

// light_source {<30, 20, -50> color rgb 0.3 looks_like {lightball}}

