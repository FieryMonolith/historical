/*

 * Copyright 2006 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * LICENSE information at the bottom of this file or the copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __Comm_Interface_Connection_h__
#define __Comm_Interface_Connection_h__

/*!
 *
 * \class Connection
 *
 * \ingroup nComm
 *
 * Connection- Connection utilizes the Functor subsystem in an attempt to
 *   replicate the QT signal/slot functionality without the use of moc.  The
 *   method chosen was to rely on managing code to manage the creation and
 *   destruction between a signal(OneToMany functor) and a slot (any callable
 *   entity).  The Connection class includes three types of calls: 1) Create
 *   adds a callable entity to the OneToMany chain, 2) Break removes a specific
 *   slot from a specific signal, and 3) removes all the signals connected to
 *   a slot and vice versa.  The primary difference between this method and
 *   the QT method is that the signals and slots do NOT disconnect themselves
 *   upon destruction.  To do so would require the creation of near identical
 *   functionality to the functors and reduces the flexibility of the method
 *   at least on the part of the slots who may be a simple function with no
 *   ability to control itself unlike a callable class.
 *
 * None of these functions require the return type.  That is because each
 *   callable entity is stored in a ReturnlessFunctor which essentially
 *   strips off the return value.
 *
 * The working of this system relies on the ability to identify signals and
 *   slots.  To do this I utilize the void* as a sort of identifier.  By
 *   casting each incoming item to a void* should give you a unique value for
 *   pointers and references under the assumption that what they refer to
 *   remain valid while they are connected.  However, I am not sure if this
 *   assumption holds for entities passed by value, since it could be feasible
 *   that the compiler would reuse temporary storage on multiple calls.
 *   While I could have prevented the use of entities by value, I thought I
 *   would leave them in on the off chance someone has a use for them I have
 *   not thought of.  Also, it means that pass by value entites can not
 *   be disconnected except by doing a BreakAll on a signal since it is not
 *   possible to look them up as their ids would not be valid.
 *
 * Also, I am not certain what happens on every system if the static data is
 *   not disconnected.  I assume from what I have learned before that the
 *   system will clean up any allocated data a program does not clean up itself.
 *   The reason for this issue is that each connection creates new instances
 *   of Returnless functors and stores them in a static list composed of
 *   void pointers.  This means that any given piece of data can not be
 *   deleted by an unknowing third party since you can not delete a void *
 *   and still call the original destructor.  Though there is a chance that
 *   I could rethink the void* thing for the storage of Returnless types and
 *   store them as iFunctors, thus making it possible to delete them.
 *   Otherwise, I would have to make the Connection class templated and each
 *   parameter list signature would cause that many static lists of signals
 *   and slots instead of one.  While this would maintain type information
 *   directly, it would also potentially bump up the static memory overhead
 *   of the program.  Though that increase may be insignificant.  This method
 *   may only need to be changed if a system is encountered that does not free
 *   up memory for a program upon termination.
 *
 */

#include <memory> // std::auto_ptr
#include <list>
#include <vector>

//#include "Type/Interface/EmptyType.h"
#include "Type/Interface/TypeTraits.h"
#include "Functor/Interface/iFunctor.h"
#include "Functor/Interface/iStorageType.h"
#include "Functor/Interface/ReturnlessMemFunWrap.h"
#include "Functor/Interface/ReturnlessFunctor.h"
#include "Functor/Interface/OneToMany.h"

namespace nComm
{

  //template <typename Allocator=EmptyType>
  class Connection {
  private:
    Connection(void);
    ~Connection(void);

  public:
    // Create- Adds the incoming callable entity(slot) to the
    //   OneToMany's(signal) set of callable entities.
    template <typename T, typename FUN, typename StorageType>
    static void Create(nFunctor::OneToMany<void, T, StorageType> &, FUN);

    template <typename T, typename OBJ, typename MemFun, typename StorageType>
    static void Create(nFunctor::OneToMany<void, T, StorageType> &,
                       OBJ, MemFun);


    // Break- Remove a the first instance of a specific callable entity from
    //   a specific OneToMany.
    template <typename T, typename FUN, typename StorageType>
    static void Break(nFunctor::OneToMany<void, T, StorageType> &, FUN);

    template <typename T, typename OBJ, typename MemFun, typename StorageType>
    static void Break(nFunctor::OneToMany<void, T, StorageType> &,
                      OBJ, MemFun);

    // BreakAll- Remove all the instances of a callable entity for all
    //   OneToManys, or remove all callable entity instances associated
    //   with a given OneToMany.
    template <typename T, typename StorageType>
    static void BreakAll(nFunctor::OneToMany<void, T, StorageType> &);

    template <typename T, typename FUN>
    static void BreakAll(FUN);

    template <typename T, typename OBJ, typename MemFun>
    static void BreakAll(OBJ, MemFun);

  private:
    // ConnId- This class holds all the information necessary to disconnect
    //   a signal/slot.  The Signal and Callable void pointers are used as ids
    //   for lookup.
    class ConnId {
    public:
      ConnId(void *vpSignal=0, void *vpCallable=0, void *vpStore=0,
             nFunctor::bFunctor *vpRFunctor=0)
      : d_vps(vpSignal),
        d_vpc(vpCallable),
        d_vpStore(vpStore),
        d_vprf(vpRFunctor)
      { return; }

      ConnId(const ConnId &ci)
      : d_vps(ci.d_vps),
        d_vpc(ci.d_vpc),
        d_vpStore(ci.d_vpStore),
        d_vprf(ci.d_vprf.release())
      { return; }

      ConnId &operator=(const ConnId &ci) {
        if(&ci != this) {
          d_vps = ci.d_vps;
          d_vpc = ci.d_vpc;
          d_vpStore = ci.d_vpStore;
          d_vprf.reset(ci.d_vprf.release());
        }
        return *this;
      }

      void *GetSignal(void)   { return d_vps;  }
      void *GetCallable(void) { return d_vpc;  }
      void *GetStore(void)    { return d_vpStore; }
      void *GetFunctor(void)  { return d_vprf.get(); }

      // Only compare signal and callable because they are the ids used for
      //   lookup
      const bool operator==(const ConnId &ci) const {
        if(d_vps == ci.d_vps && d_vpc == ci.d_vpc) { return true; }
        return false;
      }

      const bool operator!=(const ConnId &ci) const {
        if(d_vps == ci.d_vps && d_vpc == ci.d_vpc) { return false; }
        return true;
      }

    private:
      void *d_vps;
      void *d_vpc;
      void *d_vpStore;
      mutable std::auto_ptr< nFunctor::bFunctor > d_vprf;
    };

    // Data of all currently connected signal/slot pairs
    static std::list<ConnId> d_sigslots;

    // These are helper functions used to convert incoming types to void*s.
    // This converts T(a pointer) to a void*
    template <typename T>
    static void *Convert(T t, nType::Int2Type<true>) {
      //return static_cast< void* >(t);
      return reinterpret_cast< void* >(t);
    }
    // This converts T(values and references) to a void*
    template <typename T>
    static void *Convert(T t, nType::Int2Type<false>) {
      //return static_cast< void* >(&t);
      return reinterpret_cast< void* >(&t);
    }
  };

  /*** Implementation ****************************************************/

  /*** BEGIN Create ******************************************************/
  //template <typename A>
  template <typename T, typename FUN, typename StorageType>
  void Connection::Create(nFunctor::OneToMany<void, T, StorageType> &s, FUN f)
  {
    // Wrap FUN in a ReturnlessFunctor
    std::auto_ptr< nFunctor::iFunctor<void, T> > rf(
      new nFunctor::ReturnlessFunctor<FUN, void, T>(f));

    // Convert all essential values to void* for homogeneous storage and lookup
    void *vs  = static_cast< void* >(&s);
    void *vf =
      Convert<FUN>(f, nType::Int2Type< nType::TypeTraits<FUN>::isPointer >());
    void *vpStore = static_cast< void* >(s.GetStorage());
    nFunctor::bFunctor *vrf = static_cast< nFunctor::bFunctor* >(rf.get());

    // Add the new functor to the signal.  Add can throw an exception.
    s.GetStorage()->Add(rf.get());

    // Make sure not delete the newly allocated data.
    rf.release();

    // Create an identifiable unit for disconnection and deletion of the 
    //   created functor.
    d_sigslots.push_back(ConnId(vs, vf, vpStore, vrf));

    return;
  }

  //template <typename A>
  template <typename T, typename OBJ, typename MemFun, typename StorageType>
  void Connection::Create(nFunctor::OneToMany<void, T, StorageType> &s,
                          OBJ o, MemFun f)
  {
    // Wrap OBJ and MemFun in a ReturnlessMemFunWrap
    std::auto_ptr< nFunctor::iFunctor<void, T> >
      rf( new nFunctor::ReturnlessMemFunWrap<OBJ, MemFun, void, T>(o, f) );

    // Convert all essential values to void* for homogeneous storage and lookup
    void *vs  = static_cast< void* >(&s);
    void *vf =
      Convert<OBJ>(o, nType::Int2Type< nType::TypeTraits<OBJ>::isPointer >());
    void *vpStore = static_cast< void* >(s.GetStorage());
    nFunctor::bFunctor *vrf = static_cast< nFunctor::bFunctor* >(rf.get());

    // Add the new functor to the signal.  Add can throw an exception.
    s.GetStorage()->Add(rf.get());

    // Make sure not delete the newly allocated data.
    rf.release();

    // Create an identifiable unit for disconnection and deletion of the 
    //   created functor.
    d_sigslots.push_back(ConnId(vs, vf, vpStore, vrf));

    return;
  }
  /*** END Create ********************************************************/

  /*** Begin Break *******************************************************/
  //template <typename A>
  template <typename T, typename FUN, typename StorageType>
  void Connection::Break(nFunctor::OneToMany<void, T, StorageType> &s,
                         FUN f)
  {
    // Convert all essential values to void* for homogeneous storage and lookup
    void *vs  = static_cast< void* >(&s);
    void *vf =
      Convert<FUN>(f, nType::Int2Type< nType::TypeTraits<FUN>::isPointer >());

    // Create key for lookup
    ConnId ci(vs, vf, 0, 0);

    std::list<ConnId>::iterator i;

    // Find the first ConnId that matches ci
    for(i=d_sigslots.begin(); i!=d_sigslots.end(); ++i) {
      if(*i == ci) { break; }
    }

    // Disconnect by removing the slot from the signal's list, deleting
    //   the allocated wrapper, and erase the connection info.
    if(i != d_sigslots.end()) {
      nFunctor::iFunctor<void, T> *rf =
        static_cast< nFunctor::iFunctor<void, T>* >(i->GetFunctor());
      if(rf) { s.GetStorage()->Remove(rf); }

      // deletion deletes rf
      d_sigslots.erase(i);
    }

    return;
  }

  //template <typename A>
  template <typename T, typename OBJ, typename MemFun, typename StorageType>
  void Connection::Break(nFunctor::OneToMany<void, T, StorageType> &s,
                                OBJ o, MemFun f)
  {
    // Convert all essential values to void* for homogeneous storage and lookup
    void *vs  = static_cast< void* >(&s);
    void *vf =
      Convert<OBJ>(o, nType::Int2Type< nType::TypeTraits<OBJ>::isPointer >());

    // Create key for lookup
    ConnId ci(vs, vf, 0, 0);

    std::list<ConnId>::iterator i;

    // Find the first ConnId that matches ci.  Because a member function ptr
    //   can not be converted to a void*, it can not be implicitly used for
    //   lookup with ConnId.  Instead, if a void* matches that of the incoming
    //   object, then the wrapper is casted to inorder to check the specific
    //   member function pointer stored within.
    for(i=d_sigslots.begin(); i!=d_sigslots.end(); ++i) {
      if(*i == ci) {
        nFunctor::ReturnlessMemFunWrap<OBJ, MemFun, void, T> *rf =
          static_cast< nFunctor::ReturnlessMemFunWrap<OBJ, MemFun, void, T>* >
          (i->GetFunctor());

        if(rf && rf->Function() == f) {
          break;
        }
      }
    }

    if(i != d_sigslots.end()) {
      // Disconnect by removing the slot from the signal's list, deleting
      //   the allocated wrapper, and erase the connection info.
      nFunctor::iFunctor<void, T> *rf
        = static_cast< nFunctor::iFunctor<void, T>* >(i->GetFunctor());
      if(rf) { s.GetStorage()->Remove(rf); }

      // deletion deletes rf
      d_sigslots.erase(i);
    }

    return;
  }
  /*** END Break *********************************************************/

  /*** Begin BreakAll ****************************************************/
  //template <typename A>
  template <typename T, typename StorageType>
  void Connection::BreakAll(nFunctor::OneToMany<void,T,StorageType> &s)
  {
    // Convert all essential values to void* for homogeneous storage and lookup
    void *vs = static_cast< void* >(&s);

    std::list<ConnId>::iterator i;
    // Store iterators for later deletion
    std::vector< std::list<ConnId>::iterator > d_iters;

    // Find all connections that have the given signal
    for(i=d_sigslots.begin(); i!=d_sigslots.end(); ++i) {
      if(i->GetSignal() == vs) { d_iters.push_back(i); }
    }

    // Delete all the connections that were found.
    for(unsigned int index=0; index<d_iters.size(); index++) {
      i = d_iters.at(index);

      // deletion deletes created ReturnlessFunctor
      d_sigslots.erase(i);
    }

    // Clear the entities
    s.GetStorage()->Clear();

    return;
  }

  //template <typename A>
  template <typename T, typename FUN>
  void Connection::BreakAll(FUN f) {
    // Convert all essential values to void* for homogeneous storage and lookup
    void *vf =
      Convert<FUN>(f, nType::Int2Type< nType::TypeTraits<FUN>::isPointer >());

    std::list<ConnId>::iterator i;
    // Store iterators for later deletion
    std::vector< std::list<ConnId>::iterator > d_iters;

    // Find all connections that have the given entity
    for(i=d_sigslots.begin(); i!=d_sigslots.end(); ++i) {
      if(i->GetCallable() == vf) { d_iters.push_back(i); }
    }

    // Delete all the connections that were found.
    for(unsigned int index=0; index<d_iters.size(); index++) {
      i = d_iters.at(index);

      // Need to delete the created wrapper and remove an instance of the
      //   callable entity from the attached signal.  Then the connection
      //   needs to be erased.
      nFunctor::iFunctor<void, T> *rf =
        static_cast< nFunctor::iFunctor<void, T>* >(i->GetFunctor());
      nFunctor::iStorageType<void, T> *is =
        static_cast< nFunctor::iStorageType<void, T>* >(i->GetStore());
      if(rf) { is->Remove(rf); }

      // deletion deletes rf
      d_sigslots.erase(i);
    }

    return;
  }

  //template <typename A>
  template <typename T, typename OBJ, typename MemFun>
  void Connection::BreakAll(OBJ o, MemFun f) {
    // Convert all essential values to void* for homogeneous storage and lookup
    void *vf =
      Convert<OBJ>(o, nType::Int2Type< nType::TypeTraits<OBJ>::isPointer >());

    // Find all ConnIds that match <*, vf, *, *>
    std::list<ConnId>::iterator i;
    // Store iterators for later deletion
    std::vector< std::list<ConnId>::iterator > d_iters;

    // Find all the connections that match the incoming object.  Because a
    //   member function pointer can not be converted to a void*, it can not be
    //   implicitly used for lookup.  Instead, if a void* matches that of the
    //   incoming object, then the wrapper is casted to inorder to check the
    //   specific member function pointer stored within.
    for(i=d_sigslots.begin(); i!=d_sigslots.end(); ++i) {
      if(i->GetCallable() == vf) {
        nFunctor::ReturnlessMemFunWrap<OBJ, MemFun, void, T> *rf =
          static_cast< nFunctor::ReturnlessMemFunWrap<OBJ, MemFun, void, T>* >
          (i->GetFunctor());

        if(rf && rf->Function() == f) {
          d_iters.push_back(i);
        }
      }
    }

    // Delete all the connections that were found.
    for(unsigned int index=0; index<d_iters.size(); index++) {
      i = d_iters.at(index);

      // Need to delete the created wrapper and remove an instance of the
      //   callable entity from the attached signal.  Then the connection
      //   needs to be erased.
      nFunctor::iFunctor<void, T> *rf
        = static_cast< nFunctor::iFunctor<void, T>* >(i->GetFunctor());
      nFunctor::iStorageType<void, T> *is =
        static_cast< nFunctor::iStorageType<void, T>* >(i->GetStore());
      if(rf) { is->Remove(rf); }

      // deletion deletes rf
      d_sigslots.erase(i);
    }

    return;
  }
  /*** END BreakAll ******************************************************/

} // end namespace nComm

#endif // end __Comm_Interface_Connection_h__

/*

 * Boost Software License - Version 1.0 - August 17th, 2003

 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:

 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.

 */
