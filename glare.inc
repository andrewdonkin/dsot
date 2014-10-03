// Persistence of Vision Ray Tracer Scene Description File
// File: glare.inc
// Vers: 3.5
// Desc: Blinding-HighLights 
// Date: 14.06.03 (dd.mm.yy)
// Auth: Tim Nikias Wenclawiak
// Last Update: 22.06.03 (dd.mm.yy)

//Updates:
// 22.06.03 - Added Photon-Commands to the Layer-Object
// 21.06.03 - Added "Hollow" keyword, to keep media functioning...

//Description
// For proper usage of the macro, please consult the accompanying Help.

//Homepage:
// www.digitaltwilight.de/no_lights
//Email:
// Tim.Nikias@gmx.de

//My files sometimes build on each other. Checking if following parameter
//was declared/is set to true avoids my other include-files to include
//an already included file again.
#declare _glare_inc_tnw=1;

//This macro gets added almost all the time... I wonder why... :)
/****  VNormalize()  ****/
#macro VNormalize(_Vec)
 //Create proper return-value
 #if (vlength(_Vec))
  #local _Ret=vnormalize(_Vec); #else
  #local _Ret=<0,0,0>; #end
 //Return:
 _Ret
#end

//Cam-Internal Settings
#ifndef (Camera_Location)     #declare Camera_Location = <0,0,-1>;                           #end
#ifndef (Camera_Look_At)      #declare Camera_Look_At = <0,0,0>;                             #end
#ifndef (Camera_Sky)          #declare Camera_Sky = <0,1,0>;                                 #end
#ifndef (Camera_Aspect_Ratio) #declare Camera_Aspect_Ratio = <image_width/image_height,1,1>; #end

//Internal Macro which will translate a vector ranging from
// <-.5,-.5,x> to <.5,.5,x> to an onscreen position. The x
// defines the distance to the camera. Smaller/Larger values
// will lead to positions outside of viewing screen.
#macro Glare_Transform(_Pos)
 //Matrix-Setup
 #declare Cam_D = VNormalize(Camera_Look_At-Camera_Location);
 #declare Cam_X = VNormalize(vcross(VNormalize(Camera_Sky),Cam_D));
 #declare Cam_Y = -VNormalize(vcross(Cam_X,Cam_D));
 #declare Cam_Matrix =
  transform{matrix
   <Cam_X.x, Cam_X.y, Cam_X.z,
    Cam_Y.x, Cam_Y.y, Cam_Y.z,
    Cam_D.x, Cam_D.y, Cam_D.z,
    0,       0,       0>
   };
 #local _F=function{transform{Cam_Matrix}}
 #local Ret=_F(_Pos.x*Camera_Aspect_Ratio.x*_Pos.z,_Pos.y*Camera_Aspect_Ratio.y*_Pos.z,_Pos.z)+Camera_Location;
 Ret
#end

//Glare_Camera()
//Parameters:
// None
//Returns:
// Camera-Statement which uses all set declarations
//Notes:
// None
#macro Glare_Camera()
 camera{
  location Camera_Location
  right Camera_Aspect_Ratio*<1,0,0>
  up Camera_Aspect_Ratio*<0,1,0>
  sky Camera_Sky
  look_at Camera_Look_At
  }
#end

//Glare_PlaceImage()
//Parameters:
// 1. Pigment, mapped onto layer
// 2. Distance to camera
//Returns:
// 1. Layer in front of camera with given pigment
//Notes:
// Called by almost all macros...
#macro Glare_PlaceImage(_Pigment,_DistanceToCamera)
 #local _UL=Glare_Transform(<-.5, .5,_DistanceToCamera>);
 #local _LL=Glare_Transform(<-.5,-.5,_DistanceToCamera>);
 #local _UR=Glare_Transform(< .5, .5,_DistanceToCamera>);
 #local _LR=Glare_Transform(< .5,-.5,_DistanceToCamera>);
 mesh{
  triangle{_UL,_LL,_UR uv_vectors <0,1>,<0,0>,<1,1>}
  triangle{_UR,_LR,_LL uv_vectors <1,1>,<1,0>,<0,0>}
  texture{uv_mapping _Pigment}
  hollow
  no_shadow no_reflection
  photons{refraction off reflection off collect off pass_through}
  }
#end

//Glare_Layer
//Parameters:
// 1. Pigment with default image-map (<0,0,0> to <1,1,0>)
// 2. POV-Units distance of Layer to camera
// 3. Exponent for color-map
// 4. Ambient (Brightness) of layer
//Returns:
// 1. Specularity layer in front of camera
#macro Glare_Layer(_ImagePigment,_DistanceToCamera,_ExpBright)
 #local _I=function{pigment{_ImagePigment}}
 #local _Image=
  texture{
   pigment{function{_I(x,y,z).gray} color_map{[0 rgbt <1,1,1,1>][1 rgbt <1,1,1,0>]} poly_wave _ExpBright.u}
   finish{ambient _ExpBright.v diffuse 0}
   }
 Glare_PlaceImage(_Image,_DistanceToCamera)
#end

//Glare_BlurredLayer
//Parameters:
// 1. Pigment with default image-map (<0,0,0> to <1,1,0>)
// 2. POV-Units distance of Layer to camera
// 3. Samples for blurring
// 4. Radius of blurring
// 5. Exponent for color-map
// 6. Ambient (Brightness) of layer
//Returns:
// 1. Blurred specularity layer in front of camera
#macro Glare_BlurredLayer(_ImagePigment,_DistanceToCamera,_ExpBright,_Radius,_Samples)
 #ifndef (Glare_Preparse) #local Glare_Preparse=0; #end
 #local _Black=pigment{rgb 0}
 #local _S=max(min(_Samples,256),1);
 #local _ImageCroppedX=
  pigment{gradient x scale <3,1,1>
   pigment_map{
    [0 _Black]
    [1/3 _Black]
    [1/3 _ImagePigment scale <1/3,1,1> translate x]
    [2/3 _ImagePigment scale <1/3,1,1> translate x]
    [2/3 _Black]
    [1 _Black]
    }translate -x
   }
 #local _ImageCroppedXY=
  pigment{gradient y scale <1,3,1>
   pigment_map{
    [0 _Black]
    [1/3 _Black]
    [1/3 _ImageCroppedX scale <1,1/3,1> translate y]
    [2/3 _ImageCroppedX scale <1,1/3,1> translate y]
    [2/3 _Black]
    [1 _Black]
    }translate -y
   }
   
 //This part preparses the texture into an image_map for quick rendering
 #if (Glare_Preparse)
  #local _BlurredImageMapPig=
    pigment{
     average pigment_map{
     #local A=0;
     #while (A<_S)
      [1 _ImageCroppedXY translate vrotate(<_Radius,0,0>,z*360/.618*A)*sqrt(A/_S)]
      #local A=A+1;
     #end
     }
    }
  #local _BlurredImageMap=pigment{image_map{ function image_width,image_height{pigment{_BlurredImageMapPig scale <1,-1,1> translate y}}}}
  #local _BlurredImageFunction=function{pigment{_BlurredImageMap}}
 //And this part just creates the function used as pigment, for quick parsing
 #else
  #local _BlurredImageFunction=
   function{
    pigment{
     average pigment_map{
      #local A=0;
      #while (A<_S)
       [1 _ImageCroppedXY translate vrotate(<_Radius,0,0>,z*360/.618*A)*sqrt(A/_S)]
       #local A=A+1;
      #end
      }
     }
    }
 #end

 #local _BlurredImage=
  texture{
   pigment{function{_BlurredImageFunction(x,y,z).gray} color_map{[0 rgbt <1,1,1,1>][1 rgbt <1,1,1,0>]} poly_wave _ExpBright.u}
   finish{ambient _ExpBright.v diffuse 0}
   }

 Glare_PlaceImage(_BlurredImage,_DistanceToCamera)
#end

//Glare_OverblurredLayer
//Parameters:
// 1. Pigment with default image-map (<0,0,0> to <1,1,0>)
// 2. POV-Units distance of Layer to camera
// 3. Samples for blurring
// 4. Radius of blurring
// 5. Exponent for color-map
// 6. Ambient (Brightness) of layer
//Returns:
// 1. Blurred specularity layer in front of camera
#macro Glare_OverblurredLayer(_ImagePigment,_DistanceToCamera,_ExpBright,_Radius,_Samples,_Recursions)
 #local _Black=pigment{rgb 0}
 #local _S=max(min(_Samples,256),1);
 
 //First, crop outer sections of image-map. Otherwise the
 // specularity-layer appears "wrapped"
 #local _ImageCroppedX=
  pigment{gradient x scale <3,1,1>
   pigment_map{
    [0 _Black]
    [1/3 _Black]
    [1/3 _ImagePigment scale <1/3,1,1> translate x]
    [2/3 _ImagePigment scale <1/3,1,1> translate x]
    [2/3 _Black]
    [1 _Black]
    }translate -x
   }
 #local _ImageCroppedXY=
  pigment{gradient y scale <1,3,1>
   pigment_map{
    [0 _Black]
    [1/3 _Black]
    [1/3 _ImageCroppedX scale <1,1/3,1> translate y]
    [2/3 _ImageCroppedX scale <1,1/3,1> translate y]
    [2/3 _Black]
    [1 _Black]
    }translate -y
   }
  
  //The blur that pigment via averaging several samples placed inside
  // a given radius. The positioning is even-distributed.
  #local _BlurredImageMapPig=
    pigment{
     average pigment_map{
     #local A=0;
     #while (A<_S)
      [1 _ImageCroppedXY translate vrotate(<_Radius/_Recursions,0,0>,z*360/.618*A)*sqrt(A/_S)]
      #local A=A+1;
     #end
     }
    }

  //Preparse the pigment into an internal image-map for later recursion of blurring -> smoother results, longer
  // parsing times
  #local _BlurredImageMap=pigment{image_map{ function image_width,image_height{pigment{_BlurredImageMapPig scale <1,-1,1> translate y}}}}

  //Repeat the process as you like...
  #local _R=1;
  #while (_R<_Recursions)
   //Crop the new image again...
   #local _ImageCroppedX=
    pigment{gradient x scale <3,1,1>
     pigment_map{
      [0 _Black]
      [1/3 _Black]
      [1/3 _BlurredImageMap scale <1/3,1,1> translate x]
      [2/3 _BlurredImageMap scale <1/3,1,1> translate x]
      [2/3 _Black]
      [1 _Black]
      }translate -x
     }
   #local _ImageCroppedXY=
    pigment{gradient y scale <1,3,1>
     pigment_map{
      [0 _Black]
      [1/3 _Black]
      [1/3 _ImageCroppedX scale <1,1/3,1> translate y]
      [2/3 _ImageCroppedX scale <1,1/3,1> translate y]
      [2/3 _Black]
      [1 _Black]
      }translate -y
     }
   //Blur it again...
   #local _BlurredImageMapPig=
     pigment{
      average pigment_map{
      #local A=0;
      #while (A<_S)
       [1 _ImageCroppedXY translate vrotate(<_Radius/_Recursions,0,0>,z*360/.618*A)*sqrt(A/_S)]
       #local A=A+1;
      #end
      }
     }
   //Preparse the pigment into an internal image-map again...
   #local _BlurredImageMap=pigment{image_map{ function image_width,image_height{pigment{_BlurredImageMapPig scale <1,-1,1> translate y}}}}
   #local _R=_R+1;
  #end

 #local _BlurredImageFunction=function{pigment{_BlurredImageMap}}

 #local _BlurredImage=
  texture{
   pigment{function{_BlurredImageFunction(x,y,z).gray} color_map{[0 rgbt <1,1,1,1>][1 rgbt <1,1,1,0>]} poly_wave _ExpBright.u}
   finish{ambient _ExpBright.v diffuse 0}
   }
 Glare_PlaceImage(_BlurredImage,_DistanceToCamera)
#end
