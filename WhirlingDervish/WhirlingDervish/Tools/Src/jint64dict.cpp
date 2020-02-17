#include "jint64dict.h"

#include "qptrlist.h"
#include <ctype.h>
#include <string.h>

static const int op_find = 0;
static const int op_insert = 1;
static const int op_replace = 2;


class JGDItList : public QPtrList<JGDictIterator> {
public:
    JGDItList() : QPtrList<JGDictIterator>() {}
    JGDItList( const JGDItList &list ) : QPtrList<JGDictIterator>(list) {}
    ~JGDItList() { clear(); }
    JGDItList &operator=(const JGDItList &list)
	{ return (JGDItList&)QPtrList<JGDictIterator>::operator=(list); }
};


/*****************************************************************************
  Default implementation of special and virtual functions
 *****************************************************************************/

/*!
  Constructs a dictionary.

  \a len is the initial size of the dictionary.
  The key type is \a kt which may be \c StringKey, \c AsciiKey,
  \c IntKey or \c PtrKey. The case-sensitivity of lookups is set with
  \a caseSensitive. Keys are copied if \a copyKeys is TRUE.
*/

JGDict::JGDict( unsigned int len, KeyType kt, bool caseSensitive, 
		bool copyKeys ) {
  init( len, kt, caseSensitive, copyKeys );

  return;
}

void JGDict::init( unsigned int len, KeyType kt, bool caseSensitive, 
		   bool copyKeys ) {
    vec = new JBaseBucket *[vlen = len]; // allocate hash table
    Q_CHECK_PTR( vec );
    memset( (char*)vec, 0, vlen*sizeof(JBaseBucket*) );
    numItems  = 0;
    iterators = 0;
    // The caseSensitive and copyKey options don't make sense for
    // all dict types.
    switch ( (keytype = (unsigned int)kt) ) {
    default:
      cases = FALSE;
      copyk = FALSE;
      break;
    }
}

/*!
  Constructs a copy of \a dict.
*/

JGDict::JGDict( const JGDict & dict ) : JPtrCollection( dict ) {
    init( dict.vlen, (KeyType)dict.keytype, dict.cases, dict.copyk );
    JGDictIterator it( dict );
    while ( it.get() ) {			// copy from other dict
	switch ( keytype ) {
	case Int64Key:
	  look_int64( it.getKeyInt64(), it.get(), op_insert );
	  break;
	}
	++it;
    }
}


/*!
  Removes all items from the dictionary and destroys it.
*/

JGDict::~JGDict()
{
    clear();					// delete everything
    delete [] vec;
    if ( !iterators )				// no iterators for this dict
	return;
    JGDictIterator *i = iterators->first();
    while ( i ) {				// notify all iterators that
	i->dict = 0;				// this dict is deleted
	i = iterators->next();
    }
    delete iterators;
}


/*!
  Assigns \a dict to this dictionary.
*/

JGDict &JGDict::operator=( const JGDict &dict )
{
    if ( &dict == this )
	return *this;
    clear();
    JGDictIterator it( dict );
    while ( it.get() ) {			// copy from other dict
	switch ( keytype ) {
	case Int64Key:
	  look_int64( it.getKeyInt64(), it.get(), op_insert );
	  break;
	}
	++it;
    }
    return *this;
}

/*!
  \fn unsigned int JGDict::count() const

  Returns the number of items in the dictionary.
*/

/*!
  \fn unsigned int JGDict::size() const

  Returns the size of the hash array.
*/

/*!
  The do-it-all function; \a op is one of op_find, op_insert, op_replace.
  The key is \a key and the item is \a d.
*/

JPtrCollection::Item JGDict::look_int64(__int64 key, JPtrCollection::Item d, 
					int op ) {
    JInt64Bucket *n;
    unsigned int index = (unsigned int)(key % (__int64)vlen); // simple hash
    if ( op == op_find ) {			// find
	for( n=(JInt64Bucket*)vec[index]; n; n=(JInt64Bucket*)n->getNext()) {
	  if( n->getKey() == key ) {
	    return n->getData();		// item found
	  }
	}
	return 0;				// not found
    }
    if ( op == op_replace ) {			// replace
	if ( vec[index] != 0 )			// maybe something there
	    remove_int64( key );
    }
    // op_insert or op_replace
    n = new JInt64Bucket(key,newItem(d),vec[index]);
    Q_CHECK_PTR( n );
#if defined(QT_CHECK_NULL)
    if ( n->getData() == 0 )
	qWarning( "QIntDict: Cannot insert null item" );
#endif
    vec[index] = n;
    numItems++;
    return n->getData();
}

/*!
  Changes the size of the hashtable to \a newsize.
  The contents of the dictionary are preserved,
  but all iterators on the dictionary become invalid.
*/
void JGDict::resize( unsigned int newsize )
{
    // Save old information
    JBaseBucket **old_vec = vec;
    unsigned int old_vlen  = vlen;
    bool old_copyk = copyk;

    vec = new JBaseBucket *[vlen = newsize];
    Q_CHECK_PTR( vec );
    memset( (char*)vec, 0, vlen*sizeof(JBaseBucket*) );
    numItems = 0;
    copyk = FALSE;

    // Reinsert every item from vec, deleting vec as we go
    for ( unsigned int index = 0; index < old_vlen; index++ ) {
	switch ( keytype ) {
	case Int64Key:
	  {
	    JInt64Bucket *n=(JInt64Bucket *)old_vec[index];
	    while ( n ) {
	      look_int64( n->getKey(), n->getData(), op_insert );
	      JInt64Bucket *t=(JInt64Bucket *)n->getNext();
	      delete n;
	      n = t;
	    }
	  }
	  break;
	}
    }
    delete [] old_vec;

    // Restore state
    copyk = old_copyk;

    // Invalidate all iterators, since order is lost
    if ( iterators && iterators->count() ) {
	JGDictIterator *i = iterators->first();
	while ( i ) {
	    i->toFirst();
	    i = iterators->next();
	}
    }
}

/*!
  Unlinks the bucket with the specified key (and specified data pointer,
  if it is set).
*/

void JGDict::unlink_common( unsigned int index, JBaseBucket *node, 
			    JBaseBucket *prev ){
    if ( iterators && iterators->count() ) {	// update iterators
	JGDictIterator *i = iterators->first();
	while ( i ) {				// invalidate all iterators
	    if ( i->curNode == node )		// referring to pending node
		i->operator++();
	    i = iterators->next();
	}
    }
    if ( prev )					// unlink node
	prev->setNext( node->getNext() );
    else
	vec[index] = node->getNext();
    numItems--;
}

JInt64Bucket *JGDict::unlink_int64( __int64 key, JPtrCollection::Item d ) {
    if ( numItems == 0 )			// nothing in dictionary
	return 0;
    JInt64Bucket *n;
    JInt64Bucket *prev = 0;
    unsigned int index = (unsigned int)(key % (__int64)vlen);
    for ( n=(JInt64Bucket *)vec[index]; n; n=(JInt64Bucket *)n->getNext() ) {
	bool found = (n->getKey() == key);
	if ( found && d )
	    found = (n->getData() == d);
	if ( found ) {
	    unlink_common(index,n,prev);
	    return n;
	}
	prev = n;
    }
    return 0;
}

/*!
  Removes the item with the specified \a key.  If \a item is not null,
  the remove will match the \a item as well (used to remove an
  item when several items have the same key).
*/

bool JGDict::remove_int64( __int64 key, JPtrCollection::Item item ) {
    JInt64Bucket *n = unlink_int64( key, item );
    if ( n ) {
	deleteItem( n->getData() );
	delete n;
    }
    return n != 0;
}

JPtrCollection::Item JGDict::take_int64( __int64 key ) {
    JInt64Bucket *n = unlink_int64( key );
    Item d;
    if ( n ) {
	d = n->getData();
	delete n;
    } else {
	d = 0;
    }
    return d;
}

/*!
  Removes all items from the dictionary.
*/
void JGDict::clear() {
    if ( !numItems )
	return;
    numItems = 0;				// disable remove() function
    for ( unsigned int j=0; j<vlen; j++ ) {		// destroy hash table
	if ( vec[j] ) {
	    switch ( keytype ) {
	    case Int64Key:
	      {
		JInt64Bucket *n=(JInt64Bucket *)vec[j];
		while ( n ) {
		  JInt64Bucket *next = (JInt64Bucket*)n->getNext();
		  deleteItem( n->getData() );
		  delete n;
		  n = next;
		}
	      }
	      break;
	    }
	    vec[j] = 0;				// detach list of buckets
	}
    }
    if ( iterators && iterators->count() ) {	// invalidate all iterators
	JGDictIterator *i = iterators->first();
	while ( i ) {
	    i->curNode = 0;
	    i = iterators->next();
	}
    }
}

JGDictIterator::JGDictIterator( const JGDict &d ) {
    dict = (JGDict *)&d;			// get reference to dict
    toFirst();					// set to first noe
    if ( !dict->iterators ) {
	dict->iterators = new JGDItList;	// create iterator list
	Q_CHECK_PTR( dict->iterators );
    }
    dict->iterators->append( this );		// attach iterator to dict
}

/*!
  Constructs a copy of the iterator \a it.
*/

JGDictIterator::JGDictIterator( const JGDictIterator &it ) {
    dict = it.dict;
    curNode = it.curNode;
    curIndex = it.curIndex;
    if ( dict )
	dict->iterators->append( this );	// attach iterator to dict
}

/*!
  Assigns a copy of the iterator \a it and returns a reference to this
  iterator.
*/

JGDictIterator &JGDictIterator::operator=( const JGDictIterator &it ) {
    if ( dict )					// detach from old dict
	dict->iterators->removeRef( this );
    dict = it.dict;
    curNode = it.curNode;
    curIndex = it.curIndex;
    if ( dict )
	dict->iterators->append( this );	// attach to new list
    return *this;
}

/*!
  Destroys the iterator.
*/

JGDictIterator::~JGDictIterator() {
    if ( dict )					// detach iterator from dict
	dict->iterators->removeRef( this );
}

/*!
  Sets the iterator to point to the first item in the dictionary.
*/

JPtrCollection::Item JGDictIterator::toFirst()
{
    if ( !dict ) {
#if defined(QT_CHECK_NULL)
	qWarning( "JGDictIterator::toFirst: Dictionary has been deleted" );
#endif
	return 0;
    }
    if ( dict->count() == 0 ) {			// empty dictionary
	curNode = 0;
	return 0;
    }
    register unsigned int i = 0;
    register JBaseBucket **v = dict->vec;
    while ( !(*v++) )
	i++;
    curNode = dict->vec[i];
    curIndex = i;
    return curNode->getData();
}

/*!
  Moves to the next item (postfix).
*/

JPtrCollection::Item JGDictIterator::operator()() {
    if ( !dict ) {
#if defined(QT_CHECK_NULL)
	qWarning( "JGDictIterator::operator(): Dictionary has been deleted" );
#endif
	return 0;
    }
    if ( !curNode )
	return 0;
    JPtrCollection::Item d = curNode->getData();
    this->operator++();
    return d;
}

/*!
  Moves to the next item (prefix).
*/

JPtrCollection::Item JGDictIterator::operator++() {
    if ( !dict ) {
#if defined(QT_CHECK_NULL)
	qWarning( "JGDictIterator::operator++: Dictionary has been deleted" );
#endif
	return 0;
    }
    if ( !curNode )
	return 0;
    curNode = curNode->getNext();
    if ( !curNode ) {				// no next bucket
	register unsigned int i = curIndex + 1;	// look from next vec element
	register JBaseBucket **v = &dict->vec[i];
	while ( i < dict->size() && !(*v++) )
	    i++;
	if ( i == dict->size() ) {		// nothing found
	    curNode = 0;
	    return 0;
	}
	curNode = dict->vec[i];
	curIndex = i;
    }
    return curNode->getData();
}

/*!
  Moves \a jumps positions forward.
*/

JPtrCollection::Item JGDictIterator::operator+=( unsigned int jumps ) {
    while ( curNode && jumps-- )
	operator++();
    return curNode ? curNode->getData() : 0;
}
