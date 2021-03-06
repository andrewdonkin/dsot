// The proscenium alone was 85 foot high by 98 foot wide.

// Based on {stage_layout,layout_sightlines}.jpg):
// main stage 56' (17m) wide and 26' (8m) deep.
//
// Platforms either side 8' (2.5m) wide, same depth.
//
// Projection screen 6' behind the back of the main stage, at z=10.

// #include "dsot.inc"

#macro grille(siz)
  #if (siz.y = 0)
    #local siz = <siz.x, 0.02, siz.z>;
  #end

  #local absiz = <abs(siz.x), abs(siz.y), abs(siz.z)>;

  union {
    #local ix = 0.08;
    #local iz = 0.1;
    #while (ix < absiz.x-0.06)
      box {<ix, 0, 0> <ix+0.015, siz.y, absiz.z>}
      #local ix = ix + 0.06;
    #end
    #while (iz < absiz.z-0.06)
      box {<0, 0, iz> <absiz.x, siz.y, iz+0.002>}
      #local iz = iz + 0.1;
    #end
    box {<0, 0, 0> <0.03, siz.y, absiz.z>}
    box {<absiz.x, 0, 0> <absiz.x-0.03, siz.y, absiz.z>}
    box {<0, 0, 0> <absiz.x, siz.y, 0.03>}
    box {<0, 0, absiz.z> <absiz.x, siz.y, absiz.z-0.03>}
    scale <sign(siz.x), 1, sign(siz.z)>
    texture {grille_t}
  }
  
#end

//----------------------------------------------------------------------
#macro vox(xlate_v)
union {
  person(<0.5, 0, 1.7>)
  object {wedgemon(<0.95, 0.4, 0.6>) translate 0}
  person(<1.5, 0, 1.7>)
  object {wedgemon(<0.95, 0.4, 0.6>) translate x}
  person(<2.5, 0, 1.7>)
  object {wedgemon(<0.95, 0.4, 0.6>) translate 2*x}
  translate (xlate_v)
}
#end

//----------------------------------------------------------------------
// Monitor speaker.
#macro wedgemon(siz)
  intersection {
      union {
	difference {
	  superellipsoid {<0.05, 0.01>
	    matrix <
	    1, 0, 0,
	    0, 1, 0,
	    0, 0.5, 1,
	    0, 0, 0 >
	    scale <0.5, 2/3, 0.35>
	    translate <0.5, 0, 0.35>
	  }
	  // Cut a hole facing audience
	  box {<0.2, 0.1, 0.1> <0.8, 0.25, -0.1>}
	}
	object {louvre(<0.6, 0.15, 0.02>, 6, 0.5) translate <0.2, 0.1, -0.005>}
	union {
	  difference {
	    superellipsoid {<0.05, 0.05>}
	    // Cut a hole for the speaker
	    box {<-0.9, -0.5, -1.1> <0.9, 0.9, 1.1>}
	  }
	  // The speaker grille/baffle/cover/fabric facing the stage.
	  box {<-0.9, -0.5, 0.9> <0.9, 0.9, 0.95> texture {mictexture} }
	  translate <0, -1, -1>
	  rotate -40*x
	  scale <0.5, 0.85, 0.30>
	  translate <0.5, 1.01, 0.7>
	}
      }
    plane {-y 0}
    scale siz
    texture {speakerbox_t}
  }
#end

//----------------------------------------------------------------------
#macro stage(stageheight)

  #local ix = -5.5;
  #while (ix < 8.5)
    #local iz = 0;
    #while (iz < 6) 
      // This condition leaves a hole for the leftmost dalek
      #if (ix >= -4.5 | iz >= 3)
	object {grille(<1, -0.02, 1>) translate <ix, 0, iz>}
      #end
      #local iz = iz + 1;
    #end
    #local ix = ix + 1;
  #end
  // Small pieces of stage fore and aft of the leftmost dalek:
  object {grille(<1, -0.02, 0.8>) translate <-5.5, 0, 0>}
  object {grille(<1, -0.02, 0.7>) translate <-5.5, 0, 2.3>}
  object {dalekinthebox(clock*2, Green, Blue, clock*180, clock*180, 0, -90+360*clock)
    translate <-5.5, -0.02, 0.8>}
  
  // Wah wah, stage right, at stage level.
  union {
    vox(<0, 0, 0>)
    object {grille(<3, -0.02, 6>) translate 1*z}
    translate <-8.5, 0, 0>
  }

  union {
    // Back row (Scott, Gary)
    box {<-8.5, 0.3, 6> <8.5, 0, 8>}
    // Stage right riser (Tim)
    box {<-8.5, 0.3, 2.5> <-6.5, 0, 8>}
    // Stage left riser (Jon)
    intersection {
      box {<0, 0.3, 0> <8.5, 0, 8>}
      plane {-x, -6.5 rotate -30*y}
    }
    pigment {color rgb <0.4,0.4,0.4>}
  }
  
  #local i = 7;
  #local colrarray = array[8]{Orange Orange Orange Orange Orange Orange Orange Orange};
  #local colrarray = array[8]{Black Black Black Black Black Black Black Black };
  #while (i)
    #local i = i - 1;
    object {lightbank(17/7, 2, 0, colrarray)
      translate <-17/2 + 17/7*i, 0, 0>
    }
  #end
 
  
#end

//----------------------------------------------------------------------
#macro dalekinthebox(height, lightcolour1, lightcolour2, horizt, vertt, boxpitch, rot)
  union {
    #local dheight = max(height, 0);
    #local dheight = min(dheight, 1.5);
    #local hinge = 90*softly(dheight/1.5);
    #if (clock_on)
      #warning concat("clock: ", str(clock,4,2),
	" height: ", str(height,4,2), " hinge: ", str(hinge,2,0))
    #end
    object {
      dalek(lightcolour1, lightcolour2, horizt, vertt, boxpitch, rot)
      translate <0.5, height - 1, 0.6>
      texture {grille_t}
    }
    union {
      grille(<1, 0.02, 1.5>)
      intersection {
	union {
	  torus {0.12, 0.01 rotate 90*z scale <3, 1.2, 1> translate <0.85, -0.02, 1.6>}
	  torus {0.12, 0.02 rotate 90*z scale <0.2, 1.2, 1> translate <0.85, -0.02, 1.6>}
	  torus {0.12, 0.01 rotate 90*z scale <3, 1.2, 1> translate <0.15, -0.02, 1.6>}
	  torus {0.12, 0.02 rotate 90*z scale <0.2, 1.2, 1> translate <0.15, -0.02, 1.6>}
	}
	plane {y 0}
//	translate -0.02*y
      }
      translate <0, 0.02, -1.60>
      rotate hinge*x
      translate <0, -0.02, 1.60>
      texture {grille_t}
    }
  }
    
#end

//----------------------------------------------------------------------
// colours should be an array of eight colours.
// height is the height of each of face of this colour box.
// When three of these faces are connected in an equilateral triangle,
// their centre is r away from each corner and r1 away from their
// perpendicular bisectors.  h=1.
// sin(60)=h/2r  =>  r=h/2sin(60) = 1/sqrt(3)
// tan(60)=h/2r1 => r1=h/2tan(60) = 1/2sqrt(3) = r/2

#macro lightrow8(w, h, rot, colours)

  #local r = h/sqrt(3);
  #local r1 = r/2;
  #local centres = w/8;

    #local lightcone =
  merge {
    cone {
      <0, 0, -r1*1.4>, h/2.2
      <0, 0, r-0.025>, 0.012}
    sphere {<0, 0, r-0.025>, 0.012}
  };

  #local lightbulb =
  intersection {
    sphere { r/1.1*z r*1.4 }
    object { lightcone }
  };
  
  union {
    intersection {
      // equilateral triangle, sides h unit long
      prism {linear_spline linear_sweep
	0, w, 8
	<-h/2, -r1> <0, r> <h/2, -r1>
	<h/2.1, -r1>
//	  <h/6, r*0.1> <-h/6, r*0.1>
	<0.001, -r1> <-0.001, -r1>
	<-h/2.1, -r1>
	<-h/2, -r1>
	rotate -90*z // drop it from vertical to horiz: x=0 to x=width
      }
      // scoop out a channel
      cylinder {<-0.01, 0, -r1*1.4>, <w+0.01, 0, -r1*1.4>, h/2.2 scale <1, 1, 1> inverse}
      // Smooth the corners a little
      cylinder {-0.01*x, (w+0.01)*x, r*0.95} // !!! ??? 0.99
      // Cut out eight cones for the lights.
      #local i = 8;
      #while (i)
	#local i = i - 1;
	object {lightcone translate ((i+0.5)*centres) * x inverse}
      #end
      texture {
	pigment {rgb 0.4}
	finish {specular 1 roughness 0.1 metallic}
	normal { bumps 0.1 scale 0.0015}
      }
    }
    #local i = 1; // !!! 8
    #while (i)
      #local i = i - 1;
      #if (colours[i].x + colours[i].y + colours[i].z)
	light_source { 0, colours[i]
	  media_attenuation on
	  fade_power 2
	  fade_distance 0.5
	  spotlight
	  point_at -z
	  radius 10
	  falloff 40
//	area_light 0.3*x, 0.3*y, 3, 3
	  looks_like {
	    torus {0.02, 0.002 rotate 90*x
	      finish {ambient 1}
	      pigment {colours[i] * 10}
	    }
	  }
	  translate <(i+0.5)*centres, 0, 0>
	}
      #end
      union  {
	light_group {
	  light_source {0, colour colours[i]}
	  difference {
	    object {lightbulb }
	    object {lightbulb translate -0.005*z}
	    texture {pigment {rgb 1}
	      finish {ambient 0 diffuse 1 specular 0.5 reflection 0.5}
	      normal { onion 0.05 sine_wave scale 0.008 }
	    }
	  }
	}
	#if (1)
	  difference {
	  object {lightbulb}
	  object {lightbulb scale 0.9}
	  texture {
	    finish {
	      specular 1
	      roughness 0.002
	      ambient 0
	      diffuse 0
	      reflection 0
	    }
//	    pigment { rgbf <colours[i].x, colours[i].y, colours[i].z, 0.95> }
	    pigment { rgbf <1, 1, 1, 0.95> }
	    normal { onion 0.3 sine_wave scale 0.008 }
	  }
	  interior { 
	    ior 1.1
	    fade_distance 0.3 fade_power 2
	  }
	}
	#end
	translate <(i+0.5) * centres, 0, -0.001>
      }
    #end
    rotate rot*x
    translate r1*z
  }
#end

//----------------------------------------------------------------------
#macro lightrow(w, h, colr)
  lightrow8(w, h, array[8]{colr colr colr colr colr colr colr colr})
#end

//----------------------------------------------------------------------
#macro lightbank(wid, height, rot, colrarray)
  #local slop = 40;
  #local plinthh = 0.5;
  #local h = (height-plinthh) / cos(radians(slop)); // length of angled face
  union {
    union {
      #local rowh = (h-0.5) / 5;

      // Frame.
      difference {
	box {0, <wid, -h, 0.01>}
	box {<0.1, -0.1, -0.001>, <wid-0.1, -(h-0.4), 0.011>}
      }
    
      // Five light rows
      #local i = 5;
      #while (i)
	#local i = i - 1;
	object {
	  lightrow8(wid-0.2, rowh - 0.001, rot, colrarray)
	  translate <0.1, -(0.1+(i+0.5)*rowh), 0>
	}
      #end
      // Pull the bottom forward, to -z
      rotate slop * x
    }
    #local d = (height-plinthh) * tan(radians(slop)); // depth (-Z at bottom edge)
    intersection {
      box {0, <0.001, -height, -d>}
      plane {-z, -0.001 rotate slop*x}
    }
    intersection {
      box {wid*x, <wid-0.001, -height, -d>}
      plane {-z, -0.001 rotate slop*x}
    }
    object {louvre(<wid, plinthh, 0.02>, 5, 0.5) translate <0, -height, -d>}
    
    texture {
      pigment {rgb 0.5}
      finish {specular 0.5 roughness 0.02 metallic}
      normal { bumps 0.1 scale 0.0015}
    }
  }
#end

//#####################################################################
#ifdef (TEST)

  #include "dsot.inc"

  #undef TEST
  #include "dalek.inc"
  #declare TEST=1;
  
  global_settings {
    ambient_light rgb 0.08
//    ambient_light 0.0 radiosity { adc_bailout 0.01/3 error_bound 1.0 }
    max_trace_level 100
  }
  
//  sky_sphere { test_sky_sphere }

  #ifdef (TEST_GRILLE)
    plane {  y, -3 texture { pigment { checker colour Green colour Yellow} }}
    light_source { <70, 70, -70> White }
    light_source { <-10, 2, 14> White }
  
    //  camera { location <-2.3, 0.6, 2> look_at <0, 0.4, 0> } // back of wedge
    //  camera { location <-1.5, 0.6, -1> look_at <0, 0.4, 0> } // front of wedge
    camera { location <2.5, 0.2, 1.8> look_at <0.5, 0.2, 1.8> } // popup dalek

    object {grille(<1, 0, 1>) translate -x}
    grille(<1, 0, 1>)
    object {grille(<1, 0, 1>) translate x}
    object {grille(<1, 0, 1>) translate 2.5*z}
    object {grille(<1, 0, 1.5>) translate -x+z}
    object {grille(<1, 0, 1.5>) translate x+z}
    louvre(<1, 1, 0>, 20, 0.7)
    //  object {wedgemon(1) translate <-1, 0.02, 0>}
    object {wedgemon(<1, 0.4, 0.6>) translate <-1, 0.01, 0>}

    #if (clock_on)
      object {dalekinthebox(clock*1.8, Green, Blue, 0, 50, 0, -20) translate z}
    #else 
      object {dalekinthebox(0.23*1.8, Green, Blue, 0, 50, 0, -20) translate z}
    #end
  #end
  #ifdef (TEST_LIGHTBOX)
    plane {  y, 0 texture { pigment { checker colour Green colour Yellow} }}
//    camera { location <-1, 0.98, -2> look_at <0.2, 1, 0> angle 15}
    camera { location <0, 0.5, -9> look_at <0.3, 1.2, 0> angle 10}
    light_source { <-5, 5, -20> White media_interaction off}
//    light_source { <0, 1, 2> White}
    #if (0)
      sphere {y, 20 hollow
	interior {
	  media {
	    scattering {4, 0.1}
	    samples 30,30
	    intervals 1
	    method 3
	  }
	}
	texture {	pigment { rgbt <1,1,1,1> }
	  finish { ambient 0 diffuse 0 }
	}
      }
    #end
    #local colrarray = array[8]{Orange Orange Orange Orange Orange Orange Orange Orange};
    #if (clock_on)
      object {lightbank(17/7, 2, 360*clock, colrarray) translate 2*y}
    #else
      object {lightbank(17/7, 2, 330, colrarray) translate 2*y}
    #end
  #end
  
#end
