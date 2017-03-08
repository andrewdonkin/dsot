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

#local _7octwidth = 115.4;			// seven octaves in metres, off a Casio.
#local _keyboardwidth = _7octwidth * 52 / 49;	// width of an 88-key keyboard
#local _keywidth = _7octwidth / 49;             // width of one key and its gap
#local _whitewidth = (_7octwidth / 7 / 7) - 000.1; // 1mm gap
#local _blackwidth = 001.1;				    // width of a black key at base
#local _blackradius = 000.05;
#local _whiteradius = 000.03;
#local _blackheight = 000.9;
#local _blackdepth = 009.2;

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
  scale <000.2, 000.1, 004>
  // rotate <0, 1, 0>
  translate <100, 001., 0>
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
  scale <000.3, 001., 004.>
  rotate <0, 0, 10>
};

// rotate by a tiny amount in all three axes to stop all the keys being perfectly placed.
#macro _keywiggle()
  translate -020*z
  rotate <(rand(keyseed) - 0.5) * 0.2, (rand(keyseed)-0.5) * 0.01, (rand(keyseed)-0.5) * 0.01 >
  translate 020*z
#end

//######################################################################
// The Black Keys

// Straight-sided, inclined back and inward, flat top.
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
      0, 100*z/3, _blackradius
      translate <+ (_blackwidth/2 - _blackradius), _blackheight - _blackradius, _blackradius>
    }
    cylinder {						    // left top
      0, 100*z/3, _blackradius
      translate <- (_blackwidth/2 - _blackradius), _blackheight - _blackradius, _blackradius>
    }
   box { 0, < _blackwidth - 2*_blackradius, _blackheight - _blackradius, 100/3> // front
     translate x * -(_blackwidth / 2 - _blackradius)
    }
    box { _blackradius * z, <_blackwidth, _blackheight - _blackradius, 100/3>  // sides
      translate <- _blackwidth/2, 0, 0>
    }

   box { _blackradius * z, < _blackwidth - 2 * _blackradius, _blackheight, 100/3> // top
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
      box { 0, < _blackwidth - 2 * _blackradius, -001., 001. > // front
	translate < - _blackwidth/2 + _blackradius, 0, 0>
      }
      box { 0, < _blackwidth, -001., 010. > // sides
	translate < - _blackwidth / 2, 0, _blackradius>
      }
    }
    // No texture.  Add it later, outside the object defn, so it can vary between keys
    // lift it a little to show the transition point between upper and lower parts better.
    // keywiggling hides it a bit.
    translate <_blackwidth / 2, 000.1, 0>
    _keywiggle()
  }

//---------------------------------------------------------------------
// Synth style of black key with front left point at x=0, z=0.
// Top of white keys should be at y=0
#local _k2kblackkey = union {
  difference {
    object {
      superellipsoid { <0.3, 0.3> translate <1, 0, 1> // left- and front-aligned to origin.  y in [-1,1]
        scale <_blackwidth/2, _blackheight * 1, _blackdepth>}
      // shear, lean it away
      matrix <
      1, 0, 0,
      0, 1, 0.7,
      0, 0, 1,
      0, 0, 0 >
      
      rotate 3*x
      _keywiggle()
    }
    plane {-z, -_blackdepth}
  }
  // now, the unsheared part underneath, visible when a white key is down
  box { <0, 0, 005.>, < _blackwidth, -001., 010. > }
  texture {
    pigment {rgb <0,0,0>}
    finish {
      conserve_energy
      specular 0.5
      roughness 0.01
      reflection {0.01 0.1 fresnel on}
    }
  }
  interior { ior 3 }
}

//######################################################################
// The White Stripes

#local _t_whitekey = texture {
    pigment{rgb 1}
    finish {
      conserve_energy
      specular 0.1
      roughness 0.05
      reflection {0.01 0.2 fresnel on metallic 0}
    }
  }

// Square, traditional, piano key shape
#local _whitekey = union {
  object {
    superellipsoid { <0.25, 0.05> }
      texture { _t_whitekey
    normal {
      marble 0.05
      warp { turbulence 0.3 }
      sine_wave
      scale 1/2000 // wavelength of each ripple
    }
  }
    translate 1                     // put bottom front left on origin, 2x2x2 cube(ish)
    scale < _whitewidth / 2, 000.1, 008.>
    translate y * -000.2
  }
  box {<000.02 + _whiteradius, -004.1, 000.2>, <_whitewidth-000.02- _whiteradius, -000.1, 016.>}  
  box {<000.02, -004.1, 000.2+ _whiteradius>, <_whitewidth-000.02, -000.1, 016.>}
  cylinder {<000.02 + _whiteradius, -004.1, 000.2+ _whiteradius>,
    <000.02 + _whiteradius, -000.1, 000.2 + _whiteradius>, _whiteradius}
  cylinder {<_whitewidth - 000.02 - _whiteradius, -004.1, 000.2+ _whiteradius>,
    <_whitewidth - 000.02 - _whiteradius, -000.1, 000.2+ _whiteradius>, _whiteradius}
//  bounded_by {box {<0, -004.1, 0>, <_whitewidth, 0, 016.> }}
  texture { _t_whitekey } // minus the normal that the key top has - too much moire
  interior{ior 3} // needed for fresnel reflections
}

//----------------------------------------------------------------------
// White synth key
#local _k2kwhiteradius = 0.001;
#local _k2kwhitekey = union {
  object {
    synthbox(<_whitewidth, 000.3, 016.>, 000.3, 000.08)
    translate y * -000.3
  }
  box {<000.02 + _k2kwhiteradius, -001., 000.2>, <_whitewidth-000.02 - _k2kwhiteradius, -000.1, 016.>}  
  box {<000.02, -001., 000.2+ _k2kwhiteradius>, <_whitewidth-000.02, -000.1, 016.>}
  cylinder {<000.02 + _k2kwhiteradius, -001., 000.2+ _k2kwhiteradius>,
    <000.02 + _k2kwhiteradius, -000.1, 000.2 + _k2kwhiteradius>, _k2kwhiteradius}
  cylinder {<_whitewidth - 000.02 - _k2kwhiteradius, -001., 000.2+ _k2kwhiteradius>,
    <_whitewidth - 000.02 - _k2kwhiteradius, -000.1, 000.2+ _k2kwhiteradius>, _k2kwhiteradius}
  texture {
    average texture_map {
	  #local i=20; #while (i)
	    [ 1 pigment { rgb 1 }
	      normal {bozo 0.3 translate <rand(bseed),rand(bseed),rand(bseed)> scale 0.001}
	      finish{ conserve_energy specular 0.1 roughness 0.05 reflection {0.1 0.5 fresnel on metallic 0}}
	    ]
	    #local i=i-1; #end
	}
	}
	//pigment{rgb 1}
  //finish{ conserve_energy specular 0.1 roughness 0.05 reflection {0.1 0.5 fresnel on metallic 0}}
  interior{ior 1.6}
  // Pretty fly for a white key
}

//######################################################################
// Macros returning white key objects and a black key to the right, for CDFGA.
// Pass in both key objects textured.
// This effort so we can have a couple of different key models.
#macro _keybuilderc(_whitey, _blacky)
  union {
    difference  {
      object { _whitey _keywiggle() }
      box {<001.4, 001., 005.1>, <002.7, -005., 017.0>}
    }
    object {_blacky translate <001.5, 0, 005.2> }
  }
#end
#macro _keybuilderd(_whitey, _blacky)
  union {
    difference {
      object { _whitey _keywiggle() translate x * (_whitewidth + 000.1)}
      box {<001.4, 001., 005.1>, <002.7, -005., 017.0>}
      box {<004.2, 001., 005.1>, <005.5, -005., 017.0>}
    }
    object {_blacky translate <004.3, 0, 005.2> }
  }
#end
#macro _keybuildere(_whitey)
  difference {
    object { _whitey _keywiggle() translate x * 2 * (_whitewidth + 000.1)}
    box {<004.2, 001., 005.1>, <005.5, -005., 017.0>}
  }
#end
#macro _keybuilderf(_whitey, _blacky)
  union {
    difference {
      object { _whitey _keywiggle() translate x * 3 * (_whitewidth + 000.1)}
      box {<008.3, 001., 005.1>, <009.7, -005., 017.0>}
    }
    object {_blacky translate <008.4, 0, 005.2> }
  }
#end
#macro _keybuilderg(_whitey, _blacky)
  union {
    difference {
      object { _whitey _keywiggle()  translate x * 4 * (_whitewidth + 000.1)}
      box {<008.3, 001., 005.1>, <009.7, -005., 017.0>}
      box {<011.0, 001., 005.1>, <012.2, -005., 017.0>}
    }
    object {_blacky translate <011.1, 0, 005.2> }
  }
#end
#macro _keybuildera(_whitey, _blacky)
  union {
    difference {
      object { _whitey _keywiggle() translate x * 5 * (_whitewidth + 000.1)}
      box {<011.0, 001., 005.1>, <012.2, -005., 017.0>}
      box {<013.7, 001., 005.1>, <015.1, -005., 017.0>}
    }
    object {_blacky translate <013.8, 0, 005.2>}
  }
#end
#macro _keybuilderb(_whitey)
  difference {
    object { _whitey _keywiggle() translate x * 6 * (_whitewidth + 000.1)}
    box {<013.7, 001., 005.1>, <015.1, -005., 017.0>}
  }
#end
// An A with no G# subtracted from it.
#macro _keybuilderlowa(_whitey, _blacky)
  difference {
    object { _whitey _keywiggle() translate x * 5 * (_whitewidth + 000.1)}
    box {<013.7, 001., 005.1>, <015.1, -005., 017.0>}
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
      object {_blackkey translate <013.8, 0, 005.2> texture {_t_blackkey translate z*rand(keyseed)}}
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
#local _k2kkeyc = _keybuilderc(_k2kwhitekey, object {_k2kblackkey })
#local _k2kkeyd = _keybuilderd(_k2kwhitekey, object {_k2kblackkey })
#local _k2kkeye = _keybuildere(_k2kwhitekey)
#local _k2kkeyf = _keybuilderf(_k2kwhitekey, object {_k2kblackkey })
#local _k2kkeyg = _keybuilderg(_k2kwhitekey, object {_k2kblackkey })
#local _k2kkeya = _keybuildera(_k2kwhitekey, object {_k2kblackkey })
#local _k2kkeyb = _keybuilderb(_k2kwhitekey)
#local _k2kkeylowa = _keybuilderlowa(_k2kwhitekey, object {_k2kblackkey })
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
    // ambient_light 0
    // ambient_light 0 radiosity {  }
    // max_trace_level 200
    assumed_gamma 1
  }

    sky_sphere { test_sky_sphere }
  
  // camera { location <009., 005., -01>. look_at <007., 001.5, 0> angle 30 } // two blacks
  // camera { location <005., 002., -01>. look_at <003., -001., 0> angle 30 } // white fronts
  //   camera { location <-05,. 04,. 03>. look_at <01 .0, .-01>. angle 15 } // black texture
  // camera { location <01,. 007., -015.> look_at <_whitewidth * 4, 001., 0> }  // one octave
  // camera { location <000., 017., -02>. look_at <01,. 001., 0> } // left end
  //camera { location <_7octwidth*6/7, 017., -02>. look_at <_7octwidth, 001., 0> } // right end
  //camera { location <0, 022., 015.> look_at <_7octwidth*3/7, 0, 015.> angle 30} // gaps behind piano blacks
  //camera { location <_7octwidth*3/7, 017., -02>. look_at <_7octwidth*4/7, 001., 0> } // wide
  camera { location <_7octwidth*-1/7, 022., -055.> look_at <_7octwidth*1/7, 0, -015.> angle 15} // synth black shape
  //camera { location <_7octwidth*-1/7, 022., -055.> look_at <_7octwidth*0.8/7, 0, -014.> angle 8} // synth blur
  //camera { location <0, 022., -015.> look_at <_7octwidth*3/7, 0, -015.> angle 30} // gaps behind synth blacks
  //camera { location <_7octwidth*-1/7, 022., -050> look_at <_7octwidth*1/7, 00, -05.> angle 35} // wider

  plane {  y, -010. texture { pigment { checker colour rgb<0.8,0,0.8> colour rgb<1,1,1> scale 010.} }}
  light_source { <-170., 090., -40.> White }

  object {keyboard88}
  box { <0, 000.2, 014.8>, < _keyboardwidth, 001.5, 017.0> pigment { rgb 0.1} }
  box { <0, 000.05, 014.8>, < _keyboardwidth, 000.2, 015.0> pigment { rgb x/2} }

   union {
     object {k2kkeyboard61}
     box { <0, 000.2, 014.8>, < keyboard61width, 001.5, 016.0> pigment { rgb 0.1} }
     box { <0, 000.05, 014.8>, < keyboard61width, 000.2, 015.0> pigment { rgb x/2} }
     translate z * -025.
   }

#end

#if (0)
  global_settings {
    // ambient_light 1
    // ambient_light 0 radiosity {  }
    // max_trace_level 200
    assumed_gamma 1
  }

  camera { location <-010., 030., -050.> look_at <030., 001., 010.> }
//  camera { location <106., 030., -050.> look_at <1, 001., 010.> } // right corner
//  camera { location <106., 030., 0> look_at <1, 001., 030.> } // right internal corner
//  camera { location <106., 030., -030> look_at <1, 001., 0> } // right corner
//  camera { location <010., 030., 0> look_at <030., 001., 030.> } // left rear internal corner
//  camera { location <-050., 010., -0> look_at <0, 010., 010.> } // checking tilt


  plane {  y, -010. texture { pigment { checker colour Green colour Yellow} }}
  light_source { <170., 090., -40.> White }
  light_source { <-170., 090., -40.> Red }

  // wish I was doing this in cm
  union {
    union {
      difference {
        union {
          difference { // top piece of body
            object { synthbox(<104., 005., 034.>, 001., 000.2) }
            // subtract void for keyboard and controls
            box {<014., -Eps, -Eps> < 101., 005.+Eps, 031. >}
          }
          object { Round_Box_Union(<013., 0, 0>, <016., 005., 033.>, 000.2) }
          // This 100.7 is 016. plus keyboard61width, plus a gap
          object { Round_Box_Union(<100.7, 0, 0>, <102., 005., 033.>, 000.2) }
          object { Round_Box_Union(<015., 0, 030.>, <101., 005., 033.>, 000.2) }
        }
        // subtract sloped under - will rotate underside to horizontal later
        plane {y 0 rotate 3 * x translate 034.*z}
        rotate -3*x // rotate underside to horizontal
        // nudge forward to partially account for that tilt, get everying back on +z
        // and nudge right to let the base be a little larger
        // 7cm * sin(3 degrees) is 4mm, near enough
         translate <000.01, 001., 000.4>
      }
      // bottom piece of body (well - middle)
      object { synthbox(<104.2, 001.76, 034.2>, 001., 000.2) translate <0, 001., 000.3>}
      // very bottom piece
      object { synthbox(<102., 001., 032.>, 001., 000.4) translate <001., 0, 001.>}
      texture {_t_whitekey}
    }
    object {keyboard61 translate <016., 005., -0.0>}
  }

#end

#warning "keys.inc out"
