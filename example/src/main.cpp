#include "ofxGtk.h"
#include "ofApp.h"

class ExampleWindow : public ofxGtkWindow<ofApp> {
public:

	ExampleWindow():m_VBox(Gtk::ORIENTATION_VERTICAL), fillBtn("filled"), 
		radiusBtn(Gtk::ICON_SIZE_LARGE_TOOLBAR, 5, 800, 1, std::vector<Glib::ustring>()){

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
		
		
		//add the openframeworks drawing area
		add(ofWidget);
				
		//header bar
		set_titlebar(headerBar);
		headerBar.set_show_close_button(true);
		
		//add the color picker button
		headerBar.pack_start(fillBtn);
		headerBar.pack_start(buttonColorPick);
		headerBar.pack_start(radiusBtn);
		
		buttonColorPick.set_color(Gdk::Color("#fff"));
		fillBtn.set_active(true);
		radiusBtn.set_image_from_icon_name("gtk-zoom-in", Gtk::ICON_SIZE_BUTTON);

		buttonColorPick.signal_color_set().connect( sigc::mem_fun(*this, &ExampleWindow::onColorPick) );
		fillBtn.signal_clicked().connect( sigc::mem_fun(*this, &ExampleWindow::onFillChange) );
		radiusBtn.signal_value_changed().connect( sigc::mem_fun(*this, &ExampleWindow::onScaleChange) );

		show_all_children();
	}

private:
	void onColorPick() {
		app->color = toOf(buttonColorPick.get_color());
	}

	void onFillChange() {
		if(fillBtn.get_state() == Gtk::STATE_ACTIVE){
			app->drawFilled = true;
		}else{
			app->drawFilled = false;
		}
	}
	
	void onScaleChange(double value) {
		app->drawScale = value;
	}

	Gtk::Box m_VBox;
	Gtk::ButtonBox m_ButtonBox;
	Gtk::ColorButton buttonColorPick;
	Gtk::HeaderBar headerBar;
	Gtk::ToggleButton fillBtn;
	Gtk::ScaleButton radiusBtn;
	Gtk::MenuBar menu;
};


int main(int argc, char *argv[]) {

	ofxGtkApp app("cc.openframeworks.ofxGtkExample");

	ExampleWindow window;

	//uncomment to setup the of widget to fill up the entire window
	//window.setupDefault();

	return app.run(&window);
}
