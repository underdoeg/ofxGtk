#include "ofxGtkWidget.h"

//baded on https://developer.gnome.org/gtkmm-tutorial/stable/sec-custom-widgets.html.en

ofxGtkWidget::ofxGtkWidget(){

	bSetup = false;
	bFullscreen = false;

	glArea.add_events(Gdk::KEY_PRESS_MASK | Gdk::STRUCTURE_MASK | Gdk::KEY_RELEASE_MASK
						| Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK
						| Gdk::BUTTON_MOTION_MASK | Gdk::BUTTON1_MOTION_MASK | Gdk::BUTTON2_MOTION_MASK | Gdk::BUTTON3_MOTION_MASK);
	glArea.set_can_focus();
	glArea.grab_focus();

	//
	glArea.signal_realize().connect(sigc::mem_fun(this, &ofxGtkWidget::onRealize));
	glArea.signal_render().connect(sigc::mem_fun(this, &ofxGtkWidget::onRender));
	/*
	glArea.signal_key_press_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onKeyDown));
	glArea.signal_key_release_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onKeyUp));
	*/
	glArea.signal_motion_notify_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onMotionNotify));
	glArea.signal_button_press_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onButtonPress));
	glArea.signal_button_release_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onButtonRelease));
	glArea.signal_leave_notify_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onMouseLeave));
	glArea.signal_enter_notify_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onMouseEnter));

	fullscreenWindow.signal_show().connect(sigc::mem_fun(this, &ofxGtkWidget::onShowFullscreen));
	fullscreenWindow.signal_hide().connect(sigc::mem_fun(this, &ofxGtkWidget::onHideFullscreen));
	fullscreenWindow.signal_key_press_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onKeyDown));
	fullscreenWindow.signal_key_release_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onKeyUp));

	//
	app = nullptr;
	//events().setFrameRate(60);

	setFPS(200);

	glArea.set_auto_render();

	widget().set_shadow_type(Gtk::ShadowType::SHADOW_NONE);
	widget().add(glArea);
}

ofxGtkWidget::~ofxGtkWidget(){
}

void ofxGtkWidget::setApp(ofBaseApp *a){
	app = a;
}

void ofxGtkWidget::setup(const ofGLWindowSettings &settings){

}

void ofxGtkWidget::makeCurrent(){
	glArea.make_current();
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
	return glArea.get_width();
}

int ofxGtkWidget::getHeight(){
	return glArea.get_height();
}

ofPoint ofxGtkWidget::getWindowSize(){
	return  ofPoint(getWidth(), getHeight());
}

void ofxGtkWidget::hideCursor(){
	glArea.get_window()->set_cursor(Gdk::Cursor::create(Gdk::CursorType::BLANK_CURSOR));
}

void ofxGtkWidget::showCursor(){
	glArea.get_window()->set_cursor(Gdk::Cursor::create(Gdk::CursorType::X_CURSOR));
}

void ofxGtkWidget::setWindowPosition(int x, int y){
	Gtk::Window *win = dynamic_cast<Gtk::Window *>(glArea.get_toplevel());
	if(win)
		win->move(x, y);
}

void ofxGtkWidget::setWindowShape(int w, int h){
	Gtk::Window *win = dynamic_cast<Gtk::Window *>(glArea.get_toplevel());
	if(win)
		win->resize(w, h);
}

ofPoint ofxGtkWidget::getWindowPosition(){
	Gtk::Window *win = dynamic_cast<Gtk::Window *>(glArea.get_toplevel());
	if(win){
		int x, y;
		win->get_position(x, y);
		return ofPoint(x, y);
	}
	return ofPoint(0, 0);
}

ofPoint ofxGtkWidget::getScreenSize(){
	ofPoint(glArea.get_screen()->get_width(), glArea.get_screen()->get_height());
}

void ofxGtkWidget::setWindowTitle(string title){
	Gtk::Window *win = dynamic_cast<Gtk::Window *>(glArea.get_toplevel());
	if(win)
		win->set_title(title);
}

void ofxGtkWidget::setFullscreen(bool fullscreen){
	if(fullscreen){
		//glArea.get_window()->set_opacity(.1);
		/*
		Gtk::Window *win = dynamic_cast<Gtk::Window *>(bin.get_toplevel());
		if(win)
			win->hide();
		*/

		widget().remove();
		fullscreenWindow.show();
		fullscreenWindow.present();
		//fullscreenWindow.add(glArea);
		//glArea.get_window()->fullscreen();
	}else{
		fullscreenWindow.get_window()->unfullscreen();
		fullscreenWindow.hide();
		//glArea.get_window()->unfullscreen();
	}
	bFullscreen = fullscreen;
}

void ofxGtkWidget::toggleFullscreen(){
	setFullscreen(!bFullscreen);
}

//////////////////////////////////////////////////////////
void ofxGtkWidget::onRealize(){

	//glArea.set_window(Gdk::Window::create());

	//////////////////////////////////////////////////
	if(!app){
		ofLogError("ofxGtkWidget") << "no app set";
		return;
	}

	glArea.make_current();

	try
	{
		glArea.throw_if_error();

		glewExperimental = GL_TRUE;
		glewInit();

		ofGLProgrammableRenderer* renderer = new ofGLProgrammableRenderer(this);
		currentRenderer = shared_ptr<ofGLProgrammableRenderer>(renderer);

		int glMajor, glMinor;
		glArea.get_context()->get_version(glMajor, glMinor);
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

///////////////////////////////////////////////////////////
bool ofxGtkWidget::onTimeout(){
	setFPS(events().getTargetFrameRate());
	glArea.queue_render();
	return true;
}

bool ofxGtkWidget::onRender(const Glib::RefPtr<Gdk::GLContext>& /*context*/){

	glArea.make_current();

	if(!bSetup){
		glArea.get_toplevel()->signal_key_press_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onKeyDown));
		glArea.get_toplevel()->signal_key_release_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onKeyUp));

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
	glArea.grab_focus();
	events().notifyMousePressed(evt->x, evt->y, evt->button - 1);
	return true;
}

bool ofxGtkWidget::onButtonRelease(GdkEventButton *evt){
	events().notifyMouseReleased(evt->x, evt->y, evt->button - 1);
	return true;
}

bool ofxGtkWidget::onMouseLeave(GdkEventCrossing *evt){
	events().notifyMouseExited(evt->x, evt->y);
}

bool ofxGtkWidget::onMouseEnter(GdkEventCrossing *evt){
	events().notifyMouseEntered(evt->x, evt->y);
}

void ofxGtkWidget::onShowFullscreen(){
	fullscreenWindow.add(glArea);
	glArea.get_window()->fullscreen();
}

void ofxGtkWidget::onHideFullscreen(){
	glArea.get_window()->unfullscreen();
	fullscreenWindow.remove();
	widget().add(glArea);
}

