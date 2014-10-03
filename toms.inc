#declare shethick = 0.08;

#macro foopoo()

union {

#local innercyl = cylinder {
  <0, -1, 0>
  <0, 1, 0>,
  shethick
}

}

#end

