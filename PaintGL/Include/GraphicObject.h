#ifndef __GraphicObject_h__
#define __GraphicObject_h__

class RGBA;
class Texture;

class GraphicObject
{
public:
  GraphicObject(const char *imageFilename,
                const double x=0, const double y=0, const double z=0)
    throw(int);
  GraphicObject(const RGBA &fillColor,
                const unsigned int width, const unsigned int height,
                const double x=0, const double y=0, const double z=0)
    throw(int);
  GraphicObject(const GraphicObject &) throw(int);
  ~GraphicObject(void) throw();

  GraphicObject &operator=(const GraphicObject &) throw(int);

  void Draw(void) const throw();

  bool Differentiate(void) throw(int);
  bool Clear(const RGBA &fillColor) throw(int);
  bool Load(const std::string &filename)         throw(int);
  bool SaveAs(const std::string &filename) const throw(int);

  void Move(const double x, const double y) throw();
  void Move(const double x, const double y, const double z) throw();
  void Move(const double z) throw();
  const double GetX(void) const throw();
  const double GetY(void) const throw();
  const double GetZ(void) const throw();

  const unsigned int GetWidth(void)  const throw();
  const unsigned int GetHeight(void) const throw();

  const bool SetDrawLocation(const double x, const double y) throw();
  const RGBA &GetPixel(void) const throw();
  void SetPixel(const RGBA &) throw();
  void Flush(void) throw(int);

private:
  struct Vertex
  {
    double x, y, z;

    Vertex(const double x=0, const double y=0, const double z=0) throw();
  };

private:
  Texture *d_texture;
  Vertex d_quad[4]; // 0 == LL, 1 == UL, 2 == UR, 3 == LR

  void Copy(const GraphicObject &) throw();
};

#endif // __GraphicObject_h__
