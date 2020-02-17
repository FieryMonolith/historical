#ifndef __JInt64Dict_H__
#define __JInt64Dict_H__

#include "defs.h"

class JGDictIterator;
class JGDItList;

class JPtrCollection {
public:
    bool autoDelete() const	       { return del_item; }
    void setAutoDelete( bool enable )  { del_item = enable; }

    virtual unsigned int  count() const = 0;
    virtual void  clear() = 0;			// delete all objects

    typedef void *Item;				// generic collection item

protected:
    JPtrCollection() { del_item = false; }	// no deletion of objects
    JPtrCollection(const JPtrCollection &) { del_item = false; }
    virtual ~JPtrCollection() {}

    bool del_item;				// default false

    virtual Item     newItem( Item d ) { return d; } // create object
    virtual void     deleteItem( Item ) = 0;	// delete object
};

class JBaseBucket {
public:
    JPtrCollection::Item getData() { return data; }
    JPtrCollection::Item setData( JPtrCollection::Item d ) { return data = d; }
    JBaseBucket		*getNext()			   { return next; }
    void		 setNext( JBaseBucket *n)	   { next = n; }

protected:
    JBaseBucket( JPtrCollection::Item d, JBaseBucket *n ) : data(d), next(n) {}
    JPtrCollection::Item data;
    JBaseBucket		*next;
};

class JInt64Bucket : public JBaseBucket {
public:
    JInt64Bucket( __int64 k, JPtrCollection::Item d, JBaseBucket *n )
	: JBaseBucket(d,n), key(k) {}
    __int64 getKey() const { return key; }

private:
    __int64 key;
};

class JGDict : public JPtrCollection {
public:
    unsigned int count() const { return numItems; }
    unsigned int size()	const  { return vlen; }

    JPtrCollection::Item look_int64( __int64 key, JPtrCollection::Item, int );

protected:
    enum KeyType { Int64Key };

    JGDict( unsigned int len, KeyType kt, bool cs, bool ck );
    JGDict( const JGDict & );
   ~JGDict();

    JGDict     &operator=( const JGDict & );

    bool remove_int64( __int64 key, JPtrCollection::Item item=0 );
    JPtrCollection::Item take_int64( __int64 key );

    void clear();
    void resize( unsigned int );

private:
    JBaseBucket **vec;
    unsigned int vlen;
    unsigned int numItems;
    unsigned int keytype : 2;
    unsigned int cases	 : 1;
    unsigned int copyk	 : 1;
    JGDItList *iterators;
    void	unlink_common( unsigned int, JBaseBucket *, JBaseBucket * );
    JInt64Bucket *unlink_int64( __int64, JPtrCollection::Item item = 0 );
    void	init( unsigned int, KeyType, bool, bool );
    friend class JGDictIterator;
};

class JGDictIterator {
friend class JGDict;
public:
    JGDictIterator( const JGDict & );
    JGDictIterator( const JGDictIterator & );
    JGDictIterator &operator=( const JGDictIterator & );
   ~JGDictIterator();

    JPtrCollection::Item toFirst();

    JPtrCollection::Item get() const;
    __int64 getKeyInt64()      const;

    JPtrCollection::Item operator()();
    JPtrCollection::Item operator++();
    JPtrCollection::Item operator+=(unsigned int);

protected:
    JGDict *dict;

private:
    JBaseBucket *curNode;
    unsigned int curIndex;
};

inline JPtrCollection::Item JGDictIterator::get() const {
    return curNode ? curNode->getData() : 0;
}

inline __int64 JGDictIterator::getKeyInt64() const {
    return curNode ? ((JInt64Bucket*)curNode)->getKey() : 0;
}

template<class type>
class JInt64Dict : public JGDict {
public:
  JInt64Dict(unsigned int size=17) : JGDict(size,Int64Key,0,0) {}
  JInt64Dict( const JInt64Dict<type> &d ) : JGDict(d) {}
  ~JInt64Dict()	{ clear(); }

  JInt64Dict<type> &operator=(const JInt64Dict<type> &d)
    { return (JInt64Dict<type>&)JGDict::operator=(d); }

  unsigned int  count()   const { return JGDict::count(); }
  unsigned int  size()    const { return JGDict::size(); }
  bool  isEmpty() const { return JGDict::count() == 0; }

  void  insert( __int64 k, const type *d ) { JGDict::look_int64(k,(Item)d,1); }
  void  replace(__int64 k, const type *d ){ JGDict::look_int64(k,(Item)d,2); }
  bool  remove( __int64 k )		   { return JGDict::remove_int64(k);  }
  type *take(   __int64 k )	       { return (type*)JGDict::take_int64(k); }
  type *find(   __int64 k ) const 
    { return (type *)((JGDict*)this)->JGDict::look_int64(k,0,0); }
  type *operator[]( __int64 k ) const
    { return (type *)((JGDict*)this)->JGDict::look_int64(k,0,0); }
  void  clear()          { JGDict::clear(); }
  void  resize( unsigned int n ) { JGDict::resize(n); }

private:
  void  deleteItem( Item d );
};

template<class type> inline void JInt64Dict<type>::deleteItem( 
					       JPtrCollection::Item d ) {
  if ( del_item ) delete (type*)d;
}

template<class type>
class JInt64DictIterator : public JGDictIterator {
public:
  JInt64DictIterator(const JInt64Dict<type> &d) 
    : JGDictIterator((JGDict &)d) {}
  ~JInt64DictIterator()	    {}
  unsigned int  count()   const     { return dict->count(); }
  bool  isEmpty() const     { return dict->count() == 0; }
  type *toFirst()	    { return (type *)JGDictIterator::toFirst(); }
  operator type *()  const  { return (type *)JGDictIterator::get(); }
  type *current()    const  { return (type *)JGDictIterator::get(); }
  __int64 currentKey() const{ return JGDictIterator::getKeyInt64(); }
  type *operator()()	    { return (type *)JGDictIterator::operator()(); }
  type *operator++()	    { return (type *)JGDictIterator::operator++(); }
  type *operator+=(unsigned int j)  
    { return (type *)JGDictIterator::operator+=(j);}
};

#endif
