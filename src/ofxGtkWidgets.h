#ifndef OFXWIDGETS_H
#define OFXWIDGETS_H

#include <gtkmm.h>
#include "ofxGtkUtils.h"
#include "ofMain.h"
#include "ofxGtkWrapper.h"


class ofxGtkSwitch: public Gtk::Switch{
public:
	ofxGtkSwitch(ofParameter<bool>& param);
	ofxGtkSwitch();

	void set(ofParameter<bool>& param);

private:
	void update();

	ofParameter<bool> param;
	sigc::connection con;
	ofEventListener listener;
};


/////////////////

template<typename Type, typename GtkWidget>
class ofxGtkNumericWidget: public GtkWidget{
public:
	ofxGtkNumericWidget(){
		set(param);
	}

	void set(ofParameter<Type>& p){
		param.makeReferenceTo(p);

		if(con){
			con.disconnect();
		}

		listener.unsubscribe();

		if(p.isReadOnly())
			GtkWidget::set_sensitive(false);

		if(!adjustment){
			createAdjustment();
		}

		update();

		listener = param.newListener([&](const bool& b){
			GtkWidget::set_value(static_cast<double>(param.get()));
		});

		con = GtkWidget::signal_value_changed().connect([&](){
			param.set(static_cast<Type>(GtkWidget::get_value()));
		});
	}

protected:
	void update(){
		adjustment->set_lower(static_cast<double>(param.getMin()));
		adjustment->set_upper(static_cast<double>(param.getMax()));
		GtkWidget::set_value(static_cast<double>(param.get()));
	}

	void createAdjustment(){
		adjustment = Gtk::Adjustment::create(0, 0, 1024);
		std::size_t type = typeid(Type).hash_code();

		if(type == typeid(float).hash_code()){
            adjustment->set_step_increment(.01);
		}else if(type == typeid(double).hash_code()){
			adjustment->set_step_increment(.001);
		}

		GtkWidget::set_adjustment(adjustment);
	}

	ofParameter<Type> param;
	sigc::connection con;
	Glib::RefPtr<Gtk::Adjustment> adjustment;
	ofEventListener listener;
};

//////////////////////////////////////////////////////////////////////////
template<typename Type=float>
class ofxGtkScale: public ofxGtkNumericWidget<Type, Gtk::Scale>{
public:
	ofxGtkScale(){}
	ofxGtkScale(ofParameter<Type>& p){
		ofxGtkNumericWidget<Type, Gtk::Scale>::set(p);
	}
};


template<typename Type>
class ofxGtkSpinButton: public ofxGtkNumericWidget<Type, Gtk::SpinButton>{
public:
	ofxGtkSpinButton(){}

	ofxGtkSpinButton(ofParameter<Type>& p){
		ofxGtkNumericWidget<Type, Gtk::SpinButton>::set(p);
	}
};


///////////////////////////////////////////////////////////////////////////////
class ofxGtkColor: public Gtk::ColorButton{
public:

	ofxGtkColor(){}

	ofxGtkColor(ofParameter<ofColor>& p){
		set(p);
	}

	void set(ofParameter<ofColor>& p){

		param.makeReferenceTo(p);

		if(p.isReadOnly())
			set_sensitive(false);

		if(con)
			con.disconnect();

		listener.unsubscribe();

		set_rgba(toGtk(param.get()));

		listener = param.newListener([&](const ofColor& col){
			set_rgba(toGtk(col));
		});

		con = signal_color_set().connect([&] {
			param.set(toOf(get_rgba()));
		});
	}

private:
	ofParameter<ofColor> param;
	sigc::connection con;
	ofEventListener listener;
};

//////////////////////////////////////////////////////////////////////////////////
class ofxGtkEntry: public Gtk::Entry{
public:
	ofxGtkEntry(){}

	ofxGtkEntry(ofParameter<std::string>& p){
		set(p);
	}

	void set(ofParameter<std::string>& p){

		param.makeReferenceTo(p);

		if(p.isReadOnly())
			set_sensitive(false);

		if(con)
			con.disconnect();

		listener.unsubscribe();

		set_text(param.get());

		listener = param.newListener([&](const std::string& col){
			set_text(param.get());
		});

		con = signal_changed().connect([&] {
			param.set(get_text());
		});

	}

private:
	ofParameter<std::string> param;
	sigc::connection con;
	ofEventListener listener;
};

//////////////////////////////////////////////////////////////////////////////////
class ofxGtkVec2: public Gtk::HBox{
public:
    ofxGtkVec2(){
        set_spacing(2);

        adjustmentX = Gtk::Adjustment::create(0, 0, 1024);
        x.set_adjustment(adjustmentX);
        //x.set_adjustment(Gtk::Adjustment::create(0, 0, 1024));
        //y.set_adjustment(Gtk::Adjustment::create(0, 0, 1024));
        x.get_adjustment()->set_step_increment(.001);
        y.get_adjustment()->set_step_increment(.001);

        pack_start(x, true, true);
        pack_start(y, true, true);
        show_all();
    }

    ofxGtkVec2(ofParameter<ofVec2f>& p):ofxGtkVec2(){
        set(p);
    }

    void set(ofParameter<ofVec2f>& p){

        param.makeReferenceTo(p);


        ofVec2f min = p.getMin();
        ofVec2f max = p.getMax();


        //this is a hack, because of does  set min and max of ofVec2f params to 0 and 1
        if(min.distance(max) < 2){
            min = ofVec2f(std::numeric_limits<float>::min());
            max = ofVec2f(std::numeric_limits<float>::max());
        }

        x.get_adjustment()->set_lower(min.x);
        x.get_adjustment()->set_upper(max.x);
        y.get_adjustment()->set_lower(min.y);
        y.get_adjustment()->set_upper(max.y);

        if(p.isReadOnly())
            set_sensitive(false);

        if(conX)
            conX.disconnect();

        if(conY)
            conY.disconnect();

        listener.unsubscribe();

        setFromVec(p.get());

        listener = param.newListener([&](ofVec2f& col){
            setFromVec(col);
        });

        conX = x.signal_value_changed().connect([&] {
            ofVec2f v = param;
            v.x = x.get_value();
            param = v;
        });

        conY = y.signal_value_changed().connect([&] {
            ofVec2f v = param;
            v.y = y.get_value();
            param = v;
        });
    }

private:
    void setFromVec(const ofVec2f& v){
        x.set_value((double)v.x);
        y.set_value((double)v.y);
    }

    ofParameter<ofVec2f> param;
    sigc::connection conX;
    sigc::connection conY;
    ofEventListener listener;

    Glib::RefPtr<Gtk::Adjustment> adjustmentX;

    Gtk::SpinButton x;
    Gtk::SpinButton y;
};

#endif // OFXWIDGETS_H
