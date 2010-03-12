/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-9 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#ifndef __JUCE_LISTENERLIST_JUCEHEADER__
#define __JUCE_LISTENERLIST_JUCEHEADER__

#include "../containers/juce_Array.h"


//==============================================================================
/**
    Holds a set of objects and can invoke a member function callback on each object
    in the set with a single call.

    Use a ListenerList to manage a set of objects which need a callback, and you
    can invoke a member function by simply calling call() or callChecked().

    E.g.
    @code
    class MyListenerType
    {
    public:
        void myCallbackMethod (int foo, bool bar);
    };

    ListenerList <MyListenerType> listeners;
    listeners.add (someCallbackObjects...);

    // This will invoke myCallbackMethod (1234, true) on each of the objects
    // in the list...
    listeners.call (&MyListenerType::myCallbackMethod, 1234, true);
    @endcode

    If you add or remove listeners from the list during one of the callbacks - i.e. while
    it's in the middle of iterating the listeners, then it's guaranteed that no listeners
    will be mistakenly called after they've been removed, but it may mean that some of the
    listeners could be called more than once, or not at all, depending on the list's order.

    Sometimes, there's a chance that invoking one of the callbacks might result in the
    list itself being deleted while it's still iterating - to survive this situation, you can
    use callChecked() instead of call(), passing it a local object to act as a "BailOutChecker".
    The BailOutChecker must implement a method of the form "bool shouldBailOut()", and
    the list will check this after each callback to determine whether it should abort the
    operation. For an example of a bail-out checker, see the Component::BailOutChecker class,
    which can be used to check when a Component has been deleted. See also
    ListenerList::DummyBailOutChecker, which is a dummy checker that always returns false.
*/
template <class ListenerClass,
          class ArrayType = Array <ListenerClass*> >
class ListenerList
{
public:
    //==============================================================================
    /** Creates an empty list. */
    ListenerList()
    {
    }

    /** Destructor. */
    ~ListenerList()
    {
    }

    //==============================================================================
    /** Adds a listener to the list.
        A listener can only be added once, so if the listener is already in the list,
        this method has no effect.
        @see remove
    */
    void add (ListenerClass* const listenerToAdd)
    {
        // Listeners can't be null pointers!
        jassert (listenerToAdd != 0);

        if (listenerToAdd != 0)
            listeners.add (listenerToAdd);
    }

    /** Removes a listener from the list.
        If the listener wasn't in the list, this has no effect.
    */
    void remove (ListenerClass* const listenerToRemove)
    {
        // Listeners can't be null pointers!
        jassert (listenerToRemove != 0);

        listeners.removeValue (listenerToRemove);
    }

    /** Returns the number of registered listeners. */
    int size() const throw()
    {
        return listeners.size();
    }

    /** Returns true if any listeners are registered. */
    bool isEmpty() const throw()
    {
        return listeners.size() == 0;
    }

    /** Returns true if the specified listener has been added to the list. */
    bool contains (ListenerClass* const listener) const throw()
    {
        return listeners.contains (listener);
    }

    //==============================================================================
    /** Calls a member function on each listener in the list, with no parameters. */
    void call (void (ListenerClass::*callbackFunction) ())
    {
        callChecked (static_cast <const DummyBailOutChecker&> (DummyBailOutChecker()), callbackFunction);
    }

    /** Calls a member function on each listener in the list, with no parameters and a bail-out-checker.
        See the class description for info about writing a bail-out checker. */
    template <class BailOutCheckerType>
    void callChecked (const BailOutCheckerType& bailOutChecker,
                      void (ListenerClass::*callbackFunction) ())
    {
        for (Iterator<BailOutCheckerType, ThisType> iter (*this, bailOutChecker); iter.next();)
            (iter.getListener()->*callbackFunction) ();
    }

    //==============================================================================
    /** Calls a member function on each listener in the list, with 1 parameter. */
    template <typename P1, typename P2>
    void call (void (ListenerClass::*callbackFunction) (P1),
               P2& param1)
    {
        for (Iterator<DummyBailOutChecker, ThisType> iter (*this, DummyBailOutChecker()); iter.next();)
            (iter.getListener()->*callbackFunction) (param1);
    }

    /** Calls a member function on each listener in the list, with one parameter and a bail-out-checker.
        See the class description for info about writing a bail-out checker. */
    template <class BailOutCheckerType, typename P1, typename P2>
    void callChecked (const BailOutCheckerType& bailOutChecker,
                      void (ListenerClass::*callbackFunction) (P1),
                      P2& param1)
    {
        for (Iterator<BailOutCheckerType, ThisType> iter (*this, bailOutChecker); iter.next();)
            (iter.getListener()->*callbackFunction) (param1);
    }

    //==============================================================================
    /** Calls a member function on each listener in the list, with 2 parameters. */
    template <typename P1, typename P2, typename P3, typename P4>
    void call (void (ListenerClass::*callbackFunction) (P1, P2),
               P3& param1, P4& param2)
    {
        for (Iterator<DummyBailOutChecker, ThisType> iter (*this, DummyBailOutChecker()); iter.next();)
            (iter.getListener()->*callbackFunction) (param1, param2);
    }

    /** Calls a member function on each listener in the list, with 2 parameters and a bail-out-checker.
        See the class description for info about writing a bail-out checker. */
    template <class BailOutCheckerType, typename P1, typename P2, typename P3, typename P4>
    void callChecked (const BailOutCheckerType& bailOutChecker,
                      void (ListenerClass::*callbackFunction) (P1, P2),
                      P3& param1, P4& param2)
    {
        for (Iterator<BailOutCheckerType, ThisType> iter (*this, bailOutChecker); iter.next();)
            (iter.getListener()->*callbackFunction) (param1, param2);
    }

    //==============================================================================
    /** Calls a member function on each listener in the list, with 3 parameters. */
    template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    void call (void (ListenerClass::*callbackFunction) (P1, P2, P3),
               P4& param1, P5& param2, P6& param3)
    {
        for (Iterator<DummyBailOutChecker, ThisType> iter (*this, DummyBailOutChecker()); iter.next();)
            (iter.getListener()->*callbackFunction) (param1, param2, param3);
    }

    /** Calls a member function on each listener in the list, with 3 parameters and a bail-out-checker.
        See the class description for info about writing a bail-out checker. */
    template <class BailOutCheckerType, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    void callChecked (const BailOutCheckerType& bailOutChecker,
                      void (ListenerClass::*callbackFunction) (P1, P2, P3),
                      P4& param1, P5& param2, P6& param3)
    {
        for (Iterator<BailOutCheckerType, ThisType> iter (*this, bailOutChecker); iter.next();)
            (iter.getListener()->*callbackFunction) (param1, param2, param3);
    }

    //==============================================================================
    /** Calls a member function on each listener in the list, with 4 parameters. */
    template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
    void call (void (ListenerClass::*callbackFunction) (P1, P2, P3, P4),
               P5& param1, P6& param2, P7& param3, P8& param4)
    {
        for (Iterator<DummyBailOutChecker, ThisType> iter (*this, DummyBailOutChecker()); iter.next();)
            (iter.getListener()->*callbackFunction) (param1, param2, param3, param4);
    }

    /** Calls a member function on each listener in the list, with 4 parameters and a bail-out-checker.
        See the class description for info about writing a bail-out checker. */
    template <class BailOutCheckerType, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
    void callChecked (const BailOutCheckerType& bailOutChecker,
                      void (ListenerClass::*callbackFunction) (P1, P2, P3, P4),
                      P5& param1, P6& param2, P7& param3, P8& param4)
    {
        for (Iterator<BailOutCheckerType, ThisType> iter (*this, bailOutChecker); iter.next();)
            (iter.getListener()->*callbackFunction) (param1, param2, param3, param4);
    }

    //==============================================================================
    /** Calls a member function on each listener in the list, with 5 parameters. */
    template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
    void call (void (ListenerClass::*callbackFunction) (P1, P2, P3, P4, P5),
               P6& param1, P7& param2, P8& param3, P9& param4, P10& param5)
    {
        for (Iterator<DummyBailOutChecker, ThisType> iter (*this, DummyBailOutChecker()); iter.next();)
            (iter.getListener()->*callbackFunction) (param1, param2, param3, param4, param5);
    }

    /** Calls a member function on each listener in the list, with 5 parameters and a bail-out-checker.
        See the class description for info about writing a bail-out checker. */
    template <class BailOutCheckerType, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
    void callChecked (const BailOutCheckerType& bailOutChecker,
                      void (ListenerClass::*callbackFunction) (P1, P2, P3, P4, P5),
                      P6& param1, P7& param2, P8& param3, P9& param4, P10& param5)
    {
        for (Iterator<BailOutCheckerType, ThisType> iter (*this, bailOutChecker); iter.next();)
            (iter.getListener()->*callbackFunction) (param1, param2, param3, param4, param5);
    }


    //==============================================================================
    /** A dummy bail-out checker that always returns false.
        See the ListenerList notes for more info about bail-out checkers.
    */
    class DummyBailOutChecker
    {
    public:
        inline bool shouldBailOut() const throw()      { return false; }
    };

    //==============================================================================
    /** Iterates the listeners in a ListenerList. */
    template <class BailOutCheckerType, class ListType>
    class Iterator
    {
    public:
        //==============================================================================
        Iterator (const ListType& list_, const BailOutCheckerType& bailOutChecker_)
            : list (list_), bailOutChecker (bailOutChecker_), index (list_.size())
        {}

        ~Iterator() {}

        //==============================================================================
        bool next()
        {
            if (index <= 0 || bailOutChecker.shouldBailOut())
                return false;

            const int listSize = list.size();

            if (--index < listSize)
                return true;

            index = listSize - 1;
            return index >= 0;
        }

        typename ListType::ListenerType* getListener() const throw()
        {
            return list.getListeners().getUnchecked (index);
        }

        //==============================================================================
    private:
        const ListType& list;
        const BailOutCheckerType& bailOutChecker;
        int index;

        Iterator (const Iterator&);
        Iterator& operator= (const Iterator&);
    };

    typedef ListenerList<ListenerClass, ArrayType> ThisType;
    typedef ListenerClass ListenerType;

    const ArrayType& getListeners() const throw()           { return listeners; }

private:
    //==============================================================================
    ArrayType listeners;

    ListenerList (const ListenerList&);
    ListenerList& operator= (const ListenerList&);
};


#endif   // __JUCE_LISTENERLIST_JUCEHEADER__