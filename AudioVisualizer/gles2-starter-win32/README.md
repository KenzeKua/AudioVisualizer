# A GLES2 Starter Kit for WIN32

[OpenGL for Embedded Systems](http://www.khronos.org/opengles/)
(OpenGL ES) is a rendering API for 2D and 3D graphics.  The API is
cross-language and multiplatform.  The OpenGL ES2 (GLES2)
specification is available for Apple iOS 5+ (e.g., iPad, iPhone, and
iPod Touch), Android 2.0+, the Raspberry Pi, WebGL, Windows XP -
Windows 8 (via ANGLE), and Mac OS 10.9+ (via OpenGL 4.1 which has 
full API compatibility with OpenGL ES 2).

OpenGL ES 2 is generally very pleasant to program with but it only
provides a graphics API.  Working with windows and any kind of user
input is going to be very platform specific.  For at least Windows,
OS X, and Unix-like systems [GLFW](http://www.glfw.org/) can provide
an abstraction layer.

## What's this then?

This is a "starter" project that puts some of the pieces together.

There is no standard OpenGL ES2 development kit for Windows.  IMHO,
the best implementation is Google's
[ANGLE](https://code.google.com/p/angleproject/) which provides an
OpenGL ES2 API that translates ES2 calls to DirectX 9 and hardware
accelerated Direct3D calls.  You can see this technology in action
within Google Chrome's WebGL support.  Unfortunately Google doesn't
provide a precompiled OpenGL ES 2 SDK.

Likewise, while [GLFW](http://www.glfw.org/) provides a really slick
cross-platform experience it's not precompiled for OpenGL ES 2 by
default.

## What's included?

This package includes:

* GLFW3 DLL/LIB and header files 
* ANGLE LIBEGL/LIBGLESv2 DLL/LIB files 
* OpenGL ES 2 headers from Khronos
* An example with an NMAKE compatible Makefile.

ANGLE has been compiled with Visual Studio 2010 / Windows XP 32-bit
using [these directions](https://code.google.com/p/angleproject/wiki/DevSetup).

GLFW was then compiled on the same platform with EGL and OPENGLESv2
enabled in the CMake configuration.

The header files were copied from the Khronos website.  All files are
copyright their respective authors and licensed respectively under the
New BSD License (ANGLE), zlib/libpng (GLFW), royalty free distribution
(Khronos headers), and the DirectX SDK EULA (DirectX).

## How do I use this kit?

You might start by compiling the demo.

1. Launch the "Visual Studio Command Prompt"
2. CD to this directory
3. Type "nmake" to compile the demo
4. Type "demo" to test the demo

You can also build your own Visual C++ solution - just make sure to
set the include and library paths such that they include this
directory.
