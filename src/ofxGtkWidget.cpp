#include "ofxGtkWidget.h"

//baded on https://developer.gnome.org/gtkmm-tutorial/stable/sec-custom-widgets.html.en

ofxGtkWidget::ofxGtkWidget(){
	bSetup = false;
	bFullscreen = false;

	widget().add_events(Gdk::KEY_PRESS_MASK | Gdk::STRUCTURE_MASK | Gdk::KEY_RELEASE_MASK
						| Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK
						| Gdk::BUTTON_MOTION_MASK | Gdk::BUTTON1_MOTION_MASK | Gdk::BUTTON2_MOTION_MASK | Gdk::BUTTON3_MOTION_MASK);
	widget().set_can_focus();
	widget().grab_focus();

	//
	widget().signal_realize().connect(sigc::mem_fun(this, &ofxGtkWidget::onRealize));
	widget().signal_render().connect(sigc::mem_fun(this, &ofxGtkWidget::onRender));
	widget().signal_key_press_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onKeyDown));
	widget().signal_key_release_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onKeyUp));
	widget().signal_motion_notify_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onMotionNotify));
	widget().signal_button_press_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onButtonPress));
	widget().signal_button_release_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onButtonRelease));

	//
	app = nullptr;
	//events().setFrameRate(60);

	setFPS(200);

	widget().set_auto_render();
}

ofxGtkWidget::~ofxGtkWidget(){
}

void ofxGtkWidget::setApp(ofBaseApp *a){
	app = a;
}

void ofxGtkWidget::setup(const ofGLWindowSettings &settings){

}

void ofxGtkWidget::makeCurrent(){
	widget().make_current();
}

void ofxGtkWidget::update(){

}

void ofxGtkWidget::draw(){

}

ofCoreEvents &ofxGtkWidget::events(){
	return coreEvents;
}

shared_ptr<ofBaseRenderer> &ofxGtkWidget::renderer(){
	return currentRenderer;
}

void ofxGtkWidget::setFPS(int newFps){
	if(newFps == 0 || fps == newFps)
		return;

	fps = newFps;

	if(fpsConnection){
		fpsConnection.disconnect();
	}
	unsigned int ms = ceil(1000.f/double(fps));
	fpsConnection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &ofxGtkWidget::onTimeout), ms);
}

int ofxGtkWidget::getWidth(){
	return widget().get_width();
}

int ofxGtkWidget::getHeight(){
	return widget().get_height();
}

ofPoint ofxGtkWidget::getWindowSize(){
	return  ofPoint(getWidth(), getHeight());
}

void ofxGtkWidget::hideCursor(){
	widget().get_window()->set_cursor(Gdk::Cursor::create(Gdk::CursorType::BLANK_CURSOR));
}

void ofxGtkWidget::showCursor(){
	widget().get_window()->set_cursor(Gdk::Cursor::create(Gdk::CursorType::X_CURSOR));
}

void ofxGtkWidget::setWindowPosition(int x, int y){
	Gtk::Window *win = dynamic_cast<Gtk::Window *>(widget().get_toplevel());
	if(win)
		win->move(x, y);
}

void ofxGtkWidget::setWindowShape(int w, int h){
	Gtk::Window *win = dynamic_cast<Gtk::Window *>(widget().get_toplevel());
	if(win)
		win->resize(w, h);
}

ofPoint ofxGtkWidget::getWindowPosition(){
	Gtk::Window *win = dynamic_cast<Gtk::Window *>(widget().get_toplevel());
	if(win){
		int x, y;
		win->get_position(x, y);
		return ofPoint(x, y);
	}
	return ofPoint(0, 0);
}

ofPoint ofxGtkWidget::getScreenSize(){
	ofPoint(widget().get_screen()->get_width(), widget().get_screen()->get_height());
}

void ofxGtkWidget::setWindowTitle(string title){
	Gtk::Window *win = dynamic_cast<Gtk::Window *>(widget().get_toplevel());
	if(win)
		win->set_title(title);
}

void ofxGtkWidget::setFullscreen(bool fullscreen){
	if(fullscreen){
		widget().get_window()->fullscreen();
	}else{
		widget().get_window()->unfullscreen();
	}
	bFullscreen = fullscreen;
}

void ofxGtkWidget::toggleFullscreen(){
	setFullscreen(!bFullscreen);
}

//////////////////////////////////////////////////////////
void ofxGtkWidget::onRealize(){
	//ofLog() << "REALIZE";

	if(!app){
		ofLogError("ofxGtkWidget") << "no app set";
		return;
	}

	widget().make_current();

	try
	{
		widget().throw_if_error();

		glewExperimental = GL_TRUE;
		glewInit();

		ofGLProgrammableRenderer* renderer = new ofGLProgrammableRenderer(this);
		currentRenderer = shared_ptr<ofGLProgrammableRenderer>(renderer);

		int glMajor, glMinor;
		widget().get_context()->get_version(glMajor, glMinor);
		renderer->setup(glMajor, glMinor);
		//

	}

	catch(const Gdk::GLError& gle)
	{
		cerr << "An error occured making the context current during realize:" << endl;
		cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << endl;
	}

	//ofRunApp(shared_ptr<ofAppBaseWindow>(this), shared_ptr<ofBaseApp>(app));
}

bool ofxGtkWidget::onTimeout(){
	setFPS(events().getTargetFrameRate());
	widget().queue_render();
	return true;
}

bool ofxGtkWidget::onRender(const Glib::RefPtr<Gdk::GLContext>& /*context*/){

	widget().make_current();

	if(!bSetup){
		ofRunApp(shared_ptr<ofAppBaseWindow>(this), shared_ptr<ofBaseApp>(app));
		bSetup = true;
	}

	currentRenderer->startRender();
	events().notifyUpdate();

	currentRenderer->setupScreen();
	events().notifyDraw();

	currentRenderer->finishRender();
	return true;
}

bool ofxGtkWidget::onKeyUp(GdkEventKey* keyEvent){
	events().notifyKeyReleased(keyEvent->keyval);
	return true;
}

bool ofxGtkWidget::onKeyDown(GdkEventKey* keyEvent){
	events().notifyKeyPressed(keyEvent->keyval);
	return true;
}

bool ofxGtkWidget::onMotionNotify(GdkEventMotion *evt){
	if(evt->state & GDK_BUTTON1_MASK) {
		events().notifyMouseDragged(evt->x, evt->y, 0);
	} else if(evt->state & GDK_BUTTON2_MASK) {
		events().notifyMouseDragged(evt->x, evt->y, 1);
	}else if(evt->state & GDK_BUTTON3_MASK) {
		events().notifyMouseDragged(evt->x, evt->y, 2);
	} else {
		events().notifyMouseMoved(evt->x, evt->y);
	}
	return true;
}

bool ofxGtkWidget::onButtonPress(GdkEventButton *evt){
	events().notifyMousePressed(evt->x, evt->y, evt->button - 1);
	return true;
}

bool ofxGtkWidget::onButtonRelease(GdkEventButton *evt){
	events().notifyMouseReleased(evt->x, evt->y, evt->button - 1);
	return true;
}

