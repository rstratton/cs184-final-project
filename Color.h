//
//  Color.h
//  final
//
//  Created by Kavan Sikand on 11/21/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#ifndef final_Color_h
#define final_Color_h
#include <math.h>



struct Color {
  float r;
  float g;
  float b;
  float a;
  Color (float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha) {};
  Color (float red, float green, float blue) : r(red), g(green), b(blue), a(1) {};
  Color () : Color(0,0,0,1) {};
  Color operator + (Color c2) {
    return Color(r+c2.r, g + c2.g, b + c2.b, fmin(1.,a + c2.a));
  }
};
#endif
