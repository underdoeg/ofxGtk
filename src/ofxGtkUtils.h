#ifndef OFXGTKUTILS_H
#define OFXGTKUTILS_H

#include "ofMain.h"
#include <gdkmm/color.h>
#include <gdkmm/rgba.h>

ofColor toOf(Gdk::Color& color);
ofFloatColor toOf(Gdk::RGBA color);
ofFloatColor toOf(Gdk::RGBA& color);

#endif // OFXGTKUTILS_H
