#include "ofxGtk.h"
#include "ofApp.h"

class ExampleWindow : public ofxGtkWindow<ofApp> {
public:

	ExampleWindow():m_VBox(Gtk::ORIENTATION_VERTICAL), buttonColorPick("Select Color") {

	}

	virtual ~ExampleWindow() {
	}

	void setup() {
		
		/*
		add(m_VBox);

		//add the already created ofWidget to your custom layout
		m_VBox.pack_start(ofWidget, Gtk::PACK_EXPAND_WIDGET);

		//other buttons
		m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);
		m_ButtonBox.pack_start(buttonColorPick, Gtk::PACK_SHRINK);
		m_ButtonBox.set_border_width(6);
		m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
		*/
		
		add(ofWidget);
		
		buttonColorPick.signal_clicked().connect( sigc::mem_fun(*this, &ExampleWindow::onColorPick) );

		set_show_menubar(true);
		
		set_titlebar(headerBar);
		headerBar.set_show_close_button(true);
		headerBar.pack_start(buttonColorPick);

		show_all_children();
	}

private:
	void onColorPick() {
		ofLogNotice() << "Show color picker";
		Gtk::ColorChooserDialog picker;
		picker.run();
		app->color = toOf(picker.get_rgba());
	}

	Gtk::Box m_VBox;
	Gtk::ButtonBox m_ButtonBox;
	Gtk::Button buttonColorPick;
	Gtk::HeaderBar headerBar;
};


int main(int argc, char *argv[]) {

	ofxGtkApp app("cc.openframeworks.ofxGtkExample");

	ExampleWindow window;

	//uncomment to setup the of widget to fill up the entire window
	//window.setupDefault();

	return app.run(&window);
}
