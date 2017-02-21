/* This work is licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/

Nick's kits:

1983-1990: Ludwig 8-Piece Drumset with Paiste Hi-Hats (2), Crash
Cymbals (5), Ride Cymbal, China Cymbals (2), and Splash Cymbals (2);
Remo Roto Toms (7-Piece) (1987-1990)

1994: DW 10-Piece Drumset with Paiste Hi-Hats (2), Crash Cymbals (8),
Ride Cymbals (2), China Cymbals (2), & Splash Cymbals (3); 1994
Remo Roto Toms (10-Piece) (1994)


Remo ambassador

The following is what Nick Mason played around the time of Dark Side
of the Moon and The Wall.
 15" Formula 602 Heavy Hi-hats (discontinued)
 16" 2002 Crash
 18" 2002 Medium
 18" 2002 Ride
 20" 2002 Ride
 18" 2002 China
 36" Symphonic gong

*/

//#declare CHEAP_MUSIC=1;
#ifdef (CHEAP_MUSIC) 
  #warning "Using cheap instruments!"
#end

#ifdef (TEST)
  #include "dsot.inc"
#end
#include "colors.inc"		// Damn this parser
#include "metals.inc"

#declare ludwigshelltexture = texture {
  pigment {  marble
    turbulence 0.6
    color_map {
      [0.00 colour DarkPurple * DarkPurple /7]
      [0.50 colour DarkPurple * DarkPurple/5]
      [1.00 colour DarkPurple * DarkPurple/7]
    }
    rotate 90*x
    scale 0.3
    quick_colour DarkPurple
  }
  finish { phong 1 roughness 0.0001
    #ifndef (CHEAP_MUSIC) 
      reflection { 0.10, 0.2 }
    #end
  }
};

#declare shellthick = 0.008;   // 8mm, says Ludwig.
#declare bassradius = 0.28;		// External radius.
#declare bassyoffset = bassradius + 0.05;		// Centre of its axis
#declare basslength = 0.46;		// Length along Z axis.  Chrome bits protude.

#local littlelegthick = 0.005;		// smallest radius of little legs

#declare drumseed = seed(0);

#declare frameh=0.04;
#declare framed=0.08;

//----------------------------------------------------------------------
#macro bassring(flipit)
  // width of the chrome ring around each end
  #local ringwidth = 0.030;
  #local ringthick = 0.006;
  #local ringz = basslength/2;

  #local innercyl = cylinder {(ringz - 0.001) * z,
    (ringz + ringwidth + 0.001) * z,
    bassradius - ringthick/2 }

  union {
    #if (0) 
      difference {
	union {
	  cylinder {
	    <0, 0, ringz + ringthick/2>,
	    <0, 0, ringz + ringwidth - ringthick/2>,
	    bassradius+ringthick/2
	  }
	  torus {
	    bassradius, ringthick/2
	    rotate 90*x
	    translate (ringz + ringwidth - ringthick/2)*z
	  }
	  torus {
	    bassradius, ringthick/2
	    rotate 90*x
	    translate (ringz + ringthick/2)*z
	  }
	}
	object {innercyl}
      }
    #else
      difference {
	superellipsoid {
	  <1, 0.07>
	  scale <bassradius+ringthick/2, bassradius+ringthick/2, ringwidth/2>
	  translate (ringz + ringwidth/2) * z
	}
	object {innercyl}
	// This hard-edged bounding ring halves the render time.
	bounded_by {
	  difference {
	    cylinder {
	      <0, 0, ringz>
	      <0, 0, ringz + ringwidth>
	      bassradius+ringthick*1.01
	    }
	    object {innercyl}
	  }
	}
	clipped_by {bounded_by}
      }
    #end

    torus {
      bassradius, ringthick/4
      rotate 90*x
      scale <1, 1, 2>		// Make it flatter.  More like a band.
      translate (ringz-0.03)*z
    }
    
    #local angl=18;
    #while (angl < 360)
      #local angl = angl + 36;
      // Anchor on drum body
      object {
	#ifndef (CHEAP_MUSIC) 
	  superellipsoid { <0.25, 0.07> }
	#else
	  box {-1, 1}
	#end
	rotate -20*x
	translate z
	scale <0.006, 0.013, 0.025>
	translate <0, bassradius, ringz-0.11>
	rotate angl*z
      }
      // Claw hook on the ring, outside the shell, which accepts the connecting rod.
      object {
	#ifndef (CHEAP_MUSIC) 
	  superellipsoid { <0.45, 0.4> }
	#else
	  box {-1, 1}
	#end
	translate -z
	scale <0.01, 0.008, ringwidth + 0.003>
	translate <0, bassradius+0.008, ringz+ringwidth+0.001>
	rotate angl*z
      }
      // Claw hook
      object {
	#ifndef (CHEAP_MUSIC) 
	  superellipsoid { <0.2, 0.2>}
	#else
	  box {-1, 1}
	#end
	translate -z
	scale <0.015, shellthick*0.6, ringwidth*0.5>
	translate <0, bassradius-0.002, ringz+ringwidth+0.004>
	rotate angl*z
      }
      // Connecting rod
      object {
	#ifndef (CHEAP_MUSIC) 
	  superellipsoid { <1, 0.25>}
	#else
	  cylinder {-z, z, 1}
	#end
	translate -z
	// 0.01 == protrusion, 0.11 == offset of anchor
	scale <0.003, 0.003, (ringwidth + 0.01 + 0.11)/2>
	// Rod pokes out beyond claw clamp by about a centimetre, for tuning.
	translate <0, bassradius+0.006, ringz+ringwidth+0.01>
	rotate angl*z
      }
    #end
    #if (flipit)
      scale <1, 1, -1>
    #end
    texture {chrometexture}
  }
#end // bassring()

//----------------------------------------------------------------------
// Skins are actually called heads.  Who was to know?
#macro bassskin(draw_logo)
  //  ambient 0.5 diffuse 0.5 
  #local _bs_f = finish {specular 1 roughness 0.0001};
  #local _bs_xmit = 0.2;
  disc {
    0, -z, bassradius,
    pigment {
      #if (draw_logo)
	image_map {
	  png "ludwig.png"
	  map_type 0
	  interpolate 2
	  #ifndef (CHEAP_MUSIC) 
	    transmit all _bs_xmit
	  #end
	}
      #else
	color rgb 0
	#ifndef (CHEAP_MUSIC)
	  transmit _bs_xmit
	#end
      #end
      translate -0.5
      scale bassradius*2
    }
    finish {
      _bs_f
      #ifndef (CHEAP_MUSIC) 
	reflection { 0.05, 0.1 }
      #end
    }
    // Turn off reflection for the inside of the skin.  Otherwise we get a
    // tube with mirrors at each end - infinite reflection.
    interior_texture {
      pigment { colour rgb 0
	#ifndef (CHEAP_MUSIC)
	  transmit _bs_xmit
	#end
      }
      finish { _bs_f reflection 0}
    }
  }
#end

//----------------------------------------------------------------------
#macro bassleg(flipit)
  union {
    cone {
      <0, bassradius * 0.08, 0>,
      littlelegthick,
      <0, bassradius * 0.65, 0>,
      littlelegthick * 1.3
      texture{chrometexture}
    }
    sphere {
      <0, 0, 0>, littlelegthick*2
      texture{rubbertexture}
    }
    cone {
      <0, 0, 0>,
      littlelegthick * 2,
      <0, bassradius * 0.08, 0>,
      littlelegthick * 1.5
      texture{rubbertexture}
    }
    superellipsoid {		// On the drum shell
      <0.25, 0.07>
      rotate 10*z
      scale <littlelegthick * 2.5, littlelegthick * 4, littlelegthick * 3.5>
      translate <0, bassradius * 0.65, 0>
      texture{chrometexture}
    }
    rotate 10*z
    translate <bassradius, littlelegthick * 2 - bassyoffset, -(basslength/3)>
    scale (flipit)
  }
#end //bassleg

//######################################################################
#declare bassdrum = union {
  
  #local bassoutercylinder = cylinder {
    -basslength/2 * z,
    +basslength/2 * z,
    bassradius
  }
  
  // body, without skins and trim
  difference {
    object {bassoutercylinder texture { ludwigshelltexture }}
    cylinder {
      <0, 0, (-basslength/2)*1.001>,
      <0, 0, (+basslength/2)*1.001>, bassradius-shellthick
      #ifdef (NO_SHINY_DRUM_GUTS)
	texture { ludwigshelltexture }
	finish {reflection 0}
      #else
//	#warning "Shiny drum guts!"
	texture { ludwigshelltexture }
      #end
    }
  }
  // Hardware.  Let it protrude into the interior then core it.
  difference {
    union {
      bassring(0)
      bassring(1)
      bassleg(< 1,1,1>)
      bassleg(<-1,1,1>)
    }
    // Subtract protrusions.  Pigment purely to avoid a warning.
    object {bassoutercylinder pigment { rgb y } }
  }
  // Skins
  object {bassskin(1) translate -(basslength/2)*z}
  object {bassskin(0) translate -(basslength/2)*z rotate 180*y}
  
  translate <0, bassyoffset, 0>	// Lift so the feet are on y==0.
}
#undef bassring
#undef bassskin
#undef bassleg
//----------------------------------------------------------------------
#macro tomring()
  #local tomringthick = 0.009;
  #local crad = rad - shellthick/2; // Radius of centre of the shell wall
  union {
    difference {
      union {
	object{ycyl(crad+tomringthick/2, 0.02) translate -0.01*y}
	ycyl(crad+tomringthick, 0.002)
	bounded_by {difference{
	  cylinder{-0.011*y, 0.011*y, crad+tomringthick+0.001}
	  cylinder{-0.012*y, 0.012*y, crad-tomringthick+0.001}}}
	clipped_by {bounded_by}
      }
      cylinder {-0.02*y, 0.02*y, crad-tomringthick/2}
    }
    difference {
      union {
	#local i=0;
	#while (i < 360)
	  #local i = i + 360 / 7;
	  union {
	    // Flange
	    object {ycyl(0.04, 0.002)
	      translate (crad-0.025)*z
	    }
	    // Rod
	    union {
	      object {ycyl(0.0035, -0.01) translate -0.02*y}
	      object {ycyl(0.0025, -0.01) translate -0.015*y}
	      ycyl(0.0015, -0.025)
	      // square nut
	      superellipsoid {<0.25, 0.08> rotate 90*x translate -y
		scale <0.0015, 0.005, 0.0015> translate 0.005*y}
	      // washer, or flange on nut
	      ycyl(0.004, 0.002)
	      translate (crad+0.008) * z
	    }
	    // receiver?  Affixed to shell.
	    union {
	      // box
	      object {superellipsoid { <0.25, 0.08>}
		scale <0.01, 0.015, 0.018>/2
	      }
	      // fat cylinder
	      object {superellipsoid { <1, 0.08>}
		rotate 90*y
		scale <0.01, 0.015, 0.018>/2
		translate -0.005*y
	      }
	      translate <0, -0.03, rad>
	    }
	    rotate i*y
	  }
	#end
      }
      cylinder {-0.1*y, 0.02*y, crad}
    }
    texture {chrometexture}
    translate (len/2) * y
  }
#end

//----------------------------------------------------------------------

#macro tomskin(dia, draw_logo)
// ambient 0.5 diffuse 0.5 
  #local _ts_f = finish {specular 1 roughness 0.0001};
  #local _ts_xmit = 0.3;
  disc {
    0, y, dia/2
    pigment {
      #if (draw_logo)
	image_map {
	  png "remoamb.png"
	  map_type 0
	  interpolate 2
	  #ifndef (CHEAP_MUSIC) 
	    transmit all _ts_xmit
	  #end
	}
      #else
	color rgb 1
	#ifndef (CHEAP_MUSIC)
	  transmit _ts_xmit
	#end
      #end
      rotate <90, 180, 0>
      translate -0.5
      scale dia
    }
    finish { _ts_f
      #ifndef (CHEAP_MUSIC) 
	reflection { 0.05, 0.1 }
      #end
    }
    // Turn off reflection for the inside of the skin.  Otherwise we get a
    // tube with mirrors at each end - infinite reflection.
    interior_texture {
      pigment { colour rgb 1
	#ifndef (CHEAP_MUSIC)
	  transmit _ts_xmit
	#end
      }
      finish { _ts_f reflection 0}
    }
  }
#end

//----------------------------------------------------------------------

//######################################################################
// Produce a tom-tom sitting below y==0, height length, shell diameter dia.
// Chrome lugs will protrude about 0.005 into +y and below y==-height.
//
#macro tom(dia, len)

  #local rad = dia / 2;    // Even after Einstein.
  #local shelley = len - 0.02;	// Head rings about 1cm.

  #local innercyl = cylinder {-shelley/2*y*1.001, len/2*y*1.001, rad-shellthick}
  #local outercyl = cylinder {-shelley/2*y, shelley/2*y, rad}

  union {
    // shell
    difference {
      object{outercyl texture { ludwigshelltexture }}
      object{innercyl
	#ifdef (NO_SHINY_DRUM_GUTS)
	  texture { ludwigshelltexture }
	  finish {reflection 0}
	#else
//	  #warning "Shiny drum guts!"
	  texture { ludwigshelltexture }
	#end
      }
    }
    object {tomring()}
    object {tomring() scale <1,-1,1>}
    object {tomskin(dia, 1) translate (len/2)*y}
    object {tomskin(dia, 0) translate (len/2)*y rotate 180*x}
    translate -len/2*y
  }
#end

//######################################################################
// Produce a rototom sitting on the origin extending into +Y.
// Anything at the origin will look like a suitable mount.
#macro rototom(dia)
  #local rad = dia/2 - 0.004;
//  ambient 0.5 diffuse 0.5 
  #local _rs_f = finish {specular 1 roughness 0.0001};
  #local _rs_xmit = 0.7;
  union {
    disc {0.059*y, y, rad
      pigment {
	color rgb 1
	#ifndef (CHEAP_MUSIC)
	  transmit _rs_xmit
	#end
      }
      finish { _rs_f
	#ifndef (CHEAP_MUSIC) 
	  reflection { 0.05, 0.1 }
	#end
      }
    }
    disc {0.0591*y, y, rad/3.5
      pigment {rgb 0.1}
      finish {_rs_f}
    }
    
    torus { rad + 0.01, 0.003 scale <1, 2, 1> translate 0.03*y}
    torus { rad + 0.01, 0.003 scale <1, 2, 1> translate 0.01*y}
    ycyl(0.02, 0.03)
    union {
      #local numarms = ((dia > 0.22) ? 6 : 4);
      #local i = 0;
      #while (i < 360)
	cylinder {0.01*y, <0, 0.03, rad+0.01>, 0.004 scale <1.4, 1, 1> rotate i*y}
	#local i = i + 360 / numarms;
      #end
      rotate rand(drumseed) * 90 * y
    }
    torus { rad, 0.004 translate 0.056*y}
    torus { rad+0.005, 0.003 scale <1, 1.5, 1> translate 0.05*y}
    union {
      cylinder {0.02*y, <0, 0.05, rad>, 0.004}
      cylinder {0.02*y, <0, 0.05, rad>, 0.004 rotate 90*y}
      cylinder {0.02*y, <0, 0.05, rad>, 0.004 rotate 180*y}
      cylinder {0.02*y, <0, 0.05, rad>, 0.004 rotate 270*y}
      rotate rand(drumseed) * 90 * y
      texture {whiteplastic_t}
    }
    texture {chrometexture}
  }
  
#end



//######################################################################
// Produce a tom-tom sitting horizontally like a bass drum,
// centre line y=dia/2, batter head -0.12, bottom head (len-0.12)*z,
// with a mounting point at the origin ready for a 25mm cylinder along -y.
// Extends from <-dia/2, 0, -0.1> to <dia/2, dia, len-0.12> plus lugs,
// and the mount into -y.  You will want to rotate this drum ~90*x.
//
// Only makes sense on toms of reasonable depth.  Don't do it on a snare.
//
#macro _tom_mountable(dia, len)

  #local _mnt_offsety = 0.07;
  #local shelley = len - 0.02;
  #local outercyl = cylinder {-shelley/2*y, shelley/2*y, dia/2 translate <0, len/2, dia/2>}

  union {
    object {tom(dia,len) translate <0, _mnt_offsety, dia/2>}
    difference {
      softboxh(<0.1, 0.08, 0.02>, 0.5) // large plate on drum shell
      // Indents:
      object{zcyl(0.005, -0.1) translate <-0.032, 0.022, -0.008>}
      object{zcyl(0.005, -0.1) translate < 0.032, -0.022, -0.008>}
      outercyl
    }
    // Outdents:
    object{zcyl(0.005, -0.012) translate < 0.032, 0.022, 0>}
    object{zcyl(0.005, -0.012) translate <-0.032, -0.022, 0>}
    cone {-0.035*z, 0.019, -0.02*z, 0.029}
    zcyl(0.02, -0.04)
    texture {chrometexture}
    rotate -90*x
  }
#end

//######################################################################
// The row of hardware clamped to the frame that runs along the front
// of Nick's kit, above the bass drums.  From PF-08.jpg.
// Audience left to right:
//
//  1: double cymbal
//  2: (down blue) bass microphone
//  3: (blue) tom microphone
//  4: (mondo) tom
//  5: (blue) snare microphone on a long boom
//  6: splash? cymbal, possibly the larger of the two
//  7: (blue) tom microphone
//  8: (mondo) tom
//  9: splash? cymbal
// 10: (mondo) tom
// 11: (blue) tom microphone
// 12: (down blue) bass microphone
// 13: double cymbal
//
// On Nick's left, from stage front to back, from PF-10.jpg.
//
// 1: cymbal, same size or fraction larger than the lower of the doubles
// 2: cymbal microphone, long stand and long boom.  Mystery disc.
// 3: china cymbal.  Mystery disc on stand.
// The rototoms are in there too.
//
// On Nick's right, from stage front to back, yam1_inset.jpg + guesswork:
//
// 1: cymbal, larger than lower of double, mirror of other side.
// 2: tom microphone on top, tom underneath (that can't be right?)
// 3: large ride? cymbal.  Vernon Fitch says Nick used one ride.
// 4: tom microphone on top, tom underneath (no)
// 5: cymbal microphone, very long stand and long boom.
// 6: china cymbal (put a tom underneath this)
 
#macro nick(xlate_v)
  union {
    union {
      // transparent/lucent slabs Nick and his kit sits on.  Must be like
      // sitting on the surface of the sun when the banks undereath hit it.
      // But this is 2014 now and Pov 3.7 has a second parameter to
      // "diffuse" for diffusely transparent objects.  TODO - try that or double_illuminate
      box {<-1.4, 0, 0.5> <0, 0.01, -1.7>}
      box {<0.007, 0, 0.5> <1.25, 0.01, -1.7>}
      pigment {rgbt <1,1,1,0.5>}
      interior {ior 4}
    }
    union {			// Nick
      object{ycyl(0.2, 0.15) translate 0.45*y texture {rubbertexture}}	// throne
//      cylinder {0, 0.5*y, 0.2 scale<1,1,0.8>rotate -15*x translate 0.6*y texture {shirttexture}}
      object {stand_legs(0.3)}
      cylinder {0.3*y, 0.45*y, 0.02 texture {chrometexture}}
    }
    
    object {bassdrum rotate 12*y translate <-0.4, 0, -1.0> }
    object {bassdrum rotate -12*y translate <0.4, 0, -1.0>}
    object {hihat(16*0.0254, 0.8) rotate -45*y translate <0.5, 0, -0.6> }
    
//  object {tom(10*0.0254, 8*0.0254) rotate <30, 0, 15> translate <0.27, 0.85, -0.8>}
//  object {tom(11*0.0254, 9*0.0254) rotate <30, 0, 0> translate <0, 0.90, -0.9>}
//  object {tom(12*0.0254, 9*0.0254) rotate <30, 0, -15> translate <-0.31, 0.90, -0.8>}

    // snare
    object {tom(12*0.0254, 6*0.0254) rotate <20, 0, 0> translate <0, 0.72, -0.65>}
    //floor toms.  Rotate on Y axis purely to get the logos on far side of drummer.
//    object {tom(14*0.0254, 14*0.0254) rotate <0, 80, -10> translate <-0.6, 0.60, -0.5> }
//    object {tom(15*0.0254, 16*0.0254) rotate <0, 90, -10> translate <-0.7, 0.60, -0.05> }

    // Frame
    union {
      union {
	object {albox(<1.7, frameh, -framed>) }
	object {albox(<0.65, frameh, -frameh>) rotate -90*z} // front left leg
	union {
	  union { cymbal_double(12*0.025, 16*0.025, 30)
	    frame_clamp()
	    translate 0.15*x
	  }
	  union { // 2: (down blue) bass microphone
	    frame_clamp()
	    object {drum_mic(<0.015, 1/6, 0.01>, 0, 0.1, 0) scale <1, -1, 1>}
	    translate 0.30*x
	  }
	  union { // 3: (blue) tom microphone
	    frame_clamp()
	    drum_mic_held(70, -20, 0.28, -0.2, 56, 0.2, 29)
	    translate 0.39*x
	  }
	  union { // 4: (mondo) tom
	    frame_clamp()
	    tom_held(13*0.025, 9*0.025, -15, 0.2, 120, 0.35, -1)
	    translate 0.49*x
	  }
	  union { //     5: (blue) snare microphone on a long boom
	    frame_clamp()
	    drum_mic_held(30, 20, 0.51, -0.2, 90, 0.15, 24)
	    translate 0.57*x
	  }
	  //  6: splash? cymbal, possibly the larger of the two
	  union { cymbal_single_jointed(7*0.025, 0.15, 0.40, 15)
	    frame_clamp()
	    translate 0.67*x
	  }
	  union { // 7: (blue) middle tom microphone
	    frame_clamp()
	    drum_mic_held(80, 0, 0.24, -0.2, 50, 0.17, 20)
	    translate 0.78*x
	  }
	  union { // 8: middle tom
	    frame_clamp()
	    tom_held(12*0.025, 9*0.025, 0, 0.15, 120, 0.30, -6)
	    translate 0.87*x
	  }
	  union { // 9: larger splash, to audience's right
	    cymbal_single_jointed(8*0.025, 0.15, 0.4, -5)
	    frame_clamp()
	    translate 1.05*x
	  }
	  union { // 10: rightmost tom
	    frame_clamp()
	    tom_held(11*0.025, 8*0.025, 5, 0.2, 125, 0.38, -8)
	    translate 1.20*x
	  }
	  union { // 11: (blue) tom microphone
	    frame_clamp()
	    drum_mic_held(70, 4, 0.28, -0.2, 56, 0.2, -24)
	    translate 1.30*x
	  }
	  union { // 12: (down blue) bass microphone
	    frame_clamp()
	    object { drum_mic(<0.015, 1/6, 0.01>, 5, 0.1, 0) scale <1, -1, 1> }
	    translate 1.40*x
	  }
	  union { cymbal_double(11*0.025, 15*0.025, -30)
	    frame_clamp()
	    translate 1.55*x
	  }
	  translate <0, frameh/2, -framed/2>
	}
	// Left frame (Audience left, Nick's right)
	union {
	  object {albox(<-1.2, frameh, framed>) }
	  object {albox(<0.65, frameh, -frameh>) rotate -90*z translate <-1.2, 0, frameh>} // rear leg
	  union {
	    union { // 1: cymbal, larger than lower of double, mirror of other side.
	      frame_clamp()
	      cymbal_single(16*0.025, 0.30, 30, 0.3, -20)
	      translate -0.3*x
	    }
	    union { // 2: tom microphone on top, tom underneath
	      frame_clamp()
	      drum_mic_held(-30, 0, 0.28, -0.4, 130, 0.16, 0)
	      object {tom_held(14*0.025, 13*0.025, 5, 0.2, -80, 0.1, 0)
		rotate 180*x
	      }
	      translate -0.7*x
	    }
	    union { // 3: large ride? cymbal.  Vernon Fitch says Nick used one ride.
	      frame_clamp()
	      cymbal_single(18*0.025, 0.40, 40, 0.3, 0)
	      translate -0.8*x
	    }
	    union { // 4: tom microphone on top, larger tom underneath
	      frame_clamp()
	      drum_mic_held(-30, 0, 0.28, -0.4, 135, 0.16, -25)
	      object {tom_held(15*0.025, 14*0.025, -10, 0.2, -80, 0.1, 30)
		rotate 180*x
	      }
	      translate -0.9*x
	    }
	    union { // 5: cymbal microphone, very long stand and long boom.
	      frame_clamp()
	      flathead_mic_held(115, 0, 0.7, -0.2, 30, 0.7, 30)
	      translate -1.0*x
	    }
	    union {
	      // 6: china cymbal.
	      frame_clamp()
	      cymbal_chinaboom(20*0.025, 50, 0.7, 15, 0.4, 0)
	      translate -1.1*x
	    }
	    translate <0, frameh/2, framed/2>
	  }
	  rotate 90*y
	}

	translate <-0.85, 0, 0>
      }
      // Right frame (Audience right, Nick's left)
      union {
	object {albox(<1.2, frameh, -framed>) }
	object {albox(<0.65, -frameh, -frameh>) rotate -90*z} // front leg
	object {albox(<0.65, -frameh, -frameh>) rotate -90*z translate 1.2*x} // rear leg
	union {
	  // 3: china cymbal.  Mystery disc on stand.
	  union { // 1: cymbal, larger than lower of double, mirror of other side.
	    frame_clamp()
	    cymbal_single(16*0.025, 0.30, 30, 0.3, 20)
	    translate 0.3*x
	  }
	  union { // 2: cymbal microphone, long stand and long boom.  Mystery disc.
	    frame_clamp()
	    flathead_mic_held(115, 0, 0.7, -0.2, 30, 0.7, 10)
	    translate 0.6*x
	  }
	  union {
	    frame_clamp()
	    cymbal_chinaboom(20*0.025, 50, 0.7, 15, 0.4, 0)
	    translate 1.1*x
	  }
	  // Rototoms
	  union {object{rototom(0.25) rotate <30, 0, -10>}
	    cymbal_rod (0, -0.05*y)
	    translate <0.96, 0.15, 0.1>}
	  union {object{rototom(0.20) rotate 30*x}
	    cymbal_rod (0, -0.05*y)
	    translate <1.21, 0.15, 0.1>}
	  union {object {rototom(0.15) rotate <30, 0, 10>}
	    cymbal_rod (0, -0.07*y)
	    translate <1.41, 0.17, 0.1>}
	  // Horizontal rod
	  object {xcyl(0.01, 0.55) scale <1, 1, 2> translate <0.97, 0.1, 0.1>}
	  object {rototom(0.32) rotate <10, 0, 0>
	    translate <1.0, 0.02, 0.30>}
	  object {rototom(0.28) rotate <10, 0, 0>
	    translate <1.35, 0.02, 0.30>}
	  translate <0, frameh/2, -framed/2>
	}
	rotate -90*y
	translate <0.85-framed, 0, 0>
      }
      translate <0, 0.65, -1.2>
    }
    translate xlate_v
    
  }
#end

//######################################################################
#macro gary(xlate_v)
  
  object {glowbox(<2.6, 2.4, 3>, Green)
    translate xlate_v
  }
#end

#macro guy(xlate_v)
  // Guy Pratt
  person(xlate_v)
#end

//######################################################################
//######################################################################
//########################   Cymbals   #################################
//######################################################################
//######################################################################
// Anatomy of my simplified cymbal:
//
// bell: blobby nippley bit (thanks Tom for that picture) in centre
// bow: everything that isn't the bell.
//
// size==dia: largest diameter.  Use "dia" because "size" is reserved.
// bell size: diameter of bell.
// profile: height of bow.
// bell profile: height of bell above height of bow
// height: profile plus bell profile
// weight/thickness: 1mm.
//
// bell profile == 2/3 (bow) profile == 2/5 of height
//
// profile looks okay around 1/20 of size?
//
// bell size looks okay at about 1/4 to 1/6 of size.  I'd like to know
// what is typical on ride and crash cymbals: ride bells are bigger.
//
// bellratio == size / bell size

//----------------------------------------------------------------------
// Produce a non-China cymbal object hanging underneath (-y) origin.
// Extents are <-dia, -(height+0.001), -dia> to <dia, 0, dia>
#macro cymbal(dia, height, bellratio)

  #local profile = height * 3/5;
  #local bellprofile = height - profile;

  // Find the radius r of a circle centred on the origin given a point
  // (partx,r-party) on its circumference.
  #macro findrad(partx, party)
    (((partx)*(partx) + (party)*(party)) / (2*(party)))
  #end // findrad macro

  // Radius of bow sphere.  Bow sphere!  We've gone all Joe Buff.
  #local bowr = findrad((dia/2), profile);
  
  // Radius of bell sphere.
  #local bellr = findrad((dia/2) * (bellratio), bellprofile);

/*
  #warning concat("bowr(", str(dia/2, 0, 2), ",", str(profile,0,4),
    ") = ", str(bowr, 0, 8))
  #warning concat("bellr(", str(dia/2*bellratio, 0, 4), ",", str(bellprofile,0,4),
    ") = ", str(bellr, 0, 8))
*/
  
  #if (1)
    // Okay, this is cool, if I say so myself.  Just set fuzz to the curviness
    // you require at the boundary between the bell and the bow.
    // In theory it was a real measurement, but 0.0004 works well which is
    // 0.4mm, so the density multiplication must be boosting it.
    #local fuzz=0.0008;
    // Like all algebra, this was easy after it was done.
    #local bigbowr=sqrt( (bowr*bowr - sqrt(2)*(bowr+fuzz)*(bowr+fuzz)) / (1 - sqrt(2)));
    #local bowdensity = pow(1-(bowr*bowr/(bigbowr*bigbowr)), 2);
    #local bigbellr=sqrt( (bellr*bellr - sqrt(2)*(bellr+fuzz)*(bellr+fuzz)) / (1 - sqrt(2)));
    #local belldensity = pow(1-(bellr*bellr/(bigbellr*bigbellr)), 2);

//    #warning concat("bigbowr=", str(bigbowr, 0, 8), ", bigbellr=", str(bigbellr,0,8))

    // bowr = visible radius of bow
    // bellr = visible radius of bell
    // bigbowr (> bowr) = radius of blob sphere that forms bow
    // bigbellr (> bellr) = radius of blob sphere that forms bell
    // bowdensity = density of bow sphere at radius==bowr
    // belldensity = density of bell sphere at radius==bellr
    // belldensity >> bowdensity
    
    // Blobs only have one density threshold, so we have to increase
    // the strength of the sphere with lower density.
    
    #local twoballs = blob {
      threshold belldensity
      sphere {-(bowr+profile)*y, bigbowr, belldensity/bowdensity}
      sphere {-bellr*y, bigbellr, 1}
    }
  #else
    // Two hard spheres instead of two blobby ones.  Identical except at boundary.
    #local twoballs = union {
      sphere { (-bowr - profile)*y, bowr}
      sphere { -bellr*y, bellr}
    }
  #end

  intersection {
    object {twoballs}
    cylinder { 0.001*y, -(height + 0.1)*y, dia/2}
    object {twoballs translate -0.001*y inverse}
    texture {T_Copper_3B
      finish {ambient 0}
      normal { onion 0.03 scale 0.002 sine_wave}
//      normal { dents 0.15 scale 0.02 }
//          normal { bumps 0.1 scale 0.02 }
    }
  }
  
#end
//----------------------------------------------------------------------
#macro sleevenut()		// Wing nut clamping the cymbal down
  object { ycyl(0.008, 0.001)}
  object { ycyl(0.004, 0.008)}
  object { ycyl(0.01, -0.02)}
  object { ycyl(0.015, -0.005) texture{rubbertexture}}
#end

#macro cymbal_rod(oneend, otherend)
  cylinder { (oneend)*y, (otherend)*y, 0.005 
    texture {chrometexture}
  }
#end

#macro cymbal_counterweight(ylate)
  object {ycyl(0.01, -0.1) translate ylate * y}
#end

#macro cymbal_joiner(xlate)		// under and over a stacked cymbal
  union {
    object { ycyl(0.008, 0.03)}
    object { ycyl(0.015, -0.01) translate 0.005*y}
    object { ycyl(0.01, -0.03)}
    object { xcyl(0.002, -0.02) translate -0.015*y} // Tightener
    object { zcyl(0.006, 0.002) scale <0.7, 1, 1> translate <-0.02, -0.015, 0>} // Wingnut
    transform xlate
  }
#end

#macro hihat_top()		// wing nut, sleeve
  union {
    ycyl(0.004, 0.03)
    ycyl(0.008, 0.015)
    object { xcyl(0.002, -0.015) translate 0.008*y} // Tightener
    object { zcyl(0.006, 0.002) scale <0.7, 1, 1> translate <-0.015, 0.008, 0>} // Wingnut
  }
#end

#macro stand_legs(height)
  union {
    #local i = 3;
    #while (i)
      #local i = i - 1;
      union {
	object{softbox(<0.002, 0.01, height*1.36>) // 1.36 == root 2, ish.
	  rotate 45*x
	  translate <-0.005, height/2, height/2>}
	object{softbox(<0.002, 0.01, height*1.36>)
	  rotate 45*x
	  translate <0.005, height/2, height/2>}
	// Stay
	object{softbox(<0.002, 0.01, height/2>) translate <0, height/2, height/4>}
	object{xcyl (0.003, 0.008) translate <-0.004, height/2, height/4>}
	object{xcyl (0.003, 0.014) translate <-0.007, height/2, height/2>}
	union { // Foot
	  sphere {0 0.015}
	  cone {0, 0.015, 0.03*y, 0.006}
	  rotate -45*x
	  translate <0, 0.014, height-0.014>
	  texture{rubbertexture}
	}
	rotate (60+120*i)*y
      }
    #end // leg loop
    object{ycyl(0.02, -0.02) translate height*y}
    object{ycyl(0.01, -0.04) translate (height+0.005)*y}
    // Tightener poking out along z, toward drummer, at top of legs
    object{zcyl(0.003, 0.03) translate (height-0.01)*y}
    // Wing nut on that tightener
    object{xcyl(0.008, 0.003) scale <1, 1, 0.6> translate <0, height-0.01, 0.03>}
    // rod poking down between legs
    cymbal_rod(height/2-0.02, height)
    // small disc at end of that rod
    object{ycyl(0.01, 0.008) translate (height/2-0.005)*y}
    clipped_by {plane {-y 0}}
    texture {chrometexture}
  }
#end


//**********************************************************************
//**********************************************************************
// Hardware mounted on the aluminium-looking frame

// Clamp that holds vertical stands to a square frame.  Frame
// cross-section is frameh height, framed depth.  See PF-08.jpg.
#macro frame_clamp()
  #local fh = frameh + 0.01;
  #local fd = framed + 0.01;
  #if (fd > 0.12)
    #warning "Bad frame depth"
  #end
  union {
    softbox (<0.08, fh, fd>)
    ycyl (0.035, fh/2+0.008)
    ycyl (0.02, fh/2+0.05)
    object {ycyl (0.03, 0.007) translate (fh/2+0.03)*y}
  }    
#end

// Offset by which the upper part of a cymbal boom/stand part,
// the boom, is offset from the lower part, the stand, when vertical.
// The upper part should then be rotated clockwise around X.
#declare pivot_offset_v = <-0.025, 0, 0.008>;

#macro frame_stand_pivot(xlatey)
  union {
    object{xcyl(0.02,  0.025) translate -0.013*x}	// Right half of "clutch"
    object{xcyl(0.02, -0.022) translate -0.013*x}	// Left half of "clutch"
//    xcyl(0.015, -0.037)		// A bit of interest on left side
    xcyl(0.003, 0.03)		// Its axle, poking out right
    object{xcyl(0.005, 0.01) translate 0.02*x} // Also its axle, larger
    superellipsoid {		// T on axle, for tightening
      <1, 0.85>
      scale <0.005, 0.005, 0.025>
      rotate rand(drumseed)*180*x
      translate 0.03 * x
    }
    texture {chrometexture}	// the superellipsoid has no texture.
    translate height*y
  }
#end
// Matching left hand side that should be offset by pivot_offset_v
// along with a cymbal rod, then rotated as desired.  The left half
// of the clutch could be here too but leaving it in
// frame_stand_pivot saves a union.
#macro frame_stand_pivot_lhs()
  softboxh(<0.02, 0.04, 0.023>, 0.1)
#end

// Fairly beefy upright capped with a pivot
#macro frame_stand(height)
  union {
    ycyl(0.01, height)
    frame_stand_pivot(height)
  }
#end
// Very beefy upright capped with a pivot
//#macro frame_stand_mondo(height)
//  union {
//    ycyl(0.012, height)
//    frame_stand_pivot(height)
//  }
//#end
// Upright with scrawny little height adjustment capped with a pivot
#macro frame_stand_wee(height)
  union {
    object {ycyl(0.01, 0.04) translate (height-0.04)*y}
    ycyl(0.004, height)
    // Height adjust:
    object {ycyl(0.007, 0.02) translate (height-0.08)*y} // sleeve
    union {
      xcyl (0.003, 0.02)
      xcyl (0.005, 0.01)
      superellipsoid {			// Height adjust tightener
	<1, 0.8>
	scale <0.008, 0.01, 0.005>
	rotate -15*x
	translate 0.02 * x
      }
      translate (height-0.07)*y
    }
    ycyl(0.01, height-0.10)
    frame_stand_pivot(height)
    texture {chrometexture}
  }
#end
#macro frame_stand_blue(height)
  cylinder{0, 0.11*y, 0.012 texture {bluerodtexture}}
  object{ycyl(0.013, 0.025) texture {rubbertexture} translate 0.1*y}
  cylinder{0.11*y, height*y, 0.009 texture {bluerodtexture}}
  object{ycyl(0.011, -0.025) texture {rubbertexture} translate (height-0.02)*y}
  frame_stand_pivot(height)
#end

// A knee.  A hinge, the top half of which must be rotated by +y.
// Think of your knee, with +x pointing out to your left.
#declare _knee_offset_v = <0, 0, -0.01>;
#macro _cymbal_knee_top()
  object {softboxh(<0.017, 0.025, 0.019>, 0.1) translate 0.015*y}
#end
#macro _cymbal_knee_bot(rotx)
  _cymbal_knee_botxform(transform{rotate rotx*x})
#end
#macro _cymbal_knee_bot_neg(rotx)
  _cymbal_knee_botxform(transform{rotate 180*y rotate rotx*x})
#end
#macro _cymbal_knee_botxform(xform)
  union {
    // Square poking out the bottom:
    object {softboxh(<0.019, 0.04, 0.019>, 0.1) translate -0.02*y}
      union {
	object {xcyl(0.02, 0.022) translate -0.011*x}
	// rectangularish protrusion
	superellipsoid {<0.2, 0.6> scale <0.01, 0.005, 0.015> translate 0.01*x}
	// knurled knob
	difference {
	  cone {-0.01*x, 0.015, -0.02*x, 0.012}
	  #local i=0;
	  #while (i < 360)
	    cylinder {<-0.013, 0.015, 0>, <-0.021, 0.012, 0>, 0.001 rotate i*x}
	    #local i=i+20;
	  #end
	}
	rotate rand(drumseed)*180*x
	translate -_knee_offset_v
      }
    texture{chrometexture}
    transform xform
  }
#end
#macro cymbal_knee_bend(rotx, ylate) 
  _cymbal_knee_top()
  #if (rotx >= 0)
    _cymbal_knee_bot(-rotx)
    translate _knee_offset_v
    rotate rotx*x
    translate -_knee_offset_v
  #else
    _cymbal_knee_bot_neg(-rotx)
    translate -_knee_offset_v
    rotate rotx*x
    translate _knee_offset_v
  #end
  translate ylate*y
#end
//----------------------------------------------------------------------
// Another hinge in a stand, meant for ~25mm dia rods holding toms.
#macro tom_knee_bend(rotx, ylate)
  ycyl(0.011, 0.023)
  object{ycyl(0.013, 0.02) translate 0.022*y texture {rubbertexture}}
  xcyl(0.02, 0.014)
  xcyl(0.02, -0.014)
  union {
    ycyl(0.011, -0.023)
    object{ycyl(0.013, -0.02) translate -0.022*y texture {rubbertexture}}
    rotate -rotx*x 
  }
  rotate rotx*x
  translate ylate*y
#end

//======================================================================
#macro hihat(dia, height)
  union{
    union {
      union {
	hihat_top()
	cymbal(dia, .02, 1/4)
	sleevenut()
	cymbal_rod(0, -(0.42 + 0.025))
	translate (.042+ 0.025)*y	// 2.5cm apart
      }
      object {
	cymbal(dia, .02, 1/4)
	scale <1, -1, 1>
      }
      cymbal_joiner(transform{})
      object {cymbal_rod(0, -(height-0.3-0.062)) scale <2, 1, 2>}
      translate (height-0.062)*y
    }
    cymbal_joiner(transform{translate 0.6*y})
    stand_legs(0.3)
  }
#end

//======================================================================
#macro cymbal_double(smalldia, largedia, yrot)

  union {
    union {
      union {
	union {
	  union {
	    union {
	      // Top cymbal and its rod
	      cymbal(smalldia, 0.03, 1/4)
	      sleevenut()
	      cymbal_rod(0, -0.12)
	      translate 0.12*y
	    }
	    // The knee between the two cymbals
	    cymbal_knee_bend(15, 0.05)
	  }
	  // Large cymbal, rod above and below, bottom of top pivot
	  // and top of middle pivot
	  cymbal_rod(-0.15, 0.05)
	  cymbal(largedia, 0.04, 1/5)
	  sleevenut()
	  translate 0.15*y
	}
	cymbal_knee_bend(-30, 0.2)
      }
      // Boom connecting upright with lower cymbal rod
      cymbal_rod(0.2, -0.21)
      cymbal_counterweight(-0.2)
      frame_stand_pivot_lhs()
      translate pivot_offset_v
      rotate 45*x
      translate(0.5*y)
    }
    // Finally, the upright.
    frame_stand(0.5)
    rotate(yrot*y)
  }
#end
//======================================================================
#macro cymbal_single_jointed(dia, boomlen, height, yrot)
  union {
    union {
      union {
	union {
	  // Top cymbal and its rod
	  cymbal(dia, 0.025, 1/4)
	  sleevenut()
	  cymbal_rod(0, -0.12)
	  translate 0.12*y
	}
	// The knee
	cymbal_knee_bend(-30, boomlen)
      }
      // Boom
      cymbal_rod(boomlen, -0.11)
      cymbal_counterweight(-0.1)
      frame_stand_pivot_lhs()
      translate pivot_offset_v
      rotate 45*x
      translate(height*y)
    }
    // Finally, the upright.
    frame_stand_wee(height)
    rotate(yrot*y)
  }
#end

//======================================================================
// A cymbal jammed directly on to the end of a boom.
#macro cymbal_single(dia, boomlen, pitch, height, yrot)
  union {
    union {
      union {
	cymbal(dia, 0.04, 1/4)
	sleevenut()
	translate boomlen*y
      }
      cymbal_rod(boomlen, -0.2)
      cymbal_counterweight(-0.15)
      frame_stand_pivot_lhs()
      translate pivot_offset_v
      rotate pitch*x
      translate(height*y)
    }
    frame_stand(height)
    rotate(yrot*y)
  }
#end

//======================================================================
#macro cymbal_chinaboom(dia, pitch1, boomlen, pitch2, height, yrot)
  union {
    union {
      union {
	union {
	  // Top cymbal and its rod
	  cymbal(dia, 0.05, 1/5)
	  sleevenut()
	  cymbal_rod(0, -0.12)
	  translate 0.12*y
	}
	// The knee
	cymbal_knee_bend(pitch1, boomlen)
      }
      // Boom
      object{cymbal_rod(boomlen/2, -0.11) scale <1.3, 1, 1.3>}
      cymbal_joiner(transform{translate boomlen/2*y})
      cymbal_rod(boomlen/2, boomlen)
      cymbal_counterweight(-0.1)
      frame_stand_pivot_lhs()
      translate pivot_offset_v
      rotate pitch2*x
      translate(height*y)
    }
    // Finally, the upright.
    frame_stand(height)
    rotate(yrot*y)
  }
#end

//======================================================================
//
#macro tom_held(dia, depth, roll, rod1, pitch, stand, rot)
  union {
    union {
      union {
	object{_tom_mountable(dia, depth) rotate roll*y}
	cylinder {0, -rod1*y, 0.012 texture{chrometexture}}
	translate rod1*y
      }
      tom_knee_bend(pitch, stand)
      rotate(rot*y)
    }
    cylinder {0, stand*y, 0.012 texture{chrometexture}}
  }
#end

//======================================================================
//
#macro drum_mic(scaler, rotx, ylate, otherend)
  union {
    union {
      union {
	prism {
	  conic_sweep linear_spline 0.6, 1, 5,
	  -1, <1, -1>, 1, <-1, 1>, -1
	  translate -y
	  texture {mictexture}
	}
	// upside-down, biggest at y=0, smallest at y=-0.4
	softbox(<2.2, 0.08, 2.2>)
	object {softboxh(<2, 0.5, 2>, 0.5)  texture{rubbertexture}
	  clipped_by {plane {-y, 0}}
	}
	// Cut it down to the size the caller wants, and flip it upright.
	scale scaler*<1, -1, 1>
	translate 0.03*y
      }
      object{xcyl(0.012, 0.01) texture{rubbertexture} translate 0.01*x}
      object{xcyl(0.012, -0.01) texture{rubbertexture} translate -0.01*x}
      object{xcyl(0.012, 0.02) translate -0.01*x}
      rotate rotx*x
      translate ylate*y
    }
    mic_boom(ylate, otherend)
    texture {bluerodtexture}
  }
#end

#macro mic_boom(ylate, otherend)
  // If this is a boom rather than a stand, make it thinner and
  // decorate it with length adjusters.
  // Booms extend beyond the origin, so otherend<0.  Stands don't.
  #if (otherend)
    union {
      #if (ylate > 0.015)
	cylinder{ylate*y, (ylate/2)*y, 0.005}
	//	object {ycyl(0.008, 0.025) texture {rubbertexture} translate (ylate/2-0.01)*y}
	sphere{0, 1 scale <0.008, 0.025, 0.008> texture {rubbertexture} translate (ylate/2-0.01)*y}
	cylinder{(ylate/2)*y, otherend*y, 0.006}
      #else
	cylinder{ylate*y, otherend*y, 0.005 texture {bluerodtexture}}
      #end
      #if (otherend < -0.015)
	// Let's have a counterweight/grip on the end.
	object {ycyl(0.007, -0.08) texture {rubbertexture} translate (otherend+0.079)*y}
      #else
//	object {} // might generate "must have at least two objs in csg"
      #end
      texture {bluerodtexture}
    }
  #else
    // Fixed-length rod from mic pivot to a clamp.  Probably short.
    // otherend==0, unless the expression in this #if has changed.
    cylinder{ylate*y, otherend*y, 0.009 texture {bluerodtexture}}
  #end
#end
    
//======================================================================
//
#macro drum_mic_held(mic_pitch, mic_yaw, boom_len, otherend, pivot_pitch,
                     stand_height, stand_yaw)
  union {
    union {
      object {drum_mic(<0.02, 1/11, 0.008>, mic_pitch, boom_len, otherend) rotate mic_yaw*y}
      frame_stand_pivot_lhs()
      translate pivot_offset_v
      rotate pivot_pitch*x
      translate stand_height*y
    }
    frame_stand_blue(stand_height)
    rotate stand_yaw*y
  }
#end

//======================================================================
//
#macro flathead_mic_held(mic_pitch, mic_yaw, boom_len, otherend, pivot_pitch,
                     stand_height, stand_yaw)
  union {
    union {
      object {hh_flathead_mic(mic_pitch, boom_len) rotate mic_yaw*y}
      mic_boom(boom_len, otherend)
      frame_stand_pivot_lhs()
      translate pivot_offset_v
      rotate pivot_pitch*x
      translate stand_height*y
    }
    frame_stand_blue(stand_height)
    rotate stand_yaw*y
  }
#end

//######################################################################
//######################################################################
//######################################################################
//######################################################################
#ifdef (TEST)

  global_settings {
    //  ambient_light rgb 0.08
    // ambient_light 0.0 radiosity { adc_bailout 0.01/3 error_bound 1.0 }
    max_trace_level 100
  }
  
  light_source { <70, 70, -70> White }
//  light_source { <0, 7, -4> White }
  
  plane {  y, 0 texture { pigment { checker colour Green colour Yellow} }}
  sky_sphere {
    pigment { gradient y color_map {[0, 1  color SkyBlue color MidnightBlue]}}
    pigment {
      bozo
      turbulence 0.65
      octaves 6
      omega 0.7
      lambda 2
      color_map {
          [0.0 0.1 color rgb <0.85, 0.85, 0.85>
                   color rgb <0.75, 0.75, 0.75>]
          [0.1 0.5 color rgb <0.75, 0.75, 0.75>
                   color rgbt <1, 1, 1, 1>]
          [0.5 1.0 color rgbt <1, 1, 1, 1>
                   color rgbt <1, 1, 1, 1>]
      }
      scale <0.2, 0.5, 0.2>
      rotate 90*x
    }
  }
  
//    camera {location <0.9, 1, -1.0> look_at <0, 0.4, 0>}
//  camera {location <0.4, 0.5, -0.8> look_at <0, 0.3, 0>}
//   object {bassdrum}
  
// camera {location <-0.1, 0.3, -0.4> look_at <0, 0.25, 0>}
// object {tom(0.203, 0.202) rotate -90*x translate <0, 0.25, 0>}
  // object {tom(0.305, 0.355) translate <-0.2, 0.25, 0>}

//  camera {location <-0.1, 0.3, -0.4> look_at <0, 0.2, 0>}
//  object {drum_mic(40, 0.2)}

//  camera {location <-0.3, 0.2, -0.63> look_at <0, 0.35, 0> angle 35}
//  union {
//    hh_flathead_mic(120, 0.05)
//    cymbal_rod(0.05, -0.15)
//    rotate -15*y
//    translate 0.33*y
//  }

  #if (0)
    // Rototom
    camera {location <-0.4, 0.48, 0.4> look_at 0}
    rototom(0.25)
  #else
    // camera {location <0, 2.5, -0.1> look_at <0, 0.5, 0.2>}
//    camera {location 16*z rotate <-45, -30, 0> look_at <0.3, 0.6, 0> angle 17} // dscn1748
    camera {location 16*z rotate <-45, -30, 0> look_at <0.3, 0.6, 0> angle 10} // dscn1748
// camera {location 2*z rotate <-25, -80, 0> look_at <0, 0.8, -1.3>} // collisions, 4:00
//    camera {location <-2, 1.2, -0.3> look_at <0, 1, -1> angle 50} // tops of toms
//      camera {location <-2.6, 1.0, -0.5> look_at <0, 0.9, -0.5>} // left frame
//      camera {location <2.6, 0.9, -0.1> look_at <0, 0.8, -0.6> angle 50} // right frame
//    camera {location <-2, 0.9, -2.2> look_at <0, 0.7, -1.4>} // frame height
//    camera {location <0, 1, -6> look_at <0, 0.6, 0.2> angle 35} // angle 35 dscn1751, head on.
//    camera {location <0, 1, -6> look_at <0.7, 1.3, -0.6> angle 8} // angle 35 dscn1751, head on.
//      camera {location <-0.1, 2, -6> look_at <0, 0.85, -1.5> angle 10} // head on close
//    camera {orthographic location <0, 1, -3> look_at <0, 0.7, -1.4>}
//    camera {location <0, 1.6, 0> look_at <-0.2, 0.4, -1.2> angle 25} // placing snare microphone
//    camera {location <-0.6, 1.0, 1> look_at <-0.6, 0.5, -0.4> angle 25} // placing floor tom mics
//    camera {location <0, 1.6, 0> look_at <-0.2, 0.4, -1.2> angle 25} // placing snare microphone
//    camera {location <-0.8, 2, 0.4> look_at <0.7, 0.5, 0.2>} // Rototoms from drummer's pers.
//    camera {location <0.2, 0.8, 1> look_at <0.5, 0.8, 0.2>}
    nick(0)
  #end
  
#end
