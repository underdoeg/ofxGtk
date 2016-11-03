#ifndef OFXGTKUTILS_H
#define OFXGTKUTILS_H

#include <gtkmm.h>
#include "ofMain.h"

static ofColor toOf(Gdk::RGBA src){
	return ofShortColor(src.get_red_u(), src.get_green_u(), src.get_blue_u(), src.get_alpha_u());
}

static Gdk::RGBA toGtk(ofColor color){
	ofShortColor col = color;
	Gdk::RGBA ret;
	ret.set_red_u(col.r);
	ret.set_green_u(col.g);
	ret.set_blue_u(col.b);
	ret.set_alpha_u(col.a);
	return ret;
}

//////////////

static ofRectangle toOf(Gdk::Rectangle rect){
	return ofRectangle(rect.get_x(), rect.get_y(), rect.get_width(), rect.get_height());
}

#endif // OFXGTKUTILS_H
