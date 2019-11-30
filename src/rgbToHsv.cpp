#ifndef RGBTOHSV
#define RGBTOHSV

#define maximum(x, y, z) ((x) > (y)? ((x) > (z)? (x) : (z)) : ((y) > (z)? (y) : (z)))
#define minimum(x, y, z) ((x) < (y)? ((x) < (z)? (x) : (z)) : ((y) < (z)? (y) : (z)))

void RGBtoHSV(int r,int g,int b,double &h,double &s,double &v) {
  double red, green, blue;
  double max, min, delta;

  red = r / 255.0; green = g / 255.0; blue = b / 255.0; /*r , g, b to 0−1 scale*/

  max = maximum(red, green, blue);
  min = minimum(red, green, blue);

  v = max;  /*value is maximum of r , g, b*/

  if (max == 0) {
    s = 0;
    h = 0;
  } else {
    s = (max - min) / max;

    delta = max - min;
    if(delta == 0) {
      h = 0;
    }
    else {
      if(red == max) {  /*otherwise , determine hue on scale 0−360*/
        h = (green - blue) / delta;
      }
      else if(green == max) {
        h = 2.0 + (blue - red) / delta;
      }
      else{ /*(blue == max)*/
        h = 4.0 + (red - green) / delta;
      }
      h = h * 60.0;
      if(h < 0) {
        h = h + 360.0;
      }
    }
  }
}

#endif
