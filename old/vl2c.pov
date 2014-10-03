//
// Render one VL2C Vari*Lite
//
#include "vl2c.inc"
#include "colors.inc"

plane {
  y, 0
  texture {
    pigment { colour White}
  }
}

camera {location <0, 0.4, -3> look_at <0, 0.4, 0>}
light_source { <7, 7, -10> White/2 }
//light_source { <0.5, 0.1, -0.3> White }

object {
  vl2c(Orange, <0, 168, 0>)
  translate 0.650*y
}
