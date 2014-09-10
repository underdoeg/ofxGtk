#ifndef OFXGTKWIDGET_H
#define OFXGTKWIDGET_H

#include "ofxGtkUtils.h"
#include <glibmm.h>
#include <X11/Xlib.h>

#include "ofMain.h"

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092

class ofxGtkWidget: public Gtk::Widget, public ofAppBaseGLWindow {
public:

	class ButtonStates {
	public:
		ButtonStates();
		bool isPressed(int button);
		bool isAnyPressed();
		void releaseAll();
	private:
		void setPressed(int button);
		void setReleased(int button);
		std::map<int, bool> states;
		friend class ofxGtkWidgets;
	};

	ofxGtkWidget();
	~ofxGtkWidget();

	void setup(ofBaseApp* app);
	void setOpenGLVersion(int glVersionMajor, int glVersionMinor);
	virtual void setupOpenGL(int w, int h, ofWindowMode screenMode);

	int	getWidth();
	int	getHeight();
	ofPoint getWindowSize();

	void windowShouldClose();
	void toggleFullscreen();
	void setFullscreen(bool fullscreen);

	void setNumSamples(int samples);
	void setDoubleBuffering(bool doubleBuff);
	void setColorBits(int r, int g, int b);
	void setAlphaBits(int a);
	void setDepthBits(int depth);
	void setStencilBits(int stencil);
	
	void setRootWindow(Gtk::Window* win);
	
	ofBaseApp* app;

private:

	bool draw();

	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	bool on_key_press_event(GdkEventKey* key);
	bool on_configure_event(GdkEventConfigure* evt);
	bool on_motion_notify_event(GdkEventMotion* event);
	bool on_drag_motion(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, guint time);
	bool on_button_press_event(GdkEventButton* evt);
	bool on_button_release_event(GdkEventButton* evt);

	Gtk::SizeRequestMode get_request_mode_vfunc() const;
	void get_preferred_width_vfunc(int& minimum_width, int& natural_width) const;
	void get_preferred_height_for_width_vfunc(int width, int& minimum_height, int& natural_height) const;
	void get_preferred_height_vfunc(int& minimum_height, int& natural_height) const;
	void get_preferred_width_for_height_vfunc(int height, int& minimum_width, int& natural_width) const;
	void on_size_allocate(Gtk::Allocation& allocation);
	void on_map();
	void on_unmap();

	bool on_idle();

	void on_realize();
	void on_unrealize();

	Glib::RefPtr<Gdk::Window> refWinGdk;

	int glVersionMinor, glVersionMajor;
	bool bDoubleBuffered;
	int samples;
	int	rBits,gBits,bBits,aBits,depthBits,stencilBits;


	Display* display;
	GLXContext context;
	bool isSetup;
	int timerNumber;

	bool isFullscreen;
	
	Gtk::Window* rootWindow;

};

#endif // OFXGTKWIDGET_H
