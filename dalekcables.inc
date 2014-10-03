/***********************************************************************
* Some cables connecting the plinth to the lightbox underside and backside.
* Pitching the lightbox breaks them somewhat.
*
* These are rotated with the lightbox.
*
* As far as I can tell, they don't exist on the real unit,
* but I've spent too much working on the wriggly bastards to
* cut them out now.
*
* Second thoughts, they're gone.
*/

// This became a distraction in dalek.inc.

union {
  // This first lot, from the base of the box down into the plinth,
  // are a waste now that I've lowered the box.  They're hard to see.
  sphere_sweep {
    b_spline
    17,
    <-0.04, -0.05, -0.07>, 0.01,
    <-0.04, -0.01, -0.07>, 0.01,
    <-0.04,  0.02, -0.07>, 0.01,
    < 0.00,  0.03, -0.06>, 0.01,
    < 0.10,  0.04,  0.10>, 0.01,
    < 0.06,  0.04,  0.11>, 0.01,
    < 0.02,  0.05,  0.05>, 0.01,
    <-0.04,  0.06,  0.01>, 0.01,
    < 0.01,  0.07,  0.00>, 0.01,
    < 0.02, boxplinthsep,      0.05>, 0.01,
    < 0.02, boxplinthsep+0.02, 0.05>, 0.01
    // and down again
    < 0.05, boxplinthsep+0.02, 0.01>, 0.015
    < 0.05, boxplinthsep, 0.01>, 0.015
    < 0.01,  0.06, 0.00>, 0.015
    < 0.01,  0.03, 0.00>, 0.015
    <-0.01, -0.01, 0.00>, 0.015
    <-0.01, -0.05, 0.00>, 0.015
  }
  // More, from the back of the lightbox.  Head for (x,z)=(0,0) so they don't
  // translate when rotated around Y.
  #local r=0.005;
  sphere_sweep {
    b_spline
    10,
    < 0.05,  0.40,  -0.07>, r
    < 0.05,  0.38,  -0.10>, r
    < 0.04,  0.35,  -0.25>, r
    < 0.03,  0.30,  -0.25>, r
    < 0.02,  0.20,  -0.25>, r
    < 0.01,  0.20,  -0.25>, r
    < 0.01,  0.10,  -0.25>, r
    < -0.01,  0.05,  -0.15>, r
    < -0.02,  -0.01,  -0.1>, r
    < -0.02,  -0.05,  -0.1>, r
  }
  sphere_sweep {
    b_spline
    10,
    < 0.02,  0.40,  -0.07>, r
    < 0.02,  0.39,  -0.10>, r
    < 0.01,  0.37,  -0.20>, r
    < 0.00,  0.34,  -0.21>, r
    < 0.00,  0.30,  -0.22>, r
    < 0.00,  0.20,  -0.22>, r
    < 0.00,  0.10,  -0.20>, r
    < 0.00,  0.05,  -0.15>, r
    < 0.00,  -0.01,  -0.15>, r
    < 0.00,  -0.05,  -0.15>, r
  }
  sphere_sweep {
    b_spline
    10,
    < 0.04,  0.40,  -0.05>, r
    < 0.04,  0.40,  -0.10>, r
    < 0.03,  0.38,  -0.18>, r
    < 0.03,  0.35,  -0.19>, r
    < 0.04,  0.30,  -0.20>, r
    < 0.05,  0.20,  -0.21>, r
    < 0.04,  0.10,  -0.22>, r
    < 0.03,  0.05,  -0.18>, r
    < 0.00,  -0.01,  -0.12>, r
    < 0.00,  -0.05,  -0.12>, r
  }
  sphere_sweep {
    b_spline
    10,
    < -0.02,  0.40,  -0.05>, r
    < -0.02,  0.39,  -0.10>, r
    < -0.03,  0.39,  -0.21>, r
    < -0.03,  0.35,  -0.23>, r
    < -0.04,  0.29,  -0.24>, r
    < -0.06,  0.24,  -0.25>, r
    < -0.04,  0.10,  -0.25>, r
    < -0.03,  0.05,  -0.20>, r
    < -0.03,  0.02,  -0.15>, r
    < -0.03,  -0.05,  -0.15>, r
  }
  sphere_sweep {
    b_spline
    10,
    < -0.05,  0.38,  -0.05>, r
    < -0.05,  0.38,  -0.10>, r
    < -0.04,  0.37,  -0.23>, r
    < -0.04,  0.36,  -0.25>, r
    < -0.05,  0.30,  -0.28>, r
    < -0.06,  0.25,  -0.27>, r
    < -0.07,  0.10,  -0.23>, r
    < -0.04,  0.05,  -0.18>, r
    < -0.04,  -0.01,  -0.10>, r
    < -0.03,  -0.05,  -0.10>, r
  }
  texture {cable_t}
  translate plinththick*y
}

