#include "textures.inc"

#ifndef (NO_DALEK_CHOPSTICKS)
  #declare NO_DALEK_CHOPSTICKS=0;
#end

#ifndef (DALEK_LIGHT_CYLINDER)
  #declare DALEK_LIGHT_CYLINDER=0;
#end

#declare dseed = seed(4);
  
// Produce a dalek with bottom centre of the plinth on 0, facing -z.
// Most other macros face +z, but the daleks all face the audience so
// this one will place the riser +z of the plinth, and the lights
// facing -z.
#macro dalek(lightcolour1, lightcolour2, horizt, vertt, boxpitch, rot)
  
  #local plinththick = 0.220;		// More like height, really.
  #local plinthradius = 0.30;		// Radius of plinth
  #local dalekradius = 0.23;		// Radius of sides
  #local sidesep = plinthradius * 1.3;	// Separation of sides
  #local boxplinthsep = 0.13;

  #local boxheight = dalekradius * 1.59;
  #local boxdepth = dalekradius * 1.2;
  #local boxwidth = sidesep - 0.06;

  #local holewidth = boxwidth / 4;
  #local holeheight = boxheight / 9;
  
  union {
    
    // Sides and lightbox combined, translated +z
  union {
      
      /**********************************************************************
       * Sides.  On each side, a slice of a sphere extends beyond a disc.
       */
      #local ballrad = plinthradius*0.8;
      #local ballcutoffangle = radians(50); // too complicated to explain.  Read on.
      #local yaxis = plinththick + boxplinthsep + boxheight/2;
      #local side = 1;
      #while (side > -2)
	union {
	  // reflective semispheres
	  sphere {
	    <-ballrad * cos(ballcutoffangle), yaxis, 0> ballrad
//	    texture { pigment {Gray10 } finish {phong 1 phong_size 300 reflection 0.0} }
//	    texture { pigment {Gray10 } finish {phong 1 phong_size 300 reflection 0.4} }
	    blurred_tm(pigment {Gray10 }, finish {phong 1 phong_size 300 reflection 0.4})
	    clipped_by {
	      cylinder {
		<0.001, yaxis, 0>,
		<ballrad*sin(ballcutoffangle), yaxis, 0>
		ballrad * sin(ballcutoffangle)
	      }
	    }
	    //	  bounded_by {clipped_by} causes warning if left in
	  }
	  intersection {
	    union {
	      // White disc.  The inside is reflective, the outside not,
	      // so make it 2cm thick and cut 1cm off with a non-reflective plane.
	      cylinder {
		<-0.01, yaxis, 0>,
		<0.01, yaxis, 0>
		dalekradius
	      }
	      // "leg" that connects white disc to plinth.
	      prism { linear_sweep linear_spline
		-0.01, 0.01
		21,
		// The perimeter
		<0, plinththick>,
		<dalekradius*0.7,   yaxis-dalekradius*0.7>,
		<-dalekradius+0.05, yaxis-dalekradius*0.7>,
		<-dalekradius,      yaxis-dalekradius*0.9>,
		<-dalekradius,      plinththick>,
		<0, plinththick>,
		#local i=0; #while (i < 3)
		  // Subtract a slot
		  <-dalekradius+0.02+0.01*i, plinththick + 0.02 + 0.02*i>,
		  <-dalekradius+0.18+0.02*i, plinththick + 0.02 + 0.02*i>,
		  <-dalekradius+0.18+0.02*i, plinththick + 0.03 + 0.02*i>,
		  <-dalekradius+0.02+0.01*i, plinththick + 0.03 + 0.02*i>,
		  <-dalekradius+0.02+0.01*i, plinththick + 0.02 + 0.02*i> // commas optional
		  #local i=i+1;
		#end
		rotate <-90, -90, 0>
	      }
	      // Add "exponent" to this texture so that the sides of the dalek
	      // reflect the bright lights but nothing else.
	      // texture {finish {reflection {0.2 exponent 0.3}} pigment{colour rgb 0.9}}
	      // texture {
		// average texture_map {
		  // #local i=10; #while (i)
		  // [ 1 pigment { colour rgb 0.9 }
		    // normal {bozo 1 translate <rand(dseed),rand(dseed),rand(dseed)>*10 scale 0.001}
		    // finish { reflection {0.2 exponent 0.3} }
		    // ]
		  // #local i=i-1; #end
		  // }
		// }
	    }
	    // Now slice that 2cm-thick disc+leg in half, leaving the
	    // outside non-reflective.
//	    plane { x, 0.01 texture {finish {reflection 0} pigment{colour rgb 0.9}} }
	    plane { -x, 0 blurred_tm(pigment{colour rgb 0.9}, finish {reflection {0.2 exponent 0.9 falloff 2}})}
	    // ...and make the default texture reflective 0.2 exponent 0.3
	    // falloff made no difference to output file
//	    blurred_tm(pigment{colour rgb 0.9}, finish {reflection {0.2 exponent 0.9 falloff 2}})
	    texture {finish {reflection 0} pigment{colour rgb 0.9}}
	  }
	  // Small box to connect lightbox with sides.
	  box {
	    <0, yaxis + boxheight/4, boxdepth/4>,
	    <-(sidesep/2-boxwidth/2),  yaxis - boxheight/4, -boxdepth/4>
	  }
	  translate sidesep/2 * x
	  scale <side, 1, 1>		// Mirror along x=0
	}
	#local side = side - 2;
      #end // while
      
      /***********************************************************************/
      // Box containing all the lights
      union {
	difference {
	  #local holexgap = (boxwidth - 2*holewidth)/3;
	  #local holeygap = (boxheight - 4*holeheight)/5;
	  union {
	    // The big box
	    object {softboxh(<boxwidth, boxheight, boxdepth>, 0.02)
	      translate boxheight/2*y
	      texture { pigment { colour rgb <0.5, 0.5, 0.5> }
		finish { specular 0.5 roughness 0.01 reflection 0.2 }
	      }
	    }
	    object {softboxh(<boxwidth*0.5, boxheight*0.4, 0.01>, 0.15)
	      translate <0, boxheight*0.65, -boxdepth/2>
	    }
	    // Plus an eave that will be over each hole, when they're cut later.
	    #local i = 4;
	    #while (i)
	      #local i = i - 1;
	      #local loopx = -boxwidth/2 + holexgap;
	      #local loopy = (holeygap+holeheight)*i + holeygap;
	      #local side = 1;
	      #while (side > -2)
		difference {
		  box {
		    <loopx - 0.005, loopy + holeheight*0.85, boxdepth/2> * <side,1,1>,
		    <loopx + 0.005 + holewidth, loopy + holeheight*1.1, boxdepth/2+holeheight/4> * <side,1,1>
		  }
		  cylinder {
		    <loopx - 0.006, loopy+holeheight*0.8, boxdepth/2 + holeheight/4> * <side, 1, 1>,
		    <loopx + 0.006 + holewidth, loopy+holeheight*0.8, boxdepth/2 + holeheight/4> * <side, 1, 1>
		    holeheight/4
		  }
		}
		#local side = side - 2;
	      #end // while
	    #end // while
	    texture { pigment { colour rgb <0.5, 0.5, 0.5> }
	      finish { specular 0.5 roughness 0.01 reflection 0.2 }
	    }
	  }
	  
	  // Eight holes.
	  #local i = 4;
	  #while (i)
	    #local i = i - 1;
	    #local loopy = (holeygap+holeheight)*i + holeygap;
	    #if (i > 1)
	      #local colr=lightcolour1*3;
	    #else
	      #local colr=lightcolour2*3;
	    #end
	    #local side = 1;
	    #while (side > -2)
	      // !!! subtract these, then add sleeves (liners) full of media
	      light_group {
		light_source {
		  <-boxwidth/2 + holexgap + holewidth/2, loopy+holeheight/2, boxdepth/4> * <side, 1, 1>
		  color colr
		  fade_distance 0.1
		  fade_power 999
		  media_interaction on
		}
		box {
		  <-boxwidth/2+holexgap, loopy, -boxdepth/2+0.01> * <side, 1, 1>,
		  <-boxwidth/2+holexgap+holewidth, loopy+holeheight, boxdepth/2+0.01> * <side, 1, 1>
		  texture {
		    pigment {color rgb 1}
		    finish {brilliance 1.4 ambient 0 diffuse 1}
		  }
		  interior {
		    ior 1
		    media { scattering { 2, rgb 1 extinction 0} }
		  }
		}
	      }
	      #local side = side - 2;
	    #end // while
	  #end // while
	  //	translate (plinththick + boxplinthsep) * y
	}

	#macro rotateit()
	  #local noti = (i = 0 ? 4 : i);
	  #local noti = i;
	  #if (horizt)
	    rotate y * 1 * cosd(horizt+(noti+side/4)*90)
	  #end
	  #if (vertt)
	    rotate x * 5 * cosd(vertt+(noti+side/4)*90)
	    #if (i = 3)
	      rotate x * 1 // top row, pitch them down
	    #end
	    #if (i = 0)
	      // bottom pair
	      rotate x * -1 // bottom pair, pitch them up
	    #end
	  #end
	#end				// macro
	
	// The real light sources.
	#local i = 4;
	#while (i)
	  #local i = i - 1;
	  #if (i > 1)
	    #local colr=lightcolour1*7;
	  #else
	    #local colr=lightcolour2*7;
	  #end
	  #local loopy = (holeygap+holeheight)*i + holeygap; // + plinththick + boxplinthsep;
	  #local side = 1;
	  #while (side > -2)
	    // This source creates small illuminated circles on objects they strike.
	    // They will interact with environmental media, if there is any.
	    light_source {
	      #if (NO_DALEK_CHOPSTICKS)
		<0, 0, -lightsource_backoff(holeheight/2, 0.5)>
	      #else 
		<0, 0, 2.03>
	      #end
	      #if (DALEK_LIGHT_CYLINDER)
		colour colr*10
		cylinder
		radius holeheight/2-0.02
		falloff holeheight/2+0.28
		tightness 20		// Make it a bit furrier
	      #else
		colour colr*8
		spotlight radius 0.2 falloff 0.5
		projected_through {
		  disc {0, z, holeheight/2}
		}
		#if(! NO_AREA_LIGHTS) area_light 0.05*y, 0.05*x, 6, 6 adaptive 1 #end
	      #end
	      point_at 10*z
	      rotateit()
	      // Box containing scattering media extends to z=boxdepth/2+0.01
	      translate <-boxwidth/2 + holexgap + holewidth/2, loopy+holeheight/2, boxdepth/2>
	      * <side,1,1>
	    }

	    #local side = side - 2;
	  #end
	#end

	#if (NO_DALEK_CHOPSTICKS)
//	  #warning "Not rendering Dalek chopsticks!"
	#else
	  light_group {
	    #local i = 4;
	    #while (i)
	      #local i = i - 1;
	      #if (i > 1)
		#local colr=lightcolour1*7;
	      #else
		#local colr=lightcolour2*7;
	      #end
	      #local loopy = (holeygap+holeheight)*i + holeygap;
	      #local side = 1;
	      #while (side > -2)
		// Now a nearly-identical light source that only produces "laser beams"
		// inside a box of media protuding from the dalek like chopsticks.
		// Actually they shine from <0,0,2z> into the unit, so that they flow
		// right into the rectangular holes.
		light_source {
		  2*z
		  colour colr
		  cylinder
		  radius holeheight/2-0.01
		  falloff holeheight/2+0.01
		  tightness 1		// Make it a bit furrier
		  point_at -z
		  rotateit()
		  translate <-boxwidth/2 + holexgap + holewidth/2, loopy+holeheight/2, boxdepth/2>*<side,1,1>
		}
		#local side = side - 2;
	      #end
	    #end
	    #local i = 4;
	    #while (i)
	      #local i = i - 1;
	      #if (i > 1)
		#local colr=lightcolour1*7;
	      #else
		#local colr=lightcolour2*7;
	      #end
	      #local loopy = (holeygap+holeheight)*i + holeygap;
	      #local side = 1;
	      #while (side > -2)
		cylinder {
		  0.01*z, 2*z,
		   holeheight/2+0.01 texture { pigment {rgbt 1} finish {ambient 0 diffuse 0}}
//		  0.005 texture { pigment {colr} finish {ambient 2 diffuse 1}}
		  hollow
		  interior {
		    media {
		      density {
			planar
			rotate 90*x
			scale <1, 1, 2>
		      }
		      // This light is pointing into the dalek, and I want
		      // it to get brighter as it moves away from its POV source.
		      // So use negative extintion and eccentricity.
		      scattering { 4, rgb 1/4 extinction -10}
		      method 3
		      //		intervals 8
		      //		samples 4,100
		    }
		  }
		  rotateit()
		  translate <-boxwidth/2 + holexgap + holewidth/2, loopy+holeheight/2, boxdepth/2>*<side,1,1>
		}
		#local side = side - 2;
	      #end
	    #end
	  }
	#end			// #if no_dalek_chopsticks
	translate -boxheight/2 * y
	rotate x*boxpitch
	translate boxheight/2 * y
	translate (plinththick + boxplinthsep) * y

      }
      /***********************************************************************
       * Some cables connecting the plinth to the lightbox underside.
       * Pitching the lightbox breaks them somewhat.
       */
      #include "dalekcables.inc"
      translate 0.15 * z
      rotate (180+rot)*y
    }

    // More cables, from the plinth running down the riser.  Not rotated.
    #include "dalekplinthcables.inc"
    
    /**********************************************************************
     * Build a stack of 12 discs, alternating white and black, to a
     * total thickness of plinththick metres, then cut a hole in the top
     */
    difference {
      union {
	#local darkdiscpig = pigment{colour rgb <0.2, 0.2, 0.2>}
	#local lightdiscpig = pigment{colour rgb <0.8, 0.2, 0.2>}
	#local i = 3;
	#while (i)
	  #local i = i - 1;
	  union {
	    cylinder {
	      0, -plinththick/6 * 2/3 * y, plinthradius
	      texture {lightdiscpig}
	    }
	    cylinder {
	      -plinththick/6 * 2/3 * y, -plinththick/6 * y, plinthradius
	      texture {darkdiscpig}
	    }
	    translate (plinththick - i * plinththick/6) * y
	  }
	  union {
	    cylinder {
	      0, plinththick/6 * 2/3 * y, plinthradius
	      texture {lightdiscpig}
	    }
	    cylinder {
	      plinththick/6 * 2/3 * y, plinththick/6 * y, plinthradius
	      texture {darkdiscpig}
	    }
	    translate i * plinththick/6 * y
	  }
	#end // while
      }
      cylinder {plinththick/2*y, (plinththick+0.001)*y, plinthradius/4 pigment {rgb 0}}
    }
    // Now for the vertical that this thing rises on.
    // See 128068354SbVjpY_ph.jpg
    object {
      softboxh(<plinthradius*1.4, plinththick/5-0.015, plinthradius*1.3>, 0.05)
      translate <0, -(plinththick/5-0.015)/2-0.015, plinthradius-(plinthradius*1.3/2)>
    }
    box {<-plinthradius*0.7, -0.015, -plinthradius/3>,
      <plinthradius*0.7, 0, plinthradius>
      texture {grille_t}
    }
  union {
      difference {
	union {
	  superellipsoid {<0.7, 0.05>	// cylinder along Z
	    rotate 90*y
	    scale 0.103
	    translate <0, 0.0, plinthradius-0.05>
	  }
	  superellipsoid {<0.7, 0.10>
	    rotate 90*y
	    scale 0.1
	    translate <0, -0.05, plinthradius>
	  }
	  superellipsoid {<0.01, 0.05>	// box, stretched into column.
	    scale <0.1, 3, 0.1>
	    translate <0, -3, plinthradius>
	  }
	  superellipsoid {<0.01, 0.05>
	    scale <0.103, 3, 0.1>
	    translate <0, -3, plinthradius-0.05>
	  }
	}
	box {<-0.2, 0, plinthradius>, 
	  <1, -7, 0>
	}
      }
      intersection {
	superellipsoid {<0.1, 0.25> scale <0.1, 0.03, 100>}
	box {<-1, -1, plinthradius*1.42>, <1, 1, 0>}
	translate 0.03*y
	rotate 45*x
      }
      texture{grille_t}
    }
  }

#end // macro dalek

######################################################################
#ifdef (TEST)
  #include "dsot.inc"
  #include "colors.inc"
  
  global_settings {
    ambient_light rgb 0.08
    // ambient_light 0.0 radiosity { adc_bailout 0.01/3 error_bound 1.0 }
    max_trace_level 3
  }
  

  plane {
    y, 0
    texture {
      pigment { rgb 1}
    }
  }

  camera {
//  location <-0.6, 0.8, 0.8> look_at <0, 0.3, 0.2> // mount
//  location <0, 0.9, 0.9> look_at <0, 0.3, 0.3> // mount, close
//  location <0, 0.2, -3> look_at <0, 0.8, 0>  // matches dscn1746.jpg
  // for checking lights.  Use 135 degree rotation
//  location <0.6, 3, -3.5> look_at <1.5, 0, -1.5>
    location <0.3, 1.2, -1.5> look_at <0.5, 0.9, 0> // right down the barrel
//  location <-0.1, 0.3, -0.4> look_at <0, 0.2, 0> // check cables
}
//light_source { <7, 7, -10> White media_interaction off}
  light_source { <-6, 20, -20> White spotlight radius 1 falloff 2 point_at y media_interaction off}
//light_source { <-6, 20, 30> White }
//light_source { <-1, 0.2, -1> White }

//  plane { -z, -3 pigment {rgb 1}}
  
  object {
    #if (clock_on)
//      dalek(Green, Blue, 1 + clock*360, 1 + clock*360*6, 0, -135)
      dalek(Green, Blue, 0, 0, 0, -90 + 180*clock)
    #else
      dalek(Green, Blue, 0, 1, 0, -55)
    #end
    translate y/2
  }

  #if(0)box {<-2, 0, -1>, <2, 2, 1> hollow
      interior { ior 1
	media {
	  scattering {2, 0.2 extinction 0.1}
	  samples 10 intervals 1 method 3
	}
      }
      texture {	pigment {rgbt 1}}
    }
  #end
  
//  media { scattering { 2, rgb 1/5 extinction 0} }
#end
