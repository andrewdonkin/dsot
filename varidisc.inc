//
// Vari*Lite disc and and rails
//

#macro circlescreen(discradius)

#local SpotCircleRad = discradius+0.5;

#local VariRailRadius = 0.04;
#local VariRailZOffset = 0.1;

#local TiltDown = -15;
#local TiltIn = 55;
#local OddTiltDown = -10;
#local OddTiltIn = 5;

//#local TiltDown = 40;
//#local TiltIn = 0;

union {// The large white circle screen onto which the varilights project.
  cylinder {0 z*0.1 discradius
    texture {
      pigment {White}
      finish {diffuse 1.0 ambient 0.0}
    }
  }

  // Inside "track" - just a large torus
  torus {SpotCircleRad-0.15 VariRailRadius rotate 90*x translate -VariRailZOffset*z
    texture {
      pigment{Grey}
//      finish {diffuse 0.95 ambient 0.02}
    }
  }
  // Outside track.
  torus {SpotCircleRad+0.15 VariRailRadius rotate 90*x translate -VariRailZOffset*z
    texture {
      pigment{Grey}
//      finish {diffuse 0.95 ambient 0.02}
    }
  }

  #local SpotDiscZDiff=-VariRailZOffset-VariRailRadius;
  #local angl=0;
  #while (angl < 360)
    object {
      vl2c(rgb<1, 0.3, 0>, <TiltDown, TiltIn, 0>)
      rotate <90, 0, 0>
      translate <0, SpotCircleRad, SpotDiscZDiff>
      rotate angl*z
    }
    object {
      vl2c(color Red, <OddTiltDown, OddTiltIn, 0>)
      rotate <90, 0, 0>
      translate <0, SpotCircleRad, SpotDiscZDiff>
      rotate (angl+(360/28))*z
    }
    #local angl = angl + 360/14;
  #end
}
#end


