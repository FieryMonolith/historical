#ifndef __LIST_H__
#define __LIST_H__

template <class T> class List {
public:
  List(void)  { return; }
  virtual ~List(void) { return; }

  virtual void Insert(T *)=0;
  virtual void Push(T *)=0;
  virtual void Append(T *)=0;
  virtual void InsertBefore(T *)=0;
  virtual void InsertAfter(T *)=0;

  virtual T *Remove(T *)=0;
  virtual T *Remove(void)=0;
  virtual T *Pop(void)=0;
  virtual T *Chop(void)=0;

  virtual T *GetCurrentData(void)=0;
  virtual T *Next(void)=0;
  virtual T *NextLoop(void)=0;
  virtual T *Previous(void)=0;
  virtual T *PreviousLoop(void)=0;
  virtual T *Find(T *toFind)=0;

  virtual void Reset(void)=0;
  virtual bool IsEmpty(void)=0;
  virtual int Size(void)=0;
  virtual bool AtEnd(void)=0;
  virtual void Clear(void)=0;

  virtual T *operator[](int index)=0;
};

#endif
