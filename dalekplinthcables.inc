/*
 * Cables running from dalek plinth to its riser beam.
 * Dalek is facing -z, plinth is sitting on y=0
 */

union {
  #local r = 0.003; 
  #local ra = array[8] {
    <0.13, 0.05, -0.21>
    <0.14, 0.05, -0.03>
    <0.12, 0.038, 0.05>
    <0.05, -0.08, 0.13>
    <0.04, -0.30, 0.09>
    <0.02, -0.40, 0.06>
    <0.02, -1, 0.05>
    <0.02, -6, 0.05>
  };
  #local vec = array[8]{
    <0.1, 0, 1>/40
    <-0.2, -0.1, 1>/40
    <-0.8, -0.5, 1.5>/60
    <-0.1, -1, 0>/20
    <-0.05, -1, -0.3>/40
    <0, -1, -0.002>/40
    <0, -1, 0>/40
    <0, -1, 0>/40
  };

  #if (0) // draws points and vectors for the cable with no translation or scaling
    union {
      #local i = 0;
      #while (i < dimension_size(ra, 1))
	cylinder {ra[i], ra[i]-vec[i]*2, 0.002 pigment {rgb x}}
	sphere {ra[i], 0.006 pigment {rgb y}}
	cylinder {ra[i], ra[i]+vec[i]*2, 0.003 pigment {rgb z}}
	#local i = i + 1;
      #end
      translate 0.05*z
    }
  #end

  #macro cable1(xlate, scal, optional final_xlate)
    #ifndef (final_xlate)
      #local final_xlate=<0,0,0>;
    #end
    sphere_sweep {
      b_spline dimension_size(ra, 1) * 3
      #local i = 0;
      #while (i < dimension_size(ra, 1))
	(ra[i] - vec[i] + xlate) * scal, r
	(ra[i] + xlate) * scal, r,
	(ra[i] + vec[i] + xlate) * scal, r
	#local i = i + 1;
      #end
      translate 0.05*z + final_xlate
    }
  #end
  cable1(<0.01, 0, -0.01>, <1, 1, 1.3>, <0, 0.02, 0>)
  cable1(0, 1, )
  cable1(<-0.01, 0, -0.01>, <0.9, 1, 1.3>, <0, 0.02, 0>)
  cable1(<-0.015, 0.005, 0>, <0.9, 1, 1>, )

  cable1(<0.01, 0.06, 0.02>, <.1, 1, 1>, )
  cable1(<0.01, 0.065, 0.02>, <0, 1, 1>, )
  cable1(<0.04, 0.06, 0.02>, <-.2, 1, 1>, <0.01, 0, -0.01>)

  cable1(<0.01, 0.03, -0.01>, <-1, 1, 1.3>, <0, 0.03, 0>)
  cable1(0.03*y, <-1, 1, 1>, )
  cable1(<-0.01, 0.03, -0.01>, <-0.9, 1, 1.3>, )
  cable1(<-0.015, 0.035, 0>, <-0.9, 1, 1>, )

  torus {0.05, 0.005 scale <1, 3, 0.6> translate <0, -0.35, 0.11> texture {cable_t scale <10,0.1,10>}}
  // These struts supported that torus when it was floating at z=0.25
  //cylinder {<-0.08, -0.4, 0.1> <-0.05, -0.345, 0.21> 0.005}
  //cylinder {<-0.08, -0.31, 0.1> <-0.05, -0.355, 0.21> 0.005}
  //cylinder {<0.08, -0.4, 0.1> <0.05, -0.345, 0.21> 0.005}
  //cylinder {<0.08, -0.31, 0.1> <0.05, -0.355, 0.21> 0.005}
  translate <0, 0, plinthradius>
  texture {cable_t scale <1,100,1>} // these cables are generally vertical
}
