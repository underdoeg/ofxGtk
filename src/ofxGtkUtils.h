#ifndef OFXGTKUTILS_H
#define OFXGTKUTILS_H

#include <gtkmm.h>
#include "ofMain.h"

ofColor toOf(Gdk::Color color);
ofColor toOf(Gdk::Color& color);
ofFloatColor toOf(Gdk::RGBA color);
ofFloatColor toOf(Gdk::RGBA& color);

#endif // OFXGTKUTILS_H
