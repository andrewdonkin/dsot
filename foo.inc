// Snapshot of stage.inc at some point.  No longer useful.

// The proscenium alone was 85 foot high by 98 foot wide.

// Based on {stage_layout,layout_sightlines}.jpg):
// main stage 56' (17m) wide and 26' (8m) deep.
//
// Platforms either side 8' (2.5m) wide, same depth.
//
// Projection screen 6' behind the back of the main stage, at z=10.

#include "dsot.inc"

#declare lightseed = seed(0);

#declare grillepost = box {0 <0.015, 1, 1>};
#declare grillerail = box {0 <1, 1, 0.005>};

#macro grille(siz)
  #if (siz.y = 0)
    #local siz = <siz.x, 0.02, siz.z>;
  #end

  #local absiz = <abs(siz.x), abs(siz.y), abs(siz.z)>;

  union {
    #local ix = 0.08;
    #local iz = 0.1;
    #while (ix < absiz.x-0.06)
//      box {<ix, 0, 0> <ix+0.015, siz.y, absiz.z>}
      object {grillepost scale <1, siz.y, absiz.z> translate ix * x}
      #local ix = ix + 0.06;
    #end
    #while (iz < absiz.z-0.06)
//      box {<0, 0, iz> <absiz.x, siz.y, iz+0.005>}
      object {grillerail scale <absiz.x, siz.y, 1> translate iz*z}
      #local iz = iz + 0.1;
    #end
    box {<0, 0, 0> <0.03, siz.y, absiz.z>}
    box {<absiz.x, 0, 0> <absiz.x-0.03, siz.y, absiz.z>}
    box {<0, 0, 0> <absiz.x, siz.y, 0.04>}
    box {<0, 0, absiz.z> <absiz.x, siz.y, absiz.z-0.04>}
    scale <sign(siz.x), 1, sign(siz.z)>
    texture {grille_t}
//    bounded_by{box{0, siz}}
  }
  
#end

//----------------------------------------------------------------------
#macro vox(xlate_v)
union {

  #local voxmon = object {wedgemon(<0.74, 0.4, 0.6>, 0)};
  
  person(<0.4, 0, 1.7>)
  object {mic_stand(60, 1.4) translate <0.4, 0, 1.1>}
  
  // rotate speakers for randomness, and sink them into the stage
  object {voxmon translate <0.4-0.35, -0.1, 0.05>}
  person(<1.2, 0, 1.7>)
  object {mic_stand(45, 1.5) translate <1.2, 0, 1.1>}
  object {voxmon rotate 1*y translate <1.2-0.35, -0.1, 0.05>}

  person(<2.0, 0, 1.7>)
  object {mic_stand(45, 1.3) translate <2.0, 0, 1.1>}
  object {voxmon translate <2.0-0.35, -0.1, 0.05>}

  // They're wearing heels, so have to stand on something.  Make it
  // the same kind of translucent plastic that is under Nick, but
  // rougher.
 
  box {<0.1, 0, 0.9> <2.37, 0.01, 2.3>
    pigment {rgbt <1,1,1,0.5>}
    normal {wrinkles scale <0.2, 0.002, 0.01>}
    finish {specular 0.2 roughness 0.010}
    interior {ior 4}
    rotate -0.5*y
  }
  translate (xlate_v)



}
#end

//----------------------------------------------------------------------
// Monitor speaker.  Not good - some folds at the corners, because
// I used superellipsoids instead of Round_Box (duh).
#macro wedgemon(siz, typ)
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
	  // And one in the top
	  #if (typ) box {<0.3, 0.7, 0.4>, <0.7, 1, 0.6> } #end
	}
	#if (! typ) object {louvre(<0.6, 0.15, 0.02>, 6, 0.5) translate <0.2, 0.1, -0.005>} #end
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

  // First the sides and back.
  // 50' (15m) deep starting 3m back from the front of the stage.
  // 85' (26m) high.  Subtract stageheight from that.

  #local ceiling = 26 - stageheight;
  
  #local curtain_t = texture {
    pigment { rgb 0.1 }
    normal {
      // Slightly wavy vertical lines, like a curtain.
      gradient x+z
      sine_wave
      turbulence 0.8
      no_bump_scale
      scale <0.2, 10, 0.2>
    }
  };
  
  // Side of the stage area on audience's right.  This is going
  // to be mirrored along x=0.
  #local stageleft = union {
    union {
      // Three curtains, dropping from the clouds to the grass.
      box {<12, -stageheight, 3>, <12.1, ceiling, 18>}
      box {<12, -stageheight, 3>, <15.1, ceiling, 3.1>}
      box {<15, -stageheight, 0>, <15.1, ceiling, 3>}
      texture {curtain_t}
    }

    // Front louvres facing audience beside light banks.
    #local louvreslats = (stageheight+0.4)*20;
    #local anglelouvre = object {louvre(<sqrt(0.5*0.5+0.5*0.5), stageheight+0.15, 0.02>, louvreslats, 0.9)};
    #local straightlouvre = object {louvre(<5.5/7, stageheight+0.15, 0.02>, louvreslats, 0.9)};
    object {anglelouvre rotate 45*y translate <8.5, -stageheight, 0>}
    #local i = 9;
    #while (i < 14.5)
      object {straightlouvre translate <i, -stageheight, -0.5>}
      #local i = i + 5.5/7;
    #end
    // And out the outside, audience's extreme right, against the arch.
    object {anglelouvre rotate -45*y translate <14.5, -stageheight, -0.5>}

    // The platform floor.  Folks stand on this playing with the
    // widget boxes in the next section.
    // We have to cut off the audience/stage corner.
    intersection {
      object {grille(<3.5, 0.02, 10>)}
      plane {-z-x, -0.5*0.75}
      translate <8.5, -1.5, -0.45>
      // The cut ends of the grille have no texture.
      texture {grille_t}
    }
    
    // To keep the people on the monitor platform falling into the
    // pyro area in the wings.
    object {louvre(<-3.45, -1.5, 0>, 20, 0.9) rotate 90*y translate <12, 0, -0.45>}
    
    // Bench in side platforms facing the audience
    union {
      // Audience/stage corner
      box {0, <sqrt(0.5*0.5+0.5*0.5), -0.02, 0.5> rotate 45*y translate <8.5, -0.5, 0>}
      // Front bench
      box {0, <3, -0.02, 0.5> translate <9, -0.5, -0.5>}
      // Outside bench
      box {0, <0.5, -0.02, 5> translate <11.5, -0.5, 0>}
      // Stage-side bench.
      box {0, <0.5, -0.02, 5> translate <8.5, -0.5, 0>}
      object {louvre(<5, -1.5, 0>, 20, 0.3) rotate -90*y translate <8.5, 0, 0>}
      object {widgetbox(<0.3, 0.5, 0.3>) rotate 225*y translate <9, -0.5, 0>}
      object {widgetbox(<0.2, 0.5, 0.2>) rotate 200*y translate <9.25, -0.5, -0.1>}
      object {bendylight(0.05, 80, 0.15, 0.03, 25, 0.2, rgb 1) rotate 160*y translate <9.1, 0, -0.2>}
      object {widgetbox(<0.2, 0.48, 0.2>) rotate 182*y translate <9.5, -0.5, -0.15>}
      object {bendylight(0.02, 45, 0.10, 0.02, 60, 0.1, rgb 1) rotate 180*y translate <9.4, 0, -0.3>}
      object {widgetbox(<0.3, 0.5, 0.2>) rotate 181*y translate <9.9, -0.5, -0.2>}
      texture {grille_t}
    }
  };
  
  object {stageleft}
  object {stageleft scale <-1, 1, 1>}
  box {<-12.1, -stageheight, 18>, <12.1, ceiling, 18.1> texture {curtain_t}} // Back

  // Now the stage, the grille upon which everyone stands.

  // Start with the wah wah section, MT, DMcB, and RF.
  vox(<-8.5, 0, 0>)
  // Under their feet:
  object {grille(<2.5, -0.02, 2.2>) translate <-8.5, 0, 0.8>}
  // Small pieces of stage fore and aft of the leftmost dalek:
  object {grille(<1, -0.02, 0.8>) translate <-6, 0, 0>}
  object {grille(<1, -0.02, 0.7>) translate <-6, 0, 2.3>}

  // The dalek to the immediate +X of Rachel:
  // rotate it -91 to hit Dave
  object {dalekinthebox(1, Green, Blue, clock*360, clock*360, 0, -91) translate <-6, -0.02, 0.8>}

  // Next is Tim, hidden behind the vox.
  union {
    // Stage right riser
    grille(<2.5, -0.02, 3>)
    // Tim's equipment racks
    object {axerack(1) rotate 90*y translate <2, 0, 2.5>}
    object {axerack(1) rotate 90*y translate <2, 0, 3.25>}
    #local timmon = object {wedgemon(<0.4, 0.3, 0.3>, 1)};
    object {timmon rotate 20*y translate <0.1, 0, 0.2>}
    object {timmon rotate 10*y translate <0.6, 0, 0.2>}
    object {timmon rotate -10*y translate <1.4, 0, 0.2>}
    object {timmon rotate -20*y translate <2, 0, 0.2>}
    object {axeboard rotate 20*y translate <0.1, 0, 1.1>}
    translate <-7.5, 0.3, 2.5>
  }
  // Fence to stop Tim falling into the pyros behind stage right.
  // !!! Tim fence: replace with properly mitred fence
  object {louvre(<-3.5, 1.8, 0.1>, 10, 0.4) translate <-5, 0, 6>}
  

  // Between Dalek #1 and Nick, under David.
  #if (1) 
    #local gril1 = object {grille(<0.7, -0.02, 5.5>)};
    object {gril1 translate <-5, 0, 0.6>}
    object {gril1 translate <-4.3, 0, 0.6>}
    object {gril1 translate <-3.6, 0, 0.6>}
    object {gril1 translate <-2.9, 0, 0.6>}
    object {gril1 translate <-2.2, 0, 0.6>}
  #else
    // Less memory, but doesn't look as good.
    object {grille(<3.5, -0.02, 5.5>) translate <-5, 0, 0.6>}
  #end

  // David's monitors, and the grilles on either side of them.
  object {grille(<1.2, -0.02, 0.6>) translate <-5, 0, 0>}
  object {grille(<-1.1, -0.02, 0.6>) translate <-1, 0, 0>}
  union {
    #local davemon = object {wedgemon(<0.3, 0.4, 0.4>, 1)};
    object {davemon rotate 2*y translate -0.8*x}
    object {davemon translate -0.4*x}
    object {davemon translate 0*x}
    object {davemon rotate -2*y translate 0.4*x}
    translate <-2.9, -0.1, 0.15>
  }
  // Dave's effects board
  object {axeboard rotate 20*y translate <-4.3, 0, 1.3>}

    // Dave's equipment racks
  object {axerack(1) rotate -90*y translate <-4.5, 0, 2.5>}
  object {axerack(2) rotate -90*y translate <-4.5, 0, 3.25>}
  object {axerack(3) rotate -90*y translate <-4.5, 0, 4>}
  object {axerack(3) rotate -90*y translate <-4.5, 0, 4.75>}

  // Dalek #2, behind Nick.  Grille behind.
  object {dalekinthebox(1.5+clock, Green, Blue, clock*360, clock*360, 0, -45) translate <-1.5, -0.02, 3.5>}
  object {grille(<1, -0.02, 1.5>) translate <-1.5, 0, 5>}

  // In front, under, and behind Nick.  Dodge the dalek.
  object {grille(<3, -0.02, 0.6>) translate <-1, 0, 0>}
  object {grille(<3.5, -0.02, 2.9>) translate <-1.5, 0, 0.6>}
  object {grille(<2.5, -0.02, 2.5>) translate <-0.5, 0, 3.5>}
  
  // Under Guy and Rick, in front of dalek #3, left of dalek #4
  object {grille(<5, -0.02, 4>) translate <2, 0, 0>}

  // Under Guy, our left of dalek #3.
  object {grille(<1, -0.02, 2>) translate <2, 0, 4>}
  // dalek #3
  object {dalekinthebox(2+clock, White, White, clock*360, clock*360, 0, 0) translate <3, -0.02, 4>}
  // Behind and right of dalek #3, to the foot of Gary's cage and Jon.
  object {grille(<1, -0.02, 0.5>) translate <3, 0, 5.5>}
  object {grille(<2.5, -0.02, 5>) translate <4, 0, 4>}

  // Fence for pyros behind stage left.
  // !!! Stage left fence: replace with properly mitred fence
  object {louvre(<3.5, 1.8, 0.1>, 10, 0.4) translate <5, 0, 5.5>}
  
  // Jon's riser.
  union {
    grille(<2.2, -0.02, 3>)
    louvre(<2.2, -0.3, 0.02>, 5, 0.6) // Facing audience
    object{louvre(<-3, -0.3, 0.02>, 5, 0.6) rotate 90*y} // Facing -x
    jon(0)
    translate <6.3, 0.3, 3>
  }
  // Rick's monitors
  #local rickmon = object {wedgemon(<0.8, 0.5, 0.5>, 0)};
  object {rickmon rotate -110*y translate <6.5, 0, 2>}
  object {rickmon rotate -130*y translate <6, 0, 3>}

  // Dalek #4.  Z coord is a guess; I don't know where it comes from.  
  object {dalekinthebox(1+clock, Green, Blue, clock*360, clock*360, 0, 90) translate <7, -0.02, 1>}
  // Stage behind, in front, right of it
  object {grille(<1.5, -0.02, 0.5>) translate <7, 0, 2.5>}
  object {grille(<1.5, -0.02, 0.9>) translate <7, 0, 0.1>}
  object {grille(<0.5, -0.02, 2.9>) translate <8, 0, 0.1>}

  // Dick.  !!! Replace with his real walls
  union {
    grille(<-2.6, -0.03, 3>)
    object {louvre(<3, 1.5, -0.04>, 30, 0.6) rotate -90*y translate <-2.6, -0.3, 0>}
    object {louvre(<-3, 3.5, -0.04>, 30, 0.6) rotate 90*y translate <-2.6, -2.3, 0>}
    box {<-2.6, 0, 3>, <0, 1.2, 3.2>
      texture {
	grille_t
	normal {quilted 0.2 control0 0 control0 0 scale 0.2
	  translate 0.1*y} // align it top right corner.
      }
    }
    person(<-1.3, 0, 1.5>)
    translate <-1.2, 0.3, 5.5>
  }

  // Box concealing the mirror ball.  Don't get excited---there is no
  // mirror ball.
  union {
    box {<-1.2, 0, 5.5>, <-1.1, 1.5, 8.5>}
    box {<1.2, 0, 5.5>, <1.1, 1.5, 8.5>}
    box {<-1.2, 0, 8.5>, <1.2, 1.5, 8.6>}
    box {<-1.2, 0, 5.5>, <1.2, 1.5, 5.6>}
    texture {mirrorballbox_t}
  }
  
  // Gary.
  union {
    grille(<2.6, -0.02, 3>)
    gary(0)
    translate <1.2, 0.3, 5.5>
  }

  #local colrarray = array[5][8];
  #local i = 7;  #while (i) #local i = i - 1;
    #local bulb = 8; #while (bulb) #local bulb = bulb - 1;
      #local row = 5; #while (row) #local row = row - 1;
	//      #if (rand(lightseed) > 0.6)
	// mod(i+3,4) should equal mod(i-1,4), except Pov's mod() is broken.
	#local colrarray[row][bulb] = Black;
	#if (0) // pattern of diagonals
	  #if (mod(i+3,4) = row | mod(i+3,4) = 4-row)
	    #local colrarray[row][bulb] = rgb whitespline(0.8);
	  #else
	    #local colrarray[row][bulb] = Black;
	  #end
	#end
      #end
    #end
    object {lightbank(17/7, stageheight, 120, colrarray)
      translate <-17/2 + 17/7*i, 0, 0>
    }
  #end
 
  
#end

//----------------------------------------------------------------------
// We've got four of these so save memory by reusing this object?
#declare daleklid = union{
 // !!! Add diagonal /\ braces !!!
  grille(<1, 0.02, 1.5>)
  intersection {
    union {
      torus {0.12, 0.01 rotate 90*z scale <3, 1.2, 1> translate <0.85, -0.02, 1.6>}
      torus {0.12, 0.02 rotate 90*z scale <0.2, 1.2, 1> translate <0.85, -0.02, 1.6>}
      torus {0.12, 0.01 rotate 90*z scale <3, 1.2, 1> translate <0.15, -0.02, 1.6>}
      torus {0.12, 0.02 rotate 90*z scale <0.2, 1.2, 1> translate <0.15, -0.02, 1.6>}
    }
    plane {y 0}
  }
};

//######################################################################
#macro dalekinthebox(height, lightcolour1, lightcolour2, horizt, vertt, boxpitch, rot)
  union {
    #local dheight = max(height, 0);
    #local dheight = min(dheight, 1.5);
    #local hinge = 90*softly(dheight/1.5);
/*    #if (clock_on)
      #warning concat("clock: ", str(clock,4,2),
	" height: ", str(height,4,2), " hinge: ", str(hinge,2,0))
    #end
*/    object {
      dalek(lightcolour1, lightcolour2, horizt, vertt, boxpitch, rot)
      translate <0.5, height - 1, 0.6>
      texture {grille_t}
    }
    object {
      daleklid
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

  #local lightcone = merge {
    cone {
      <0, 0, -r1*1.4>, h/2.2
      <0, 0, r-0.025>, 0.012}
    sphere {<0, 0, r-0.025>, 0.012}
  };

  #local lightbulb = intersection {
    sphere { r/1.1*z r*1.4 }
    object { lightcone }
  };

  #macro filament(colr)
    torus {0.02, 0.002 rotate 90*x
      finish {ambient 1}
      pigment {colr * 10}
      no_shadow
    }
  #end
  
  union {
    intersection {
      // equilateral triangle, sides h unit long
      prism {linear_spline linear_sweep
	0, w, 8
	<-h/2, -r1> <0, r> <h/2, -r1>
	<h/2.1, -r1>
	<h/2.4, -r1*0.6> <-h/2.4, -r1*0.6>
//	<0.001, -r1> <-0.001, -r1>
	<-h/2.1, -r1>
	<-h/2, -r1>
	rotate -90*z // drop it from vertical to horiz: x=0 to x=width
      }
      // scoop out a channel
//      cylinder {<-0.01, 0, -r1*1.4>, <w+0.01, 0, -r1*1.4>, h/2.2 scale <1, 1, 1> inverse}
      // Smooth the corners a little
      cylinder {-0.01*x, (w+0.01)*x, r*0.95} // !!! ??? 0.99
      // Cut out eight cones for the lights.
      #local i = 8;
      #while (i)
	#local i = i - 1;

	#if (colours[i].red + colours[i].green + colours[i].blue)
	  light_group {
	    light_source {0, colour colours[i]}
	    object {lightbulb scale 0.99 inverse
	      texture {pigment {rgb 1}
		finish {ambient 0 diffuse 0.5 specular 0.5 reflection 0.4}
		normal { onion 0.05 sine_wave scale 0.008 }
	      }
	    }
	    //	  global_lights
	  translate (i+0.5)*centres * x
	}
	#else
	  object {lightbulb scale 0.99 inverse
	    texture {pigment {rgb 1}
	      finish {ambient 0 diffuse 0.5 specular 0.5 reflection 0.4}
	      normal { onion 0.05 sine_wave scale 0.008 }
	    }
	    translate (i+0.5)*centres * x
	  }
	#end
      #end
      texture {
	pigment {rgb 0.4}
	finish {specular 1 roughness 0.1 metallic}
	normal { bumps 0.1 scale 0.0015}
      }
    }
    #local i = 8; // !!! 8
    #while (i)
      #local i = i - 1;
      #if (colours[i].red + colours[i].green + colours[i].blue)
	light_source { 0, colours[i]
	  media_attenuation on
	  fade_power 2
	  fade_distance 0.5
	  spotlight
	  point_at -z
	  radius 5
	  falloff 50
	  tightness 20
	  #if (!NO_AREA_LIGHTS) area_light 0.3*x, 0.3*y, 5, 5 circular orient adaptive 1 #end
	  looks_like {
	    object { filament(colours[i])}
	  }
	  translate <(i+0.5)*centres, 0, 0>
	}
//      #else
//	object { filament(colours[i]) translate <(i+0.5)*centres, 0, 0> }
////	#warning "No lights in banks"
      #end
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
	    ior 1.8
	    fade_distance 0.3 fade_power 2
	  }
	  translate <(i+0.5) * centres, 0, -0.001>
	}
	#end
    #end
    rotate (rot+3-6*rand(lightseed))*x
    translate r1*z
  }
#end

//----------------------------------------------------------------------
#macro lightrow(w, h, colr)
  #error "I thought lightrow() was unused?"
  lightrow8(w, h, array[8]{colr colr colr colr colr colr colr colr})
#end

//----------------------------------------------------------------------
#macro lightrow200(w, h, rot, c, row)
  lightrow8(w, h, rot,
    array[8]{c[row][0] c[row][1] c[row][2] c[row][3] c[row][4] c[row][5] c[row][6] c[row][7]
    })
#end

//----------------------------------------------------------------------
#macro lightbank(wid, height, rot, colrarray)
  #local slop = 40;
  #local plinthh = 1.5;
  #local h = (height-plinthh) / cosd(slop); // length of angled face
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
	  lightrow200(wid-0.2, rowh - 0.001, rot, colrarray, i)
	  translate <0.1, -(0.1+(i+0.5)*rowh), 0>
	}
      #end
      // Pull the bottom forward, to -z
      rotate slop * x
    }
    #local d = (height-plinthh) * tand(slop); // depth (-Z at bottom edge)
    intersection {
      box {0, <0.001, -height, -d>}
      plane {-z, -0.001 rotate slop*x}
    }
    intersection {
      box {wid*x, <wid-0.001, -height, -d>}
      plane {-z, -0.001 rotate slop*x}
    }
    object {louvre(<wid, plinthh, 0.02>, 15, 0.5) translate <0, -height, -d>}
    
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
//      camera { location <2.5, 0.6, -0.5> look_at <1, 0.2, 0.6> } // front of wedge
 camera { location <2.5, 0.2, 1.8> look_at <0.5, 0.2, 1.8> } // popup dalek

    object {grille(<1, 0, 1>) translate -x}
    grille(<1, 0, 1>)
    object {grille(<1, 0, 1>) translate x}
    object {grille(<1, 0, 1>) translate 2.5*z}
    object {grille(<1, 0, 1.5>) translate -x+z}
    object {grille(<1, 0, 1.5>) translate x+z}
    louvre(<1, 1, 0>, 20, 0.7)
    //  object {wedgemon(1) translate <-1, 0.02, 0>}
//    object {wedgemon(<1, 0.4, 0.6>, 0) translate <-1, 0.01, 0>}
//    object {wedgemon(<1, 0.4, 0.6>, 1) translate <1, 0.01, 0>}

    #if (clock_on)
      object {dalekinthebox(clock, Green, Blue, 0, 50, 0, -20) translate z}
    #else 
      object {dalekinthebox(0.23*1.8, Green, Blue, 0, 50, 0, -20) translate z}
    #end
  #end
  #ifdef (TEST_LIGHTBOX)
    plane {  y, 0 texture { pigment { checker colour Green colour Yellow} }}
//    camera { location <-1, 1.5, -1.3> look_at <0.2, 1.3, -0.8> angle 35}
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
    #local colrarray = array[8]{Orange White Orange Orange Orange Orange Orange Orange};
    #if (clock_on)
      object {lightbank(17/7, 2, 360*clock, colrarray) translate 2*y}
    #else
      object {lightbank(17/7, 2, 0, colrarray) translate 2*y}
    #end
  #end
  
#end
