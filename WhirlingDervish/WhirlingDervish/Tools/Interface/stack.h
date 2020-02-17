#ifndef __STACK_H__
#define __STACK_H__

template <class T> class Stack {
public:
  Stack(void)  { return; }
  virtual ~Stack(void) { return; }

  virtual void Push(T *)=0;
  virtual T *Pop(void)=0;
  virtual bool IsEmpty(void)=0;
  virtual int Size(void)=0;
  virtual void Clear(void)=0;
};

#endif
