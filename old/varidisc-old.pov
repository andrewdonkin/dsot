//
// Vari-light disc and and rails
//

#local VariRailRadius = 0.05;
#local VariRailZOffset = 0.1;

// The large white disc onto which the varilights project.
cylinder {origin z*-0.1 DiscRadius
  texture {
    pigment {White}
    finish {diffuse 1.0 ambient 0.0}
  }
}

// Inside "track" - just a large torus
torus {SpotCircleRad-0.3 VariRailRadius rotate 90*x translate -VariRailZOffset*z
  texture {
    pigment{Grey}
    finish {diffuse 0.95 ambient 0.02}
  }
}
// Outside track.
torus {SpotCircleRad+0.3 VariRailRadius rotate 90*x translate -VariRailZOffset*z
  texture {
    pigment{Grey}
    finish {diffuse 0.95 ambient 0.02}
  }
}

#local SpotDiscZDiff=-SpotBaseOffset-VariRailZOffset-VariRailRadius;
#local angl=0;
#while (angl < 360)
  union {
    object{OrangeSpot rotate TiltDown*y}
    object{SpotBase}
    rotate TiltIn*z
    translate <SpotCircleRad,0,SpotDiscZDiff>
    rotate angl*z
  }
  union {
    object{RedSpot rotate OddTiltDown*y}
    object{SpotBase}
    rotate OddTiltIn*z
    translate <SpotCircleRad,0,SpotDiscZDiff>
    rotate (angl+10)*z
  }
  #local angl = angl + 20;
#end

// ----------------------------------------------------------------------

