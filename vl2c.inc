/* (c) 1988-2020 Andrew Donkin.
 * This work is licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/
 */
#macro vl2c(lightcolour, rot)

#local bigdimen = 0.445;   // Length of the tube and max dimen of base, specs.
#local tubewidth = 0.240;  // estimated
#local tubeheight = 0.273; // from specs
#local baseheight = 0.159; // from specs
#local baseyokegap = 0.01;
#local tubeyokegap = 0.02;
#local lensrad = 0.05; // radius of the spotlight's opening

// tube pitches on this axis.  654mm from specs
#local tubeaxisy = -(0.654 - tubeheight / 2);

#local yokethick = 0.05; // thickness of arms
#local yokewidth = 0.10; // width of arms: front to back
#local outerradius = yokethick * 2;

#declare SpotLightRad = 2;
#declare SpotLightFall = 12;
//#declare SpotLightRad = 2;	// test
//#declare SpotLightFall = 5;
#declare SpotLightTight = 5;
#declare SpotLightFade = 1;

// Computed values
#local yokemaxx = (tubewidth+tubeyokegap) / 2 + yokethick;
#local yokecornerx = yokemaxx - outerradius;
#local yokecornery = -(baseheight + outerradius + baseyokegap);
#local yokecorner_v = <yokecornerx, yokecornery, 0>;

union {
  //----------------------------------------------------------------------
  // Head unit and light source
  union {
    intersection {
      difference {
	box {
	  <-tubewidth/2, -tubeheight/2, -bigdimen/2>,
	  < tubewidth/2,  tubeheight/2,  bigdimen/2>
 	}
	light_group {
	  light_source {
	    <0, 0, bigdimen/3>
	    color lightcolour 
	    media_interaction on
	  }
	  cone {
	    <0, 0, 0>, lensrad / 1.5,
	    <0, 0, bigdimen/2 + 0.01>, lensrad
	    pigment {White}
	    finish {specular 0.9 reflection 0.5}
	  }
	  sphere {<0, 0, 0>, lensrad/1.5
	    pigment {White}
	    finish {specular 0.9 reflection 0.5}
	  }
	}
      }
      // Trim off the corners
      box {
	<-tubewidth/1.65, -tubewidth/1.65, -bigdimen/1.9>,
	< tubewidth/1.65,  tubewidth/1.65,  bigdimen/1.9>
	rotate 45*z
      }
    }

    // Half-shafts on which the head pitches
    cylinder {
      <tubewidth/2, 0, 0>,
      <(tubewidth/2 + tubeyokegap), 0, 0>
      yokewidth/3
    }
    cylinder {
      <-tubewidth/2, 0, 0>,
      <-(tubewidth/2 + tubeyokegap), 0, 0>
      yokewidth/3
    }

    light_source {
      // nudge it away from the origin, toward the opening in the head
      // unit, else the opening clips the light.
      <0, 0, bigdimen/2 - lightsource_backoff(lensrad,SpotLightFall)>
      color lightcolour
      spotlight
      point_at z
      radius SpotLightRad
      tightness SpotLightTight
      falloff SpotLightFall
      fade_distance SpotLightFade fade_power 2
      media_attenuation on
//      area_light lensrad/2*x lensrad/2*y 3 3 circular orient adaptive on
      projected_through {
	disc {lightsource_backoff(lensrad, SpotLightFall)*z, z, lensrad}
      }
    }

    rotate <rot.x, rot.y, 0> // Do no rotation about z axis.
    translate y*tubeaxisy
  }
  
  //----------------------------------------------------------------------
  // Yoke
  difference {
    union {
      cylinder { // right corner
	yokecorner_v - z*yokewidth/2,
	yokecorner_v + z*yokewidth/2
	outerradius
      }
      cylinder { // left corner
	<-1, 1, 1>*yokecorner_v - z*yokewidth/2,
	<-1, 1, 1>*yokecorner_v + z*yokewidth/2
	  outerradius
      }
      box { // connecting corners
	< yokecornerx, yokecornery, -yokewidth/2>,
	<-yokecornerx, yokecornery + outerradius, yokewidth/2>
      }
      box { // arms hanging down to head unit (tube)
	< yokemaxx, tubeaxisy, -yokewidth/2>,
	<-yokemaxx, yokecornery, yokewidth/2>
      }
      cylinder { // radiused caps on ends of the arms
	<-yokemaxx, tubeaxisy, 0>,
	< yokemaxx, tubeaxisy, 0>
	yokewidth / 2
      }
    }
    union {
      #local innerradius = outerradius - yokethick;
      cylinder { // right corner
	yokecorner_v - z*yokewidth/1.9,
	yokecorner_v + z*yokewidth/1.9
	innerradius
      }
      cylinder { // left corner
	<-1, 1, 1>*yokecorner_v - z*yokewidth/1.9,
	<-1, 1, 1>*yokecorner_v + z*yokewidth/1.9
	innerradius
      }
      box { // connecting corners
	< yokecornerx, yokecornery, -yokewidth/1.9>,
	<-yokecornerx, yokecornery + innerradius, yokewidth/1.9>
      }
      box { // gap between arms
	// tubeheight is positive, tubeaxisy is negative
	<  yokemaxx-yokethick, -1*(abs(tubeaxisy) + tubeheight), -yokewidth/1.9>,
	<-(yokemaxx-yokethick), yokecornery, yokewidth/1.9>
      }
    }
    rotate y*rot
  }
  //----------------------------------------------------------------------
  // base unit
  union {
    intersection {
      difference {
	#local baseboxwidth = bigdimen * 0.82;
	box {
	  <-baseboxwidth/2, 0, -baseboxwidth/2>,
	  < baseboxwidth/2, -baseheight,  baseboxwidth/2>
	}
	#local angl=0;
	#while (angl < 360)
	  #local angl = angl + 90;
	  box {
	    <baseboxwidth/6,        -0.01, -baseboxwidth/1.9>,
	    <baseboxwidth/6 - 0.04, -0.02, -baseboxwidth/2.1>
	    rotate angl * y
	  }
	#end
	rotate 45 * y
      }
      box {
	<-bigdimen/2, 0, -bigdimen/2>,
	< bigdimen/2, -baseheight*1.1,  bigdimen/2>
      }
    }
    cylinder {
      <0, -baseheight, 0>, <0, -baseheight-baseyokegap, 0> yokewidth/2
    }
  }
  texture {pigment {Gray50} finish {ambient 0 specular 0.9 metallic}
    normal { bumps 0.1 scale 0.0015}
  }
}

#end

#ifdef (TEST)
  //
  // Render one VL2C Vari*Lite
  //
  #include "colors.inc"
  #include "dsot.inc"

  plane {y, 0 texture { pigment { colour White}  } }
  #if (1)
    box {<-1, 0, -2>, <1, 1, 1> hollow
      interior {
	ior 1
	media {
	  scattering {2, 0.4 extinction 0.1}
	  samples 30,30
	  intervals 1
	  method 3
	}
      }
      texture {	pigment {rgbt 1}}
    }
  #end

  camera {location <0, 0.4, -3> look_at <0, 0.2, 0> angle 30}
  light_source { <7, 7, -5> White/2
    spotlight point_at 0.4*y radius 1 falloff 4
    media_interaction on
    media_attenuation on
//    area_light 0.2*x 0.2*y 10 10 circular orient adaptive 1
  }
//  light_source { <0.5, 2, -0.3> White }

  object {
    vl2c(Orange, <0, 160, 0>)
    translate 0.650*y
  }

#end
