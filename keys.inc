/* This work is licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/
 */

// Keyboards are about 300-350 deep.
// Stack three high, each offset by about 150, makes about 700 deep.

// 1100 wide, and leave room for stuff in the corner (Jon's especially).

// But leave room for another guy on Rick's right during Comfortably Numb.

// Guess 1m high?

// ffwd to 2014 and uberpov.

// ffwd to 2017 and the Kurzweil K2000.  104cm x 34.1cm x 10.5cm

#version 3.7;

// #include "woods.inc"
#include "dsot.inc"                                         // synthbox
#include "shapes.inc"

#declare keyseed = seed(0);

#declare keys = union {
//  object {glowbox(<1.5, 1.0, 0.7>, Blue) translate (0.7+0.7*sind(22.5))*x}
  object {glowbox(<1.5, 1.0, 0.7>, Blue) translate 0.7*x}
  object {louvre(<1.5, 1.0, 0.02>, 20, 0.9) translate 0.7*x}
  object {glowbox(<-sqrt(0.7*0.7*2), 1.0, 0.7>, Blue) rotate 45*y translate 0.7*x}
  object {louvre(<sqrt(0.7*0.7*2), 1.0, -0.02>, 20, 0.9) rotate 45*y translate 0.7*z}
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

#local _7octwidth = 1.154;			// seven octaves in metres, off a Casio.
#local _keyboardwidth = _7octwidth * 52 / 49;	// width of an 88-key keyboard
#local _keywidth = _7octwidth / 49;             // width of one key and its gap
#local _whitewidth = (_7octwidth / 7 / 7) - 0.001; // 1mm gap
#local _blackwidth = 0.011;				    // width of a black key at base
#local _blackradius = 0.0005;
#local _whiteradius = 0.0003;
#local _blackheight = 0.009;
#local _blackdepth = 0.092;

#local _p_whitekey = colour rgbt <1, 1, 1, 0.005> ; // alpha required for uberpov bug
#local _p_blackkey = colour rgbt <0.02, 0.02, 0.02, 0.005> ; // alpha required for uberpov bug

// White key texture.
// Faux-ivory of a digital piano.
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

// Black key texture.
// This faux-ebony texture of a digital piano is too fancy for a synth.
// Synths are plastic and shinier.
#local _t_blackkey = texture {
  gradient x
  turbulence <0.6, 0, 0>
  texture_map {
    [0   pigment{colour _p_blackkey} finish {specular 1   roughness 0.0001}]
    [0.1 pigment{colour _p_blackkey} finish {specular 1   roughness 0.0001}]
    [0.1 pigment{colour _p_blackkey} finish {specular 0.5 roughness 0.001}]
    [0.5 pigment{colour _p_blackkey} finish {specular 0.5 roughness 0.001}]
    [0.6 pigment{colour _p_blackkey} finish {specular 0.5 roughness 0.001}]
    [0.7 pigment{colour _p_blackkey} finish {specular 0.5 roughness 0.001}]
    [1   pigment{colour _p_blackkey * 0.8 } finish {specular 0}]
  }
  scale <0.003, 0.01, 0.04>
  rotate <0, 0, 10>
};

// rotate by a tiny amount in all three axes to stop all the keys being perfectly placed.
#macro _keywiggle()
  translate -0.2*z
  rotate <(rand(keyseed) - 0.5) * 0.2, (rand(keyseed)-0.5) * 0.1, (rand(keyseed)-0.5) * 0.1 >
  translate 0.2*z
#end

//---------------------------------------------------
// Black key, straight-sided, inclined back and inward, flat top.
// Piano-style.  Synths are more curved.
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
  plane { z, _blackdepth} // and the front.  Very sharp cut-off.
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
    // now, the unsheared part underneath, visible when a white key is down
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
    // No texture.  Add it later, outside the object defn, so it can vary between keys
    // lift it a little to show the transition point between upper and lower parts better.
    // keywiggling hides it a bit.
    translate <_blackwidth / 2, 0.001, 0>
    _keywiggle()
  }

//---------------------------------------------------------------------
// Synth style of black key with front left point at x=0, z=0.
// Top of white keys should be at y=0
#local _k2kblackkey = union {
  difference {
    object {
      superellipsoid { <0.2, 0.3> translate <1, 0, 1> // left- and front-aligned to origin.  y in [-1,1]
        scale <_blackwidth/2, _blackheight * 1, _blackdepth>}
      // shear, lean it away
      matrix <
      1, 0, 0,
      0, 1, 0.8,
      0, 0, 1,
      0, 0, 0 >
      
      rotate 3*x
      _keywiggle()
    }
    plane {-z, -_blackdepth}
  }
  // now, the unsheared part underneath, visible when a white key is down
  box { <0, 0, 0.05>, < _blackwidth, -0.01, 0.10 > }
}

//######################################################################
// Square, traditional, piano key shape
#local _whitekey = union {
  object {
  #ifdef (CHEAP_MUSIC) 
    box {-1, 1}
#else
    superellipsoid { <0.25, 0.05>}
  #end
    translate 1                     // put bottom front left on origin, 2x2x2 cube(ish)
    scale < _whitewidth / 2, 0.001, 0.08>
    translate y * -0.002
  }
  box {<0.0002 + _whiteradius, -0.041, 0.002>, <_whitewidth-0.0002- _whiteradius, -0.001, 0.16>}  
  box {<0.0002, -0.041, 0.002+ _whiteradius>, <_whitewidth-0.0002, -0.001, 0.16>}
  cylinder {<0.0002 + _whiteradius, -0.041, 0.002+ _whiteradius>,
    <0.0002 + _whiteradius, -0.001, 0.002 + _whiteradius>, _whiteradius}
  cylinder {<_whitewidth - 0.0002 - _whiteradius, -0.041, 0.002+ _whiteradius>,
    <_whitewidth - 0.0002 - _whiteradius, -0.001, 0.002+ _whiteradius>, _whiteradius}
//  bounded_by {box {<0, -0.041, 0>, <_whitewidth, 0, 0.16> }}
  blurred_tm(pigment{rgb 1}, finish{
    conserve_energy
    specular 0.1
    roughness 0.05
    reflection {0.1 0.5 fresnel on metallic 0}
  }, 10)
  interior{ior 1.6}
}

// For a synth
#local _k2kwhiteradius = 0.001;
#local _k2kwhitekey = union {
  object {
    synthbox(<_whitewidth, 0.003, 0.16>, 0.003, 0.0008)
    translate y * -0.003
  }
  box {<0.0002 + _k2kwhiteradius, -0.01, 0.002>, <_whitewidth-0.0002 - _k2kwhiteradius, -0.001, 0.16>}  
  box {<0.0002, -0.01, 0.002+ _k2kwhiteradius>, <_whitewidth-0.0002, -0.001, 0.16>}
  cylinder {<0.0002 + _k2kwhiteradius, -0.01, 0.002+ _k2kwhiteradius>,
    <0.0002 + _k2kwhiteradius, -0.001, 0.002 + _k2kwhiteradius>, _k2kwhiteradius}
  cylinder {<_whitewidth - 0.0002 - _k2kwhiteradius, -0.01, 0.002+ _k2kwhiteradius>,
    <_whitewidth - 0.0002 - _k2kwhiteradius, -0.001, 0.002+ _k2kwhiteradius>, _k2kwhiteradius}
  blurred_tm(pigment{rgb 1}, finish{
      conserve_energy
      specular 0.1
      roughness 0.05
      reflection {0.1 0.5 fresnel on metallic 0}
    }, 10)
  interior{ior 1.6}
}

//######################################################################
// Macros returning white key objects and a black key to the right, for CDFGA.
// Pass in both key objects textured.
// This effort so we can have a couple of different key models.
#macro _keybuilderc(_whitey, _blacky)
  union {
    difference  {
      object { _whitey _keywiggle() }
      box {<0.014, 0.01, 0.051>, <0.027, -0.05, 0.170>}
    }
    object {_blacky translate <0.015, 0, 0.052> }
  }
#end
#macro _keybuilderd(_whitey, _blacky)
  union {
    difference {
      object { _whitey _keywiggle() translate x * (_whitewidth + 0.001)}
      box {<0.014, 0.01, 0.051>, <0.027, -0.05, 0.170>}
      box {<0.042, 0.01, 0.051>, <0.055, -0.05, 0.170>}
    }
    object {_blacky translate <0.043, 0, 0.052> }
  }
#end
#macro _keybuildere(_whitey)
  difference {
    object { _whitey _keywiggle() translate x * 2 * (_whitewidth + 0.001)}
    box {<0.042, 0.01, 0.051>, <0.055, -0.05, 0.170>}
  }
#end
#macro _keybuilderf(_whitey, _blacky)
  union {
    difference {
      object { _whitey _keywiggle() translate x * 3 * (_whitewidth + 0.001)}
      box {<0.083, 0.01, 0.051>, <0.097, -0.05, 0.170>}
    }
    object {_blacky translate <0.084, 0, 0.052> }
  }
#end
#macro _keybuilderg(_whitey, _blacky)
  union {
    difference {
      object { _whitey _keywiggle()  translate x * 4 * (_whitewidth + 0.001)}
      box {<0.083, 0.01, 0.051>, <0.097, -0.05, 0.170>}
      box {<0.110, 0.01, 0.051>, <0.122, -0.05, 0.170>}
    }
    object {_blacky translate <0.111, 0, 0.052> }
  }
#end
#macro _keybuildera(_whitey, _blacky)
  union {
    difference {
      object { _whitey _keywiggle() translate x * 5 * (_whitewidth + 0.001)}
      box {<0.110, 0.01, 0.051>, <0.122, -0.05, 0.170>}
      box {<0.137, 0.01, 0.051>, <0.151, -0.05, 0.170>}
    }
    object {_blacky translate <0.138, 0, 0.052>}
  }
#end
#macro _keybuilderb(_whitey)
  difference {
    object { _whitey _keywiggle() translate x * 6 * (_whitewidth + 0.001)}
    box {<0.137, 0.01, 0.051>, <0.151, -0.05, 0.170>}
  }
#end
// An A with no G# subtracted from it.
#macro _keybuilderlowa(_whitey, _blacky)
  difference {
    object { _whitey _keywiggle() translate x * 5 * (_whitewidth + 0.001)}
    box {<0.137, 0.01, 0.051>, <0.151, -0.05, 0.170>}
  }
#end
// A C with no C# subtracted from it.
#macro _keybuilderhighc(_whitey)
  object { _whitey _keywiggle() }
#end

//######################################################################
// Traditional shape keys.
// These are objects, not macros, so hopefully save a bit of mem.
#local _keyc = _keybuilderc(_whitekey, object {_blackkey texture {_t_blackkey translate z*rand(keyseed)}})
#local _keyd = _keybuilderd(_whitekey, object {_blackkey texture {_t_blackkey translate z*rand(keyseed)}})
#local _keye = _keybuildere(_whitekey)
#local _keyf = _keybuilderf(_whitekey, object {_blackkey texture {_t_blackkey translate z*rand(keyseed)}})
#local _keyg = _keybuilderg(_whitekey, object {_blackkey texture {_t_blackkey translate z*rand(keyseed)}})
#local _keya = _keybuildera(_whitekey, object {_blackkey texture {_t_blackkey translate z*rand(keyseed)}})
#local _keyb = _keybuilderb(_whitekey)
#local _keylowa = _keybuilderlowa(_whitekey, object {_blackkey texture {_t_blackkey translate z*rand(keyseed)}})
#local _keyhighc = _keybuilderhighc(_whitekey)

#macro _octave(which_octave)
  union {
    object {_keyc}
    object {_keyd}
    object {_keye}
    object {_keyf}
    object {_keyg}
    object {_keya}
    object {_keyb}
    translate x * _7octwidth / 7 * which_octave
  }
#end

#declare keyboard88 = object {
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
    // highest C, tacked on the end
    object {_keyhighc
      translate x * _7octwidth
    }
    // Shift it right to move the left side of the low A to x=0
    // 52 white keys on an 88-key kbd
    translate x * _keyboardwidth / 52 * 2
  }
}

//######################################################################
// Synth keys
#local _k2kkeyc = _keybuilderc(_k2kwhitekey, object {_k2kblackkey texture {_t_blackkey translate z*rand(keyseed)}})
#local _k2kkeyd = _keybuilderd(_k2kwhitekey, object {_k2kblackkey texture {_t_blackkey translate z*rand(keyseed)}})
#local _k2kkeye = _keybuildere(_k2kwhitekey)
#local _k2kkeyf = _keybuilderf(_k2kwhitekey, object {_k2kblackkey texture {_t_blackkey translate z*rand(keyseed)}})
#local _k2kkeyg = _keybuilderg(_k2kwhitekey, object {_k2kblackkey texture {_t_blackkey translate z*rand(keyseed)}})
#local _k2kkeya = _keybuildera(_k2kwhitekey, object {_k2kblackkey texture {_t_blackkey translate z*rand(keyseed)}})
#local _k2kkeyb = _keybuilderb(_k2kwhitekey)
#local _k2kkeylowa = _keybuilderlowa(_k2kwhitekey, object {_k2kblackkey texture {_t_blackkey translate z*rand(keyseed)}})
#local _k2kkeyhighc = _keybuilderhighc(_k2kwhitekey)

#macro _k2koctave(which_octave)
  union {
    object {_k2kkeyc}
    object {_k2kkeyd}
    object {_k2kkeye}
    object {_k2kkeyf}
    object {_k2kkeyg}
    object {_k2kkeya}
    object {_k2kkeyb}
    translate x * _7octwidth / 7 * which_octave
  }
#end

#declare keyboard61width = _keyboardwidth / 52 * 36;  // call it 84.5cm
#declare k2kkeyboard61 = object {
  union {
    _k2koctave(0)
    _k2koctave(1)
    _k2koctave(2)
    _k2koctave(3)
    _k2koctave(4)

    // highest C, tacked on the end
    object {_k2kkeyhighc
      translate x * _7octwidth / 7 * 5
    }
  }
}

#if (1)
  global_settings {
    // ambient_light 1
    //ambient_light 0 radiosity {  }
    // max_trace_level 200
    assumed_gamma 1
  }

    sky_sphere { test_sky_sphere }
  
  // camera { location <0.09, 0.05, -0.1> look_at <0.07, 0.015, 0> angle 30 } // two blacks
  // camera { location <0.05, 0.02, -0.1> look_at <0.03, -0.01, 0> angle 30 } // white fronts
  //   camera { location <-0.5, 0.4, 0.3> look_at <0.1 0., -0.1> angle 15 } // black texture
  // camera { location <0.1, 0.07, -0.15> look_at <_whitewidth * 4, 0.01, 0> }  // one octave
  // camera { location <0.00, 0.17, -0.2> look_at <0.1, 0.01, 0> } // left end
  //camera { location <_7octwidth*6/7, 0.17, -0.2> look_at <_7octwidth, 0.01, 0> } // right end
  camera { location <0, 0.22, 0.15> look_at <_7octwidth*3/7, 0, 0.15> angle 30} // gaps behind piano blacks
  //camera { location <_7octwidth*3/7, 0.17, -0.2> look_at <_7octwidth*4/7, 0.01, 0> } // wide
  //camera { location <_7octwidth*-1/7, 0.22, -0.55> look_at <_7octwidth*1.2/7, 0, -0.15> angle 10} // synth black shape
  //camera { location <0, 0.22, -0.15> look_at <_7octwidth*3/7, 0, -0.15> angle 30} // gaps behind synth blacks
  camera { location <_7octwidth*-1/7, 0.22, -0.5> look_at <_7octwidth*1.5/7, 0.01, -0.1> angle 15} // wider

  plane {  y, -0.1 texture { pigment { checker colour Green colour Yellow} }}
  light_source { <-1.7, 0.9, -.4> White }

  object {keyboard88}
  box { <0, 0.002, 0.148>, < _keyboardwidth, 0.015, 0.170> pigment { rgb 0.1} }
  box { <0, 0.0005, 0.148>, < _keyboardwidth, 0.002, 0.150> pigment { rgb x/2} }

   union {
     object {k2kkeyboard61}
     box { <0, 0.002, 0.148>, < keyboard61width, 0.015, 0.160> pigment { rgb 0.1} }
     box { <0, 0.0005, 0.148>, < keyboard61width, 0.002, 0.150> pigment { rgb x/2} }
     translate z * -0.25
   }

#end

#if (0)
  global_settings {
    // ambient_light 1
    // ambient_light 0 radiosity {  }
    // max_trace_level 200
    assumed_gamma 1
  }

  camera { location <-0.1, 0.30, -0.5> look_at <0.3, 0.01, 0.1> }
//  camera { location <1.06, 0.30, -0.5> look_at <1, 0.01, 0.1> } // right corner
//  camera { location <1.06, 0.30, 0> look_at <1, 0.01, 0.3> } // right internal corner
//  camera { location <1.06, 0.30, -0.3> look_at <1, 0.01, 0> } // right corner
//  camera { location <0.1, 0.30, 0> look_at <0.3, 0.01, 0.3> } // left rear internal corner
//  camera { location <-0.5, 0.10, -0> look_at <0, 0.1, 0.1> } // checking tilt


  plane {  y, -0.1 texture { pigment { checker colour Green colour Yellow} }}
  light_source { <1.7, 0.9, -.4> White }
  light_source { <-1.7, 0.9, -.4> Red }

  // wish I was doing this in cm
  union {
    union {
      difference {
        union {
          difference { // top piece of body
            object { synthbox(<1.04, 0.05, 0.34>, 0.01, 0.002) }
            // subtract void for keyboard and controls
            box {<0.14, -Eps, -Eps> < 1.01, 0.05+Eps, 0.31 >}
          }
          object { Round_Box_Union(<0.13, 0, 0>, <0.16, 0.05, 0.33>, 0.002) }
          // This 1.007 is 0.16 plus keyboard61width, plus a gap
          object { Round_Box_Union(<1.007, 0, 0>, <1.02, 0.05, 0.33>, 0.002) }
          object { Round_Box_Union(<0.15, 0, 0.30>, <1.01, 0.05, 0.33>, 0.002) }
        }
        // subtract sloped under - will rotate underside to horizontal later
        plane {y 0 rotate 3 * x translate 0.34*z}
        rotate -3*x // rotate underside to horizontal
        // nudge forward to partially account for that tilt, get everying back on +z
        // and nudge right to let the base be a little larger
        // 7cm * sin(3 degrees) is 4mm, near enough
         translate <0.0001, 0.01, 0.004>
      }
      // bottom piece of body (well - middle)
      object { synthbox(<1.042, 0.0176, 0.342>, 0.01, 0.002) translate <0, 0.01, 0.003>}
      // very bottom piece
      object { synthbox(<1.02, 0.01, 0.32>, 0.01, 0.004) translate <0.01, 0, 0.01>}
      texture {_t_whitekey}
    }
    object {keyboard61 translate <0.16, 0.05, -0.0>}
  }

#end

#warning "keys.inc out"
