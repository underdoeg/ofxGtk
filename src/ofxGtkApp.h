#ifndef OFXGTKAPP_H
#define OFXGTKAPP_H

#include "ofxGtkUtils.h"

class ofxGtkApp
{
public:
	ofxGtkApp(std::string appName="cc.openFrameworks.app");
	~ofxGtkApp();
	int run(Gtk::Window* window);
	void onClose(ofEventArgs& args);
	
	Glib::RefPtr<Gtk::Application> app;
};

#endif // OFXGTKAPP_H
