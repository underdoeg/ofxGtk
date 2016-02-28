#include "ofxGtk.h"
#include "ofApp.h"

class ExampleWindow: public Gtk::Window{

	Gtk::Box m_VBox {Gtk::ORIENTATION_VERTICAL, false};
	ofxGtkWidget ofWidget;
	Gtk::HBox box;
	Gtk::Button btn;

public:
	ExampleWindow():btn("BUTTON"){

		set_title("ofxGtk Example");

		ofWidget.setApp(new ofApp);


		add(box);

		box.pack_start(btn, Gtk::PACK_SHRINK);
		box.pack_start(ofWidget, Gtk::PACK_EXPAND_WIDGET);

		ofWidget.widget().show();

		show_all();
	}
};

int main(int argc, char *argv[]) {

	auto app = Gtk::Application::create(argc, argv, "cc.openframeworks.ofxGtk.example");

	ExampleWindow window;
	//window.set_default_size(1280, 720);
	window.set_size_request(1280, 720);

	return app->run(window);
}
