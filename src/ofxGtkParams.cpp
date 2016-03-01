#include "ofxGtkParams.h"

ofColor toOf(Gdk::RGBA src){
	return ofShortColor(src.get_red_u(), src.get_green_u(), src.get_blue_u(), src.get_alpha_u());
}

Gdk::RGBA toGtk(ofColor color){
	ofShortColor col = color;
	Gdk::RGBA ret;
	ret.set_red_u(col.r);
	ret.set_green_u(col.g);
	ret.set_blue_u(col.b);
	ret.set_alpha_u(col.a);
	return ret;
}

////////////////////////////////////////////////
class ofxGtkScale: public ofxGtkWrapper<Gtk::Scale>{
public:
	ofxGtkScale(){
		adjustment = Gtk::Adjustment::create(0, 0, 1024);
		setWidget(new Gtk::Scale(adjustment));
	}

	void set(const ofParameter<float>& param){
		widget().set_range(param.getMin(), param.getMax());
		widget().set_value(param);
		widget().signal_value_changed().connect([&] {
			//TODO: Why is this necessary?!?! reference passing only seems to work with const for some reason...
			ofParameter<float>(param).set(widget().get_value());
		});
	}

	Glib::RefPtr<Gtk::Adjustment> adjustment;
};

class ofxGtkColor: public ofxGtkWrapper<Gtk::ColorButton>{
public:
	void set(const ofParameter<ofColor>& param){
		//widget().set_value(param);
		Gdk::RGBA col = toGtk(param.get());
		widget().set_rgba(col);
		widget().signal_color_set().connect([&] {
			ofLog() << "CHANGED";
			ofParameter<ofColor>(param).set(toOf(widget().get_rgba()));
		});
	}
};

class ofxGtkSwitch: public ofxGtkWrapper<Gtk::Switch>{
public:
	void set(const ofParameter<bool>& param){
		widget().set_state(param);
		widget().signal_state_set().connect([&](bool state)->bool {
			//ofLog() << type;
			//if(type == Gtk::StateType)
			//if(type == 2)
			ofLog() << state;
			ofParameter<bool>(param).set(state);
			return true;
		});
	}
};


////////////////////////////////////////////////////
template<>
Gtk::Widget* widgetFromParameter(const ofParameter<float>& param){
	ofxGtkScale* item = new ofxGtkScale();
	item->set(param);
	return &item->widget();
}

template<>
Gtk::Widget* widgetFromParameter(const ofParameter<ofColor>& param){
	ofxGtkColor* item = new ofxGtkColor();
	item->set(param);
	return &item->widget();
}

template<>
Gtk::Widget* widgetFromParameter(const ofParameter<bool>& param){
	ofxGtkSwitch* item = new ofxGtkSwitch();
	item->set(param);
	return &item->widget();
}

////////////////////////////////////////////////////////////////////////////////
Gtk::Widget* widgetFromParameterGroup(const ofParameterGroup& params){
	Gtk::VBox* ret = new Gtk::VBox();
	Gtk::Label* label = new Gtk::Label(params.getName());
	ret->pack_start(*label, false, false, 10);
	for(std::size_t i = 0; i < params.size(); i++){
		Gtk::Widget* widget = nullptr;
		string type = params.getType(i);
		if(type == typeid(ofParameter <int32_t> ).name()){
			//widget = widgetFromParameter(params.getInt(i));
		}else if(type == typeid(ofParameter <uint32_t> ).name()){
			auto p = params.get<uint32_t>(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameter <int64_t> ).name()){
			auto p = params.get<int64_t>(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameter <uint64_t> ).name()){
			auto p = params.get<uint64_t>(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameter <int8_t> ).name()){
			auto p = params.get<int8_t>(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameter <uint8_t> ).name()){
			auto p = params.get<uint8_t>(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameter <int16_t> ).name()){
			auto p = params.get<int16_t>(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameter <uint16_t> ).name()){
			auto p = params.get<uint16_t>(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameter <float> ).name()){
			auto& p = params.getFloat(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameter <double> ).name()){
			auto p = params.get<double>(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameter <bool> ).name()){
			auto p = params.getBool(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameter <ofVec2f> ).name()){
			auto p = params.getVec2f(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameter <ofVec3f> ).name()){
			auto p = params.getVec3f(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameter <ofVec4f> ).name()){
			widget = widgetFromParameter(params.getVec4f(i));
		}else if(type == typeid(ofParameter <ofColor> ).name()){
			widget = widgetFromParameter(params.getColor(i));
		}else if(type == typeid(ofParameter <ofShortColor> ).name()){
			auto p = params.getShortColor(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameter <ofFloatColor> ).name()){
			auto p = params.getFloatColor(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameter <string> ).name()){
			auto p = params.getString(i);
			widget = widgetFromParameter(p);
		}else if(type == typeid(ofParameterGroup).name()){
			auto p = params.getGroup(i);
			//TODO: subgroups
		}else{
			ofLogWarning() << "ofxBaseGroup; no control for parameter of type " << type;
		}

		if(widget){
			ret->pack_start(*widget, false, true, 10);
		}
	}

	return ret;
}

