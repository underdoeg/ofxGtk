#include "ofxGtkUtils.h"

ofColor toOf(Gdk::Color color){
	return ofColor(color.get_red(), color.get_green(), color.get_blue());
}

ofColor toOf(Gdk::Color& color){
	return ofColor(color.get_red(), color.get_green(), color.get_blue());
}

ofFloatColor toOf(Gdk::RGBA color){
	return ofFloatColor(color.get_red(), color.get_green(), color.get_blue());
}

ofFloatColor toOf(Gdk::RGBA& color){
	return ofFloatColor(color.get_red(), color.get_green(), color.get_blue());
}

