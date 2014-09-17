#ifndef GB_CONFIG_H
#define GB_CONFIG_H

// major version
#define GB_CONFIG_VERSION_MAJOR         [major]

// minor version
#define GB_CONFIG_VERSION_MINOR         [minor]

// alter version
#define GB_CONFIG_VERSION_ALTER         [alter]

// build version
#define GB_CONFIG_VERSION_BUILD         [build]

// small
#define GB_CONFIG_SMALL                 [small]

// the fixed type for float
#define GB_CONFIG_FLOAT_FIXED

// gl
#define GB_CONFIG_GL_APICALL            __tb_cdecl__

// third party
#define GB_CONFIG_THIRD_HAVE_GL
#define GB_CONFIG_THIRD_HAVE_GLUT
#define GB_CONFIG_THIRD_HAVE_SKIA
//#define GB_CONFIG_THIRD_HAVE_X11
#define GB_CONFIG_THIRD_HAVE_SDL
//#define GB_CONFIG_THIRD_HAVE_FRAMEBUFFER
#define GB_CONFIG_THIRD_HAVE_PNG
//#define GB_CONFIG_THIRD_HAVE_JPG
//#define GB_CONFIG_THIRD_HAVE_FREETYPE

// app window
//#define GB_CONFIG_APP_WINDOW_NATIVE
#define GB_CONFIG_APP_WINDOW_SDL
//#define GB_CONFIG_APP_WINDOW_FRAMEBUFFER


#endif
