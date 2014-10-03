#declare GantryWidth = 3;
#declare GantryHeight = 0.1;
#declare GantryDepth = 3;

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
