#include <fstream>

#include "Loader.h"

#include "Pixel.h"
#include "GraphicObject.h"

using namespace std;

void CleanUpObjects(ifstream &, vector<GraphicObject*> &data);

void LoadLayout(const string &filename,
                vector<GraphicObject*> &out,
                GraphicObject **objectOut)
{
  ifstream in(filename.c_str());
  if(!in) { throw 1; }

  while(!in.eof())
  {
    in >> ws;

    if(in.eof()) { return; }

    if(in.peek() == '#')
    {
      char buffer[4096];
      in.getline(buffer, 4096);
      continue;
    }

    string type;
    in >> type;

    if(type == "FILE")
    {
      char imageFilename[256];
      double x=0, y=0, z=0;

      in >> imageFilename;
      in >> x >> y >> z;

      if(!in.good())
      {
        in.clear();
        CleanUpObjects(in, out);
        throw 2;
      }

      GraphicObject *go=0;
      try
      {
        go = new GraphicObject(imageFilename, x, y, z);
        if(go == 0) { throw 0; }
      }
      catch(...) { CleanUpObjects(in, out); throw 3; }

      out.push_back(go);
    }
    else if(type == "TEXTURE_COLOR")
    {
      unsigned int red=0, green=0, blue=0, alpha=0;
      unsigned int width=0, height=0;
      string name;
      double x=0, y=0, z=0;

      in >> red >> green >> blue >> alpha;
      in >> width >> height;
      in >> name;
      in >> x >> y >> z;

      if(!in.good())
      {
        in.clear();
        CleanUpObjects(in, out);
        throw 4;
      }

      GraphicObject *go=0;
      try
      {
        go = new GraphicObject(RGBA(static_cast<unsigned char>(red),
                                    static_cast<unsigned char>(green),
                                    static_cast<unsigned char>(blue),
                                    static_cast<unsigned char>(alpha)),
                               width, height,
                               x, y, z);
        if(go == 0) { throw 0; }
      }
      catch(...) { CleanUpObjects(in, out); throw 3; }

      if(name == "DrawingSurface")
      {
        *objectOut = go;
      }

      out.push_back(go);
    }
    else
    {
      CleanUpObjects(in, out);
      throw 6;
    }
  }

  in.close();

  return;
}

void CleanUpObjects(ifstream &file, vector<GraphicObject*> &data)
{
  vector<GraphicObject*>::iterator i = data.end();
  for(i=data.begin(); i!=data.end(); ++i)
  {
    if(*i != 0) { delete *i; *i = 0; }
  }

  file.close();

  return;
}
