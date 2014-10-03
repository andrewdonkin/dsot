camera {location <0, 0.05, -0.5> look_at 0}

light_source {<10, 10, -10> rgb 1}
plane {y, -0.2 texture { pigment { checker colour y colour y+x} }}

union {
  difference {
    sphere {0, 0.1
      hollow
      interior {ior 1.3
	//      dispersion 1.01
      }
    }
    sphere {0, 0.09 hollow}
  }
  texture {
    finish {specular 1 roughness 0.001 reflection {0}}
    pigment { rgbf <1, 1, 1, 0.9> }
  }
}

sphere {0, 0.089
//  interior {
//    ior 1
//    media {
//      emission 1
//    }
//  }
  texture {
    finish {specular 0 reflection 0 ambient 1}
    pigment {rgbt <1, 0.5, 0, 0>}
  }
}
