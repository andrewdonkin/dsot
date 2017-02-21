// Illustrates peculiar behaviour of cylindrical lights in media.

camera {
  location <0, 0.5, -1> look_at y/3
}
plane {-z, -1 pigment {rgb 1}}   // Backdrop
plane { y,  0 pigment {rgb 1}}   // Floor
// Illuminate the background so that the black discs at the ends
// off the cylindrical lights are more obvious
light_source {<10, 10 -10> rgb 0.3}
// Blue light on left, aimed right, striking the floor:
light_source {
  <-0.5, 0.4, 0>
  colour rgb <0, 0, 5>
  cylinder
  parallel
  radius 0.05
  falloff 0.10
  point_at <0.3, 0, -0.1>
}
// Red light aimed nearly at the camera
light_source {
  <0.1, 0.6, 0.5>
  colour rgb <5, 0, 0>
  cylinder
  radius 0.03
  falloff 0.07
  point_at <-0.05, 0.35, -1>
}
media { scattering { 2, rgb 1/2 extinction 0} }
