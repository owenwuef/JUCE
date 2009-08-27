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

#include "../../../core/juce_StandardHeader.h"

BEGIN_JUCE_NAMESPACE

#include "juce_MouseCursor.h"
#include "../juce_Component.h"
#include "../../../threads/juce_ScopedLock.h"

void* juce_createMouseCursorFromImage (const Image& image, int hotspotX, int hotspotY) throw();
void* juce_createStandardMouseCursor (MouseCursor::StandardCursorType type) throw();
// isStandard set depending on which interface was used to create the cursor
void juce_deleteMouseCursor (void* const cursorHandle, const bool isStandard) throw();


//==============================================================================
static CriticalSection mouseCursorLock;
static VoidArray standardCursors (2);

//==============================================================================
class RefCountedMouseCursor
{
public:
    RefCountedMouseCursor (const MouseCursor::StandardCursorType t) throw()
        : refCount (1),
          standardType (t),
          isStandard (true)
    {
        handle = juce_createStandardMouseCursor (standardType);
        standardCursors.add (this);
    }

    RefCountedMouseCursor (Image& image,
                           const int hotSpotX,
                           const int hotSpotY) throw()
        : refCount (1),
          standardType (MouseCursor::NormalCursor),
          isStandard (false)
    {
        handle = juce_createMouseCursorFromImage (image, hotSpotX, hotSpotY);
    }

    ~RefCountedMouseCursor() throw()
    {
        juce_deleteMouseCursor (handle, isStandard);
        standardCursors.removeValue (this);
    }

    void decRef() throw()
    {
        if (--refCount == 0)
            delete this;
    }

    void incRef() throw()
    {
        ++refCount;
    }

    void* getHandle() const throw()
    {
        return handle;
    }

    static RefCountedMouseCursor* findInstance (MouseCursor::StandardCursorType type) throw()
    {
        const ScopedLock sl (mouseCursorLock);

        for (int i = 0; i < standardCursors.size(); i++)
        {
            RefCountedMouseCursor* const r = (RefCountedMouseCursor*) standardCursors.getUnchecked(i);

            if (r->standardType == type)
            {
                r->incRef();
                return r;
            }
        }

        return new RefCountedMouseCursor (type);
    }

    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    void* handle;
    int refCount;
    const MouseCursor::StandardCursorType standardType;
    const bool isStandard;

    const RefCountedMouseCursor& operator= (const RefCountedMouseCursor&);
};


//==============================================================================
MouseCursor::MouseCursor() throw()
{
    cursorHandle = RefCountedMouseCursor::findInstance (NormalCursor);
}

MouseCursor::MouseCursor (const StandardCursorType type) throw()
{
    cursorHandle = RefCountedMouseCursor::findInstance (type);
}

MouseCursor::MouseCursor (Image& image,
                          const int hotSpotX,
                          const int hotSpotY) throw()
{
    cursorHandle = new RefCountedMouseCursor (image, hotSpotX, hotSpotY);
}

MouseCursor::MouseCursor (const MouseCursor& other) throw()
    : cursorHandle (other.cursorHandle)
{
    const ScopedLock sl (mouseCursorLock);
    cursorHandle->incRef();
}

MouseCursor::~MouseCursor() throw()
{
    const ScopedLock sl (mouseCursorLock);
    cursorHandle->decRef();
}

const MouseCursor& MouseCursor::operator= (const MouseCursor& other) throw()
{
    if (this != &other)
    {
        const ScopedLock sl (mouseCursorLock);

        cursorHandle->decRef();
        cursorHandle = other.cursorHandle;
        cursorHandle->incRef();
    }

    return *this;
}

bool MouseCursor::operator== (const MouseCursor& other) const throw()
{
    return cursorHandle == other.cursorHandle;
}

bool MouseCursor::operator!= (const MouseCursor& other) const throw()
{
    return cursorHandle != other.cursorHandle;
}

void* MouseCursor::getHandle() const throw()
{
    return cursorHandle->getHandle();
}

void MouseCursor::showWaitCursor() throw()
{
    const MouseCursor mc (MouseCursor::WaitCursor);
    mc.showInAllWindows();
}

void MouseCursor::hideWaitCursor() throw()
{
    if (Component::getComponentUnderMouse()->isValidComponent())
    {
        Component::getComponentUnderMouse()->getMouseCursor().showInAllWindows();
    }
    else
    {
        const MouseCursor mc (MouseCursor::NormalCursor);
        mc.showInAllWindows();
    }
}

END_JUCE_NAMESPACE