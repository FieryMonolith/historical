#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <stdio.h>
#include <stdlib.h>
#include <qstring.h>

class Support {
public:
  static char *itoa(int integer);
  static char *utoa(unsigned int interger);
  static void Encode(QString &str);
  static void Decode(QString &str);
  static void RawEncode(unsigned char **string, long length, long *newLength);
  static void RawDecode(unsigned char *string, long length, long *newLength);

protected:
  Support() { return; }
  ~Support() { return; }
};

#endif
