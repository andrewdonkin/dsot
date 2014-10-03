#declare SpotRad = 0.2;
#declare SpotLightRad = 10;
#declare SpotLightFall = 15;
#declare SpotLightTight = 5;

#declare TiltDown = -10;
#declare TiltIn = 130;
#declare OddTiltDown = -10;
#declare OddTiltIn = 170;

#declare SpotCircleRad = 15;
#declare DiscRadius = SpotCircleRad - 1;

#declare SpotTubeRidgeHeight = 0.01;

#declare GantryWidth = 3;
#declare GantryHeight = 0.1;
#declare GantryDepth = 3;

#declare SpotTube = union {
  cylinder {-x/2 x/2 SpotRad open}
  cylinder {-x/2 <-0.47,0,0> SpotRad+0.01}
  cylinder {<-0.44,0,0> <-0.41,0,0> SpotRad+SpotTubeRidgeHeight}
  cylinder {<-0.38,0,0> <-0.35,0,0> SpotRad+SpotTubeRidgeHeight}
  cylinder {<-0.32,0,0> <-0.29,0,0> SpotRad+SpotTubeRidgeHeight}
  texture {pigment {Grey} finish {ambient 0.04 specular 0.9 metallic}}
};

#declare WhiteSpotBulb = light_source {0 color White spotlight point_at x
  radius SpotLightRad tightness SpotLightTight falloff SpotLightFall};

#declare GreenSpotBulb = light_source {0 color Green spotlight point_at x
  radius SpotLightRad tightness SpotLightTight falloff SpotLightFall};

#declare OrangeSpotBulb = light_source {0 color Orange spotlight
  point_at x
  radius SpotLightRad
  tightness SpotLightTight
  falloff SpotLightFall
  fade_distance SpotCircleRad / 2
//   looks_like {
//     sphere {x/2 SpotRad/3
//       pigment {color rgb <1, 1, 1>}
//       finish {ambient 1 diffuse 0}
//     }
  //   }
//  area_light SpotRad/2*y, SpotRad/2*z, 2,2
  
};

#declare RedSpotBulb = light_source {0 color rgb <1, 0, 0> spotlight point_at x
	radius SpotLightRad tightness SpotLightTight falloff SpotLightFall
  looks_like {
    sphere {x/2 SpotRad/3
      pigment {color rgb <1, 0, 0>}
      finish {ambient 1 diffuse 0}
    }
  }
};

#declare WhiteSpot = union {
	object {WhiteSpotBulb}
	object {SpotTube}
	texture {pigment {Grey} finish {specular 0.9 metallic}}
};
#declare GreenSpot = union {
	object {GreenSpotBulb}
	object {SpotTube}
	texture {pigment {Grey} finish {specular 0.9 metallic}}
};
#declare OrangeSpot = union {
	object {OrangeSpotBulb}
	object {SpotTube}
};
#declare RedSpot = union {
	object {RedSpotBulb}
	object {SpotTube}
	texture {pigment {Grey} finish {specular 0.9 metallic}}
};

#declare SpotBaseArm = union {
  intersection {
    torus {0.6 0.05}
    plane {z 0}
    plane {x 0}
    scale <0.5, 1, 1>
  }
  cylinder {<0,0,-0.6> <0.3,0,0> 0.05}
  sphere {<0,0,-0.6> 0.058}
}

#declare SpotBaseOffset = 0.7;
#declare SpotBase = union {
  object {SpotBaseArm translate <0,-SpotRad-0.1,0.6>}
  object {SpotBaseArm translate <0,SpotRad+0.1,0.6>}
  cylinder {<0, -SpotRad, 0> <0, -SpotRad-0.1, 0> 0.05}
  cylinder {<0, SpotRad, 0> <0, SpotRad+0.1, 0> 0.05}
  intersection {
    cylinder {<0,0,0.6> <0,0,SpotBaseOffset> 0.5}
    box {<-0.5,-SpotRad-0.2,-0.1> <+0.5,SpotRad+0.2,0.7>}
  }
  texture {
    pigment {Grey}
    finish {
      specular 0.9
      ambient 0.04
      metallic
    }
  }
}

/*----------------------------------------------------------------------*/

#declare Gantry = union {
  // Inners
  cylinder {<-GantryWidth/3, 0.8, -0.6> <GantryWidth/3, 0.8, -0.6> 0.05}
  cylinder {<-GantryWidth/3, 0.8, 0.6> <GantryWidth/3, 0.8, 0.6> 0.05}
  // Innersides
  cylinder {<-GantryWidth/3, 0.8, -0.6> <-GantryWidth/3, 0.8, 0.6> 0.05}
  cylinder {<GantryWidth/3, 0.8, -0.6> <GantryWidth/3, 0.8, 0.6> 0.05}
  // Outers
  cylinder {<-GantryWidth/2, 0.8, -1.2> <GantryWidth/2, 0.8, -1.2> 0.05}
  cylinder {<-GantryWidth/2, 0.8, 1.2> <GantryWidth/2, 0.8, 1.2> 0.05}
  // Outersides
  cylinder {<-GantryWidth/2, 0.8, 1.2> <-GantryWidth/2, 0.8, -1.2> 0.05}
  cylinder {<GantryWidth/2, 0.8, 1.2> <GantryWidth/2, 0.8, -1.2> 0.05}
  // Diagonals
  cylinder {<-GantryWidth/2, 0.8, -1.2> <-GantryWidth/3, 0.8, -0.6> 0.05}
  cylinder {<-GantryWidth/2, 0.8, 1.2> <-GantryWidth/3, 0.8, 0.6> 0.05}
  cylinder {<GantryWidth/2, 0.8, -1.2> <GantryWidth/3, 0.8, -0.6> 0.05}
  cylinder {<GantryWidth/2, 0.8, 1.2> <GantryWidth/3, 0.8, 0.6> 0.05}

  // Cables
  cylinder {<-GantryWidth/3, 0.8, -0.6> <-GantryWidth/3, 800, -0.6> 0.05}
  cylinder {<-GantryWidth/3, 0.8, 0.6> <-GantryWidth/3, 800, 0.6> 0.05}
  cylinder {<GantryWidth/3, 0.8, -0.6> <GantryWidth/3, 800, -0.6> 0.05}
  cylinder {<GantryWidth/3, 0.8, 0.6> <GantryWidth/3, 800, 0.6> 0.05}
}
