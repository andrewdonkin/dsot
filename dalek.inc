#include "textures.inc"
#include "shapes.inc"

#ifndef (RADIOSITY_PASS)
  #local RADIOSITY_PASS = ""
#end

#ifndef (NO_DALEK_CHOPSTICKS)
  #declare NO_DALEK_CHOPSTICKS=1;
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
  #local plinthradius = 0.32;		// Radius of plinth
  #local dalekradius = 0.23;		// Radius of sides
  #local sidesep = plinthradius * 1.4;	// Separation of sides
  #local boxplinthsep = 0.1;

  #local boxheight = dalekradius * 1.7;// 1.59
  #local boxdepth = dalekradius * 1.2;
  #local boxwidth = sidesep - 0.12;

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
      #local yaxis = plinththick + boxplinthsep + boxheight/1.8;
      #local side = 1;
      #while (side > -2)
	union {
	  // reflective semispheres
	  #local zoff = ballrad/9; // Nudge it forward a little.
	  sphere {
	    <-ballrad * cos(ballcutoffangle), yaxis, zoff> ballrad
	    #ifdef(TEST)
	      texture { pigment {Gray10 } finish {phong 1 phong_size 300 reflection 0} }
	      #warning "Non-reflective, non-blurred Dalek semispheres"
	    #else
	      blurred_tm(pigment {Gray10 }, finish {phong 1 phong_size 300 reflection 0.4})
	    #end
	    clipped_by {
	      cylinder {
		<0.001, yaxis, ballrad/9>,
		<ballrad*sin(ballcutoffangle), yaxis, zoff>
		ballrad * sin(ballcutoffangle)
	      }
	    }
	    //	  bounded_by {clipped_by} causes warning if left in
	  }
	  intersection {
	    // White disc.  The inside is reflective, the outside not,
	    // so make it 2cm thick and cut 1cm off with a non-reflective plane.
	    union {
	      // Round-off radius.
	      #local roundrad = 0.002;
	      cylinder {<-0.01, yaxis, 0>, <0.01, yaxis, 0> dalekradius-roundrad }
	      cylinder {<roundrad, yaxis, 0>, <0.01-roundrad, yaxis, 0> dalekradius}
	      torus {dalekradius - roundrad, roundrad rotate 90*z translate <roundrad, yaxis, 0>}
	      torus {dalekradius - roundrad, roundrad rotate 90*z translate <0.01-roundrad, yaxis, 0>}
	    }
	    // Now slice that 2cm-thick disc+leg in half, leaving the
	    // edge and outside non-reflective, adding reflectivity to the inside.
	    // Add exponent to this texture so that the sides of the dalek
	    // reflect the bright lights a little better.
	    // falloff did nothing.
	    plane { -x, 0
	      #ifdef(TEST)
		texture{pigment{colour rgb 0.9} finish {reflection {0}}}
		#warning "Non-reflective, non-blurred Dalek sides"
	      #else
		blurred_tm(pigment{colour rgb 0.9}, finish {reflection {0.2 exponent 0.9 falloff 2}})
	      #end
	    }
	  }
	  // "leg" that connects white disc to plinth.  It has a coincident
	  // surface with the cylinder, so make the texture the same
	  // apart from the reflectivity.
	  prism { linear_sweep linear_spline
	    0, 0.01
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
	  // Small box to connect lightbox with sides.  The real object sat
	  // on legs and the sides were stuck on the outside of those legs.
	  union {
	    box {
	      <0, yaxis-boxheight/5, 0>,
	      <-0.03,  yaxis - boxheight/2.5, -boxdepth/4>
	    }
	    object {Round_Box_Union(
	      <-0.02, 0.05, boxdepth/5>,
	      <-(sidesep/2-boxwidth/2)+0.005, -(yaxis-plinththick+0.04), -boxdepth/4>,
	      0.005)
	      rotate 15*x
	      translate yaxis * y
	    }
	    texture {pigment {Gray50} finish {specular 0.5 metallic}
	      normal { bumps 0.1 scale 0.0015}
	    }
	  }
	  // ...and make the default texture reflective 0.2 exponent 0.3
	  // falloff made no difference to output file
	  texture {finish {reflection 0} pigment{colour rgb 0.9}}
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
	    Round_Box_Union(<-boxwidth/2, 0, -boxdepth/2>,
	      <boxwidth/2, boxheight, boxdepth/2>, 0.002)
	    // Plus eaves that will be over each hole, when they're cut later.
	    #local i = 4;
	    #while (i)
	      #local i = i - 1;
	      #local loopx = -boxwidth/2 + holexgap;
	      #local loopy = (holeygap+holeheight)*i + holeygap;
	      #local side = 1;
	      #while (side > -2)
		difference {
		  object { box{
		    <loopx - 0.006, loopy + holeheight*0.85, boxdepth/2-0.01> * <side,1,1>,
		    <loopx + 0.006 + holewidth, loopy + holeheight*1.15, boxdepth/2+holeheight/4>*<side,1,1>
		    }
		  }
		  cylinder {
		    <loopx - 0.007, loopy+holeheight*0.8, boxdepth/2 + holeheight/4> * <side, 1, 1>,
		    <loopx + 0.007 + holewidth, loopy+holeheight*0.8, boxdepth/2 + holeheight/4>*<side,1,1>
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
	    #local side = 1;
	    #while (side > -2)
	      // cut the holes all the way through.
	      object {Round_Box_Union(
		<-holewidth/2, -holeheight/2, -boxdepth/2-0.01>,
		<holewidth/2, holeheight/2, boxdepth/2+0.01>, 0.003)
		texture {
		  pigment {color rgb 1}
		  finish {ambient 0.1 }
		}
		scale <1.001, 1.001, 1>
		translate <-boxwidth/2+holexgap+holewidth/2, loopy+holeheight/2, 0>
		* <side, 1, 1>
	      }
	      #local side = side - 2;
	    #end // while
	  #end // while
	}

	#macro rotateit()
	  #local noti = i;
	  #if (horizt)
	    rotate y * 1 * cosd(horizt+(noti+side/4)*90)
	  #end
	  #if (vertt)
	    rotate x * 5 * cosd(vertt+(noti+side/4)*90)
	    #if (0 & i = 3)
	      rotate x * 1 // top row, pitch them down
	    #end
	    #if (0 & i = 0)
	      // bottom pair
	      rotate x * -1 // bottom pair, pitch them up
	    #end
	  #end
	#end
	
	// Now fill those eight holes will glorious light
	#local i = 4;
	#while (i)
	  #local i = i - 1;
	  #local loopy = (holeygap+holeheight)*i + holeygap;
	  #if (i > 1)
	    #local colr=lightcolour1;
	  #else
	    #local colr=lightcolour2;
	  #end
	  #local side = 1;
	  #while (side > -2)
	    union {
	      object {Round_Box_Merge(
		<-holewidth/2, -holeheight/2, -boxdepth/2+0.01>,
		<holewidth/2, holeheight/2, boxdepth/2+0.03>, 0.01)
		texture { pigment {color rgbt 1} finish {ambient 0 diffuse 0} }
		hollow interior {
		  media { emission 6
		    density { cylindrical
		      colour_map { [0.0 colr] [0.4 colr] [1.0 rgb 1] }
		      rotate 90*x scale <holewidth/2, holeheight/2, 1>
		    }
		  }
		}
		no_shadow
	      }
	      union {
		// A small box visible from the back as a silhouette,
		// and some mounts for interest.  Perhaps it holds the bulb.
		Round_Box_Union(<-holewidth/4, -holeheight/3, -0.01>,
		  <holewidth/ 4, holeheight/3, 0.01>, 0.005)
		cylinder{-holewidth/1.9*x, holewidth/1.9*x, 0.004}
		texture {pigment {rgb 0}}
		translate (-boxdepth/2+0.01) * z
	      }
	      // Glass lens out the front.
	      sphere {0, holeheight/2.2
		texture {
		  finish {ambient 1 diffuse 0 reflection 0.0}
//		  pigment {rgbt <colr.red, colr.green, colr.blue, 0.1>*5}
		  pigment {rgbt <1, 1, 1, 1>}
		}
		interior {ior 1.3}
		scale <1, 1, 0.5>	// Squish from a sphere to a lens
		no_shadow
		translate 0.04*z
		rotateit()
		translate (boxdepth/2-holeheight/2-0.04)*z
	      }
	      translate <-boxwidth/2+holexgap+holewidth/2, loopy+holeheight/2, 0> * <side,1,1>
	    }
	    #local side = side - 2;
	  #end
	#end
	    
	#if(1)
	// The real light sources.
	#local i =4;
	#while (i >= 1)
	  #local i = i - 1; // !!! 1
	  #if (i > 1)
	    #local colr=lightcolour1;
	  #else
	    #local colr=lightcolour2;
	  #end
	  #local loopy = (holeygap+holeheight)*i + holeygap; // + plinththick + boxplinthsep;
	  #local side = 1;// !!! 1
	  #while (side > -2)
	    // This source creates small illuminated circles on objects they strike.
	    // They will interact with environmental media, if there is any.
	    light_source {
	      #if (NO_DALEK_CHOPSTICKS)
		#if(1)
		  <0, 0, -lightsource_backoff(holeheight/2, 1)>
		  colour colr*2
		  projected_through {disc {0, z, holeheight/2}}
		#else
		  <0, 0, 0.01>
		  colour colr
		#end
	      #else 
		<0, 0, 2.03>
		colour colr*10
	      #end
	      spotlight radius 0.3 falloff 1
	      #if(! NO_AREA_LIGHTS) area_light 0.05*y, 0.05*x, 6, 6 adaptive 1 #end
	      point_at z
	      rotateit()
	      // Box containing scattering media extends to z=boxdepth/2+0.01
	      translate <-boxwidth/2 + holexgap + holewidth/2, loopy+holeheight/2, boxdepth/2>
	      * <side,1,1>
	    }

	    #local side = side - 2;
	  #end
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
       * Omit: not on the real thing
       */
      // #include "dalekcables.inc"
      translate 0.15 * z
      rotate (180+rot)*y
    }

    // More cables, from the plinth running down the riser.  Not rotated.
    #include "dalekplinthcables.inc"
    
    /**********************************************************************
     * Build a stack of 12 discs, alternating white and black, to a
     * total thickness of plinththick metres, then (don't) cut a hole in the top
     */
//    difference {
      union {
	#local darkdisc_t = texture{pigment{colour rgb <0.2, 0.2, 0.2>}
	  normal { bumps 0.0002
	    scale <0.2, 0.0005, 0.2>
	  }
	  finish {specular 0.7 roughness 0.1}
	}
	#local lightdisc_t = texture{pigment{colour rgbt <0.8, 0.8, 0.8, 0.2>}
	  normal {
	    bumps 0.0005 scale <1, 0.001, 1>
	  }
	  finish {specular 1 roughness 0.006}}
	#local i = 3;
	#while (i)
	  #local i = i - 1;
	  union {
	    cylinder {
	      0, -plinththick/6 * 2/3 * y, plinthradius
	      texture {lightdisc_t}
	    }
	    cylinder {
	      -plinththick/6 * 2/3 * y, -plinththick/6 * y, plinthradius
	      texture {darkdisc_t}
	    }
	    translate (plinththick - i * plinththick/6) * y
	  }
	  union {
	    cylinder {
	      0, plinththick/6 * 2/3 * y, plinthradius
	      texture {lightdisc_t}
	    }
	    cylinder {
	      plinththick/6 * 2/3 * y, plinththick/6 * y, plinthradius
	      texture {darkdisc_t}
	    }
	    translate i * plinththick/6 * y
	  }
	#end // while
      }
  // This was a hole for the cables, now deleted, to disappear into.
  //      cylinder {plinththick/2*y, (plinththick+0.001)*y, plinthradius/4 pigment {rgb 0}}
//    }
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
	  #if (0) 
	    superellipsoid {<0.7, 0.05>	// cylinder along Z
	      rotate 90*y
	      scale 0.103
	      translate <0, 0.0, plinthradius-0.05> }
	    superellipsoid {<0.7, 0.10>
	      rotate 90*y
	      scale 0.1
	      translate <0, -0.05, plinthradius> }
	    superellipsoid {<0.01, 0.05>	// box, stretched into column.
	      scale <0.1, 3, 0.1>
	      translate <0, -3, plinthradius> }
	    superellipsoid {<0.01, 0.05>
	      scale <0.103, 3, 0.1>
	      translate <0, -3, plinthradius-0.05>  }
	  #else
	    Round_Cylinder_Union(<-0.103, 0.005, plinthradius-0.05>, <0.103, 0.005, plinthradius-0.05>,
	      0.1, 0.005)
	    Round_Cylinder_Union(<-0.1, -0.05, plinthradius>, <0.1, -0.05, plinthradius>,
	      0.1, 0.005)
	    Round_Box_Union(<-0.1, -6, plinthradius-0.1>, <0.1, -0.0495, plinthradius+0.1>, 0.005)
	    Round_Box_Union(<-0.103, -6, plinthradius>, <0.103, 0.005, plinthradius+0.05>, 0.005)
	  #end
	}
	box {<-0.2, 0, plinthradius>, 
	  <0.2, -7, 0>
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
    max_trace_level 30
  }
  

  plane {
    y, 0
    texture {
      pigment { rgb 1}
    }
  }

//light_source { <7, 7, -10> White media_interaction off}
  light_source { <-6, 6, -20> White spotlight radius 1 falloff 12 point_at y media_interaction off}
//light_source { <-6, 20, 30> White }
//light_source { <-1, 0.2, -1> White }

  plane { -z, -3 pigment {rgb 1}}
  
  object {
    #if (clock_on)
      dalek(Green, Blue, 1 + clock*360, 1 + clock*360*2, 0, -30 + clock*60)
//      dalek(Green, Blue, 0, 0, 0, -90 + 180*clock)
    #else
      dalek(Green, Blue, 0, 0, 0, 0)
    #end
    rotate 130*y
    translate y/2
  }

  #if(0)box {<-2, 0, -1.5>, <2, 2, 3> hollow
      interior { ior 1
	media {
	  scattering {2, 0.4 extinction 0}
	  samples 10 intervals 1 method 3
	}
      }
      texture {	pigment {rgbt 1}}
    }
  #end
  
//  media { scattering { 2, rgb 1/5 extinction 0} }

    camera {
//  location <-0.6, 0.8, 0.8> look_at <0, 0.3, 0.2> // mount
//  location <0, 0.9, 0.9> look_at <0, 0.3, 0.3> // mount, close
//  location <0, 0.2, -3> look_at <0, 0.8, 0>  // matches dscn1746.jpg
  // for checking lights.  Use 135 degree rotation
//  location <0.6, 3, -3.5> look_at <1.5, 0, -1.5>
//    location <0.3, 1.2, -1.5> look_at <0.5, 0.9, 0> // Angle at -55
//    location <0, 0.8, -1.2> look_at <0, 0.2, 0>  // Checking plinth and cables
    location <0, 0.8, -1.2> look_at <0, 0.55, 0>  angle 70 // Checking disc textures
//    location <0, 1, -1.5> look_at <0, 1, 0>  // right down the barrel
//    location <0.3, 1.2, -1.5> look_at <0.41, 1, 0> angle 30 // checking blur
//    location <0.3, 1.2, -1.5> look_at <0.3, 0.95, 0> angle 20 // checking rounding
//  location <-0.1, 0.3, -0.4> look_at <0, 0.2, 0> // check cables
}


#end

