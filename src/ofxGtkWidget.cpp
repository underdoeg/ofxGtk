#include "ofxGtkWidget.h"
#include <gtk-3.0/gtk/gtkx.h>
#include "ofxGtkUtils.h"
#include "ofxGtkApp.h"

void ofGLReadyCallback();

/////////////////////////////////////////////////////////////////////////////////////////////

ofxGtkWidget::ButtonStates::ButtonStates() {
	for(unsigned int i=0; i<4; i++) {
		states[i] = false;
	}
}

bool ofxGtkWidget::ButtonStates::isPressed(int button) {
	return states[button];
}

bool ofxGtkWidget::ButtonStates::isAnyPressed() {
	for(auto state: states) {
		if(state.second == true)
			return true;
	}
	return false;
}

void ofxGtkWidget::ButtonStates::setPressed(int button) {
	states[button] = true;
}

void ofxGtkWidget::ButtonStates::setReleased(int button) {
	states[button] = false;
}

void ofxGtkWidget::ButtonStates::releaseAll() {
	for(auto state: states)
		setReleased(state.first);
}

/////////////////////////////////////////////////////////////////////////////////////////////

//code based on
// https://developer.gnome.org/gtkmm-tutorial/3.12/sec-custom-widgets.html.en
// http://ewgeny.wordpress.com/2012/12/11/turorial-for-opengl-and-gtk3-combined-written-in-vala/

ofxGtkWidget::ofxGtkWidget():
	Glib::ObjectBase("ofxWidget"),
	Gtk::Widget() {
	app = NULL;
	set_has_window(true);
	isSetup = false;
	isFullscreen = false;

	add_events(Gdk::KEY_PRESS_MASK | Gdk::STRUCTURE_MASK | Gdk::KEY_RELEASE_MASK
	           | Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK
	           | Gdk::BUTTON_MOTION_MASK | Gdk::BUTTON1_MOTION_MASK | Gdk::BUTTON2_MOTION_MASK | Gdk::BUTTON3_MOTION_MASK);

	//signal_key_press_event().connect ( sigc::mem_fun(*this, &ofxGtkWidget::on_key_press_event) );
}

ofxGtkWidget::~ofxGtkWidget() {

}

void ofxGtkWidget::setup(ofBaseApp* a) {
	samples				= 0;
	rBits=gBits=bBits=aBits = 8;
	depthBits			= 24;
	stencilBits			= 0;

	//orientation 		= OF_ORIENTATION_DEFAULT;

	bDoubleBuffered		= true;

	//ofAppPtr			= NULL;
	//instance			= this;

	//ofSetFrameRate()

	glVersionMinor=glVersionMajor=-1;

	setNumSamples(4);


	app = a;

	//rest happens on first realize when the X11 window is created
}


void ofxGtkWidget::setupOpenGL(int w, int h, int screenMode) {
	//opengl
	display = gdk_x11_get_default_xdisplay();

	//ofSetLogLevel("ofAppRunner", OF_LOG_VERBOSE);

	/*
	int attrlist[] = {
		GLX_RGBA, GLX_DOUBLEBUFFER,
		GLX_RED_SIZE, rBits,
		GLX_GREEN_SIZE, gBits,
		GLX_BLUE_SIZE, bBits,
		GLX_ALPHA_SIZE, aBits,
		GLX_DEPTH_SIZE, depthBits,
		GLX_STENCIL_SIZE, stencilBits,
		None
	};
	*/

	int doubleBufferedAttribList [] = {
		GLX_RGBA, GLX_DOUBLEBUFFER,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_DEPTH_SIZE, 16,
		None
	};

	//int glx_major, glx_minor;
	//glXQueryVersion(display, &glx_major, &glx_minor);
	//cout << "OPENGL VERSION IS " << glx_major << "." << glx_minor << endl;
	//cout << "OPENGL VERSION SHOULD BE " << glVersionMajor << "." << glVersionMinor << endl;


	XVisualInfo* info = glXChooseVisual(display, gdk_x11_get_default_screen(), doubleBufferedAttribList);
	context = glXCreateContext(display, info, NULL, true);

	//Create a GL 3.0 context
	/*
	int attribs[] = {
	    GLX_CONTEXT_MAJOR_VERSION_ARB, 3,//we want a 3.0 context
	    GLX_CONTEXT_MINOR_VERSION_ARB, 0,
	    0 //zero indicates the end of the array
	};

	GLXFBConfig* fbConfigs = glXChooseFBConfig( display, DefaultScreen(display), attribs, &amp;numberOfFramebufferConfigurations );

	context = glXCreateContextAttribs(display, GLXFBConfig, 0, true, &attribs[0]);
	 */

	if( glXMakeCurrent(display, gdk_x11_window_get_xid(refWinGdk->gobj()), context ) )
		ofGLReadyCallback();
}

/////////////////////////////////////////////////////////

bool ofxGtkWidget::on_idle() {
	ofNotifyUpdate();
	draw();
	return true;
}

bool ofxGtkWidget::draw() {
	if(!app)
		return false;

	if( !glXMakeCurrent(display, gdk_x11_window_get_xid(refWinGdk->gobj()), context ) )
		return false;


	if(!isSetup) {
		app->setup();
		isSetup = true;
	}

	//cout << toOf( get_style_context()->get_background_color(Gtk::STATE_FLAG_NORMAL) ) << endl;

	//begin opengl rendering
	ofViewport();		// used to be glViewport( 0, 0, width, height );

	float * bgPtr = ofBgColorPtr();
	bool bClearAuto = ofbClearBg();

#ifdef TARGET_WIN32
	if (bClearAuto == false) {
		glDrawBuffer (GL_FRONT);
	}
#endif

	if ( bClearAuto == true ) {
		ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);
	}

	ofSetupScreen();

	ofNotifyDraw();

	glXSwapBuffers(display, gdk_x11_window_get_xid(refWinGdk->gobj()) );

	return true;
}

bool ofxGtkWidget::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	ofNotifyUpdate();
	return draw();
}


//////// OF WRAP

int ofxGtkWidget::getWidth() {
	return get_allocated_width();
}

int ofxGtkWidget::getHeight() {
	return get_allocated_height();
}

ofPoint ofxGtkWidget::getWindowSize() {
	return ofPoint(getWidth(), getHeight());
}

void ofxGtkWidget::windowShouldClose() {
	rootWindow->close();
}

void ofxGtkWidget::setFullscreen(bool fullscreen) {
	Gtk::Window* win = static_cast<Gtk::Window*>(get_toplevel());
	if(win) {
		if(fullscreen) {
			win->fullscreen();
		} else {
			win->unfullscreen();
		}
	}

	isFullscreen = fullscreen;
}

void ofxGtkWidget::toggleFullscreen() {
	setFullscreen(!isFullscreen);
}

///OpenGL

//------------------------------------------------------------
void ofxGtkWidget::setNumSamples(int _samples) {
	samples=_samples;
}

//------------------------------------------------------------
void ofxGtkWidget::setDoubleBuffering(bool doubleBuff) {
	bDoubleBuffered = doubleBuff;
}

//------------------------------------------------------------
void ofxGtkWidget::setColorBits(int r, int g, int b) {
	rBits=r;
	gBits=g;
	bBits=b;
}

//------------------------------------------------------------
void ofxGtkWidget::setAlphaBits(int a) {
	aBits=a;
}

//------------------------------------------------------------
void ofxGtkWidget::setDepthBits(int depth) {
	depthBits=depth;
}

//------------------------------------------------------------
void ofxGtkWidget::setStencilBits(int stencil) {
	stencilBits=stencil;
}

void ofxGtkWidget::setOpenGLVersion(int major, int minor) {
	glVersionMajor = major;
	glVersionMinor = minor;
}

/////////////

bool ofxGtkWidget::on_configure_event(GdkEventConfigure* evt) {
	return true;
}

bool ofxGtkWidget::on_key_press_event(GdkEventKey* key) {
	return true;
}

bool ofxGtkWidget::on_motion_notify_event(GdkEventMotion* evt) {
	if(evt->state & GDK_BUTTON1_MASK) {
		ofNotifyMouseDragged(evt->x, evt->y, 0);
	} else if(evt->state & GDK_BUTTON2_MASK) {
		ofNotifyMouseDragged(evt->x, evt->y, 1);
	}
	if(evt->state & GDK_BUTTON3_MASK) {
		ofNotifyMouseDragged(evt->x, evt->y, 2);
	} else {
		ofNotifyMouseMoved(evt->x, evt->y);
	}
	return true;
}

bool ofxGtkWidget::on_button_press_event(GdkEventButton* evt) {
	ofNotifyMousePressed(evt->x, evt->y, evt->button);
	return true;
}

bool ofxGtkWidget::on_button_release_event(GdkEventButton* evt) {
	ofNotifyMouseReleased(evt->x, evt->y, evt->button);
	return true;
}

bool ofxGtkWidget::on_drag_motion(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, guint time) {

	return true;
}

void ofxGtkWidget::get_preferred_height_for_width_vfunc(int width, int& minimum_height, int& natural_height) const {
	minimum_height = 150;
	natural_height = 9999;
}

void ofxGtkWidget::get_preferred_height_vfunc(int& minimum_height, int& natural_height) const {
	minimum_height = 150;
	natural_height = 9999;
}

void ofxGtkWidget::get_preferred_width_for_height_vfunc(int height, int& minimum_width, int& natural_width) const {
	minimum_width = 150;
	natural_width = 9999;
}

void ofxGtkWidget::get_preferred_width_vfunc(int& minimum_width, int& natural_width) const {
	minimum_width = 150;
	natural_width = 9999;
}

Gtk::SizeRequestMode ofxGtkWidget::get_request_mode_vfunc() const {
	return Gtk::Widget::get_request_mode_vfunc();
}

void ofxGtkWidget::on_map() {
	Gtk::Widget::on_map();
}

void ofxGtkWidget::on_unmap() {
	Gtk::Widget::on_unmap();
}

void ofxGtkWidget::on_size_allocate(Gtk::Allocation& allocation) {
	set_allocation(allocation);

	//GLsizei width = allocation.get_width();
	//GLsizei height = allocation.get_height();
	//GLsizei size = min( width, height );

	//glViewport( (width - size ) / 2, (height - size ) / 2, size , size );

	if(refWinGdk) {
		refWinGdk->move_resize( allocation.get_x(), allocation.get_y(),
		                        allocation.get_width(), allocation.get_height() );
	}
}

void ofxGtkWidget::on_realize() {
	//Do not call base class Gtk::Widget::on_realize().
	//It's intended only for widgets that set_has_window(false).

	//Gtk::Widget::on_realize();

	set_realized();

	if(!refWinGdk) {
		//Create the GdkWindow:

		GdkWindowAttr attributes;
		memset(&attributes, 0, sizeof(attributes));

		Gtk::Allocation allocation = get_allocation();

		//Set initial position and size of the Gdk::Window:
		attributes.x = allocation.get_x();
		attributes.y = allocation.get_y();
		attributes.width = allocation.get_width();
		attributes.height = allocation.get_height();

		attributes.event_mask = get_events () | Gdk::EXPOSURE_MASK;
		attributes.window_type = GDK_WINDOW_CHILD;
		attributes.wclass = GDK_INPUT_OUTPUT;

		refWinGdk = Gdk::Window::create(get_parent_window(), &attributes,
		                                GDK_WA_X | GDK_WA_Y);
		set_window(refWinGdk);

		//make the widget receive expose events
		refWinGdk->set_user_data(gobj());

		ofSetupOpenGL(this, 100, 100, OF_WINDOW);
		ofRunApp(app);

		ofBackground(120);

		//connect idle function
		Glib::signal_idle().connect( sigc::mem_fun(*this, &ofxGtkWidget::on_idle) );
	}
}

void ofxGtkWidget::on_unrealize() {
	if(refWinGdk)
		refWinGdk.reset();

	//Call base class:
	Gtk::Widget::on_unrealize();
}

void ofxGtkWidget::setRootWindow(Gtk::Window* win) {
	rootWindow = win;
}
