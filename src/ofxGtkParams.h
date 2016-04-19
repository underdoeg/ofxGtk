#ifndef OFXGTKPARAMS_H
#define OFXGTKPARAMS_H

#include <gtkmm.h>
#include "ofxGtkWidgets.h"
#include "ofMain.h"
#include "ofxGtkWrapper.h"

template<typename Type>
Gtk::Widget* widgetFromParameter(ofParameter<Type>& param){
	ofLogWarning() << "no default widget for type (" << typeid(Type).name() << ") found";
	return nullptr;
}

Gtk::Widget* widgetFromParameter(ofParameterGroup& group);
Gtk::Widget* widgetFromParameter(ofAbstractParameter& param);

Gtk::Expander* expanderFromParameterGroup(ofParameterGroup& group);
Gtk::VBox* vBoxFromParameterGroup(ofParameterGroup& group);
Gtk::Frame* frameFromParameterGroup(ofParameterGroup& group);

#endif // OFXGTKPARAMS_H
