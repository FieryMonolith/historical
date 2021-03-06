#ifndef __Support_Interface_OutputStream_h__
#define __Support_Interface_OutputStream_h__

#include <ostream>
#include <string>

class OutputStream
{
private:
  std::ostream *o;
  bool showState;

public:
  OutputStream(std::ostream *stream=0, bool show=true)
    : o(stream), showState(show)
  { return; }

  OutputStream(const OutputStream &om)
  { o = om.o; showState = om.showState; return; }

  ~OutputStream(void) { o = 0; showState = true; }

  const OutputStream &operator=(const OutputStream &om)
  { o = om.o; showState = om.showState; return *this; }

  /*** State information ****************************************************/
  const bool GetShowState(void) const       { return showState;          }
  void       SetShowState(const bool state) { showState = state; return; }

  std::ostream *GetStream(void) const           { return o;           }
  void          SetStream(std::ostream *stream) { o = stream; return; }

  /*** Output operators *****************************************************/
  OutputStream &operator<<(const std::string &s)
  { if(o != 0 && showState == true) { *o << s; } return *this; }

  OutputStream &operator<<(const char c)
  { if(o != 0 && showState == true) { *o << c; } return *this; }

  OutputStream &operator<<(const int i)
  { if(o != 0 && showState == true) { *o << i; } return *this; }

  OutputStream &operator<<(const unsigned int ui)
  { if(o != 0 && showState == true) { *o << ui; } return *this; }

  OutputStream &operator<<(const double d)
  { if(o != 0 && showState == true) { *o << d; } return *this; }

  OutputStream &operator<<(std::streambuf* sb )
  { if(o != 0 && showState == true) { *o << sb; } return *this; }
   
  OutputStream &operator<<(std::ostream& ( *pf )(std::ostream&))
  { if(o != 0 && showState == true) { *o << pf; } return *this; }

  OutputStream &operator<<(std::ios& ( *pf )(std::ios&))
  { if(o != 0 && showState == true) { *o << pf; } return *this; }

  OutputStream &operator<<(std::ios_base& ( *pf )(std::ios_base&))
  { if(o != 0 && showState == true) { *o << pf; } return *this; }

  OutputStream &operator<<(const std::ostream &pf)
  { if(o != 0 && showState == true) { *o << pf; } return *this; }
};

#endif // __Support_Interface_OutputStream_h__
