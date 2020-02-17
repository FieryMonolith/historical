#ifndef __SUPPORT_H__
#include "support.h"
#endif

char *Support::itoa(int integer) {
  int isNegative = 0;
  unsigned int value;

  if(integer < 0) {
    isNegative++;
    value = -1 * integer;
  } else {
    value = integer;
  }

  char *signedNumber;
  char *number = utoa(value);

  if(isNegative) {
    signedNumber = new char[strlen(number)+2];
    if(signedNumber == NULL) { return NULL; }
    signedNumber[0] = '-';
    signedNumber[1] = '\0';
  } else {
    signedNumber = new char[strlen(number)+1];
    if(signedNumber == NULL) { return NULL; }
    signedNumber[0] = '\0';
  }
  strcat(signedNumber, number);
  delete number;

  return signedNumber;
}

char *Support::utoa(unsigned int integer) {
  int value = integer;
  char temp[512];
  temp[0] = '0';
  temp[1] = '\0';

  int index = 0;
  while(value != 0 && index < 511) {
    int remainder = value % 10;
    temp[index] = '0' + remainder;
    index++;
    temp[index] = '\0';

    value /= 10;
  }
  if(index == 0) {
    index++;
  }

  char *number = new char[strlen(temp)+1];
  if(number == NULL) { return NULL; }

  int numberIndex = 0;
  index--;
  while(index >= 0) {
    number[numberIndex] = temp[index];
    numberIndex++;
    index--;
  }
  number[numberIndex] = '\0';

  return number;
}

void Support::Encode(QString &str) {
  str.replace('=', "&equ;");

  return;
}

void Support::Decode(QString &str) {
  QString temp("=");
  str.replace("&equ;", temp);

  return;
}

void Support::RawEncode(unsigned char **string, long length, long *newLength) {
  if(string == NULL) { return; }

  long ff = 0;
  long extra = 0;
  for(int i=0; i<length; i++) {
    unsigned char a = (*string)[i];
    if(a == 0xFF) {
      ff++;
    } else if(ff) {
      ff = 0;
      extra += 2;
    }
    if(a == '\n') {
      extra++;
    }
  }
  // Ignore FFs that go to the end of the string

  *newLength = length + extra;
  unsigned char *newString = new unsigned char[*newLength];

  ff=0;
  long index = 0;
  for(int i=0; i<length; i++) {
    unsigned char a = (*string)[i];
    if(a == 0xFF) {
      ff++;
    } else if(ff) {
      ff = 0;
      newString[index] = 0xFF;
      index++;
      newString[index] = 0x00;
      index++;
    }
    if(a == '\n') {
      newString[index] = 0xFF;
      index++;
      newString[index] = 0x00;
      index++;
    } else {
      newString[index] = a;
      index++;
    }
  }

  delete *string;
  *string = newString;

  return;
}

void Support::RawDecode(unsigned char *string, long length, long *newLength) {
  // String will only decrease
  long index = 0;
  int ff=0;
  for(int i=0; i<length; i++) {
    unsigned char a = string[i];
    if(!ff && a == 0xFF && i+1 < length && string[i+1] == 0x00) {
      string[index] = '\n';
      index++;
      // extra ++ for the 0x00
      i++;
      continue;
    } else if(ff && a == 0xFF && i+1 < length && string[i+1] == 0x00) {
      i++;
      // skip the last 0xff and 0x00
      continue;
    } else if(a == 0xFF) {
      ff++;
    }
    string[index] = a;
    index++;
  }

  *newLength = index;

  return;
}
