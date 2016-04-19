#include "ofxGtkParams.h"

static int spacing = 15;

////////////////////////////////////////////////////


template<>
Gtk::Widget* widgetFromParameter(ofParameter<float>& param){
	return new ofxGtkSpinButton<float>(param);
}

template<>
Gtk::Widget* widgetFromParameter(ofParameter<int>& param){
	return new ofxGtkSpinButton<int>(param);
}

template<>
Gtk::Widget* widgetFromParameter(ofParameter<double>& param){
	return new ofxGtkSpinButton<double>(param);
}

template<>
Gtk::Widget* widgetFromParameter(ofParameter<ofColor>& param){
	return new ofxGtkColor(param);
}

template<>
Gtk::Widget* widgetFromParameter(ofParameter<bool>& param){
	return new ofxGtkSwitch(param);
}

////////////////////////////////////////////////////////////////////////////////

Gtk::Widget* widgetFromParameter(ofAbstractParameter& param){
	if(param.type() == typeid(ofParameter<float>).name()){
		return widgetFromParameter(param.cast<float>());
	}else if(param.type() == typeid(ofParameter<int>).name()){
		return widgetFromParameter(param.cast<int>());
	}else if(param.type() == typeid(ofParameter<double>).name()){
		return widgetFromParameter(param.cast<double>());
	}else if(param.type() == typeid(ofParameter<ofColor>).name()){
		return widgetFromParameter(param.cast<ofColor>());
	}else if(param.type() == typeid(ofParameter<std::string>).name()){
		return widgetFromParameter(param.cast<std::string>());
	}else if(param.type() == typeid(ofParameter<bool>).name()){
		return widgetFromParameter(param.cast<bool>());
	}

	return nullptr;
}

Gtk::Widget* labeledWidgetFromParameter(ofAbstractParameter& param){
	Gtk::HBox* hBox = new Gtk::HBox();
	hBox->set_spacing(spacing);

	Gtk::Label* label = new Gtk::Label(param.getName());
	hBox->pack_start(*label, false, false);

	Gtk::Widget* widget = widgetFromParameter(param);
	if(widget)
		hBox->pack_end(*widget, false, false);
	return hBox;
}

/////////////////////////////////////////////////////////////////////////////////

Gtk::ListBox* listBoxFromParameterGroup(ofParameterGroup& group){
	Gtk::ListBox* listBox = new Gtk::ListBox();

	for(auto param: group){
		Gtk::ListBoxRow* row = new Gtk::ListBoxRow();
		row->add(*labeledWidgetFromParameter(*param));
		listBox->append(*row);
	}

	return listBox;
}

Gtk::VBox* vBoxFromParameterGroup(ofParameterGroup& group){
	Gtk::VBox* vBox = new Gtk::VBox();
	vBox->set_spacing(spacing);

	for(auto param: group){
		vBox->add(*labeledWidgetFromParameter(*param));
	}
	return vBox;
}

Gtk::ScrolledWindow* scrolledWindowFromParameterGroup(ofParameterGroup& group){
	Gtk::ScrolledWindow* window = new Gtk::ScrolledWindow();
	window->add(*vBoxFromParameterGroup(group));
	return window;
}

Gtk::Expander* expanderFromParameterGroup(ofParameterGroup& group){
	Gtk::Expander* expander = new Gtk::Expander(group.getName());
	expander->set_spacing(spacing);
	expander->set_use_underline(true);
	expander->set_expanded(true);
	expander->add(*vBoxFromParameterGroup(group));
	return expander;
}

Gtk::Widget* widgetFromParameter(ofParameterGroup& group){
	Gtk::Widget* topLevel;
	Gtk::ListBox* listBox = new Gtk::ListBox();

	bool allChildrenGroups = true;
	bool hasChildGroup = false;
	for(auto param: group){
		if(param->type() != typeid(ofParameterGroup).name()){
			allChildrenGroups = false;
		}else{
			hasChildGroup = true;
		}
	}

	if(allChildrenGroups){

	}else if(!hasChildGroup){
		return expanderFromParameterGroup(group);
	}

	return topLevel;
}


