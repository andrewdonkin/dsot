#include "colors.inc"
#include "textures.inc"

#if (CHEAP_MUSIC) 
  #warning "Using cheap instruments!"
#end

#local bodytexture = texture {
  pigment { colour Red}
}

#local chrometexture = texture {
//  Polished_Chrome
  Chrome_Metal
}

#local rubbertexture = texture {
  pigment {colour Gray20 }
}

#local bassradius = 0.48;		// External radius
#local basscylinderthickness = 0.03;
#local bassyoffset = 0.5;		// Centre of its axis
#local basslength = 0.7;		// Length along Z axis
#local bassoutercylinder = cylinder {
  <0, bassyoffset, -(basslength/2 - basscylinderthickness/2)>,
  <0, bassyoffset, +(basslength/2 - basscylinderthickness/2)>,
  bassradius
}

#local littlelegthick = 0.007;		// smallest radius of little legs

//----------------------------------------------------------------------
#macro bassring(flipit)
// half of the width of the chrome ring around each end
#local ringthick2 = 0.025;
#local ringz = basslength/2 - 0.035;
union {
  difference {
    #if (CHEAP_MUSIC) 
      cylinder {
	<0, 0, ringz - ringthick2>,
	<0, 0, ringz + ringthick2>,
	bassradius*1.02
	translate <0, bassyoffset, 0>
      }
    #else
      superellipsoid {
	<1, 0.07>
	scale <bassradius*1.02, bassradius*1.02, ringthick2>
	translate <0, bassyoffset, ringz>
      }
    #end // CHEAP_MUSIC
    object { bassoutercylinder scale <1.0, 1.0, 1.1> }
    #if (! CHEAP_MUSIC) 
      // This hard-edged bounding ring halves the render time.
      bounded_by {
	difference {
	  cylinder {
	    <0, bassyoffset, ringz - ringthick2>
	    <0, bassyoffset, ringz + ringthick2>
	    0.5
	  }
	  object { bassoutercylinder scale <0.8, 0.8, 1.1> }
	}
      }
    #end
  }
  #local angl=30;
  #while (angl < 360)
    #local angl = angl + 60;
    superellipsoid {
      <0.25, 0.07>
      translate -z
      scale <0.01, 0.015, basslength/8>
      translate <0, bassradius, ringz*1.1>
      rotate angl*z
      translate <0, bassyoffset, 0>
    }
  #end
  #if (flipit)
    scale <1, 1, -1>
  #end
  texture {chrometexture}
}
#end // bassring()
//----------------------------------------------------------------------

#local bassendcap = torus {
  bassradius - basscylinderthickness/2, basscylinderthickness/2
  rotate x*90
}

#macro bassskin(ringz)
cylinder {
  <0, bassyoffset, ringz>
  <0, bassyoffset, ringz-0.001>
  bassradius
  pigment { color Wheat transmit 0.7 }
}
#end

#macro bassleg(flipit)
union {
  cone {
    <0, 0, 0>,
    littlelegthick,
    <0, bassradius * 0.5, 0>,
    littlelegthick * 1.2
    texture{chrometexture}
  }
  sphere {
    <0, 0, 0>, littlelegthick*2
    texture{rubbertexture}
  }
  cone {
    <0, 0, 0>,
    littlelegthick * 2,
    <0, bassradius * 0.08, 0>,
    littlelegthick * 1.5
    texture{rubbertexture}
  }
  superellipsoid {
    <0.25, 0.07>
    scale <littlelegthick * 2.5, littlelegthick * 3, littlelegthick * 3.5>
    translate <0, bassradius * 0.48, 0>
    texture{chrometexture}
  }
  rotate 20*z
  translate <bassradius*1, littlelegthick * 2, -(basslength/2.8)>
  scale (flipit)
}
#end //bassleg

//----------------------------------------------------------------------
#declare bassdrum = union {
  // body, without skins and trim
  merge {
    difference {
      object {bassoutercylinder}
      cylinder {
	<0, bassyoffset, (-basslength/2)*1.001>,
	<0, bassyoffset, (+basslength/2)*1.001>, bassradius-basscylinderthickness
      }
    }
    object {
      bassendcap
      translate <0, bassyoffset, -(basslength/2-basscylinderthickness/2)>
    }
    object {
      bassendcap
      translate <0, bassyoffset, +(basslength/2-basscylinderthickness/2)>
    }
    texture { bodytexture }
  }
  // interior_texture 
  // Rings around the ends of the body
  bassring(0)
  bassring(1)
  // Skins
  bassskin(+(basslength/2-basscylinderthickness/2))
  bassskin(-(basslength/2-basscylinderthickness/2))
  // Legs
  bassleg(< 1,1, 1>)
  bassleg(<-1,1, 1>)
  
}
#undef bassring
#undef bassskin
#undef bassleg

