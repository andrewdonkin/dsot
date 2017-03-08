
#version 3.7;
#ifndef(DSOT_INC)
  #declare DSOT_INC = 1;

  #include "colors.inc"
  #include "textures.inc"
  #include "functions.inc"
  #include "math.inc"
  #include "shapes.inc"

  #declare Eps=1/100000;
  
  // So I can "#if (!NO_AREA_LIGHTS)"
  #ifndef (NO_AREA_LIGHTS)
    #declare NO_AREA_LIGHTS=0;
  #end

  #default {
    texture {
      //    pigment { rgb <1,1,0>*30 }
      finish { ambient 0 diffuse 1 }
    }
  }

  #declare bseed=seed(3);

  #declare chrometexture = texture {
    Polished_Chrome finish{ambient 0}
    //    Chrome_Metal
    #ifdef (CHEAP_MUSIC) 
      finish {reflection 0}
    #end
  };

  #declare rubbertexture = texture {
    pigment {
      bozo
      pigment_map {
	[ 0 colour Gray10 ]
	[ 1 colour Gray20 ]
      }
      scale 0.002
      //    rgb <1,1,0>*30

    }
  };
  #declare pantstexture = texture {pigment {colour Gray20 }};
  #declare shirttexture = texture {pigment {rgb 1}};
  #declare mictexture = texture {
    pigment {rgb 0.1}
    finish { specular 0 phong 0}
    normal { bumps 0.6 scale 0.0015}
  };
  #declare speakerbox_t = texture {
    pigment {rgb 0}
    finish {specular 0.1 roughness 0.15}
    normal { bumps 0.1 scale 0.0015}
  };
  #declare mirrorballbox_t = texture {
    pigment {rgb 0.4}
    finish {specular 0.1 roughness 0.15}
    normal { bumps 0.1 scale 0.002}
  };
  #declare blackplastictexture = texture {pigment {rgb 0.02} finish {Glossy}};
  #declare blackspecklyplastic_t = texture {
    pigment {rgb 0.04}
    finish { specular 0.6 roughness 0.005 reflection 0.02}
    normal {leopard 0.6  scale 0.05 warp {turbulence 0.2} scale 0.002}
  };

  #declare cable_old_t = texture {
    pigment { dents pigment_map {
      [ 0 colour rgb 0.05 ]
      [ 0.6 colour rgb 0.5 ]
      [ 1 colour rgb 1.4 ]
    }
      translate rand(bseed)
      scale 0.003
    }
    finish {specular 0.05 roughness 0.02}
  };
  #declare cable_p = pigment {
    dents pigment_map {
      [ 0 colour rgb 0.05 ]
      [ 0.1 colour rgb 0.1 ]
      [ 0.6 colour rgb 0.05 ]
      [ 0.95 colour rgb 0.2 ]
      [ 1 colour rgb 0.4 ]
    }
    scale <1, 1, 3>
  };
  #declare cable2_p = pigment {
    dents pigment_map {
      [ 0 colour rgb 0.05 ]
      [ 0.6 colour rgb 0.1 ]
      [ 1 colour rgb 0.4 ]
    }
  };
  #declare cable2_t = texture {
    pigment {cable2_p}
    translate rand(bseed)
  };
  #declare cable_t =
  texture { cells turbulence 1 lambda 4
    texture_map {
      [0 cable2_t finish {specular 0.1 roughness 0.01 reflection 0.01}]
      [1 cable2_t finish {specular 0.1 roughness 0.2 reflection 0}]
    }
    scale 3/1000
  };
  
  #declare whiteplastic_t =  texture {pigment {rgb 1} finish {Glossy}};
  #declare bluerodtexture = texture {pigment {rgb <0.2, 0.2, 1>} finish {Dull}}  ;
  #declare grille_t = texture {pigment {Gray20} finish {specular 0.9 metallic}}; // ambient 0.04 
  #declare handrail_t = texture {pigment {Yellow} finish {emission 1}};
  
  //----------------------------------------------------------------------
  #macro sign(a) 
    #if (a)
      (a/abs(a))
    #else
      0
    #end
  #end

  //----------------------------------------------------------------------
  #declare test_sky_sphere = sky_sphere {
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
  };
  #declare test_sky_sphere_dark = sky_sphere {
    pigment { gradient y color_map {[0, 1  color Navy/2 color Black]}}
    pigment {
      bozo
      turbulence 0.65
      octaves 6
      omega 0.7
      lambda 2
      color_map {
	[0.0 color rgb 0.15 ]
	[0.1 color rgb 0.25 ]
	[0.5 color rgbt <0.25, 0.25, 0.25, 1> ]
	[1 color rgbt <0.45, 0.45, 0.45, 1> ]
      }
      scale <0.2, 0.5, 0.2>
      rotate 90*x
    }
  };
  #declare test_sky_sphere_sesame = sky_sphere {
    pigment { image_map { jpeg "light_probe.jpg" map_type 1 }
    }
  };
  
  //======================================================================
  // Macros that are only used in testing; low-quality renders.
  #if (1)
    // Texture for use in placeholders, for layouts.  Luminous.
    #macro layout_t(pigm)
      texture {
	#if (pigm.x | pigm.y | pigm.z)
	  pigment {pigm}
	#else
	  pigment {rgb y}		// Default green
	#end
	finish {emission 0.8 diffuse 0.2}
      }
    #end

    #macro person(xlate)
      union {
	cone{0, 0.15, 1.1*y, 0.2 scale<1,1,0.8> texture {pantstexture}}
	cylinder{1.1*y, 1.6*y, 0.2 scale<1,1,0.8> texture {shirttexture}}
	translate(xlate)
      }
    #end
    
    // Wireframe rectangular prism, lots of ambience to be visible in the dark.
    // Sticks out like a "TODO" should.
    #macro glowbox(dimens, pigm)
      union {
	cylinder {<0,0,0>*dimens, <1,0,0>*dimens, 0.03}	// Across stage
	cylinder {<0,1,0>*dimens, <1,1,0>*dimens, 0.03}
	cylinder {<0,1,1>*dimens, <1,1,1>*dimens, 0.03}
	cylinder {<0,0,1>*dimens, <1,0,1>*dimens, 0.03}
	cylinder {<0,0,0>*dimens, <0,0,1>*dimens, 0.03} // Fore/aft
	cylinder {<0,1,0>*dimens, <0,1,1>*dimens, 0.03}
	cylinder {<1,1,0>*dimens, <1,1,1>*dimens, 0.03}
	cylinder {<1,0,0>*dimens, <1,0,1>*dimens, 0.03}
	cylinder {<0,0,0>*dimens, <0,1,0>*dimens, 0.03} // Vertical
	cylinder {<0,0,1>*dimens, <0,1,1>*dimens, 0.03}
	cylinder {<1,0,1>*dimens, <1,1,1>*dimens, 0.03}
	cylinder {<1,0,0>*dimens, <1,1,0>*dimens, 0.03}
	sphere {<0,0,0>*dimens, 0.09} sphere {<1,0,0>*dimens, 0.09}
	sphere {<0,1,0>*dimens, 0.09} sphere {<1,1,0>*dimens, 0.09}
	sphere {<0,0,1>*dimens, 0.09} sphere {<1,0,1>*dimens, 0.09}
	sphere {<0,1,1>*dimens, 0.09} sphere {<1,1,1>*dimens, 0.09}
	layout_t(pigm)
      }
    #end

  #end

  //======================================================================
  // A Round_Box with a sloped top.
  // nearh is height of the box at the near, Z=0, end.
  // farh is the height at the Z=zed end.
  // rad is the radius passed to Round_Box
  // Used in stomp boxes.
  #macro sloped_box(wid, nearh, farh, zed, rad)
    union {
      #if (nearh <= 0 | farh <= 0 | nearh >= farh)
	#error "Bad y coords to sloped_box"
      #end
      #if (zed <= 0)
	#error "Bad z coord to sloped_box"
      #end
      #if (nearh < rad*2)
	#error "nearh too small"
      #end

      #local deltah = farh - nearh;
      intersection {
	object {
	  Round_Box_Union(<0, -deltah, 0>, <wid, nearh, zed>, rad)
	  matrix <
	  1, 0, 0,
	  0, 1, 0,
	  0, 1/zed*deltah, 1,
	  0, 0, 0 >
	}
	plane {-y, -(nearh-rad)}
      }
      Round_Box_Union(<0,0,0>, <wid, nearh, zed>, rad)
    }
  #end
  
  //======================================================================
  #macro ycyl(rad, len)
    #ifdef (CHEAP_MUSIC)
      cylinder{0, (len)*y, (rad)
	texture {chrometexture}
      }
    #else
      superellipsoid { <1, 0.10>
	translate -z
	scale <rad, rad, (len)/2>
	rotate 90*x
	texture {chrometexture}
      }
    #end
  #end
  #macro zcyl(rad, len)
    object{ycyl(rad, len) rotate 90*x}
  #end
  #macro xcyl(rad, len)
    object{ycyl(rad, len) rotate -90*z}
  #end

  //======================================================================
  // Brushed metal from Rune http://runevision.com/3d/povgoodies/
  #local Brushed_Depth = 10; // Bump size
  #local Brushed_Pigment = pigment {colour rgb 0.73}
  #local Brushed_Finish = finish {
    ambient 0 diffuse 0.95
    specular 0.96 roughness 0.05
    phong 0.63 phong_size 50 brilliance 5
    reflection 0.33 metallic on
  }
  #declare Brushed_Texture =
  texture {
    average texture_map {
      [ pigment {Brushed_Pigment}
	normal {wood +Brushed_Depth ramp_wave rotate 90*x} finish {Brushed_Finish}
      ]
      [ pigment {Brushed_Pigment}
	normal {wood -Brushed_Depth ramp_wave rotate 90*x} finish {Brushed_Finish}
      ]
    }
    scale 0.01
  };
     
//======================================================================
  // A texture with averaged normals.  Bring the pigment and finish, and
  // I'll supply the normal.  Finish should be "reflection X".
  #macro blurred_tm(pigm, fin, optional _n)
    #if(0 & NO_AREA_LIGHTS)
      texture {pigment {pigm} finish {fin}}
    #else
      #ifndef(_n) #local _n=20; #end
      texture {
	average texture_map {
	  #local i=_n; #while (i)
	    [ 1 pigment { pigm }
	      normal {bozo 1 translate <rand(bseed),rand(bseed),rand(bseed)>*100 scale 0.0001}
	      finish { fin }
	    ]
	    #local i=i-1; #end
	}
      }
    #end
  #end

  /*      #local i=10; #while (i)
  [ 1 pigment { pigm }
  normal {bozo 1 translate <rand(bseed),rand(bseed),rand(bseed)>*10 scale 1000}
  finish { fin }
  ]
  #local i=i-1; #end
  */

  //======================================================================
  // Aluminium box extrusion along X axis.
  // No ends (because brushed AL end caps look bad)
  #macro albox(dimen_v)
    #local al_t = texture {
      //    Aluminum
      //    Chrome_Metal
      pigment { rgb <0.55, 0.5, 0.45> } finish { Metallic_Finish ambient 0}
      normal { bumps -1 scale <1, 0.005, 0.005>}
    };
/* This could be better:	    texture {
	      average texture_map {
		[ pigment {rgb 0.75}
		  normal {gradient z +0.01 sine_wave} finish {specular 1 roughness 0.005}
		] [ pigment {rgb 0.75}
		  normal {gradient z -0.01 sine_wave} finish {specular 1 roughness 0.005}
		]
	      } scale 0.0005 } */

    
    #ifndef (CHEAP_MUSIC)
      box{0, dimen_v texture { al_t }}
    #else
      #local bigger_v = <0, dimen_v.y * 1.01, dimen_v.z * 1.01>;
      superellipsoid {<0.25, 0.25> translate <0.5, 1, 1>
	scale <dimen_v.x, dimen_v.y/2, dimen_v.z/2>
	clipped_by {box {dimen_v - bigger_v, bigger_v}}
	//      bounded_by {clipped_by}
	texture { al_t }
      }
    #end
  #end

  //======================================================================
  #macro softbox(scaler)
    softboxh(scaler, 0.25)
  #end

  #macro softboxh(scaler, softness)
    #ifdef (CHEAP_MUSIC)
      box {-scaler/2, scaler/2 texture {chrometexture}}
    #else
      superellipsoid { <softness, softness> scale scaler/2 texture {chrometexture}}
    #end
  #end


  //======================================================================
  // Produce a hand-held microphone pointing +y pivoted at 0.
  #macro hh_flathead_mic(pitch, ylate)
    #local zoff = 0.025;
    #local splin = array[12] {
      vrotate(<0, -0.18, 0>, -pitch*x),
      vrotate(<0, -0.16, 0>, -pitch*x),
      vrotate(<0.005, -0.15, 0>, -pitch*x),
      vrotate(<0.01, -0.14, -zoff*0.2>, -pitch*x),
      vrotate(<0.01, -0.14, -zoff*0.5>, -pitch*0.8*x),
      vrotate(<0.01, -0.15, -zoff*0.8>, -pitch*0.5*x),
      vrotate(<0.005, -0.13, -zoff>, -pitch*0.02*x),
      <0.001, -0.11, -zoff>,
      <0, -0.10, -zoff>,
      <0, -0.09, -zoff>,
      <0, -0.08, -zoff>,
      <0, -0.06, -zoff>
    }
    
    union {
      union {
	union {
	  object {ycyl(0.02, 0.05) texture {mictexture} translate 0.08*y} // fuzz
	  object {ycyl(0.017, 0.005) translate 0.078*y} // chrome ring
	  cone {-0.08*y, 0.009, 0.08*y, 0.013 texture {blackplastictexture}}
	  cone {-0.08*y, 0.007, -0.095*y, 0.006 texture {rubbertexture}}
	  // Four rings around cable as it leaves the microphone
	  cylinder {-0.097*y, -0.098*y, 0.006 texture {rubbertexture}}
	  cylinder {-0.100*y, -0.101*y, 0.006 texture {rubbertexture}}
	  cylinder {-0.103*y, -0.104*y, 0.006 texture {rubbertexture}}
	  cylinder {-0.106*y, -0.107*y, 0.006 texture {rubbertexture}}
	  cone {-0.02*y, 0.013, 0.02*y, 0.014 texture {blackplastictexture}} // clip
	  object {xcyl(0.015, 0.01)
	    scale <1, 2, 1>
	    rotate 5*x
	    translate <-0.005, 0, 0.008>
	    texture {blackplastictexture}
	  }
	  translate -zoff*z
	}
	// #local i = dimension_size(splin, 1);
	//      #while (i)
	//	#local i = i-1;
	//	sphere {splin[i], 0.003 pigment { Blue}}
	//      #end
	sphere_sweep {b_spline dimension_size(splin, 1)
	  #local i = dimension_size(splin, 1);
	  #while (i)
	    #local i = i-1;
	    splin[i], 0.003
	  #end
	  texture {rubbertexture}
	}
	object {xcyl(0.015, 0.03) texture {blackplastictexture} translate <-0.015, 0, 0>}
	rotate pitch*x
      }
      object {ycyl(0.01, -0.03) texture {blackplastictexture}}
      translate ylate*y
    }
  #end

  //======================================================================
  // Produce a hand-held microphone pointing +y pivoted at 0.
  #macro hh_roundhead_mic(pitch, ylate)
    union {
      union {
	union {
	  cone {-0.06*y, 0.008, 0.11*y, 0.015 texture {blackplastictexture}}
	  cone {-0.02*y, 0.013, 0.02*y, 0.015 texture {blackplastictexture}} // clip
	  object {ycyl(0.017, 0.005) translate 0.108*y} // chrome ring
	  sphere {0.13*y, 0.025 texture {mictexture}}
	  object {xcyl(0.015, 0.01)
	    scale <1, 2, 1>
	    rotate 5*x
	    translate <-0.005, 0, 0.008>
	    texture {blackplastictexture}
	  }
	  translate -0.025*z
	}
	object {xcyl(0.015, 0.03) texture {blackplastictexture} translate <-0.015, 0, 0>}
	rotate pitch*x
      }
      object {ycyl(0.01, -0.03) texture {blackplastictexture}}
      translate ylate*y
    }
  #end

  //======================================================================
  // mic_stand() macro: produce microphone on a stand suitable for
  // vocalists: one vertical rising from a disc base.
  #declare _mic_base = union {
    isosurface { // Base of microphone stand.
      // Very useful: http://www.econym.demon.co.uk/isotut/
      function {
	(x*x + z*z - 0.18*0.18 + max(0, min(0.036, pow(max(0, (y-0.02)), 2)*20)))
      }
      contained_by { box {<-0.19, 0, -0.19>, <0.19, 0.06, 0.19>}}
      max_gradient 2.1
      //    texture {pigment {rgb 0.2 } finish {phong 0.2 specular 0.2 roughness 0.01 reflection 0.2}}
      blurred_tm(pigment {rgb 0.2 }, finish {phong 0.2 specular 0.2 roughness 0.01 reflection 0.1}, 20)
    }
    ycyl(0.013, 0.9)		// Lower part of stand, 2.6cm diameter.
    isosurface {			// Tightener
      function{
	-f_superellipsoid(x*1.8, y*1.8, z, 1, 0.1)+0.97
	+  sin(atan2(x,y)* 15) * 0.001
      }
      contained_by {box {<-0.03, -0.03, -0.03>, <0.03, 0.03, 0.03>}}
      max_gradient 3.5
      rotate 90*x
      translate 0.89*y
      texture {Chrome_Metal}
    }
  };
  #macro mic_stand(pitch, height)
    union {
      object{_mic_base}
      // Upper part, 1.6cm diameter.
      object {ycyl(0.008, height-0.9) translate 0.9*y}
      // Mics have a 2cm dia for the stand.
      hh_flathead_mic(pitch, height*y)
    }
  #end

  //######################################################################
  // opacity is how much you can see through this set of louvres
  // (how of the separation between louvres is filled by their angle).
  // Louvres 8mm thick (look for 0.004).  Default depth (along +z) 4cm.
  // Louvres slope up along +z; rotate 180*x if you want them down.

  #macro louvre(siz, slats, opacity)

    #if (siz.z = 0)
      #local siz = <siz.x, siz.y, 0.04>;
    #end

    #local absiz = <abs(siz.x), abs(siz.y), abs(siz.z)>;
    
    #local sepn=(siz.y/(slats+1));
    
    union {
      #local iy = siz.y-sepn;
      #while (abs(iy) >= abs(sepn))
	box {<0, -0.004, -absiz.z/2> <absiz.x, 0.004, absiz.z/2>
	  matrix <
	  1, 0, 0,
	  0, 1, 0,
	  0, abs(sepn)/absiz.z*opacity, 1,
	  0, 0, 0 >
	  translate <0, iy, siz.z/2>
	}
	//      box {<0, -0.004, -siz.z/2> <siz.x, 0.004, siz.z/2>
	  //	translate <0, iy-(sepn*(opacity/2)), siz.z/2>}
	#local iy = iy - sepn;
      #end
      box {<0, 0, 0> <absiz.x, sepn*0.5, siz.z>}
      box {<0, siz.y-sepn*0.5, 0> <absiz.x, siz.y, siz.z>}
      box {<0, 0, 0> <sepn*0.5, siz.y, siz.z>}
      box {<absiz.x-sepn*0.5, 0, 0> <absiz.x, siz.y, siz.z>}
      scale <sign(siz.x), 1, 1>
      texture {grille_t}
    }
    
  #end

  //####################################################################
  // Wrote this before finding Interpolate().
  #macro softly(in)
    #if (in > 1)
      #if (in < 1.00001)
	// Bloody epsilon
	#local in = 1;
      #else
	#error concat("Bad softly parameter ", str(in,0,0))
      #end
    #end
    #if (in < 0)
      #error concat("Bad softly parameter ", str(in,0,0))
    #end
    select(in-0.5,
      0.5*(1-cosd(90*in))/(1-sind(45)),
      0.5+0.5*(sind(90*in) - sind(45)) / (1-sind(45))
    )
  #end

  //####################################################################
  // whitespline: simulate a lighting curve, from black at t=0 to white
  // at t=1, but moving through red, orange, yellow instead of grey.
  // To do that, jump to <r,g,b>=<1,0.5,0> at t=0.5.

  #declare whitespline = spline {
    linear_spline
    -1, <-2, -1, -1>,		// Set the <2,1,1> slope if using cubics
    0,   <0, 0,   0>,
    0.5, <1, 0.5, 0>,
    1,   <1, 1, 1>,
    2,   <1, 2, 3>
  };

  //######################################################################
  // To simulate a spotlight projecting through a circle radius lensrad,
  // with a spread of angl degrees from the centre, back it away from
  // the projecting object (the lens) by this amount:
  #macro lightsource_backoff(lensrad, angl)
    (lensrad / tan(radians(angl)))
    //  #warning concat("backoff rad: ", str(lensrad,4,2),
      //	" angl: ", str(angl,4,2), " backoff: ", str((lensrad / tan(radians(angl))),4,2))

  #end

  //######################################################################

  #macro widgetbox(siz) 

    #local rightside = union {
      object {softboxh(<0.005, siz.y+0.005, siz.z+0.005>, 0.01)
	translate (siz.x/2-0.03)*x texture {rubbertexture}}
      object {softboxh(<0.005, siz.y+0.005, siz.z+0.005>, 0.01)
	translate (siz.x/2-0.02)*x texture {rubbertexture}}
      object {louvre(<siz.z*0.6, 0.05, 0.01>, 3, 0.1)
	rotate -90*y translate <siz.x/2+0.001, 0, -siz.x/3>}
    }
    
    union {
      difference {
	superellipsoid { <0.02, 0.02> scale siz/2}
	box {0, <0.06, -0.005, 0.1> translate <-0.03, siz.y/2+0.001, -0.06>}
	#local i = 4;
	#while (i)
	  #local i = i - 1;
	  cylinder {-0.01*y, 0.01*y, 0.01 translate <0.02, siz.y/2, 0.02*i - 0.04>}
	  cylinder {-0.01*y, 0.01*y, 0.01 translate <-0.01, siz.y/2, 0.02*i - 0.05>}
	  box {0, <siz.x/4, siz.y/6, 0.005> translate <-siz.x/3, siz.y/2 - siz.y/5*i, -siz.z/2-0.001>}
	  box {0, <siz.x/4, siz.y/6, 0.005> translate <0, siz.y/2 - siz.y/5*i, -siz.z/2-0.001>}
	#end
      }
      rightside
      object {rightside scale <-1, 1, 1>}
      object {zcyl(0.01, -0.003) scale <1, 0.5, 1> translate <0.03, -0.08, -(siz.z/2)>}
      object {zcyl(0.01, -0.003) scale <1, 0.5, 1> translate <0, -0.08, -(siz.z/2)>}
      object {zcyl(0.01, -0.003) scale <1, 0.5, 1> translate <-0.03, -0.08, -(siz.z/2)>}
      texture{speakerbox_t}
      translate siz/2
    }
    
  #end


  //######################################################################

  #declare _bendylight_r = 0.004;

  #macro bendylightbulb(radiu1, angl1, length1, radiu2, angl2, length2, colr)

    #if (angl1 > 180) #error "bendylight angle cannot be > 180"  #end
    #if (angl2 > 180) #error "bendylight angle cannot be > 180"  #end
    
    light_source {-2*_bendylight_r*z colr*10
      #if(! NO_AREA_LIGHTS) area_light 0.06*y, 2*_bendylight_r*x, 30, 3 adaptive 4 #end
      spotlight radius 20 falloff 45
      point_at -z
      fade_distance 0.08 fade_power 3.5
      // top length and bend
      translate <0, length2-0.02, radiu2>
      rotate -angl2*x
      translate -radiu2*z

      translate <0, length1, radiu1>
      rotate -angl1*x
      translate <0, 0, -radiu1>
      
      translate 0.02*y
    }
    
  #end

  //----------------------------------------------------------------------

  #macro bendylightnobulb(radiu1, angl1, length1, radiu2, angl2, length2, colr)

    #if (angl1 > 180) #error "bendylight angle cannot be > 180"  #end
    #if (angl2 > 180) #error "bendylight angle cannot be > 180"  #end
    
    #local bulb = object {ycyl(_bendylight_r, -0.05)};

    union {
      union {
	union { // Tube from top bend to light
	  difference {
	    object {ycyl(_bendylight_r, length2) texture {blackplastictexture}}
	    bulb
	  }
	  object {
	    bulb
	    texture {
	      pigment { colr }
	      finish {emission 0.7 diffuse 0.3}
	    }
	    translate <0, length2-0.005, -0.002>
	  }
	  translate radiu2*z
	  rotate -angl2*x
	  translate -radiu2*z
	}
	// Top bend
	intersection {
	  torus {radiu2, _bendylight_r*0.8 rotate 90*z}
	  plane {-y 0}
	  plane {y 0 rotate -angl2*x}
	  translate -radiu2*z
	}
	#local i = 0;
	#while (i <= 1)
	  sphere {0, _bendylight_r*1.3 scale <1,0.5,1>
	    translate radiu2*z rotate -angl2*i*x translate -radiu2*z}
	  #local i = i + 0.001/radiu2 * 360/angl2;
	#end
	// tube between two bends
	cylinder {<0, 0, 0>, <0, -length1, 0>, _bendylight_r}
	translate <0, length1, radiu1>
	rotate -angl1*x
	translate <0, 0, -radiu1>
      }
      
      // lower bend
      intersection {
	torus {radiu1, _bendylight_r*0.8 rotate 90*z}
	plane {-y 0}
	plane {y 0 rotate -angl1*x}
	translate -radiu1*z
      }
      #local i = 0;
      #while (i <= 1)
	sphere {0, _bendylight_r*1.3 scale <1,0.5,1>
	  translate radiu1*z rotate -angl1*i*x translate -radiu1*z}
	#local i = i + 0.001/radiu1 * 360/angl1;
      #end
      cylinder {-0.02*y, 0, _bendylight_r} // bottom stalk, vertical.
      translate 0.02*y
      
      texture { blackplastictexture }
    }
    
    
  #end
  //----------------------------------------------------------------------
  #macro bendylight(radiu1, angl1, length1, radiu2, angl2, length2, colr)
    union {
      bendylightbulb(radiu1, angl1, length1, radiu2, angl2, length2, colr)
      bendylightnobulb(radiu1, angl1, length1, radiu2, angl2, length2, colr)
    }
  #end
    
  //######################################################################
  // Like a softbox, but flat sides (not a superquadric) and a larger
  // radius on the corners.  Think of a Mac Mini, or a square beer coaster.
  // Called synthbox for the Kurzweil.
  // Bounded by box origin to siz
  #macro synthbox (siz, radbig, radsmall)
    #local Xx=siz.x-radsmall;
    #local XX=siz.x-radbig;
    #local Yy=siz.y-radsmall;
    #local ZZ=siz.z-radbig;
    #local Zz=siz.z-radsmall;
    union {
      // verticals
      Round_Cylinder_Union(<radbig, 0, radbig>, <radbig, siz.y, radbig>, radbig, radsmall) // FL
      Round_Cylinder_Union(<radbig, 0, ZZ>,     <radbig, siz.y, ZZ>, radbig, radsmall) // RL
      Round_Cylinder_Union(<XX, 0, radbig>, <XX, siz.y, radbig>, radbig, radsmall) // FR
      Round_Cylinder_Union(<XX, 0, ZZ>,     <XX, siz.y, ZZ>, radbig, radsmall) // RR
      
      // bottom edges
      cylinder{<radsmall, radsmall, radbig>, <radsmall, radsmall, ZZ>, radsmall}
      cylinder{<Xx, radsmall, radbig>, <Xx, radsmall, ZZ>, radsmall}
      cylinder{<radbig, radsmall, radsmall>, <XX, radsmall, radsmall>, radsmall}
      cylinder{<radbig, radsmall, ZZ>, <XX, radsmall, ZZ>, radsmall}
	
      // top edges
      cylinder{<radsmall, Yy, radbig>, <radsmall, Yy, ZZ>, radsmall}
      cylinder{<Xx, Yy, radbig>, <Xx, Yy, ZZ>, radsmall}
      cylinder{<radbig, Yy, radsmall>, <XX, Yy, radsmall>, radsmall}
      cylinder{<radbig, Yy, ZZ>, <XX, Yy, ZZ>, radsmall}
      
      // Now it looks like a boxing ring, only two ropes.
      box{<0, radsmall, radbig>, <siz.x, Yy, ZZ>}
      box{<radbig, radsmall, 0>, <XX, Yy, siz.z>}
      
      // Now it looks like someone put sides on and nearly filled it with water
      box{<radsmall, 0, radbig>, <Xx, siz.y, ZZ>}
      box{<radbig, 0, radsmall>, <XX, siz.y, Zz>}
      
    }
  #end
  
  //######################################################################

//  #declare TEST_SYNTHBOX=1;
  #ifdef (TEST_SYNTHBOX)
    camera { location <-0.5, 1, -2> look_at <0.5, 0.5, 0> }
    object {synthbox (<2, 0.6, 2>, 0.2, 0.02)
      pigment {rgb 0.5}
    }
    plane {  y, 0 texture { pigment { checker colour Green colour Yellow} }}
    light_source {<-0.3, 3, -3>, White spotlight point_at y}
  #end
  
  #ifdef (TEST_SOFTLY)
    camera { location <0.5, 0.5, -2> look_at <0.5, 0.5, 0> }
    cylinder {0, x, 0.003 finish { ambient 1 } pigment {rgb z}}
    cylinder {0, y, 0.003 finish { ambient 1 } pigment {rgb z}}
    cylinder {x, x+y, 0.003 finish { ambient 1 } pigment {rgb z}}
    cylinder {y, x+y, 0.003 finish { ambient 1 } pigment {rgb z}}
    #local i = 0;
    #while (i <= 1)
      sphere {<i, softly(i), -0.01>, 0.005 finish { ambient 1 } pigment {rgb y}}
      sphere {<i, Interpolate(i, 0, 1, 0, 1, 0), -0.02>, 0.005 finish { ambient 1 } pigment {rgb z}}
      sphere {<i, -0.01, 0> 0.005 finish {ambient 1} pigment {rgb whitespline(i)}}
      #warning concat(str(i, 2, 2),
	": <", str(whitespline(i).x, 2, 2),
	",", str(whitespline(i).y, 2, 2),
	",", str(whitespline(i).z, 2, 2),
	">")
      
      #local i = i + 0.02;
    #end
  #end
//  #declare TEST_BENDYLIGHT = 1;
  #ifdef (TEST_BENDYLIGHT)
//    camera {location <-0.4, 0.3, -0.3> look_at <0, 0.2, 0> angle 30}
    //  camera {location <-0.2, 0.3, -0.2> look_at <0.1, 0.2, 0.1> angle 20}
      camera {location <-0.3, 0.3, -0.9> look_at <0.1, 0.15, 0> angle 30} // for checking shadow
//    light_source {<-0.3, 1, -1>, White spotlight point_at 0}
    plane {  y, 0 texture { pigment { checker colour Green colour Yellow} }}
    object {bendylight(0.05, 40, 0.10, 0.03, 70, 0.1, rgb 1) translate <0.1, 0.19, 0.15>}
    object {widgetbox(<0.2, 0.2, 0.2>)}
  #end
//  #declare TEST_MICROPHONE = 1;
  #ifdef (TEST_MICROPHONE)
    camera {location <0, 1.4, -1.7> look_at <0, 0, 0> angle 20}
//    camera {location <-0.2, 0.3, -0.2> look_at <0.1, 0.2, 0.1> angle 20}
    light_source {<-0.3, 3, -3>, White spotlight point_at y}
    plane {  y, 0 texture { pigment { checker colour Green colour Yellow} }}
    object{mic_stand(45, 1.6) rotate 45*y}
  #end
//  #declare TEST_SLOPEDBOX = 1;
  #ifdef (TEST_SLOPEDBOX)
    camera {location <-1, 1, -1> look_at <0, 0, 0> angle 50}
    light_source {<-1, 2, -3>, White spotlight point_at y radius 10}
    plane {  y, 0 texture { pigment { checker colour Green colour Yellow} }}
    object{sloped_box(0.6, 0.2, 0.5, 0.8, 0.04)
      translate <-0.5, 0, -0.5>
      texture {pigment {rgb 0.5}}
    }
  #end
#end

#if (0)
 sky_sphere {
//      pigment {
//          function { max(min(y,1),0) }
//          color_map { cm_realsky_01 }
//      }
      scale (1/2)
      pigment {
          function { abs(max(min(x,1),-1)) }
          color_map {
              [ 0.0 rgbt <0.1, 0.1, 0.1, 1.0> ]
              [ 0.2 rgbt <0.1, 0.1, 0.1, 1.0> ]
              [ 0.6 rgbt <0.1, 0.1, 0.1, 0.9> ]
              [ 1.0 rgbt <0.1, 0.1, 0.1, 0.6> ]
          }
      }  
  }
#end
#warning "dsot.inc out"
