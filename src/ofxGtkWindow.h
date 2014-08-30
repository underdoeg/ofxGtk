#ifndef OFXGTKWINDOW_H
#define OFXGTKWINDOW_H

#include "ofxGtkWidget.h"

template<typename ofApp>
class ofxGtkWindow: public Gtk::Window {
public:
	ofxGtkWindow() {
		set_default_size(1280, 720);
		set_title("openFrameworks");

		app = new ofApp();
		ofWidget.setup(app);
	}

	void setupDefault() {
		Gtk::VBox* vBox = new Gtk::VBox();
		add(*vBox);
		vBox->pack_start(ofWidget, Gtk::PACK_EXPAND_WIDGET);
		show_all_children();
	}

	virtual ~ofxGtkWindow() {

	}

	//Override default signal handler:
	virtual bool on_key_press_event(GdkEventKey* event) {
		ofNotifyKeyPressed(event->keyval);
		return false;
	}

	ofApp* app;
	ofxGtkWidget ofWidget;
};

#endif // OFXGTKWINDOW_H
