#ifndef __Texture_h__
#define __Texture_h__

/*

The current iteration of PaintGL treats all textures as two dimensional 
images that are applied and read from primitives.  Essentially, the texture 
holds a picture.  To conserve space, when two textures use the same image 
they share that texture rather than each having its own copy.  This is 
accomplished through reference counting.  Only the data that applies 
directly to the image itself such as dimensions and actual color information 
are shared.  To allow for flexibility, modification of a texture applies to 
all textures that share that data.  Because this is not desirable in all 
circumstances a branch method is added that will force a copy of the data
to be replicated and stored essentially unlinking the texture with the others 
that share it.

Textures are intended to never be invalid or empty.  A texture must always 
be associated with data.  I try to ensure this by not allowing a default
constructor, throwing exceptions when creation fails, and not destroying 
data until the new data is successfully created.

 */

#include <string>

#include "RefCount.h"

class RGBA;
class TextureData;

class Texture
{
public:
  /*** Begin Constructor/Destructor/Assignment *******************************/
  // The constructors and destructor load or unload an opengl texture
  //   if the same one is not already loaded.  Otherwise, it uses the already 
  //   loaded texture.  This occurs due to the reference counting done with 
  //   the textures.

  // This method loads an image from file as the texture.
  Texture(const std::string &filename) throw(int);
  // Creates an image as a texture with the given dimensions and filled
  //   with the supplied color.
  Texture(const RGBA &fillColor,
          const unsigned int width, const unsigned int height) throw(int);
  // Creates a new texture using the given texture to copy from.
  // *Note: The current implementation is referenced counted so no new
  //   image data is created, it instead references the already created
  //   texture information.
  Texture(const Texture &) throw();
  // This function copies the given texture data and information into 
  //   this texture.
  // *Note: The current implementation is referenced counted so no new
  //   image data is created, it instead references the already created
  //   image and texture information.  Deletion of the old texture occurs
  //   but follows the procedure of reference counting.
  Texture &operator=(const Texture &) throw();
  // Destroys the texture and unloads it from opengl.
  // If an error occurs, the function does its best to recover and finish
  //   cleaning up.  If it can not clean something up it is ignored and
  //   the destruction finishes.  It does not throw an exception.
  // *Note: The current implementation is referenced counted so the texture
  //   is only unloaded and deleted if the texture is not shared by others.
  //   If the texture is in use by another texture, it is not actually
  //   unloaded from opengl nor is the texture memory freed.
  ~Texture(void) throw();
  /*** End Constructor/Destructor/Assignment *********************************/


  /*** Begin Get/Set Texture Information *************************************/
  // Returns the identifier used by OpenGL to determine the correct texture.
  unsigned int GetGLTextureName(void) const throw();
  // The pixel width and height of the texture
  unsigned int GetWidth(void)  const throw();
  unsigned int GetHeight(void) const throw();
  // The percentage width and height of the texture relative to the true 
  //   power of two width of height used internally by OpenGL.
  double GetWidthRatio(void)  const throw();
  double GetHeightRatio(void) const throw();
  // Returns the RGBA color value at the specified location.
  const RGBA &GetPixel(const unsigned int x, const unsigned int y) const
    throw(int);
  // Returns the color of the pixel at the current draw position.
  const RGBA &GetPixel(void) const throw();
  // This stores a (x,y) position that is used with SetPixel to change
  //   the pixel color at that position.
  // This decouples the changing of a pixel into the setting of the position
  //   and the changing of the color.  While this adds extra information, it
  //   standardizes the interface and make that interface more usable.  The
  //   reason is error checking and recovery.
  void SetDrawLocation(const unsigned int x, const unsigned int y)
    throw(int);
  /*** End Get/Set Texture Information *************************************/


  /*** Begin Texture Modifiers ***********************************************/
  // This method tells the texture to decouple from all the other textures 
  //   sharing this information.  If this does not occur and the texture is 
  //   modified, all textures sharing this information will be modified.  If 
  //   there are no textures sharing this information nothing happens.  Also,
  //   if an exception is thrown nothing happens to the internal data.
  void Differentiate(void) throw(int);
  // Loads an image file and modifies the current texture data with the new
  //   information. This method flushes the texture change.  Nothing changes
  //   if an exception occurs during load.
  void Load(const std::string &filename) throw(int);
  // Changes the size of the texture and fills any new areas with the fill
  //   color.  This method flushes the texture change.  Nothing changes if an 
  //   exception occurs during creation of a new texture.
  void Resize(const unsigned int width, const unsigned int height,
              const RGBA &fillColor) throw(int);
  // Clears the image using the given color.  This method flushes the texture
  //   change.
  void Clear(const RGBA &fillColor) throw(int);
  // This changes the pixel color at the current pixel location.  It does not
  //   change the opengl texture.
  void SetPixel(const RGBA &) throw();
  // Flush causes changes done to the texture to propogate to the opengl
  //   texture.
  void Flush(void) throw(int);
  /*** End Texture Modifiers *************************************************/


  /*** Begin Texture Output **************************************************/
  // Save the texture using the given filename
  void SaveAs(const std::string &filename) const throw(int);
  /*** End Texture Output ****************************************************/

private:
  // Texture data is created elsewhere because it is managed by another
  //   data structure that utilizes reference counting.
  RefCount<TextureData> d_data;
  // Data for managing changes to the texture
  // Define the minimal rectangle that needs to be copied to update the 
  //   texture after changes have occurred.
  unsigned int d_minX, d_minY, d_maxX, d_maxY;
  // Pixel position for changing color
  unsigned int d_drawX, d_drawY;
  bool d_changed;

private:
  /*** Begin Helper Methods **************************************************/
  TextureData *Copy(void) const;
  TextureData *CreateTexture(const RGBA &fillColor,
                             const unsigned int width,
                             const unsigned int height);
  TextureData *LoadFromFile(const std::string &imageFilename);
  unsigned int NextPowerOf2(unsigned int value) const;
  /*** End Helper Methods ****************************************************/

private:
  // Can not allow the creation of a texture with no information
  Texture(void);
}; // class Texture

#endif // __Texture_h__
