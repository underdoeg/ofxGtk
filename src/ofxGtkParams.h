#ifndef OFXGTKPARAMS_H
#define OFXGTKPARAMS_H

#include <gtkmm.h>
#include "ofMain.h"
#include "ofxGtkWrapper.h"

template<typename Type>
Gtk::Widget* widgetFromParameter(const ofParameter<Type>& param){
	ofLogWarning() << "no default widget for type found";
	return nullptr;
}

Gtk::Widget* widgetFromParameterGroup(const ofParameterGroup& params);

#endif // OFXGTKPARAMS_H
