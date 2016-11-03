#include "ofxGtkWidget.h"

//based on https://developer.gnome.org/gtkmm-tutorial/stable/sec-custom-widgets.html.en

ofxGtkWidget::ofxGtkWidget(){

	bCursor = true;
	bSetup = false;
	bFullscreen = false;

	glArea.add_events(Gdk::KEY_PRESS_MASK | Gdk::STRUCTURE_MASK | Gdk::KEY_RELEASE_MASK
					  | Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK
					  | Gdk::BUTTON_MOTION_MASK | Gdk::BUTTON1_MOTION_MASK | Gdk::BUTTON2_MOTION_MASK | Gdk::BUTTON3_MOTION_MASK
					  | Gdk::SCROLL_MASK);
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
	glArea.signal_scroll_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onScroll));

	fullscreenWindow.signal_show().connect(sigc::mem_fun(this, &ofxGtkWidget::onShowFullscreen));
	fullscreenWindow.signal_hide().connect(sigc::mem_fun(this, &ofxGtkWidget::onHideFullscreen));
	fullscreenWindow.signal_key_press_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onKeyDown));
	fullscreenWindow.signal_key_release_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onKeyUp));

	//events().setFrameRate(60);

	setFPS(200);

	glArea.set_auto_render();

	set_shadow_type(Gtk::ShadowType::SHADOW_NONE);
	add(glArea);
}

//ofxGtkWidget::~ofxGtkWidget(){
//}

void ofxGtkWidget::setApp(ofBaseApp *a){
	//auto app = ;
	ofRunApp(shared_ptr<ofxGtkWidget>(this), shared_ptr<ofBaseApp>(a));
}

void ofxGtkWidget::setup(const ofGLWindowSettings &settings){
	makeCurrent();
}

void ofxGtkWidget::makeCurrent(){
	std::shared_ptr<ofMainLoop> mainLoop = ofGetMainLoop();
	if(mainLoop){
		mainLoop->setCurrentWindow(this);
	}else{
		ofLog() << "NO MAINLOOP";
	}
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

glm::vec2 ofxGtkWidget::getWindowSize(){
	return  {getWidth(), getHeight()};
}

void ofxGtkWidget::hideCursor(){
	bCursor = false;
	glArea.get_window()->set_cursor(Gdk::Cursor::create(Gdk::CursorType::BLANK_CURSOR));
}

void ofxGtkWidget::showCursor(){
	bCursor = true;
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

glm::vec2 ofxGtkWidget::getWindowPosition(){
	Gtk::Window *win = dynamic_cast<Gtk::Window *>(glArea.get_toplevel());
	if(win){
		int x, y;
		win->get_position(x, y);
		return {x, y};
	}
	return {0, 0};
}

glm::vec2 ofxGtkWidget::getScreenSize(){
	return {glArea.get_screen()->get_width(), glArea.get_screen()->get_height()};
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

		remove();
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
//	if(!app){
//		ofLogError("ofxGtkWidget") << "ofApp is not set";
//		return;
//	}

	glArea.make_current();

	try{
		glArea.throw_if_error();

		glewExperimental = GL_TRUE;
		glewInit();

		ofGLProgrammableRenderer* renderer = new ofGLProgrammableRenderer(this);
		currentRenderer = shared_ptr<ofGLProgrammableRenderer>(renderer);

		int glMajor, glMinor;
		glArea.get_context()->get_version(glMajor, glMinor);
		renderer->setup(glMajor, glMinor);
		//

	}catch(const Gdk::GLError& gle){
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
	makeCurrent();

	if(!bSetup){
		bSetup = true;
		glArea.get_toplevel()->signal_key_press_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onKeyDown));
		glArea.get_toplevel()->signal_key_release_event().connect(sigc::mem_fun(this, &ofxGtkWidget::onKeyUp));
	}

	currentRenderer->startRender();
	events().notifyUpdate();

	currentRenderer->setupScreen();
	events().notifyDraw();

	currentRenderer->finishRender();
	return true;
}

int getKeyVal(GdkEventKey* keyEvent){
	guint key = keyEvent->keyval;
	int ret = gdk_keyval_to_unicode(key);

	//handle special keys
	/*
	switch(key){
	case GDK_KEY_Return:
		ret = OF_KEY_RETURN;
		break;
	case GDK_KEY_Escape:
		ret = OF_KEY_ESC;
	}
	*/

	return ret;
}

bool ofxGtkWidget::onKeyUp(GdkEventKey* keyEvent){
	events().notifyKeyReleased(getKeyVal(keyEvent));
	return true;
}

bool ofxGtkWidget::onKeyDown(GdkEventKey* keyEvent){
	events().notifyKeyPressed(getKeyVal(keyEvent));
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
	if(!bCursor){
		showCursor();
		bCursor = false;
	}
	events().notifyMouseExited(evt->x, evt->y);
	return true;
}

bool ofxGtkWidget::onMouseEnter(GdkEventCrossing *evt){
	if(!bCursor)
		hideCursor();
	events().notifyMouseEntered(evt->x, evt->y);
	return true;
}

bool ofxGtkWidget::onScroll(GdkEventScroll *evt){
	int scrollX = 0;
	int scrollY = 0;

	//TODO: smooth scrolling
	if(evt->direction == GDK_SCROLL_UP){
		scrollY = -1;
	}else if(evt->direction == GDK_SCROLL_DOWN){
		scrollY = 1;
	}else if(evt->direction == GDK_SCROLL_LEFT){
		scrollX = -1;
	}else if(evt->direction == GDK_SCROLL_RIGHT){
		scrollX = 1;
	}

	events().notifyMouseScrolled(evt->x, evt->y, scrollX, scrollY);
	return true;
}

void ofxGtkWidget::onShowFullscreen(){
	fullscreenWindow.add(glArea);
	glArea.get_window()->fullscreen();
}

void ofxGtkWidget::onHideFullscreen(){
	glArea.get_window()->unfullscreen();
	fullscreenWindow.remove();
	add(glArea);
}

