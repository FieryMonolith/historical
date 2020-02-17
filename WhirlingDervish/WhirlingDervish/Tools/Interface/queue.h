#ifndef __QUEUE_H__
#define __QUEUE_H__

template <class T> class Queue {
public:
  Queue(void)  { return; }
  virtual ~Queue(void) { return; }

  virtual void Append(T *)=0;
  virtual T *Pop(void)=0;
  virtual bool IsEmpty(void)=0;
  virtual int Size(void)=0;
};

#endif
