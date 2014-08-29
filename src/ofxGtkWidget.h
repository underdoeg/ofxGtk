#ifndef OFXGTKWIDGET_H
#define OFXGTKWIDGET_H

#include <gtkmm/widget.h>
#include <gtkmm/cssprovider.h>
#include <X11/Xlib.h>
#include "ofMain.h"

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092

class ofxGtkWidget: public Gtk::Widget, public ofAppBaseGLWindow {
public:
	ofxGtkWidget();
	~ofxGtkWidget();

	void setup(ofBaseApp* app);
	void setupOpenGL(int w, int h, int screenMode);
	void setOpenGLVersion(int glVersionMajor, int glVersionMinor);

	int	getWidth();
	int	getHeight();
	ofPoint getWindowSize();

	void setNumSamples(int samples);
	void setDoubleBuffering(bool doubleBuff);
	void setColorBits(int r, int g, int b);
	void setAlphaBits(int a);
	void setDepthBits(int depth);
	void setStencilBits(int stencil);

private:

	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	bool on_key_press_event(GdkEventKey* key);
	bool on_configure_event(GdkEventConfigure* evt);
	bool on_motion_notify_event(GdkEventMotion* event);

	Gtk::SizeRequestMode get_request_mode_vfunc() const;
	void get_preferred_width_vfunc(int& minimum_width, int& natural_width) const;
	void get_preferred_height_for_width_vfunc(int width, int& minimum_height, int& natural_height) const;
	void get_preferred_height_vfunc(int& minimum_height, int& natural_height) const;
	void get_preferred_width_for_height_vfunc(int height, int& minimum_width, int& natural_width) const;
	void on_size_allocate(Gtk::Allocation& allocation);
	void on_map();
	void on_unmap();


	void on_realize();
	void on_unrealize();

	Glib::RefPtr<Gdk::Window> m_refGdkWindow;

	int glVersionMinor, glVersionMajor;
	bool bDoubleBuffered;
	int samples;
	int	rBits,gBits,bBits,aBits,depthBits,stencilBits;


	ofBaseApp* app;
	Display* display;
	GLXContext context;
	bool isSetup;
};

#endif // OFXGTKWIDGET_H
