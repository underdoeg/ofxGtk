#ifndef OFXGTKWINDOW_H
#define OFXGTKWINDOW_H

#include <gtkmm.h>
#include "ofxGtkWidget.h"

template<typename ofApp>
class ofxGtkWindow: public Gtk::ApplicationWindow, public ofAppBaseGLWindow {
public:
	ofxGtkWindow() {
		set_default_size(1280, 720);
		set_title("openFrameworks");

		app = new ofApp();
		ofWidget.setRootWindow(this);
		ofWidget.setup(app);
		
		//set_show_menubar(true);
		
		ofAddListener(ofEvents().update, this, &ofxGtkWindow::onOfUpdate);
	}

	void setupDefault() {
		Gtk::VBox* vBox = new Gtk::VBox();
		add(*vBox);
		vBox->pack_start(ofWidget, Gtk::PACK_EXPAND_WIDGET);
		show_all_children();
	}

	virtual ~ofxGtkWindow() {

	}

	void onOfUpdate(ofEventArgs& args) {
		onOfUpdate();
	}

	virtual void onOfUpdate() {

	}

	//Override default signal handler:
	virtual bool on_key_press_event(GdkEventKey* event) {
		
		guint keyval = event->keyval;
		
		switch(keyval){
			case GDK_KEY_Escape:
			keyval = OF_KEY_ESC;
			break;
		}
		
		ofNotifyKeyPressed(keyval);
		return false;
	}

	virtual bool on_key_release_event(GdkEventKey* event) {
		ofNotifyKeyReleased(event->keyval);
		return false;
	}

	ofApp* app;
	ofxGtkWidget ofWidget;
};

#endif // OFXGTKWINDOW_H
