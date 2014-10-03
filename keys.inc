// Keyboards are about 300-350 deep.
// Stack three high, each offset by about 150, makes about 700 deep.

// 1100 wide, and leave room for stuff in the corner (Jon's especially).

// But leave room for another guy on Rick's right during Comfortably Numb.

// Guess 1m high?

// ffwd to 2014 and uberpov.

#version 3.7;

// #include "woods.inc"
#declare TEST=1;
#ifdef (TEST)
  #undef TEST
  #include "dsot.inc"
  #declare TEST=1;
#end

#declare keyseed = seed(0);

#declare keys = union {
//  object {glowbox(<1.5, 1.0, 0.7>, Blue) translate (0.7+0.7*sind(22.5))*x}
  object {glowbox(<1.5, 1.0, 0.7>, Blue) translate 0.7*x}
  object {louvre(<1.5, 1.0, 0.02>, 20, 0.9) translate 0.7*x}
  object {glowbox(<-sqrt(0.7*0.7*2), 1.0, 0.7>, Blue) rotate 45*y translate 0.7*x}
  object{louvre(<sqrt(0.7*0.7*2), 1.0, -0.02>, 20, 0.9) rotate 45*y translate 0.7*z}
  object {glowbox(<0.7, 1.0, 1.6>, Blue) translate 0.7*z}
  object {louvre(<-1.6, 1.0, 0.02>, 20, 0.9) rotate 90*y translate 0.7*z}

  // +X end.
  object {louvre(<0.7, 1, 0.02>, 20, 0.9) rotate -90*y translate 2.2*x}
  // +Z end, facing back of stage.
  object {louvre(<-0.7, 1, -0.02>, 20, 0.9) rotate 180*y translate 2.3*z}
  
};

#macro rick(xlate_v)
  union {object {keys }
    // Guy's rack
    object{axerack(1) rotate -90*y translate <0.5, 0, 2.3>}
    translate xlate_v
  }
#end

#macro jon(xlate_v)
  object {keys 
    translate xlate_v
  }
#end

#local _7octwidth = 1.154;			// seven octaves in metres
#local _keyboardwidth = _7octwidth * 52 / 49;	// width of an 88-key keyboard
#local _whitewidth = (_7octwidth / 7 / 7) - 0.001;
#local _blackwidth = 0.011;				    // width of a black key at base
#local _blackradius = 0.0005;
#local _whiteradius = 0.0003;
#local _blackheight = 0.013;

#local _p_whitekey = colour rgbt <1, 1, 1, 0.005> ; // alpha required for uberpov bug
#local _p_blackkey = colour rgbt <0.02, 0.02, 0.02, 0.005> ; // alpha required for uberpov bug

#local _t_whitekey = texture {
  gradient x  
  turbulence <0.7, 0, 0>
  texture_map {
    [0   pigment{_p_whitekey} finish {specular 1    roughness 0.0001 reflection 0.01}]
    [0.1 pigment{_p_whitekey} finish {specular 1    roughness 0.0001 reflection 0.01}]
    [0.5 pigment{_p_whitekey} finish {specular 0.5  roughness 0.001 reflection 0.01}]
    [0.6 pigment{_p_whitekey} finish {specular 0.01 roughness 0.01 reflection 0.01}]
    [0.7 pigment{_p_whitekey} finish {specular 0.01 roughness 0.01 reflection 0.01}]
    [0.97   pigment{_p_whitekey * 0.99  } finish {specular 0 reflection 0.01}]
    [1   pigment{ _p_whitekey * 0.950  } finish {specular 0}]
  }
  octaves 4
  omega 0.25
  scale <0.002, 0.001, 0.04>
  // rotate <0, 1, 0>
  translate <1, 0.01, 0>
};

#local _t_blackkey = texture {
  wood  
  turbulence <0.06, 0, 0>
  texture_map {
    [0   pigment{rgb  _p_blackkey} finish {specular 1    roughness 0.0001}]
    [0.1 pigment{rgb  _p_blackkey} finish {specular 1    roughness 0.0001}]
    [0.1 pigment{rgb  _p_blackkey} finish {specular 0.5  roughness 0.001}]
    [0.5 pigment{rgb  _p_blackkey} finish {specular 0.5  roughness 0.001}]
    [0.6 pigment{rgb  _p_blackkey} finish {specular 0.5 roughness 0.001}]
    [0.7 pigment{rgb  _p_blackkey} finish {specular 0.5 roughness 0.001}]
    [1   pigment{rgb _p_blackkey / 2 } finish {specular 0}]
  }
  octaves 1
  //omega 0
  scale <0.003, 0.001, 0.04>
  //rotate <0, 1, 0>
  translate 4 * x
};

// rotate by a tiny amount in all three axes to stop all the keys being perfectly placed.
#macro _keywiggle()
  translate -0.2*z
  rotate <(rand(keyseed) - 0.5) * 0.2, (rand(keyseed)-0.5) * 0.1, (rand(keyseed)-0.5) * 0.1 >
  translate 0.2*z
#end

#local _blackhalftop =
intersection {
    union {
    cylinder {						    // top front
      <-1, 0, 0>,
      < 1, 0, 0>,
      _blackradius
      scale <_blackwidth/2 - _blackradius, 1, 1>
      translate <0, _blackheight - _blackradius, _blackradius>
    }
    cylinder {						    // left front
      <0, 0, 0>,
      <0, _blackheight - _blackradius, 0>,
      _blackradius
      translate <- (_blackwidth/2 - _blackradius), 0, _blackradius>
    }
    cylinder {						    // right front
      <0, 0, 0>,
      <0, _blackheight - _blackradius, 0>,
      _blackradius
      translate <+ (_blackwidth/2 - _blackradius), 0, _blackradius>
    }
    sphere { <- (_blackwidth/2 - _blackradius), _blackheight - _blackradius, _blackradius>, _blackradius }
    sphere { <+ (_blackwidth/2 - _blackradius), _blackheight - _blackradius, _blackradius>, _blackradius }
    // These balls at the bottom, should intersect them with +y,
    // because as is they have their lower halves sheared forward toward -z
    // Not very noticeable.
    sphere { <- (_blackwidth/2 - _blackradius), 0, _blackradius>, 0 }
    sphere { <+ (_blackwidth/2 - _blackradius), 0, _blackradius>, 0 }
    cylinder {						    // right top
      0, z/3, _blackradius
      translate <+ (_blackwidth/2 - _blackradius), _blackheight - _blackradius, _blackradius>
    }
    cylinder {						    // left top
      0, z/3, _blackradius
      translate <- (_blackwidth/2 - _blackradius), _blackheight - _blackradius, _blackradius>
    }
   box { 0, < _blackwidth - 2*_blackradius, _blackheight - _blackradius, 1/3> // front
     translate x * -(_blackwidth / 2 - _blackradius)
    }
    box { _blackradius * z, <_blackwidth, _blackheight - _blackradius, 1/3>  // sides
      translate <- _blackwidth/2, 0, 0>
    }

   box { _blackradius * z, < _blackwidth - 2 * _blackradius, _blackheight, 1/3> // top
     translate <- _blackwidth/2 + _blackradius, 0, 0>
   }
    // shear.  first term leans it to the right, second leans it away
   matrix <
    1, 0, 0,
    0.08, 1, 0.3, 
    0, 0, 1,
    0, 0, 0 >
    }
  plane { x, 0} // only interested in the left half
  plane { z, 0.087} // and the front.  Very sharp cut-off.
}

//######################################################################
// black key with front left point at x=0, z=0.
// Top of white keys should be at y=0
#local _blackkey =
  union {
    object{ _blackhalftop}				    // left
    object {_blackhalftop
      scale <-1, 1, 1>					    // right
    }
    // now, the unsheared part underneath
    union {
      cylinder {						    // front bottom
	<-1, 0, 0>,
	< 1, 0, 0>,
	_blackradius
	translate <0, 0, _blackradius>
	scale <_blackwidth/2 - _blackradius, 1, 1>
      }
      cylinder {						    // left front
	<0, 0, 0>,
	<0, -0.01, 0>,
	_blackradius
	translate <- (_blackwidth/2 - _blackradius), 0, _blackradius>
      }
      cylinder {						    // right front
	<0, 0, 0>,
	<0, -0.01, 0>,
	_blackradius
	translate <+ (_blackwidth/2 - _blackradius), 0, _blackradius>
      }
      box { 0, < _blackwidth - 2 * _blackradius, -0.01, 0.01 > // front
	translate < - _blackwidth/2 + _blackradius, 0, 0>
      }
      box { 0, < _blackwidth, -0.01, 0.10 > // sides
	translate < - _blackwidth / 2, 0, _blackradius>
      }
    }
    // texture {_t_blackkey   }
    // lift it a little to show the transition point between upper and lower parts better.
    // keywiggling hides it a bit.
    translate <_blackwidth / 2, 0.001, 0>
    _keywiggle()
  }

//######################################################################
#local _whitekey = union {
  object {
  #ifdef (CHEAP_MUSIC) 
    box {-1, 1}
#else
    superellipsoid { <0.25, 0.05>}
  #end
    translate 1 						    // put bottom front left on origin
    scale < _whitewidth / 2, 0.001, 0.08>
    translate y * -0.002
  }
  box {<0.0002 + _whiteradius, -0.041, 0.002>, <_whitewidth-0.0002- _whiteradius, -0.001, 0.16>}  
  box {<0.0002, -0.041, 0.002+ _whiteradius>, <_whitewidth-0.0002, -0.001, 0.16>}
  cylinder {<0.0002 + _whiteradius, -0.041, 0.002+ _whiteradius>,
    <0.0002 + _whiteradius, -0.001, 0.002 + _whiteradius>, _whiteradius}
  cylinder {<_whitewidth - 0.0002 - _whiteradius, -0.041, 0.002+ _whiteradius>,
    <_whitewidth - 0.0002 - _whiteradius, -0.001, 0.002+ _whiteradius>, _whiteradius}
  bounded_by {box {<0, -0.041, 0>, <_whitewidth, 0, 0.16> }}
}



#local _keyc = difference  {
  object { _whitekey _keywiggle() }
  box {<0.014, 0.01, 0.051>, <0.027, -0.05, 0.170>}
  texture { _t_whitekey }
}
#local _keyd = difference {
  object { _whitekey _keywiggle() translate x * (_whitewidth + 0.001)}
  box {<0.014, 0.01, 0.051>, <0.027, -0.05, 0.170>}
  box {<0.042, 0.01, 0.051>, <0.055, -0.05, 0.170>}
  texture { _t_whitekey }
}
#local _keye = difference {
  object { _whitekey _keywiggle() translate x * 2 * (_whitewidth + 0.001)}
  box {<0.042, 0.01, 0.051>, <0.055, -0.05, 0.170>}
  texture { _t_whitekey }
}
#local _keyf = difference {
  object { _whitekey _keywiggle() translate x * 3 * (_whitewidth + 0.001)}
  box {<0.083, 0.01, 0.051>, <0.097, -0.05, 0.170>}
  texture { _t_whitekey }
}
#local _keyg = difference {
  object { _whitekey _keywiggle()  translate x * 4 * (_whitewidth + 0.001)}
  box {<0.083, 0.01, 0.051>, <0.097, -0.05, 0.170>}
  box {<0.110, 0.01, 0.051>, <0.122, -0.05, 0.170>}
  texture { _t_whitekey }
}
#local _keya = difference {
  object { _whitekey _keywiggle() translate x * 5 * (_whitewidth + 0.001)}
  box {<0.110, 0.01, 0.051>, <0.122, -0.05, 0.170>}
  box {<0.137, 0.01, 0.051>, <0.151, -0.05, 0.170>}
  texture { _t_whitekey }
}
#local _keyb = difference {
  object { _whitekey _keywiggle() translate x * 6 * (_whitewidth + 0.001)}
  box {<0.137, 0.01, 0.051>, <0.151, -0.05, 0.170>}
  texture { _t_whitekey }
}

#local _keylowa = difference {
  object { _whitekey _keywiggle() translate x * 5 * (_whitewidth + 0.001)}
  box {<0.137, 0.01, 0.051>, <0.151, -0.05, 0.170>}
  texture { _t_whitekey }
}
#local _keyhighc = object {
  _whitekey
  _keywiggle()
  texture { _t_whitekey }
}


#macro _octave(which_octave)
  union {
    object {_keyc}
    object {_blackkey translate <0.015, 0, 0.052> texture {_t_blackkey translate z*rand(keyseed)}}
    object {_keyd}
    object {_blackkey translate <0.043, 0, 0.052> texture {_t_blackkey translate z*rand(keyseed)}}
    object {_keye}
    object {_keyf}
    object {_blackkey translate <0.084, 0, 0.052> texture {_t_blackkey translate z*rand(keyseed)}}
    object {_keyg}
    object {_blackkey translate <0.111, 0, 0.052> texture {_t_blackkey translate z*rand(keyseed)}}
    object {_keya}
    object {_blackkey translate <0.138, 0, 0.052> texture {_t_blackkey translate z*rand(keyseed)}}
    object {_keyb}
    translate x * _7octwidth / 7 * which_octave
  }
#end

#ifdef (TEST)
  global_settings {
  // ambient_light 1
  //   ambient_light 0 radiosity {  }
  // max_trace_level 200
  assumed_gamma 1
  }

//  sky_sphere { test_sky_sphere }

  
  camera { location <0.05, 0.05, -0.1> look_at <0.03, 0.015, 0> angle 30 } // two blacks
//  camera { location <0.05, 0.02, -0.1> look_at <0.03, -0.01, 0> angle 30 } // white fronts
//   camera { location <-0.5, 0.4, 0.3> look_at <0.1 0., 0.1> angle 15 } // black texture
//   camera { location <0.06, 0.07, -0.2> look_at <_whitewidth * 3, 0.01, 0> }  // one octave
//  camera { location <0.00, 0.17, -0.2> look_at <0.1, 0.01, 0> } // left end
  //camera { location <_7octwidth*6/7, 0.17, -0.2> look_at <_7octwidth, 0.01, 0> } // right end
  //camera { location <_7octwidth*3/7, 0.17, -0.2> look_at <_7octwidth*4/7, 0.01, 0> } // wide
  plane {  y, -0.1 texture { pigment { checker colour Green colour Yellow} }}
  light_source { <1.7, 0.9, -.4> White }

  union {
    _octave(0)
    _octave(1)
    _octave(2)
    _octave(3)
    _octave(4)
    _octave(5)
    _octave(6)

    // three keys below lowest C
    union {
      object {_keylowa}
      object {_keyb}
      object {_blackkey translate <0.138, 0, 0.052> texture {_t_blackkey translate z*rand(keyseed)}}
      translate x * _7octwidth / 7 * -1
    }
    // highest C
    object {_keyhighc
      translate x * _7octwidth / 7 * 7
    }
    translate x * _keyboardwidth / 52 * 2
  }
  
  box { <0, 0.002, 0.148>, < _keyboardwidth, 0.015, 0.170> 
    texture {
      pigment { rgb 0}
    }
  }
  box { <0, 0.0005, 0.148>, < _keyboardwidth, 0.002, 0.150> 
    texture {
      pigment { rgb x/2}
    }
  }
  
#end
#warning "keys.inc out"
