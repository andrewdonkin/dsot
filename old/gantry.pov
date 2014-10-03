#include "colors.inc"
#include "spots.inc"

#declare g1ftilt = 65;
#declare g1btilt = 85;
#declare g1f1pan = -130;
#declare g1f2pan = -100;
#declare g1f3pan = -70;
#declare g1b1pan = g1f1pan;
#declare g1b2pan = g1f2pan;
#declare g1b3pan = g1f3pan;

#declare g2ftilt = 10;
#declare g2btilt = 120;
#declare g2f1pan = -90;
#declare g2f2pan = -90;
#declare g2f3pan = -90;
#declare g2b1pan = g2f1pan;
#declare g2b2pan = g2f2pan;
#declare g2b3pan = g2f3pan;


union {
	object {Gantry}
	union {	object{GreenSpot rotate g1ftilt*y}
		object{SpotBase}
		rotate g1f1pan*z
		rotate -90*x
		translate <-1,0,-1>}
	union {	object{GreenSpot rotate g1ftilt*y}
		object{SpotBase}
		rotate g1f2pan*z
		rotate -90*x
		translate <0,0,-1>}
	union {	object{GreenSpot rotate g1ftilt*y}
		object{SpotBase}
		rotate g1f3pan*z
		rotate -90*x
		translate <1,0,-1>}
	union {	object{GreenSpot rotate g1btilt*y}
		object{SpotBase}
		rotate g1b1pan*z
		rotate -90*x
		translate <-1,0,1>}
	union {	object{GreenSpot rotate g1btilt*y}
		object{SpotBase}
		rotate g1b2pan*z
		rotate -90*x
		translate <0,0,1>}
	union {	object{GreenSpot rotate g1btilt*y}
		object{SpotBase}
		rotate g1b3pan*z
		rotate -90*x
		translate <1,0,1>}
	translate <5,6,-15>
  texture {pigment {Grey} finish {specular 0.9 metallic
    ambient 0.05}
  }
}

union {
  object {Gantry}
  union {	object{WhiteSpot rotate g2ftilt*y}
    object{SpotBase}
    rotate g2f1pan*z
    rotate -90*x
    translate <-1,0,-1>}
  union {	object{WhiteSpot rotate g2ftilt*y}
    object{SpotBase}
    rotate g2f2pan*z
    rotate -90*x
    translate <0,0,-1>}
  union {	object{WhiteSpot rotate g2ftilt*y}
    object{SpotBase}
    rotate g2f3pan*z
    rotate -90*x
    translate <1,0,-1>}
  union {	object{WhiteSpot rotate g2btilt*y}
    object{SpotBase}
		rotate g2b1pan*z
    rotate -90*x
    translate <-1,0,1>}
  union {	object{WhiteSpot rotate g2btilt*y}
    object{SpotBase}
    rotate g2b2pan*z
    rotate -90*x
    translate <0,0,1>}
  union {
    object{WhiteSpot rotate g2btilt*y}
    object{SpotBase}
    rotate g2b3pan*z
    rotate -90*x
    translate <1,0,1>
  }
  translate <-8,-7,-10>
  texture {
    pigment {Grey}
    finish {
      specular 0.9
      metallic
      ambient 0.05
    }
  }
}
