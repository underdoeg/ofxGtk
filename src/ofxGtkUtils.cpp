#include "ofxGtkUtils.h"

ofColor toOf(Gdk::Color color){
	return ofShortColor(color.get_red(), color.get_green(), color.get_blue());
}

ofColor toOf(Gdk::Color& color){
	return ofShortColor(color.get_red(), color.get_green(), color.get_blue());
}

ofFloatColor toOf(Gdk::RGBA color){
	return ofFloatColor(color.get_red(), color.get_green(), color.get_blue());
}

ofFloatColor toOf(Gdk::RGBA& color){
	return ofFloatColor(color.get_red(), color.get_green(), color.get_blue());
}

///
Gdk::Color toGdk(ofColor color){
	ofShortColor colorShort(color);
	Gdk::Color ret;
	ret.set_rgb(colorShort.r, colorShort.g, colorShort.b);
	return ret;
}
