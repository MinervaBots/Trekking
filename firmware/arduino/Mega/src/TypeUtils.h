#ifndef TYPE_UTILS_H
#define TYPE_UTILS_H

float mapf(float value, float istart, float istop, float ostart, float ostop)
{
  return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

#endif
