 /*************************************************************************
  * GLFW 3.2 - www.glfw.org
  * A library for OpenGL, window and input
  *------------------------------------------------------------------------
  * Copyright (c) 2002-2006 Marcus Geelnard
  * Copyright (c) 2006-2016 Camilla Berglund <elmindreda@glfw.org>
  *
  * This software is provided 'as-is', without any express or implied
  * warranty. In no event will the authors be held liable for any damages
  * arising from the use of this software.
  *
  * Permission is granted to anyone to use this software for any purpose,
  * including commercial applications, and to alter it and redistribute it
  * freely, subject to the following restrictions:
  *
  * 1. The origin of this software must not be misrepresented; you must not
  *    claim that you wrote the original software. If you use this software
  *    in a product, an acknowledgment in the product documentation would
  *    be appreciated but is not required.
  *
  * 2. Altered source versions must be plainly marked as such, and must not
  *    be misrepresented as being the original software.
  *
  * 3. This notice may not be removed or altered from any source
  *    distribution.
  *
  *************************************************************************/
 
 #ifndef _glfw3_native_h_
 #define _glfw3_native_h_
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 
 /*************************************************************************
  * Doxygen documentation
  *************************************************************************/
 
 /*************************************************************************
  * System headers and types
  *************************************************************************/
 
 #if defined(GLFW_EXPOSE_NATIVE_WIN32)
  // This is a workaround for the fact that glfw3.h needs to export APIENTRY (for
  // example to allow applications to correctly declare a GL_ARB_debug_output
  // callback) but windows.h assumes no one will define APIENTRY before it does
  #undef APIENTRY
  #include <windows.h>
 #elif defined(GLFW_EXPOSE_NATIVE_COCOA)
  #include <ApplicationServices/ApplicationServices.h>
  #if defined(__OBJC__)
   #import <Cocoa/Cocoa.h>
  #else
   typedef void* id;
  #endif
 #elif defined(GLFW_EXPOSE_NATIVE_X11)
  #include <X11/Xlib.h>
  #include <X11/extensions/Xrandr.h>
 #elif defined(GLFW_EXPOSE_NATIVE_WAYLAND)
  #include <wayland-client.h>
 #elif defined(GLFW_EXPOSE_NATIVE_MIR)
  #include <mir_toolkit/mir_client_library.h>
 #endif
 
 #if defined(GLFW_EXPOSE_NATIVE_WGL)
  /* WGL is declared by windows.h */
 #endif
 #if defined(GLFW_EXPOSE_NATIVE_NSGL)
  /* NSGL is declared by Cocoa.h */
 #endif
 #if defined(GLFW_EXPOSE_NATIVE_GLX)
  #include <GL/glx.h>
 #endif
 #if defined(GLFW_EXPOSE_NATIVE_EGL)
  #include <EGL/egl.h>
 #endif
 
 
 /*************************************************************************
  * Functions
  *************************************************************************/
 
 #if defined(GLFW_EXPOSE_NATIVE_WIN32)
 
 GLFWAPI const char* glfwGetWin32Adapter(GLFWmonitor* monitor);
 
 GLFWAPI const char* glfwGetWin32Monitor(GLFWmonitor* monitor);
 
 GLFWAPI HWND glfwGetWin32Window(GLFWwindow* window);
 #endif
 
 #if defined(GLFW_EXPOSE_NATIVE_WGL)
 
 GLFWAPI HGLRC glfwGetWGLContext(GLFWwindow* window);
 #endif
 
 #if defined(GLFW_EXPOSE_NATIVE_COCOA)
 
 GLFWAPI CGDirectDisplayID glfwGetCocoaMonitor(GLFWmonitor* monitor);
 
 GLFWAPI id glfwGetCocoaWindow(GLFWwindow* window);
 #endif
 
 #if defined(GLFW_EXPOSE_NATIVE_NSGL)
 
 GLFWAPI id glfwGetNSGLContext(GLFWwindow* window);
 #endif
 
 #if defined(GLFW_EXPOSE_NATIVE_X11)
 
 GLFWAPI Display* glfwGetX11Display(void);
 
 GLFWAPI RRCrtc glfwGetX11Adapter(GLFWmonitor* monitor);
 
 GLFWAPI RROutput glfwGetX11Monitor(GLFWmonitor* monitor);
 
 GLFWAPI Window glfwGetX11Window(GLFWwindow* window);
 #endif
 
 #if defined(GLFW_EXPOSE_NATIVE_GLX)
 
 GLFWAPI GLXContext glfwGetGLXContext(GLFWwindow* window);
 
 GLFWAPI GLXWindow glfwGetGLXWindow(GLFWwindow* window);
 #endif
 
 #if defined(GLFW_EXPOSE_NATIVE_WAYLAND)
 
 GLFWAPI struct wl_display* glfwGetWaylandDisplay(void);
 
 GLFWAPI struct wl_output* glfwGetWaylandMonitor(GLFWmonitor* monitor);
 
 GLFWAPI struct wl_surface* glfwGetWaylandWindow(GLFWwindow* window);
 #endif
 
 #if defined(GLFW_EXPOSE_NATIVE_MIR)
 
 GLFWAPI MirConnection* glfwGetMirDisplay(void);
 
 GLFWAPI int glfwGetMirMonitor(GLFWmonitor* monitor);
 
 GLFWAPI MirSurface* glfwGetMirWindow(GLFWwindow* window);
 #endif
 
 #if defined(GLFW_EXPOSE_NATIVE_EGL)
 
 GLFWAPI EGLDisplay glfwGetEGLDisplay(void);
 
 GLFWAPI EGLContext glfwGetEGLContext(GLFWwindow* window);
 
 GLFWAPI EGLSurface glfwGetEGLSurface(GLFWwindow* window);
 #endif
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* _glfw3_native_h_ */