/* (c) 1988-2020 Andrew Donkin.
 * This work is licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/
 */
// http://www.mikesguitarsite.co.uk/gear/pink_floyd/?page=effects

#ifdef(TEST)
  #undef TEST
  #include "dsot.inc"
  #declare TEST=1;
#end

#include "shapes.inc"
#include "metals.inc"
#declare guitarseed = seed(0);

// Power cable, plug faces -Z on X=Y=0, cable goes -Y.
#declare stompboxpower = 
  union {
    // Lots'o'zeros
    cylinder{-0.001*z, 0.001*z, 0.002 texture {chrometexture}}
    Round_Cylinder_Union(0.001*z, 0.007*z, 0.003, 0.0005)
    Round_Box_Union(<-0.003, -0.006, 0.002>, <0.003, 0, 0.007>, 0.0005)
    Round_Box_Union(<-0.002, -0.005, 0.002>, <0.002, -0.009, 0.007>, 0.0005)
    Round_Box_Union(<-0.002, -0.009, 0.003>, <0.002, -0.0101, 0.006>, 0.0005)
    Round_Box_Union(<-0.002, -0.011, 0.003>, <0.002, -0.0121, 0.006>, 0.0005)
    Round_Box_Union(<-0.002, -0.013, 0.003>, <0.002, -0.0141, 0.006>, 0.0005)
    cylinder{<-0.001, -0.01, 0.005>, <-0.001, -0.05, 0>, 0.001}
    cylinder{<0.001, -0.01, 0.005>, <0.001, -0.05, 0>, 0.001}
    texture {cable_t}
  }

#declare knurled_knob =
  union {
    cone {-0.5*z, 4, -5*z, 3}
    cylinder {0, z, 2}
    #local i=0; #while (i<=360) #local i=i+20;
      object {Round_Cylinder_Union (<0, 3, -5>, <0, 4, -0.5>, 1, 0.5) rotate i*z}
    #end
    texture {pigment {rgb 0.1} finish {reflection 0.05 specular 1 roughness 0.001}}
    scale 1/1000		// millimetres please
  }

#macro _sboxcable(xlen, zlen, twist)
  union {
    union {
      xcyl(0.004, -0.013)
      sphere{-0.013*x, 0.0005} // Tiny bump
      union {
	superellipsoid { <1, 0.4> rotate 90*y scale <0.004, 0.01, 0.01>}
	torus { 0.0095, 0.001 rotate 90*z}
	translate -0.008*x
      }
      texture {chrometexture}
    }
    union {
      object {zcyl(0.004, 0.02)}
      object {zcyl(0.0035, 0.03)}
      cylinder{0.015*z, (zlen-0.015)*z, 0.003}
      intersection {
	torus {0.015, 0.003}
	box {<-0.03, -0.03, 0> <0, 0.01, 0.03>}
	translate <0.015, 0, zlen-0.015>
      }
      #if (xlen > 0.03) cylinder{<0.015, 0, zlen>, <xlen-0.015, 0, zlen>, 0.003} #end
      intersection {
	torus {0.015, 0.003 rotate 90*x}
	box {<0, 0, -0.01>, <0.03, 0.03, 0.01>}
	translate <xlen-0.015, -0.015, zlen>
      }
      // Stump into y=0
      cylinder {<0, -0.015, 0> <0, -0.03, 0> 0.003 translate <xlen, 0, zlen>}
      rotate twist*z
      translate -0.008*x
      texture {cable_t}
    }
  }
#end

#macro _boxknob(typ, colr)
  union {
    object {Round_Cylinder_Union(0, 0.022*y, 0.007, 0.001)
      texture {rubbertexture scale <1,3,1>}
      texture {
	pigment {
	  function{abs(min(y,1))} turbulence 0.3  colour_map {
	    [0.0 rgbt <0, 0, 0, 1>]
	    [0.5 rgbt <0, 0, 0, 1>]
	    [1 rgbt 0.1]
	  }
	  translate rand(guitarseed)*z
	  scale 0.022
	}
      }
    }
    object {Round_Box_Union(<-0.0006, 0, 0.001>, <0.0005, 0.0175, 0.0075>, 0.0005)
      texture{ pigment { function{clip(y, 0, 1)}
	colour_map {
	  [0 colr] [1 colr / 2]
	} } 
	scale <1, 0.0175, 1>
      }
      translate 0.005*y
    }
    #if(typ>0.7)
      object {Round_Cylinder_Union(0, 0.003*y, 0.006, 0.0005)
	texture {pigment { function{clip(x*x+z*z, 0, 1)}
	  warp{turbulence 0.2}
	  colour_map {
	    [0 colr] [1 colr / 2]
	  }}
	  scale 0.006
	  rotate 360*rand(guitarseed)*y
	}
	translate 0.02*y
      }
    #end
    rotate (-30+60*rand(guitarseed))*y
  }
#end


//######################################################################
// Fairly generic-looking stomp box, very approximately 70*50*125mm.
// Extends from 0 to +XYZ, some trim in -X and -Z.
#macro stompbox(colr)
  #local knobcolr = (2*colr + <1,1,1>)/3;
  stompbox2(colr, knobcolr)
#end

#macro stompbox2(colr, knobcolr)
  #if(0)
    #local sb_t = blurred_tm(
      pigment {
	bumps turbulence 3
	colour_map {
	  [0.0 colr]
	  [1.0 colr*0.8]
	}
      scale 1/10
      },
      finish {specular 0.6 roughness 0.1 reflection 0.2 metallic}
    )
  #else
    #local sb_t = texture{	// Faster than above, for previews.
      pigment{colr}
      finish {specular 0.3 roughness 0.1 reflection 0.2 metallic}
      normal { dents 0.01 scale 0.01}
  };
  #end
  
  union {
    union {			// The pedal.
      superellipsoid { <0.1, 0.1> scale 0.5 texture{sb_t translate -0.5*y} scale <0.1, 0.04, 0.11> }
      // rubber stomp pad
  // Picture a really cheap door mat, like at a camping ground, or
  // in the footwell of my grandmother's 120Y.
  //
  // A bump map would have done, since the contours don't reach the
  // edges.
      blob {
	#local i = 11; #while (i) #local i = i - 1;
	  cylinder {0.04*x + i*z/10, 0.96*x + i*z/10, 0.08 strength 1}
	  #if (i)
	    sphere {0.96*x + i*z/10 - z/20, 0.06 strength 1}
	    sphere {0.04*x + i*z/10 - z/20, 0.06 strength 1}
	  #end
	#end
	threshold 0.5
	texture {rubbertexture}
	texture {
	  pigment {
	    function{clip(z*x*abs(x-1),0,1)} warp{turbulence 0.2} colour_map {
	      [0.0 rgbt <0, 0, 0, 0.4>]
	      [0.2 rgbt <0, 0, 0, 1>]
	    }
	  }
	}
	scale <0.09, 0.02, 0.06>
	translate <-0.045, 0.02, -0.05>
      }
      // Words
      box {0, <0.025 + 0.01*rand(guitarseed), 0.0001, 0.005>
	translate <-0.04, 0.02, 0.04> texture {rubbertexture}}
      box {0, <0.025 + 0.01*rand(guitarseed), 0.0001, 0.005>
	translate <-0.04, 0.02, 0.03> texture {rubbertexture}}
      box {0, <-(0.025 + 0.01*rand(guitarseed)), 0.0001, 0.005>
	translate <0.04, 0.02, 0.04> texture {rubbertexture}}
      box {0, <-(0.025 + 0.01*rand(guitarseed)), 0.0001, 0.005>
	translate <0.04, 0.02, 0.03> texture {rubbertexture}}
      // Clip on the front.  Player-side.
      object{Round_Box_Union(<0,0,0>, <0.01, 0.01, 0.005>, 0.002) translate <-0.005, -0.01, -0.056>
	texture {blackplastictexture}}
      object{Round_Cylinder_Union(0, 0.01*z, 0.004, 0.002) translate <0, -0.005, -0.063>
	texture {rubbertexture}}
      translate <0, -0.04, -0.11>/2 // Get the top front to X axis for pitching
      rotate (6+rand(guitarseed)*8)*x
      translate -<0, -0.04, -0.11>/2 
      translate <0, 0.008, -0.04>
    }
    // Something to match the clip on the pedal
    difference {
      object{Round_Box_Union(-<0.005,0.005,0>, <0.005, 0.005, 0.005>, 0.002)}
      cylinder{-0.01*z, 0.01*z, 0.002}
      translate <0, -0.01, -0.092>
      texture {blackplastictexture}
    }
    // One cable in the left...
    union {
      object {xcyl(0.004, -0.01)}
      superellipsoid { <1, 0.60> rotate 90*y scale <0.006, 0.01, 0.01>
	translate -0.01*x
	texture {chrometexture}
      }
      union {
	#local varilength = rand(guitarseed)*3/100;
	#local zoff=0.05+rand(guitarseed)*0.01;
	object {zcyl(0.005, 0.02)}
	object {zcyl(0.004, 0.03)}
	cylinder{0.02*z, zoff*z, 0.003}
	intersection {
	  torus {0.02, 0.003}
	  box {<-0.03, -0.03, 0> <0, 0.01, 0.03>}
	  translate <0.02, 0, zoff>
	}
	cylinder{<0.02, 0, zoff+0.02>, <0.021+varilength, 0, zoff+0.02>, 0.003}
	intersection {
	  torus {0.015, 0.003 rotate 90*x}
	  box {<0, 0, -0.01>, <0.03, 0.03, 0.01>}
	  translate <0.021+varilength, -0.015, zoff+0.02>
	}
	// Stump into y=0
	cylinder {<0, -0.015, 0> <0, -0.03, 0> 0.003 translate <0.015+0.021+varilength, 0, zoff+0.02>}
	rotate (5+rand(guitarseed)*10)*x
	translate -0.01*x
	texture {cable_t}
      }
      translate <-0.045, 0, 0.03>
    }
    // Two cables out the right
    object{xcyl(0.006, -0.01) translate <0.05, -0.005, 0.02>}
    object{xcyl(0.004, -0.01) translate <0.055, -0.005, 0.02>}
    union {
      intersection {
	torus {0.02, 0.003}
	box {<0, -0.01, -0.03>, <0.03, 0.01, 0>}
	translate 0.02*z 
      }
      intersection {
	torus {0.01, 0.003 rotate 90*z}
	box {<-0.01, 0.03, 0>, <0.01, -0.03, 0.03>}
	translate <0.02, -0.01, 0.02>
      }
      scale<0.7,1,1> 
      rotate (20+70*rand(guitarseed))*x 
      translate <0.053, -0.005, 0.02>
      texture {cable_t}
    }
    object{xcyl(0.006, -0.01) translate <0.05, -0.005, 0.035>}
    object{xcyl(0.004, -0.01) translate <0.055, -0.005, 0.035>}
    union {
      intersection {
	torus {0.02, 0.003}
	box {<0, -0.01, 0> <0.03, 0.01, -0.03>}
	translate 0.02*z
      }
      cylinder {<0.02, 0, 0.02>, <0.02, 0, 0.035>, 0.003}
      intersection {
	torus {0.03, 0.003}
	box {<0, -0.01, 0> <0.04, 0.01, 0.04>}
	translate <-0.01, 0, 0.035>
      }
      // Varilength = [0,0.03)
      #local varilength = 0.03-varilength+(zoff-0.05);
      // Varilength = (0,0.03].
      cylinder {<-0.01, 0, 0.065>, <-0.02-varilength, 0, 0.065>, 0.003}
      intersection {
	torus {0.015, 0.003 rotate 90*x}
	box {<0, 0, -0.01>, <-0.03, 0.03, 0.01>}
	translate <-0.02-varilength, -0.015, 0.065>
      }
      // Stump into y=0
      cylinder {<0, -0.015, 0> <0, -0.03, 0> 0.003 translate <-0.015-0.02-varilength, 0, 0.065>}
      
      rotate 10*rand(guitarseed)*x
      translate <0.055, -0.005, 0.035>
      texture {cable_t}
    }

    // Power cable out the top
    object { stompboxpower translate 0.09*z}

    // GREENLED means three knobs instead of four
    #if (rand(guitarseed) > 0.5)
      #local REDLED = 1;
      #if (rand(guitarseed) > 0.5)
	#local GREENLED = 1;
      #else
	#local GREENLED = 0;
      #end
    #else
      #local REDLED = 0;
      #local GREENLED = 1;
    #end
    
    light_group {
      #if (GREENLED)
	light_source {<-0.025, 0.0265, 0.055>, Green/4 fade_distance 0.01 fade_power 2
	  looks_like {box {-0.003*x, <3,-5,1>/1000 pigment {Green} finish {ambient 1 diffuse 0.2}}}
	}
      #end
      #if (REDLED)
	// An LED.
	light_source {<0, 0.03, 0.08>,
	  Red/4
	  looks_like {
	    sphere {-0.004*y 0.003
	      texture{pigment {Red}
		finish {ambient 0.7 diffuse 0.3 specular 1 roughness 1/100}
	      }
	      scale <0.6, 1, 0.6>
	    }
	  }
	  fade_distance 0.01 fade_power 2
	  #if(! NO_AREA_LIGHTS)
	    area_light x*0.006, y*0.006, 10, 10 adaptive 1 orient circular
	  #end
	}
      #else
	sphere {0, 0.003
	  texture {pigment {rgbt <1, 0.1, 0.1, 0.5>} finish {specular 0.5 roughness 0.01
	    reflection 0.2}}
	  interior {ior 1.4}
	  scale <0.6, 1, 0.6>
	  translate <0, 0.03-0.004, 0.08>
	}
      #end
      // The main box.
      superellipsoid { <0.1, 0.1> scale 0.5 texture{sb_t translate-0.5*y}
	scale <0.095, 0.05, 0.18> no_reflection}
      // Now some controls.  Three or four?
      union {
	#local knobcaps = rand(guitarseed);
	#if (GREENLED)
	  // Three knobs.  The left one gets a cap and a green LED
	  object {_boxknob(1, knobcolr) translate <-0.025, 0, 0.002>}
	  _boxknob(knobcaps, knobcolr)
	  object {_boxknob(knobcaps, knobcolr) translate <0.025, 0, 0>}
	#else			// Four knobs.  No LED.
	  object {_boxknob(knobcaps, knobcolr) translate <-0.035, 0, 0.002>}
	  object {_boxknob(knobcaps, knobcolr) translate <-0.011, 0, 0>}
	  object {_boxknob(knobcaps, knobcolr) translate <0.013, 0, 0>}
	  object {_boxknob(knobcaps, knobcolr) translate <0.035, 0, 0>}
	#end
	translate <0, 0.0255, 0.065>
	pigment { rgb <1,1,0>*30 } /// BUG !!! in light_groups, no pigment type given
      }
      global_lights on
    }
    translate <0.05, 0.025, 0.09>
    // I got the scale right, but the size wrong, so get it down to
    // typical stomp-box size of 70*50*125mm.
    scale 0.7
  }
#end

//######################################################################
// Support objects for axerack.  Knobs.
#macro _bigshinyknob(radiu, locn)
  // locn is a vector.  locn.z is used as size.
  cylinder {0, locn.z*y, radiu
    texture {Brushed_Texture scale 3}
    rotate -90*x
    translate <locn.x, locn.y, 0>
  }

#end

#macro _smallblackknob(radiu, locn)
  // locn is a vector.  locn.z is used as size.
  cylinder {<locn.x, locn.y, 0>, <locn.x, locn.y, -locn.z>, radiu
    texture {
      pigment {rgb 0.02}
      finish {ambient 0 diffuse 1}
    }
  }
#end

//######################################################################
// Produce a box from origin to (0.75, 1ish, 0.5) laden with guitar
// equipment facing -Z.
// pedals[12].jpg, rack.jpg
#macro axerack(typ)
  #if (typ = 0)
    // Bugger, this is scaled differently from the image map
    #local thumbarray = array[18] {
      0.94, 0.91, 
      0.89, 0.85,
      0.83, 0.56,
      0.54, 0.48,
      0.45, 0.38,
      0.355, 0.325,
      0.305, 0.275,
      0.245, 0.12,
      0.10, 0.02
    }
  #else
    #local thumbarray = array[16] {
      0.94, 0.90, 
      0.88, 0.80,
      0.78, 0.56,
      0.54, 0.43,
      0.405, 0.37,
      0.305, 0.275,
      0.245, 0.12,
      0.10, 0.02
    }
  #end
  
  light_group {
    #if (typ = 0)
      object {bendylightbulb(0.1, 45, 0.17, 0.05, 30, 0.05, rgb 1) rotate 75*y translate<0.59, 1.05, 0.35> }
      object {bendylightnobulb(0.1, 45, 0.17, 0.05, 30, 0.05, rgb 1) rotate 75*y translate<0.59, 1.05, 0.35>}
    #else
      object {bendylightbulb(0.1, 60, 0.15, 0.05, 10, 0.05,rgb 1) rotate -75*y translate<0.01, 1.05, 0.32>}
      object {bendylightnobulb(0.1, 60, 0.15, 0.05, 10, 0.05,rgb 1) rotate -75*y translate<0.01, 1.05, 0.32>}
    #end
    object { louvre(<-0.5, 1.05, 0.02>, 50, 1) rotate 90*y }
    object { louvre(<0.5, 1.05, 0.02>, 50, 1) rotate -90*y translate 0.6*x }
    object { louvre(<-0.6, 1.05, 0.02>, 50, 1) rotate 180*y translate 0.5*z }
    #local i = dimension_size(thumbarray, 1); #while (i)
      #local i = i-1;
      _smallblackknob(0.005, <0.04, thumbarray[i], 0.008>)
      _smallblackknob(0.005, <0.55, thumbarray[i], 0.008>)
    #end
    _bigshinyknob(0.015, <0.28, 0.185, 0.015>)
    #if (typ = 0)
      _bigshinyknob(0.005, <0.51, 0.75, 0.02>)
      _bigshinyknob(0.005, <0.51, 0.67, 0.02>)
      _bigshinyknob(0.005, <0.51, 0.61, 0.02>)
    #else
      _bigshinyknob(0.022, <0.21, 0.915, 0.015>)
      _bigshinyknob(0.022, <0.42, 0.915, 0.015>)
      _bigshinyknob(0.01, <0.14, 0.392, 0.01>)
      _bigshinyknob(0.008, <0.20, 0.392, 0.01>)
      _bigshinyknob(0.008, <0.24, 0.392, 0.01>)
      _bigshinyknob(0.008, <0.28, 0.392, 0.01>)
      _bigshinyknob(0.007, <0.13, 0.285, 0.01>)
      _bigshinyknob(0.007, <0.15, 0.285, 0.01>)
      _bigshinyknob(0.007, <0.19, 0.285, 0.01>)
      _bigshinyknob(0.007, <0.24, 0.285, 0.01>)
      _bigshinyknob(0.007, <0.37, 0.285, 0.01>)
      _bigshinyknob(0.007, <0.39, 0.285, 0.01>)
      _bigshinyknob(0.007, <0.48, 0.285, 0.01>)
    #end
    box {0, 1			// Front
      texture {
	pigment {image_map { jpeg "rack_map_gamma.jpg" map_type 0 interpolate 2 once}}
	finish {specular 0.3 }
	#if (typ = 1)
	  // Right side
	  translate -0.5*x
	#end
	scale <2, 1, 1>
      }
      scale <0.56, 0.95, 0.1> translate 0.02 * x
    }
    union { // Effects boxes on top, sloped.
      union {
	cylinder {<0.01, -0.01, 0.48>, <-0.01, -0.09, 0.45>, 0.005} // Prop
	cylinder {<0.55, -0.01, 0.48>, <0.57, -0.09, 0.45>, 0.005}
	Round_Box_Union(<0,0,0>, <0.56, -0.015, 0.5>, 0.005)
	texture { // black painted ply, or summat.
	  pigment { colour 0.01}
	  finish { specular 0.3 reflection 0.0 }
	  normal { dents 0.01 scale 0.01}
	}
      }
      #if (typ = 0)
	object {bosscs2() translate <0.05, 0, 0.24> }
	object {stompbox(rgb 0.1) scale <1.2, 1, 1> translate <0.15, 0, 0.23>}
	object {stompbox(rgb 0.1) translate <0.29, 0, 0.23>}
	object {bossge7() translate <0.42, 0, 0.31> }
	// Bottom row.
	object {stompbox(rgb <1, 0.4, 0>) translate <0.05, 0, 0.07>}
	object {ibanez() translate <0.17, 0, 0.07> }
	object {stompbox2(rgb 0.02, rgb <1, 0.4, 0>) translate <0.26, 0, 0.07>}
	object {tubedriver(rgb <1, 0.4, 0>) translate <0.40, 0, 0.07>}
      #else
	object {stompbox2(rgb 0.1, rgb 0) scale <1.3, 1, 1.1> translate <0.05, 0, 0.16>}
	object {stompbox2(rgb 0.1, rgb 0) scale <1.2, 1, 1> translate <0.25, 0, 0.23>}
	object {tubedriver(rgb <1, 1, 0>) translate <0.40, 0, 0.22>}
	// Bottom row.
	object {bossge7() translate <0.19, 0, 0.06> }
	object {bossge7() translate <0.31, 0, 0.06> }
	object {bossge7() translate <0.43, 0, 0.06> }
      #end
      translate -0.25*z rotate -23*x translate <0.02, 1.05, 0.25>
    }

    global_lights on
  }
#end

//#######################################################################
// Pedal board, origin to +XYZ, "facing" +Z (correct for audience at -Z).
// board.jpg
#declare axeboard = object {
  glowbox(<0.8, 0.15, 0.3>, Red)
}

//######################################################################
#macro dave(xlate_v)
//  object { person(0)  translate xlate_v }
#end

//#####################################################################
// Boss GE-7 graphic equaliser and CS-2 compressor/sustainer.
// 60x120mm, about 50mm high.  Pedal 75mm.
// EQ panel 50x35mm on a 45mm flat.
#macro boss(cs2)
  union {
    
    #if (cs2)
      #local led_v = <0.03, 0.036, 0.109>;
      #local topmap = "cs2_map.png";
      #local pedalmap = "cs2_map_switch_alpha.png";
      #local boss_colr = rgbt <0, 0, 1, 0>;
      #local boss_t = texture {
	pigment { boss_colr}
	finish {specular 0.3 roughness 0.03 reflection 0}
	//      normal { leopard 1 scale 0.0002 turbulence 0 }
      };

      #local _cs2knob1 = union {	// Big ones, level and sustain.
	difference {Round_Cylinder_Union(0, 0.01*y, 0.007, 0.001)
	  #local i = 360+360/7/2; #while (i > 0) #local i = i - 360/7;
	    cylinder{0, 0.011*y, 0.001 scale <1.5, 1, 1> translate 0.007*z rotate i*y}
	  #end
	  // Now subtract the white indicator line
	  union {
	    cylinder{0, 0.011*y, 0.0004 translate 0.007*z}
	    cylinder{0, 0.008*z, 0.0004 translate 0.01*y}
	    pigment {rgb 1}
	  }
	}
	difference { // Flange, less white indicator.
	  torus { 0.005, 0.004 scale <1, 0.25, 1> }
	  cylinder { <0, 0.001, 0>, <0, 0.0008, 0.01>, 0.0004 pigment {rgb 1}}
	}
	// Brushed steel top.
	object {Round_Cylinder_Union(0.009*y, 0.0105*y, 0.005, 0.0005)
	  texture {Brushed_Texture}
	}
	texture {pigment {rgb 0.01} finish {specular 1 roughness 0.0001 reflection 0.1}}
      }
      #local _cs2knob2 = union {	// Smaller, attack.
	difference {Round_Cylinder_Union(0, 0.01*y, 0.006, 0.001)
	  cylinder {0, 0.011*y, 0.0045}
	  // Subtract the white indicator line
	  union {
	    cylinder{0.003*y, 0.011*y, 0.0004 translate 0.006*z}
	    cylinder{0, 0.008*z, 0.0004 translate 0.01*y}
	    // Okay, here's the thing.  I'm placing a sphere, radius
	    // 0.4mm, on a knob that will be viewed from 10m away.
	    // From the other side of the knob.  In the dark.
	    sphere{<0, 0.003, 0.006> 0.0004}
	    pigment {rgb 1}
	  }
	  #local i = 360+360/18/2; #while (i > 0) #local i = i - 360/18;
	    object {Round_Box_Union(<0,0,0>, <0.001, 0.02, 0.01>, 0.0005)
	      translate <-0.0005, 0.002, 0.0055> rotate i*y}
	  #end
	}
	cylinder{0.009*y, 0.0095*y, 0.005 texture {Brushed_Texture}}
	texture {pigment {rgb 0.01} finish {specular 1 roughness 0.0001 reflection 0.1}}
      }
    #else
      #local led_v = <0.0474, 0.036, 0.1095>;
      #local topmap = "ge7_map.png";
      #local pedalmap = "ge7_map_switch_alpha.png";
      #local boss_colr = rgb <0.7, 0.7, 0.6>;
      #local boss_t = texture {
	pigment { boss_colr}
	finish {specular 0.3 roughness 0.03 reflection 0}
	//      normal { leopard 1 scale 0.0002 turbulence 0 }
      };
      #macro _eqknob()
	union {
	  Round_Cylinder_Union (-0.001*y, 0.006*y, 0.0018, 0.001)
	  torus{0.0018, 0.0004}
	  texture{
	    pigment {rgb 1}
	    finish { specular 0.5 roughness 0.005 reflection 0.1}
	  }
	  texture {
	    pigment {
	      function{clip(sqrt(x*x+z*z)*y,0,1)} turbulence 0 colour_map {
		[0 rgbt 1]
		[1 rgbt <0.35, 0.35, 0.15, 0.3>]
	      }
	      scale <0.0018, 0.006, 0.0018>
	    }
	  }
	  scale <1, 1, 1.7>
	}
      #end
    #end
    #local boss_grubby = boss_colr + <0,0,0,1>;

    light_group {
      // LED
      light_source {led_v, Red
	looks_like {
	  // Make the reflected image very bright so that the
	  // reflection value on the box can be low, so that the
	  // sliders don't reflect too much.
	  union {
	    // The one seen directly.
	    merge {
	      sphere{0.0008*y, 0.0016}
	      cylinder{-0.005*y, 0.0008*y, 0.0016}
	      hollow interior {ior 1.3} 
	      texture { pigment {rgbt <1, 0, 0, 0.2>}
		finish{ambient 0.6 diffuse 0.4 reflection 0 specular 1 roughness 1/100}
	      }
	      no_reflection
	    }
	    // The one reflected off the box.  Much brighter.
	    sphere{0, 0.0016
	      texture { pigment {rgb <6, 0, 0>}
		finish{ambient 0.6 diffuse 0.4}
	      }
	      no_image
	      scale <1, 1.4, 1> 
	    }
	  }
	}
	fade_distance 0.005 fade_power 2
      }
      
      // Underneath the pedal switch
      sloped_box(0.06, 0.02, 0.04, 0.075, 0.003)
      // Top, housing EQ sliders
      difference {
	#if (cs2)
	  object {Round_Box_Union(<0, 0, 0.06>, <0.06, 0.035, 0.12>, 0.003)
	    texture { boss_t}
	    texture {
	      pigment { image_map { png topmap map_type 0 interpolate 2 once } rotate 90*x }
	      finish {specular 0.3 reflection 0.03}
	      scale <0.054, -0.01, 0.038>
	      translate <0.003, 0, 0.077>
	    }
	  }
	#else
	  union {
	    object {Round_Box_Union(<0, 0, 0.06>, <0.06, 0.035, 0.12>, 0.003) texture { boss_t} }
	    box {0, 1
	      texture {
		pigment { image_map { png topmap map_type 0 interpolate 2 once } rotate 90*x }
		finish {specular 0.3 reflection 0.03}
		normal {leopard 3 scale 0.003}
	      }
	      scale <0.05, -0.01, 0.04>
	      translate <0.005, 0.0350001, 0.075>
	    }
	  }
	#end
	// Cut a hole for the LED
	cylinder {<led_v.x, 0.033, led_v.z> <led_v.x, 0.036, led_v.z>, 0.0022 pigment {rgb 0}}
	// Cut slots for the EQ sliders.
	#if (! cs2)
	  union {
	    #local i = 7; #while (i) #local i = i - 1;
	      #local xi = 0.009+(i*0.0056);
	      box { <xi, 0.033, 0.08>, <xi+0.003, 0.05, 0.107>}
	      #local xi = xi+0.0015;
	      cylinder {<xi, 0.033, 0.08>, <xi, 0.05, 0.08>, 0.0015}
	      cylinder {<xi, 0.033, 0.107>, <xi, 0.05, 0.107>, 0.0015}
	    #end
	    #local xi = 0.049;
	  box { <xi, 0.033, 0.08>, <xi+0.003, 0.05, 0.107>}
	    cylinder {<xi+0.0015, 0.033, 0.08>, <xi+0.0015, 0.05, 0.08>, 0.0015}
	    cylinder {<xi+0.0015, 0.033, 0.107>, <xi+0.0015, 0.05, 0.107>, 0.0015}
	    pigment {rgb 0.01}
	  }
	#end
      }

      // lip of the LED hole.  Make it black, if a GE7.
      torus {0.0022, 0.0005 translate <led_v.x, 0.0345, led_v.z> texture {boss_t
	#if (!cs2) pigment {rgb 0} #end }}
      
      #if (cs2)
	object {_cs2knob1 rotate -30*y translate <0.014, 0.036, 0.1>}
	object {_cs2knob1 rotate 30*y translate <0.046, 0.036, 0.1>}
	object {_cs2knob2 rotate -40*y translate <0.03, 0.036, 0.086>}
      #else
	// EQ sliders in a curve, highest at the ends.
	#local amplitud = 0.004+rand(guitarseed)*0.003;
	#local amplitud = 0.008;
	#local offse = 0.003*rand(guitarseed)+amplitud/2;
	#local offse = 0.00+amplitud/2;
	#local i = 7; #while (i) #local i = i - 1;
	  #local xi = 0.009+0.0015+(i*0.0056);
	  #local zed = sin(radians(i/6*180));
	  object {_eqknob()
	    translate <xi, 0.036, 0.091+offse - amplitud*zed + rand(guitarseed)/400>
	  }
	#end
	// Main level
	#local xi = 0.049+0.0015;
	object {_eqknob() translate <xi, 0.036, 0.092+0.004*rand(guitarseed)>}
      #end
      global_lights on
    }
      
    // Screw heads.
    #local _screwhead = difference {
      sphere {0, 0.002 scale <1, 0.7, 1>}
      sphere {0.002*y, 0.002 scale <0.3, 1, 1>}
      sphere {0.002*y, 0.002 scale <1, 1, 0.3>}
      texture {T_Chrome_1C finish {ambient 0 roughness 0.001}} 
    }
    
    // Pedal switch
    union {
      // That which will be sheared:
      union {
	difference {
	  union {
	    sloped_box(0.06, 0.008, 0.02, 0.074, 0.002)
	    // Two screws, at the hinge
	    object {_screwhead rotate 90*z translate <-0.0005, 0.004, 0.005>}
	    object {_screwhead rotate -90*z translate <0.0605, 0.004, 0.005>}
	    rotate 180*x
	  }
	  Round_Box_Union(<0.005, -0.001, -0.035>, <0.055, 0.003, -0.07>, 0.002)
	  texture { boss_t}	// base colour
	  texture {		// text
	    pigment {
	      image_map { png pedalmap map_type 0 interpolate 2 once }
	      rotate 90*x
	      scale <0.05, 1, 0.03>
	      translate <0.005, 0, -0.033>
	    }
	  }
	  #local _grubby_p = <0.35, 0.35, 0.15, 0.3> * boss_grubby; // Avoid suspicious exp
	  texture {        	// grubbiness at the front of the pedal
	    pigment {
	      function{clip(abs((x+0.1)*(x-1.1)*(z+1)*(y-0.1)),0,1)} turbulence 0.01 colour_map {
		[0.0 _grubby_p]
		[0.01 rgbt <0, 0, 0, 1>]
		[1 rgbt <0, 0, 0, 1>]
	      }
	      scale <0.06, 0.01, 0.078>
	    }
	  }
	}
	// The rubber pad
	union {
	  Round_Box_Union(<0, -0.01, 0>, <0.048, 0.001, 0.033>, 0.002)
	  height_field { png
	    "boss_hmap.png" // smooth
	    scale <0.036, 0.002, 0.008>
	    translate <0.005, 0, 0.019>
	  }
	  texture {
	    pigment { bozo pigment_map {
	      [ 0 colour rgb 0.05 ]
	      [ 1 colour rgb 0.1 ] }
	      scale 0.0005
	    }
	  }
	  texture {		// Grubbiness at front of pad
	    pigment { function{clip(z,0,1)} turbulence 0.5  colour_map {
		[0.0 rgbt <0, 0, 0, 0.7>]
		[0.3 rgbt <0, 0, 0, 1>]
		[1 rgbt <0, 0, 0, 1>]
	      }
	      scale <0.048, 1, 0.033>
	    }
	  }
	  translate <0.006, 0, -0.069>
	}
	matrix <
	1, 0, 0,
	0, 1, 0,
	0, 0.015/0.07, 1,
	0, 0, 0 >
      }
      object {knurled_knob scale 0.9 translate <0.03, -0.025, -0.0745>}
      rotate 8*x
      translate <0, 0.048, 0.074>
    }
    union {
      #local shift = rand(guitarseed);
      object { stompboxpower rotate (-5+30*shift)*z translate <0, 0.018, 0.12>}
      object {_sboxcable(0.031+shift*0.008, 0.06,0) rotate 5*x translate <-0.03, 0.02, 0.069>}
      object {_sboxcable(0.041-shift*0.008, 0.063,-10) rotate 7*x scale<-1,1,1> translate<0.03, 0.02, 0.069>}
      #if (rand(guitarseed) > 0.5) scale<-1,1,1> #end
      translate 0.03*x
    }
    texture { boss_t}
  }
#end

#macro bossge7() boss(0) #end
#macro bosscs2() boss(1) #end

//#####################################################################
// Ibanez effects box.  60x120mm, about 50mm high.  Pedal 75mm.
// CP-9 in AD-9 colour.  Should fix that.
#macro ibanez() union {
    #local led_v = <0.03, 0.036, 0.112>;
    #local ibanez_colr = rgbft <1, 0, 0.9, 0, 0>;
    #local ibanez_t = texture {
      pigment { ibanez_colr}
      finish {specular 0.3 roughness 0.03 }
    };

    #local _ibknob = union {
      Round_Cylinder_Union(0, 0.01*y, 0.008, 0.0005)
      difference {Round_Cylinder_Union(0, 0.014*y, 0.007, 0.0005)
	cylinder {0, 0.015*y, 0.0055}
      }
      // Brushed steel top.
      cylinder {0.009*y, 0.0139*y, 0.006
	texture { average
	  texture_map {
	    [ pigment { colour rgb 0.4 } finish { specular 1 roughness 0.05 reflection 0}
	      normal {wood 1 sine_wave rotate 90*x}
	    ]
	    [ pigment { colour rgb 0.4 } finish { specular 1 roughness 0.05 reflection 0}
	      normal {wood -1 sine_wave rotate 90*x}
	    ]
	  }
	  scale <0.0008, 0.001, 0.0008>
	}
      }
      // Indicator, black.
      box {<-0.0005, 0.009, 0.002>, <0.0005, 0.014, 0.006>}
      cylinder {0, -0.001*y, 0.0005 translate <0, 0.014, 0.002>}
      // Pitch it off axis so that if an animation rotates this knob,
      // it will wobble a bit and won't look like the indicator is
      // orbiting a stationary cylinder.
      rotate <-2+rand(guitarseed)*4, 0, -2+rand(guitarseed)*4>
      texture {blackspecklyplastic_t}
    }

    light_group {
      // LED
      light_source {led_v, Red
	#if(!NO_AREA_LIGHTS) area_light 0.004*x, 0.004*y, 10, 10 adaptive 1 orient circular #end
	looks_like {
	  merge {
	    sphere{0.0008*y, 0.002}
	    cylinder{-0.005*y, 0.0008*y, 0.002}
	    hollow interior {ior 1.3} 
	    texture { pigment {rgbt <1, 0, 0, 0.2>}
	      finish{ambient 0.6 diffuse 0.4 reflection 0 specular 1 roughness 1/100}
	    }
	  }
	}
	fade_distance 0.005 fade_power 2
      }
      
      // Main body, a plain flat box.
      difference {
	object {Round_Box_Union(<0, 0, 0.005>, <0.06, 0.035, 0.118>, 0.003) }

	// Cut a hole for the LED
	cylinder {<led_v.x, 0.033, led_v.z> <led_v.x, 0.036, led_v.z>, 0.0026 pigment {rgb 0}}
      }

      // Bounds are +/- <24, 1, 29>
      #local faceplate_outer = union {
	torus {5, 1 translate <-18, 0, -23>}
	torus {5, 1 translate <18, 0, -23>}
	torus {2, 1 translate <-21, 0, 26>}
	torus {2, 1 translate <21, 0, 26>}
	cylinder {<-23, 0, -23>, <-23, 0, 26>, 1}
	cylinder {<23, 0, -23>, <23, 0, 26>, 1}
	cylinder {<-18, 0, -28>, <18, 0, -28>, 1}
	cylinder {<-21, 0, 28>, <21, 0, 28>, 1}
	scale 1/1000
      }
      #local faceplate = union {
	object {faceplate_outer scale 1000}
	cylinder {-y, +y, 5 translate <-18, 0, -23>}
	cylinder {-y, +y, 5 translate <18, 0, -23>}
	cylinder {-y, +y, 2 translate <-21, 0, 26>}
	cylinder {-y, +y, 2 translate <21, 0, 26>}
	box {<-23, -1, -23>, <-20, 1, 26>}
	box {<23, -1, -23>, <20, 1, 26>}
	box {<-18, -1, -28>, <18, 1, -20>}
	box {<-21, -1, 28>, <21, 1, 0>}
	scale 1/1000
      }
      #local nameplate = object {
	superellipsoid { <0.1, 0.5> rotate 90*x scale <22, 0.5, 11>}
	scale 1/1000
      };
      
      // Sloped front, holding pedal switch and name plate.  Rotated.
      union {
	// Everything but the name plate.  This will be sheared
	difference {
	  Round_Box_Union(<-0.03, -0.025, 0>, <0.03, 0, 0.07>, 0.003)
	  // Subtract indentation for name plate and switch.
	  object {faceplate_outer scale <1.03, 1, 1.03> translate <0, 0.0005, 0.035>}
	  // push -Y toward +Z
	  matrix <
	  1, 0, 0,
	  0, 1, -0.004/0.015,
	  0, 0, 1,
	  0, 0, 0 >
	}
	// That shear messed up the join at the hinge, faintly, okay
	// it's only visible from 10cm away but anyway these make a
	// slightly cleaner joint.  They are the same cylinders that
	// are in the Round_Box_Union above, but aren't sheared.
	cylinder {<-0.027, -0.003, 0.003>, <-0.027, -0.003, 0.01>, 0.003}
	cylinder {<0.027, -0.003, 0.003>, <0.027, -0.003, 0.01>, 0.003}
	union {
	  difference {
	    object {faceplate}
	    object {nameplate scale <1.03, 2, 1.03> translate <0, 0.001, 0.016>}
	    union {		// Hole for the switch
	      torus {0.5, 1 translate <-20, 0, 0>}
	      torus {0.5, 1 translate <20, 0, 0>}
	      torus {0.5, 1 translate <-20, 0, -24>}
	      torus {0.5, 1 translate <20, 0, -24>}
	      cylinder {<-20.5, 0, -24>, <-20.5, 0, 0>, 1}
	      cylinder {<20.5, 0, -24>, <20.5, 0, 0>, 1}
	      cylinder {<-20, 0, -24.5>, <20, 0, -24.5>, 1}
	      cylinder {<-20, 0, 0.5>, <20, 0, 0.5>, 1}
	      translate 1*y
	      scale 1/1000
	    }
	    texture {blackspecklyplastic_t}
	  }
	  object {nameplate translate <0, 0.0005, 0.016>
	    texture {
	      pigment {rgb 0.72}
	      finish {specular 0.9 roughness 0.002 reflection 0.1}
	    }
	    texture {
	      pigment { image_map { png "ibanez_plate_map_ad9.png" map_type 0 interpolate 2 once } rotate 90*x }
	      scale <0.044, 1, 0.021> translate <-0.022, 0, 0.0055>
	    }
	    texture { // grubbiness at the rear of the Ibanez nameplate
	      pigment {
		function{clip(z*z+x*x+y*y,0,1)} warp {turbulence 0.3}
		colour_map {
		  [0 rgbft <0.35, 0.15, 0.15, 0, 0.7>]
		  [1 rgbft <0.35, 0.15, 0.15, 0, 1>]
		}
	      }
	      scale <0.03, 0.01, 0.005>
	      translate <0.02, 0.0, 0.025>
	    }
	  }
	  union {		// The switch
	    Round_Box_Union(<-0.021, -0.02, -0.024>, <0.021, 0.001, 0.001>, 0.001)
	    height_field { png
	      "ibanez_hmap.png" // smooth
	      scale <0.04, 0.0004, 0.023>
	      translate <-0.02, 0.0008, -0.023>
	    }
	    rotate 5*x
	    translate <0, 0.001, 0>
	    texture {
	      pigment {rgb 0.72}
	      finish {specular 0.9 roughness 0.001 reflection 0.1}
	      normal {leopard 0.5  scale 0.05 warp {turbulence 0.2} scale 0.001}
	    }
	    texture { // grubbiness at the front of the Ibanez switch
	      pigment {
		function{clip(z*z+x*x+y*y,0,1)} warp {turbulence 0.3}
		colour_map {
		  [0 rgbft <0.35, 0.15, 0.15, 0, 0.8>]
		  [1 rgbft <0.35, 0.15, 0.15, 0, 1>]
		}
	      }
	      scale <0.03, 0.01, 0.005>
	      translate <0.01, 0.0, -0.022>
	    }
	  }
	  translate <0, 0, 0.035>
	}

	translate <0, 0.003, -0.003>
	rotate -x * degrees(atan(1.5/7.5))
	translate <0.03, 0.032, 0.008>
      }
      
      // lip of the LED hole.
      torus {0.0026, 0.0005 translate <led_v.x, 0.0345, led_v.z>}
            
      object {_ibknob rotate -30*y translate <0.0135, 0.036, 0.1025>}
      object {_ibknob rotate -10*y translate <0.0465, 0.036, 0.1025>}
      object {_ibknob rotate 30*y scale <0.86, 1, 0.86> translate <0.03, 0.036, 0.09>}
      
      global_lights on
    }
      
    union {
      #local shift = rand(guitarseed);
      object { stompboxpower rotate (-5+30*shift)*z translate <0, 0.018, 0.12>}
      object {_sboxcable(0.031+shift*0.008, 0.06,15) rotate 0*x translate <-0.03, 0.02, 0.069>}
      object {_sboxcable(0.041-shift*0.008, 0.063,-10)rotate 7*x scale<-1,1,1> translate<0.03, 0.02, 0.069>}
      #if (rand(guitarseed) > 0.5) scale<-1,1,1> #end
      translate 0.03*x
    }

  texture { ibanez_t}
  texture {
/*    image_pattern {png "ibanez_knob_map.png" use_alpha once}
    texture_map {
      [0 pigment {rgb 0} ]
      [1 ibanez_t ]
    }*/
    pigment { image_map { png "ibanez_knob_map.png" map_type 0 interpolate 2 once } }
    rotate 90*x
    scale <0.056, -0.01, 0.035>
    translate <0.002, 0, 0.080>
  }
  #local _grubby_p = rgbft <0.35, 0.35, 0.15, 0, 0> * ibanez_colr;
  texture { // grubbiness on back curve by knobs (Ibanez)
    pigment {
      function{clip(z*z+x*x+y*y,0,1)} warp {turbulence 0.3}
      colour_map {
	[0 rgbft _grubby_p + <0,0,0,0,0.5>]
	[0.15 rgbft _grubby_p + <0,0,0,0,0.4>]
	[0.2 rgbft _grubby_p + <0,0,0,0,0.2>]
	[0.9 rgbft ibanez_colr + <0,0,0,0,1>]
	[1 rgbft ibanez_colr + <0,0,0,0,1>]
      }
    }
    scale <0.08, 0.01, 0.02>
    translate <0.04, 0.05, 0.07>
  }
  texture { // grubbiness on front curve (Ibanez)
    pigment {
      function{clip(z*z+x*x+y*y,0,1)} warp {turbulence 0.3}
      colour_map {
	[0 rgbft _grubby_p + <0,0,0,0,0.5>]
	[0.15 rgbft _grubby_p + <0,0,0,0,0.4>]
	[0.2 rgbft _grubby_p + <0,0,0,0,0.2>]
	[0.9 rgbft ibanez_colr + <0,0,0,0,1>]
	[1 rgbft ibanez_colr + <0,0,0,0,1>]
      }
    }
    scale <0.06, 0.01, 0.02>
    translate <0.04, 0.035, 0>
  }
}
#end


//#####################################################################
// Specs: 100mm wide, 75mm high, 175mm long.  Extends from 0 to +(XYZ).
// Some trim in -X, -Z.
#macro tubedriver(colr)
  #local zed = 0.175;
  #local rad = 0.01;
  #local angl = 12;
  #local deltay = sin(radians(angl))*zed;
  #local basey = 0.075 - rad - deltay;
  
  #local side = union {
    object{Round_Cylinder_Union(<0.05, 0, rad>, <0.0515, 0, rad>, rad+0.001, 0.0007)}
    object{Round_Cylinder_Union(<0.05, 0, zed-rad>, <0.0515, 0, zed-rad>, rad+0.001, 0.0007)}
    object{Round_Box_Union(<0.05, -0.075, rad>, <0.0515, rad+0.001, zed-rad>, 0.0007)}
    object{Round_Box_Union(<0.05, -0.075, -0.001>, <0.0515, 0, zed+0.001>, 0.0007)}
    #if(0)
      blurred_tm(pigment {rgb 0.05}, finish {reflection 0.05 metallic})
    #else
      texture{pigment {rgb 0.05} finish {reflection 0.01 metallic}
	normal {leopard 0.7 scale 0.0002} // !!! 0.0002
      }
    #end
  }
  
  union {
    // The box with sides, no controls
    //    intersection {
      light_group {
	// Power and drive LEDs
	light_source {<-0.024, rad+0.002, 0.14>, rgb <1,0,0>
	  looks_like {
	    union {
	      sphere{0, 0.002} cylinder{-0.01*y, 0, 0.002}
	      // This is sheared to a slope.  This rotate helps the look a bit.
	      rotate -angl*x
	      texture {pigment{Red} finish{ambient 0.7 diffuse 0.3 specular 1 roughness 1/100}}
	      }
	  }
	  fade_distance 0.002 fade_power 2
	}
	light_source {<0.024, rad+0.002, 0.14>, rgb <0,1,0>
	  looks_like {
	    union {sphere{0, 0.002} cylinder {-0.01*y, 0, 0.002}
	      rotate -angl*x
	      texture {pigment{Green} finish{ambient 0.7 diffuse 0.3 specular 1 roughness 1/100}}
	    }
	  }
	  fade_distance 0.002 fade_power 2
	}
	intersection {
	  // The box, no sides
	  union {
	    object{Round_Cylinder_Union(<-0.05, 0, rad>, <0.05, 0, rad>, rad, 0.001)}
	    object{Round_Cylinder_Union(<-0.05, 0, zed-rad>, <0.05, 0, zed-rad>, rad, 0.001)}
	    object{Round_Box_Union(<-0.05, 0, rad>, <0.05, rad, zed-rad>, 0.001)}
	    object{Round_Box_Union(<-0.05, -0.075, 0>, <0.05, 0.001, zed>, 0.001)}
	    // Sides
	    object {side} object {side scale <-1,1,1>}
	    texture {
	      pigment { rgb <0.9, 0.9, 0.8>}
	      finish {specular 1 roughness 0.01 reflection 0}
//	      normal { leopard 1 scale 0.002 turbulence 0 } // !!! 0.0002
	    }
	    texture { // grubbiness around knobs
	      pigment {
		function{clip((x*x+z*z),0,1)} warp {turbulence <0, 0, 0.1>}
		colour_map {
		  [0 rgbt <0.35, 0.35, 0.15, 0.7>]
		  [0.15 rgbt <0.35, 0.35, 0.15, 0.8>]
		  [0.2 rgbt <0.35, 0.35, 0.15, 0.8>]
		  [0.4 rgbt <0.35, 0.35, 0.15, 0.7>]
		  [0.9 rgbt 1]
		}
	      }
	      scale <0.015, 0.02, 0.02>
	      translate <-0.037, 0, 0.125>
	    }
	    texture {
	      pigment {
		function{clip((x*x+z*z),0,1)} turbulence 0.1*z colour_map {
		  [0 rgbt <0.35, 0.35, 0.15, 0.7>]
		  [0.15 rgbt <0.35, 0.35, 0.15, 0.8>]
		  [0.2 rgbt <0.35, 0.35, 0.15, 0.8>]
		  [0.4 rgbt <0.35, 0.35, 0.15, 0.7>]
		  [0.9 rgbt 1]
		}
	      }
	      scale 0.02
	      translate <-0.012, 0, 0.125>
	    }
	    texture {
	      pigment {
		function{clip((x*x+z*z),0,1)} turbulence 0.1*z colour_map {
		  [0 rgbt <0.35, 0.35, 0.15, 0.7>] [0.15 rgbt <0.35, 0.35, 0.15, 0.8>]
		  [0.2 rgbt <0.35, 0.35, 0.15, 0.8>] [0.4 rgbt <0.35, 0.35, 0.15, 0.7>]
		  [0.9 rgbt 1] }
	      }
	      scale 0.02 translate <0.012, 0, 0.125>
	    }
	    texture {
	      pigment {
		function{clip((x*x+z*z),0,1)} turbulence 0.1*z colour_map {
		  [0 rgbt <0.35, 0.35, 0.15, 0.6>] [0.15 rgbt <0.35, 0.35, 0.15, 0.7>]
		  [0.2 rgbt <0.35, 0.35, 0.15, 0.7>] [0.4 rgbt <0.35, 0.35, 0.15, 0.5>]
		  [0.9 rgbt 1] }
	      }
	      scale <0.015, 0.02, 0.02> translate <0.037, 0, 0.125>
	    }
	    texture { // grubbiness on front curve (tube driver)
	      pigment {
		function{clip(z*z+x*x+y*y,0,1)} warp {turbulence 0}
		colour_map {
		  [0 rgbt <0.35, 0.35, 0.15, 0.8>]
		  [0.2 rgbt <0.35, 0.35, 0.15, 0.6>]
		  [0.9 rgbt 1]
		  [1 rgbt 1]
		}
	      }
	      scale <0.08, 0.01, 0.01>
	      // X=0 would be centred; bump it right a little.
	      translate <0.01, rad, 0.008>
	    }
	    // Raise the back to put the face at angl degrees off horizontal
	    matrix <
	    1, 0, 0,
	    0, 1, 0,
	    0, 1/zed*deltay, 1,
	    0, 0, 0 >
	    texture {
	      finish {specular 0 reflection 0}
	      pigment {
		image_map { png "tubedriver_map_alpha.png" map_type 0 interpolate 2 once }
		rotate 90*x
		translate -0.5
		scale <0.095, 1, zed*0.91>
		translate 0.087*z
	      }
	    }
	  }
	  plane {-y, basey-0.005 pigment{rgb 0}}	// Leave 5mm for feet
	  pigment { rgb <1,1,0>*30 } /// BUG !!! in light_groups, no pigment type given
	}
	global_lights on
      }
      //    }
    // The controls.  Place them flat then rotate them up.
    union {
      object{_boxknob(1, colr) scale <1, 0.8, 1> translate <-0.037, 0, 0.128>}
      object{_boxknob(1, colr) scale <1, 0.8, 1> translate <-0.012, 0, 0.128>}
      object{_boxknob(1, colr) scale <1, 0.8, 1> translate <0.012, 0, 0.128>}
      object{_boxknob(1, colr) scale <1, 0.8, 1> translate <0.036, 0, 0.128>}
      // Chrome on/off knob.
      union{
	// A superquad gives better reflections.
	//	Round_Cylinder_Union(0, 0.012*y, 0.005, 0.001)
	superellipsoid { <1, 0.1> rotate 90*x scale <0.005, 0.006, 0.005> translate 0.006*y}
	// A tiny bump in the middle
	sphere{0.0117*y, 0.0005}
	torus{0.006, 0.001 translate 0.0015*y}
	intersection {
	  torus{0.007, 0.002}
	  box{<-0.0075, 0, -0.01>, <0.0075, 0.01, 0.01> rotate 20*y}
	  box{<-0.0075, 0, -0.01>, <0.0075, 0.01, 0.01> rotate 80*y}
	  box{<-0.0075, 0, -0.01>, <0.0075, 0.01, 0.01> rotate 140*y}
	}
	translate 0.049*z
	texture {chrometexture}
      }
      rotate -angl*x
      translate rad*y
    }
    // Feet.
    union {
      Round_Cylinder_Union(<-0.04, 0, 0>, <-0.04, 0.005, 0>, 0.007, 0.001)
      Round_Cylinder_Union(<0.04, 0, 0>, <0.04, 0.005, 0>, 0.007, 0.001)
      translate <0, -basey, 0.006>
      texture{rubbertexture}
    }
    union {
      sphere{<-0.02, 0, 0>, 0.003}
      sphere{<0.02, 0, 0>, 0.003}
      scale <1, 1, 0.3> translate -0.01*y
      texture {pigment {rgb 0.05 }}
    }
    
    translate <0.05, basey, 0>
  }
#end

//#####################################################################
#ifdef (TEST)

/*  light_source { <0.2, 2, -1> colour White spotlight point_at 0 radius 2 falloff 6
    #if(!NO_AREA_LIGHTS) area_light x/10, y/10, 10, 10 adaptive 1 orient circular #end
  }
*/
//  #include "stage.inc" object{grille(<1.8, -0.02, 2>) scale 0.2 translate <-0.22, 0, -0.1>}

  #if(0)
    global_settings {
      max_trace_level 15
      assumed_gamma 1
    }
  
    plane {  y, 0 texture { pigment { rgb 1 } finish {reflection 0}}}
    light_source { <-10, 10, -5> colour White spotlight point_at <0.6, 0.5, 0> radius 2 falloff 7}
    camera { location <-0.1, 1.5, -0.8> look_at <0.6, 0.5, 0.25> angle 80}
//    camera { location <-0.1, 1.5, -0.8> look_at <0.9, 0.9, 0> angle 15}
//    camera {  orthographic location <0.8, 0.6, -0.8> look_at <0.8, 0.5, 0.25> angle 80}
//    camera { location <0.3, 2, -0.1> look_at <0.3, 0.5, 0.25> angle 40}
    object { axerack(0) }
    object { axerack(1) translate 0.61*x}
  #else
    global_settings {
      max_trace_level 15
//    radiosity { count 80 nearest_count 8 error_bound 0.1 pretrace_end 0.02 adc_bailout 0.01}
//    radiosity { brightness 0.2}
      assumed_gamma 1
    }
  sky_sphere { test_sky_sphere_sesame }
  
  plane {  y, 0 texture { pigment { rgb 0 }}}
//  plane {  y, -0.0 texture { pigment { checker colour Green colour Yellow} }}
    light_source { <1, 2, -1> colour White/2 spotlight point_at 0 radius 5 falloff 10
      #if(!NO_AREA_LIGHTS) area_light x/10, y/10, 10, 10 adaptive 1 orient circular #end
    }
//    camera {location <-0.27, 0.3, -0.13> look_at <-0.06,0,0.1> angle 50} // Good of all
//    camera {location <-0.04, 0.2, -0.1> look_at <0.08,0.0,0.10>} // close up tube driver
//    camera {location <0, 0.07, -0.15> look_at 0} // close up top right corner
//    camera {location <-0.04, 0.1, -0.1> look_at <0.08,0.0,0.10>} // tube driver clipping?
//    camera {location <-0.15, 0.2, 0.03> look_at <-0.04,0.0,0.09> angle 50 } // close up ge-7
//    camera {location <-0.23, 0.2, 0.03> look_at <-0.12,0.0,0.09> angle 50 } // close up cs-2
//    camera {location <-0.13, 1, 0.13> look_at <-0.13, 0, 0.13> angle 5} // knobs
//    camera {location <-0.15, 0.2, 0.03> look_at <-0.03,0.03,0.14> angle 10 } // close up LED
    camera {location <-0.15, 0.25, -0.03> look_at <-0.10,0,0.19> angle 40} // Ibanez CP-9
//    camera {location <-0.32, 0.15, 0> look_at <-0.22,0.03,0.08> angle 45 } // Ibanez knobs
//    camera {location <-0.32, 0.15, 0.2> look_at <-0.22,0.03,0.15> angle 45 } // Ibanez cables
    // Matches ibanezad9-plan.jpg with ibanez() translate <-0.25, 0, 0.03>:
//    camera {location <-0.22, 0.35, 0.09> look_at <-0.22,0.035,0.09> angle 29 }

    light_source { <-1, 1.5, 0.3> colour White/20 spotlight point_at <-0.05,0,0.1> radius 5 falloff 6
      #if(!NO_AREA_LIGHTS) area_light x/10, y/10, 10, 10 adaptive 1 orient circular #end
    }

//    object {stompbox(rgb <1, 0.3, 0>) translate <-0.04, 0, -0.08>  rotate -40*y}// translate <-0.08, 0, -0.1>}
//    object {bossge7() translate <-0.04, 0, -0.08>  rotate 140*y}// translate <-0.08, 0, -0.1>}
//    object {tubedriver(rgb <1, 0.4, 0>) translate <0, 0, 0.0>}
//    object {bossge7() translate <-0.08, 0, -0.06> }
    object {ibanez() translate <-0.1, 0, 0.09> }
//    object {bosscs2() translate <-0.19, 0, 0.02> }
  #end

#end

