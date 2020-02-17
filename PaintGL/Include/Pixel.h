#ifndef __Pixel_h__
#define __Pixel_h__

class RGBA
{
public:
  unsigned char red, green, blue, alpha;

  RGBA(const unsigned char Red =0, const unsigned char Green=0,
       const unsigned char Blue=0, const unsigned char Alpha=0)
  : red(Red), green(Green), blue(Blue), alpha(Alpha)
  {
    return;
  }

};

#endif // __Pixel_h__
