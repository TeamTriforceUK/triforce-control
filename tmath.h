#ifndef MATH_H
#define MATH_H

/******************************************************************************/

float map(float in, float inMin, float inMax, float outMin, float outMax) {
  // check it's within the range
  if (inMin<inMax) { 
    if (in <= inMin) 
      return outMin;
    if (in >= inMax)
      return outMax;
  } else {  // cope with input range being backwards.
    if (in >= inMin) 
      return outMin;
    if (in <= inMax)
      return outMax;
  }
  // calculate how far into the range we are
  float scale = (in-inMin)/(inMax-inMin);
  // calculate the output.
  return outMin + scale*(outMax-outMin);
}
    
float clamp(float d, float min, float max) {
  const float t = d < min ? min : d;
  return t > max ? max : t;
}

#define BETWEEN(value, min, max) (value < max && value > min)
/******************************************************************************/

#endif