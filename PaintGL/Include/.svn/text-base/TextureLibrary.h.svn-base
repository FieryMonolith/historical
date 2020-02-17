#ifndef __TextureLibrary_h__
#define __TextureLibrary_h__

#include <string>
#include <map>
#include <set>

class RGBA;
class Texture;
class TextureData;

// Only DeleteTexture method checks for a valid TextureData before proceeding.
//   If for some reason the TextureData was deleted and it should not be
//   then there are other issues that need to be fixed.  The check for
//   a valid data for deletion is due to the cleanup code for the library.
//   When the library is deleted the code deletes all the TextureData but
//   is unable to update the Textures.  So when they ask to delete their
//   data but it is already deleted then nothing happens, it just continues
//   on with the rest of the program cleanup.  I don't know how often that
//   would occur anyways.

class TextureLibrary
{
public:
  TextureLibrary(void);
  ~TextureLibrary(void);

  void         ClearTexture(TextureData &, const RGBA &fillColor) const throw(int);
  TextureData *CreateTexture(const RGBA &fillColor,
                             const unsigned int width,
                             const unsigned int height) throw(int);
  TextureData *CopyTexture(TextureData &) throw();
  void         DeleteTexture(TextureData &) throw();
  TextureData *DifferentiateTexture(TextureData &) throw(int);
  TextureData *LoadTexture(const std::string &imageFilename) throw(int);
  void         SaveTexture(const std::string &filename, TextureData &) const throw(int);
  void         UpdateTexture(TextureData &) throw(int);

private:
  RGBA *GetBuffer(const unsigned int width, const unsigned int height) throw(int);
  unsigned int NextPowerOf2(unsigned int) const; // 0 == 0

private:
  std::map<Texture*, std::string> d_textureToFilename;
  std::map<Texture*, std::string> d_textureToName;
  std::map<std::string, Texture*> d_nameToTexture;

  unsigned int d_bufferSize;
  RGBA *d_buffer;

  unsigned int d_nonameId;

private:
  // Prevent the library from being copied.
  TextureLibrary(const TextureLibrary &);
  TextureLibrary &operator=(const TextureLibrary &);
}; // class TextureLibrary

// Define a global texture library.
extern TextureLibrary gTextureLibrary;

#endif // __TexutreLibrary_h__
