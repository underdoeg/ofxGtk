/*
#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
*/


#include <gtkmm.h>
#include "ofxGtkWidget.h"
#include "ofApp.h"

class ExampleWindow : public Gtk::Window {
public:

	ExampleWindow():m_VBox(Gtk::ORIENTATION_VERTICAL), m_Button_Quit("Quit") {
		set_default_size(1280, 720);
		set_title("ofxGtk Example");

		add(m_VBox);

		ofWidget.setup(new ofApp());

		m_VBox.pack_start(ofWidget, Gtk::PACK_EXPAND_WIDGET);
		ofWidget.show();

		m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

		m_ButtonBox.pack_start(m_Button_Quit, Gtk::PACK_SHRINK);
		m_ButtonBox.set_border_width(6);
		m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
		//m_Button_Quit.signal_clicked().connect( sigc::mem_fun(*this, &ExampleWindow::on_button_quit) );

		show_all_children();

	}

	virtual ~ExampleWindow() {

	}

private:
	//Override default signal handler:
	virtual bool on_key_press_event(GdkEventKey* event) {
		return false;
	}

	ofxGtkWidget ofWidget;
	Gtk::Box m_VBox;
	Gtk::ButtonBox m_ButtonBox;
	Gtk::Button m_Button_Quit;
};


int main(int argc, char *argv[]) {
	Glib::RefPtr<Gtk::Application> app =
	    Gtk::Application::create(argc, argv,
	                             "cc.openframeworks.ofxGtkExample");
	ExampleWindow window;

	return app->run(window);
}
