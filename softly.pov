#version 3.7;
#include "dsot.inc"

    camera { orthographic location <0.5, 0.5, -2> look_at <0.5, 0.5, 0> angle 40}
    cylinder {0, x, 0.003 finish { emission 1 } pigment {rgb z}}
    cylinder {0, y, 0.003 finish { emission 1 } pigment {rgb z}}
    cylinder {x, x+y, 0.003 finish { emission 1 } pigment {rgb z}}
    cylinder {y, x+y, 0.003 finish { emission 1 } pigment {rgb z}}
    #local i = -0.2;
    #while (i <= 1.2)
      sphere {<i, softly(i), -0.03>, 0.005 finish { emission 1 } pigment {rgb x}}
      sphere {<i, Interpolate(i, 0, 1, 0, 1, 0), -0.01>, 0.005 finish { emission 1 } pigment {rgb z}}
//      sphere {<i, Interpolate(i, 0, 1, 0, 1, 2), -0.01>, 0.005 finish { emission 1 } pigment {rgb z}}
//      sphere {<i, Interpolate(i, 0, 1, 0, 1, 3), -0.01>, 0.005 finish { emission 1 } pigment {rgb z}}
      sphere {<i, softly_3(i, 2), -0.04>, 0.005 finish { emission 1 } pigment {rgb x+y}}
      sphere {<i, SharpBell(i, 0.3, 0.6), -0.04>, 0.005 finish { emission 1 } pigment {rgb x+z}}
//      sphere {<i, SoftlyP2(i, 3), -0.04>, 0.005 finish { emission 1 } pigment {rgb y}} identical to softly_3
      sphere {<i, softly_3(i, 3), -0.04>, 0.005 finish { emission 1 } pigment {rgb y}}
      sphere {<i, -0.02, 0> 0.007 finish {emission 1} pigment {rgb whitespline(i)}}
      #warning concat(str(i, 2, 2),
	": <", str(whitespline(i).x, 2, 2),
	",", str(whitespline(i).y, 2, 2),
	",", str(whitespline(i).z, 2, 2),
	">")
      
      #local i = i + 0.02;
    #end
