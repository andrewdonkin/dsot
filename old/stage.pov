#declare StageWidth = 40;
#declare StageDepth = 20;
#declare StageHeight = 1;
#declare StageYOffset = -18;

box {<-StageWidth/2, StageYOffset, -StageDepth> <StageWidth/2, StageYOffset+StageHeight, 0>
  texture {
    pigment {
      hexagon
      color rgb <0.4,0.4,0.4>
      color rgb <0.6,0.6,0.6>
      color rgb <0.9,0.9,0.9>
    }
    scale 0.3
  }
  finish {specular 0.9 metallic}
}
