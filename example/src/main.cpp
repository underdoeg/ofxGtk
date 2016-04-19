#include "ofxGtk.h"
#include "ofApp.h"

class ExampleWindow: public Gtk::Window{

	Gtk::Box m_VBox {Gtk::ORIENTATION_VERTICAL, false};
	ofxGtkWidget ofWidget;
	Gtk::HBox box;
	Gtk::Button btn;
	ofApp app;

public:
	ExampleWindow():btn("BUTTON"){
		set_title("ofxGtk Example");

		ofWidget.setApp(&app);

		box.set_margin_left(10);
		box.property_spacing() = 10;

		add(box);

		box.pack_start(*widgetFromParameter(app.params), Gtk::PACK_SHRINK);
		box.pack_start(ofWidget, Gtk::PACK_EXPAND_WIDGET);

		show_all_children();

	}

	// Widget interface
protected:

};

int main(int argc, char *argv[]) {

	auto app = Gtk::Application::create(argc, argv, "cc.openframeworks.ofxGtk.example");

	ExampleWindow window;
	//window.set_default_size(1280, 720);
	window.set_size_request(1280, 720);

	return app->run(window);
}
