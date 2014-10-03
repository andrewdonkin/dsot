//
// Vari*Lite disc and and rails
//

#declare DiscRadius = 4.9	// 32' diameter.
#declare SpotCircleRad = DiscRadius+1;

#local VariRailRadius = 0.04;
#local VariRailZOffset = 0.1;

#declare TiltDown = -3;
#declare TiltIn = 50;
#declare OddTiltDown = -10;
#declare OddTiltIn = 5;

#include "vl2c.inc"

// The large white circle screen onto which the varilights project.
cylinder {origin z*-0.1 DiscRadius
  texture {
    pigment {White}
    finish {diffuse 1.0 ambient 0.0}
  }
}

// Inside "track" - just a large torus
torus {SpotCircleRad-0.2 VariRailRadius rotate 90*x translate -VariRailZOffset*z
  texture {
    pigment{Grey}
    finish {diffuse 0.95 ambient 0.02}
  }
}
// Outside track.
torus {SpotCircleRad+0.2 VariRailRadius rotate 90*x translate -VariRailZOffset*z
  texture {
    pigment{Grey}
    finish {diffuse 0.95 ambient 0.02}
  }
}

#local SpotDiscZDiff=-VariRailZOffset-VariRailRadius;
#local angl=0;
#while (angl < 360)
  object {
    vl2c(color Orange, <TiltDown, TiltIn, 0>)
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

// ----------------------------------------------------------------------

