/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef JUCE_OPENGL_H_INCLUDED
#define JUCE_OPENGL_H_INCLUDED

#include "../juce_gui_extra/juce_gui_extra.h"

#undef JUCE_OPENGL
#define JUCE_OPENGL 1

#if JUCE_IOS || JUCE_ANDROID
 #define JUCE_OPENGL_ES 1
 #define JUCE_USE_OPENGL_FIXED_FUNCTION 0
#endif

#if JUCE_WINDOWS
 #ifndef APIENTRY
  #define APIENTRY __stdcall
  #define CLEAR_TEMP_APIENTRY 1
 #endif
 #ifndef WINGDIAPI
  #define WINGDIAPI __declspec(dllimport)
  #define CLEAR_TEMP_WINGDIAPI 1
 #endif
 #include <gl/GL.h>
 #ifdef CLEAR_TEMP_WINGDIAPI
  #undef WINGDIAPI
  #undef CLEAR_TEMP_WINGDIAPI
 #endif
 #ifdef CLEAR_TEMP_APIENTRY
  #undef APIENTRY
  #undef CLEAR_TEMP_APIENTRY
 #endif
#elif JUCE_LINUX
 #include <GL/gl.h>
 #undef KeyPress
#elif JUCE_IOS
 #include <OpenGLES/ES2/gl.h>
#elif JUCE_MAC
 #include <OpenGL/gl.h>
 #include "OpenGL/glext.h"
#elif JUCE_ANDROID
 #include <GLES2/gl2.h>
#endif

#if ! defined (JUCE_USE_OPENGL_SHADERS)
 #define JUCE_USE_OPENGL_SHADERS 1
#endif

#ifndef JUCE_USE_OPENGL_FIXED_FUNCTION
 #define JUCE_USE_OPENGL_FIXED_FUNCTION 1
#endif

//=============================================================================
namespace juce
{

#include "opengl/juce_OpenGLHelpers.h"

// START_AUTOINCLUDE opengl
#include "opengl/juce_Draggable3DOrientation.h"
#include "opengl/juce_Matrix3D.h"
#include "opengl/juce_OpenGLContext.h"
#include "opengl/juce_OpenGLFrameBuffer.h"
#include "opengl/juce_OpenGLGraphicsContext.h"
#include "opengl/juce_OpenGLHelpers.h"
#include "opengl/juce_OpenGLImage.h"
#include "opengl/juce_OpenGLPixelFormat.h"
#include "opengl/juce_OpenGLRenderer.h"
#include "opengl/juce_OpenGLShaderProgram.h"
#include "opengl/juce_OpenGLTexture.h"
#include "opengl/juce_Quaternion.h"
#include "opengl/juce_Vector3D.h"
// END_AUTOINCLUDE

}

#endif   // JUCE_OPENGL_H_INCLUDED
