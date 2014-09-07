#include "ofxGtkApp.h"

ofxGtkApp::ofxGtkApp(std::string appName) {
	app = Gtk::Application::create(appName);
	//ofAddListener(ofEvents().exit, this, &ofxGtkApp::onClose);
}

ofxGtkApp::~ofxGtkApp() {
}

int ofxGtkApp::run(ofxGtkBaseWindow* window) {
	//window->set_application(app);
	window->setup();
	return app->run(*window, 0, NULL);
}

void ofxGtkApp::onClose(ofEventArgs& args) {
	app->quit();
}
