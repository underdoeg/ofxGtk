#include "ofxGtkWidgets.h"


ofxGtkSwitch::ofxGtkSwitch(ofParameter<bool> &p){
	set(p);
}

ofxGtkSwitch::ofxGtkSwitch(){
	set(param);
}

void ofxGtkSwitch::set(ofParameter<bool> &p){
	param.makeReferenceTo(p);
	if(p.isReadOnly())
		set_sensitive(false);

	update();

	param.newListener([&](const bool& b){
		set_state(param);
	});

	if(!con){
		con = connect_property_changed_with_return("state", [&](){
			param.set(get_state());
		});
	}


}

void ofxGtkSwitch::update(){
	set_state(param);
}

/////////////////////////////////////////////////////////////////////////////////////

template<>
ofxGtkNumericWidget<float, Gtk::SpinButton>::ofxGtkNumericWidget(): Gtk::SpinButton(0.0, 2){
	set(param);
}

template<>
ofxGtkNumericWidget<double, Gtk::SpinButton>::ofxGtkNumericWidget(): Gtk::SpinButton(0.0, 3){
	set(param);
}

//////////////////////////////////////////////////////////////////////////////////////////

