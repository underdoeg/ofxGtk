#ifndef OFXGTKWIDGET_H
#define OFXGTKWIDGET_H

#include <gtkmm.h>
#include <ofMain.h>

class ofxGtkWidget: public ofAppBaseGLWindow, public Gtk::Frame{
public:
	ofxGtkWidget();
	//~ofxGtkWidget();

	//
	static bool doesLoop(){ return true; }
	static void loop(){};
	//

	void setApp(ofBaseApp* app);
	void setup(const ofGLWindowSettings &settings);
	void makeCurrent();
	void update();
	void draw();
	ofCoreEvents &events();
	shared_ptr<ofBaseRenderer> &renderer();
	void setFPS(int fps);

	int getWidth();
	int getHeight();
	glm::vec2 getWindowSize();

	void hideCursor();
	void showCursor();
	void setWindowPosition(int x, int y);
	void setWindowShape(int w, int h);
	glm::vec2 getWindowPosition();
	glm::vec2 getScreenSize();
	void setWindowTitle(string title);
	void setFullscreen(bool fullscreen);
	void toggleFullscreen();

private:
	void onRealize();
	bool onTimeout();
	bool onRender(const Glib::RefPtr<Gdk::GLContext>& context);
	bool onKeyUp(GdkEventKey* keyEvent);
	bool onKeyDown(GdkEventKey* keyEvent);
	bool onMotionNotify(GdkEventMotion* evt);
	bool onButtonPress(GdkEventButton* evt);
	bool onButtonRelease(GdkEventButton* evt);
	bool onMouseLeave(GdkEventCrossing* evt);
	bool onMouseEnter(GdkEventCrossing* evt);
	bool onScroll(GdkEventScroll* evt);

	void onShowFullscreen();
	void onHideFullscreen();

	unsigned int fps;
	sigc::connection fpsConnection;

	shared_ptr<ofBaseRenderer> currentRenderer;
	ofCoreEvents coreEvents;
	//shared_ptr<ofBaseApp> app;
	bool bSetup;
	bool bFullscreen;
	//Glib::RefPtr<Gdk::Window> m_refGdkWindow;

	Gtk::Window fullscreenWindow;
	Gtk::GLArea glArea;

	bool bCursor;
};

#endif // OFXGTKWIDGET_H
