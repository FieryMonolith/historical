#ifndef __SETTING_PARSER_H__
#define __SETTING_PARSER_H__

#include <string>
#include <istream>
#include <ostream>
#include <map>

class SettingParser {
public:
  SettingParser(void);
  ~SettingParser(void);

  class InputBase {
  public:
    InputBase(void) : d_count(1) { return; }
    virtual ~InputBase(void) { return; }
    virtual const std::string &Name(void) const=0;
    virtual void Data(std::ostream &) const=0;
    virtual bool GetData(std::istream &)=0;

    unsigned int &Count(void) { return d_count; }
    void Release(void) {
      if(d_count > 1) {
	d_count--;
      } else {
	delete this;
      }
      return;
    }

  protected:
    unsigned int d_count;
  };

  template <typename RETURN, typename CLASS>
  class Input : public InputBase {
  public:
    Input(const std::string &name, CLASS *c, RETURN (CLASS::*f)(void))
      : d_name(name), d_class(c), d_callback(f) {
      return;
    }
    ~Input(void) { return; }

    const std::string &Name(void) const { return d_name; }
    void Data(std::ostream &) const;
    bool GetData(std::istream &);

  private:
    const std::string &d_name;
    CLASS *d_class;
    RETURN (CLASS::*d_callback)(void);
  };

  static bool ParseStream(std::istream &,
			  const std::map<std::string, InputBase *> &);
  static bool ParseVariable(std::istream &, const std::map<std::string,
			    InputBase *> &);

private:
};

template <typename T, typename U>
void SettingParser::Input<T,U>::Data(std::ostream &out) const {
  out << (d_class->*d_callback)();

  return;
}

template <typename T, typename U>
bool SettingParser::Input<T,U>::GetData(std::istream &in) {
  if(in.eof()) { return false; }
  in >> (d_class->*d_callback)();
  if(in.good()) { return true; }
  in.clear();
  return false;
}

#endif
