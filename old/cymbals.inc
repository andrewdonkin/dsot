#include "colors.inc"
#include "dsot.inc"
#include "metals.inc"

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
    texture {T_Copper_2C
      normal { onion 0.05 scale 0.002 sine_wave}
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


//----------------------------------------------------------------------
// Hardware mounted on the aluminium-looking frame

// Clamp that holds vertical stands to a square frame.  Frame
// cross-section is fh height, fd depth.  See PF-08.jpg.
#macro frame_clamp(fh, fd)
  #local fh = fh + 0.01;
  #local fd = fd + 0.01;
  #if (fd > 0.12)
    #warning "Bad frame depth"
  #end
  union {
    softbox (<0.12, fh, fd>)
    ycyl (0.04, fh/2+0.008)
    ycyl (0.02, fh/2+0.05)
    object {ycyl (0.03, 0.007) translate (fh/2+0.03)*y}
  }    
#end

// Offset by which the upper part of a cymbal boom/stand part,
// the boom, is offset from the lower part, the stand, when vertical.
// The upper part should then be rotated clockwise around X.
#local pivot_offset_v = <-0.025, 0, 0.008>;

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
      rotate -45*x
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
#macro frame_stand_mondo(height)
  union {
    ycyl(0.012, height)
    frame_stand_pivot(height)
  }
#end
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
	rotate 15*x		// RAND
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

//======================================================================
#macro hihat(dia, height)
  union{
    union {
      union {
	hihat_top()
	cymbal(dia, .02, 1/4)
	sleevenut()
	cymbal_rod(0, -0.062)
	translate (.042+ 0.020)*y	// 2cm apart
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
	  cymbal_rod(-0.2, 0.05)
	  cymbal(largedia, 0.04, 1/5)
	  sleevenut()
	  translate 0.2*y
	}
	cymbal_knee_bend(-30, 0.2)
      }
      // Boom connecting upright with lower cymbal rod
      cymbal_rod(0.2, -0.21)
      cymbal_counterweight(-0.2)
      frame_stand_pivot_lhs()
      translate pivot_offset_v
      rotate 45*x
      translate(0.6*y)
    }
    // Finally, the upright.
    frame_stand(0.6)
    rotate(yrot*y)
  }
#end
//======================================================================
#macro cymbal_single(dia, boomlen, height, yrot)

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
// The row of hardware clamped to the frame the runs along the front
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
// On Nick's right, from stage front to back, yam1-inset.jpg + guesswork:
//
// 1: cymbal, larger than lower of double, mirror of other side.
// 2: tom microphone
// 3: large ride? cymbal.  Vernon Fitch says Nick used one ride.
// 4: tom microphone
// 5: cymbal microphone, very long stand and long boom.
// 6: china cymbal
 

//======================================================================
#ifdef (TEST)
  
  #include "colors.inc"
  #include "dsot.inc"
  
  global_settings {
    //  ambient_light rgb 0.08
    // ambient_light 0.0 radiosity { adc_bailout 0.01/3 error_bound 1.0 }
    max_trace_level 100
  }

  light_source { <50, 70, -100> White }
  
  plane {
    y, 0
    texture {
      pigment { checker colour Green colour Yellow}
    }
  }
  sky_sphere {
    //      pigment { checker colour Green colour Yellow}
    pigment { gradient y color_map {[0, 1  color LightBlue/2 color LightBlue]}}
  }

  #if (1)
    camera {location <0, 2.9, -1.2> look_at <-0.4, 1.0, 0> angle 45}
    camera {location <0, 1, -2> look_at <-0.4, 0.5, 0> angle 50}
    union {
      cymbal_double(12*0.025, 18*0.025, 30)
      frame_clamp(0.04, 0.08)
      translate <-0.7, 0.04, 0>
    }
    union {
      cymbal_single(8*0.025, 0.30, 0.50, 25)
      frame_clamp(0.04, 0.08)
      translate <-0.3, 0.04, 0>
    }
    union {
      cymbal_single(6*0.025, 0.30, 0.50, -25)
      frame_clamp(0.04, 0.08)
      translate <0.3, 0.04, 0>
    }
    union {
      cymbal_double(11*0.025, 17*0.025, -30)
      frame_clamp(0.04, 0.08)
      translate <0.7, 0.04, 0>
    }
  #end
  #if (0)
 camera {location <-0.1, 0.9, -1> look_at 0.4*y} // whole object
//     camera {location <-0.1, 0.9, -0.30> look_at 0.75*y} // top
    //  camera {location <0.1, 0.8, -0.35> look_at 0.7*y} light_source { <50, 0.5, -70> White } // underside
    //  camera {location <0.1, 0.4, -0.35> look_at 0.12*y} // legs
    //  camera {location <0.1, 0.3, -0.45> look_at 0*y} // feet
    hihat(14 * 0.0254, 0.8)
  #end
  #if (0)
  camera {location <-0.4, 0.5, -1> look_at 0.35*y angle 50}
    union {
      union {
	frame_clamp(0.04, 0.08)
	frame_stand_mondo(0.3)
	translate <-0.2, 0, 0>
      }

      frame_clamp(0.04, 0.08)
      union {
	union {
	  union {
	    union {
	      union {
		cymbal(16*0.0254, .03, 1/4)
		sleevenut()
		cymbal_rod(0, -0.15)
		translate 0.15*y
	      }
	      cymbal_knee_bend(20, 0.09)
	    }
//	    cymbal_knee_boty(0.09)
	    cymbal_rod(0, 0.09)
	    cymbal_knee_bend(-20, 0.09)
	  }
	  cymbal_rod(0.09, -0.1)
	  cymbal_counterweight(-0.1)
	  frame_stand_pivot_lhs()
	  translate pivot_offset_v
	  rotate 30*x
	  translate 0.3*y
	}
	frame_stand(0.3)
	rotate -25*y
	texture {chrometexture}
      }
      union {
	frame_clamp(0.04, 0.08)
	frame_stand_wee(0.3)
	translate <0.2, 0, 0>
      }
      translate <0, 0.03, 0>
    }

  #end
  
#end
