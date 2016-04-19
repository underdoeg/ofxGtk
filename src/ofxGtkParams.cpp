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

template<>
Gtk::Widget* widgetFromParameter(ofParameter<std::string>& param){
	return new ofxGtkEntry(param);
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
	}else if(param.type() == typeid(ofParameter<std::string>).name()){
		return widgetFromParameter(param.cast<std::string>());
	}

	return nullptr;
}

Gtk::Widget* labeledWidgetFromParameter(ofAbstractParameter& param){

	if(param.type() == typeid(ofParameterGroup).name()){
		return frameFromParameterGroup(static_cast<ofParameterGroup&>(param));
		//return expanderFromParameterGroup(static_cast<ofParameterGroup&>(param));
	}

	Gtk::HBox* hBox = new Gtk::HBox();
	hBox->set_valign(Gtk::Align::ALIGN_START);
	hBox->set_spacing(spacing);

	Gtk::Label* label = new Gtk::Label(param.getName());
	hBox->pack_start(*label, false, false);

	Gtk::Widget* widget = widgetFromParameter(param);
	if(widget){
		hBox->pack_end(*widget, false, false);

	}
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
	vBox->set_valign(Gtk::Align::ALIGN_START);
	vBox->set_spacing(spacing);

	for(auto param: group){
		vBox->add(*labeledWidgetFromParameter(*param));
	}
	return vBox;
}

Gtk::Frame *frameFromParameterGroup(ofParameterGroup &group){
	Gtk::Frame* frame = new Gtk::Frame();

	frame->set_label_align(.5, .5);
	//frame->set_shadow_type(Gtk::ShadowType::SHADOW_NONE);

	Gtk::Label* frameTitle = new Gtk::Label(group.getName());

	Pango::AttrList attr;
	Pango::Attribute fontWeight = Pango::Attribute::create_attr_weight(Pango::WEIGHT_BOLD);
	attr.insert(fontWeight);
	frameTitle->set_attributes(attr);

	frameTitle->set_margin_bottom(spacing/2);
	frameTitle->set_margin_top(spacing/2);

	frame->set_label_widget(*frameTitle);

	Gtk::VBox* vBox = vBoxFromParameterGroup(group);

	vBox->set_margin_bottom(spacing);
	vBox->set_margin_left(spacing);
	vBox->set_margin_right(spacing);
	vBox->set_margin_top(spacing);

	frame->add(*vBox);

	return frame;
}

Gtk::FlowBox* flowBoxFromParameterGroup(ofParameterGroup& group){
	Gtk::FlowBox* flowBox = new Gtk::FlowBox();
	flowBox->set_homogeneous(false);
	flowBox->set_column_spacing(spacing);
	flowBox->set_row_spacing(spacing);
	flowBox->set_selection_mode(Gtk::SELECTION_NONE);
	flowBox->set_valign(Gtk::Align::ALIGN_START);

	for(auto param: group){
		Gtk::FlowBoxChild* child = new Gtk::FlowBoxChild();
		child->set_valign(Gtk::Align::ALIGN_START);
		child->add(*labeledWidgetFromParameter(*param));
		flowBox->add(*child);
		//flowBox->add(*labeledWidgetFromParameter(*param));
	}
	return flowBox;
}

Gtk::ScrolledWindow* scrolledWindowFromParameterGroup(ofParameterGroup& group){
	Gtk::ScrolledWindow* window = new Gtk::ScrolledWindow();
	window->add(*flowBoxFromParameterGroup(group));
	return window;
}

Gtk::Expander* expanderFromParameterGroup(ofParameterGroup& group){
	Gtk::Expander* expander = new Gtk::Expander(group.getName());
	//expander->set_spacing(spacing);
	expander->set_use_underline(true);
	expander->set_expanded(true);
	expander->add(*flowBoxFromParameterGroup(group));
	return expander;
}

Gtk::Notebook* notebookFromParameterGroup(ofParameterGroup& group){
	Gtk::Notebook* notebook = new Gtk::Notebook();
	notebook->set_tab_pos(Gtk::PositionType::POS_LEFT);

	for(auto param: group){
		if(param->type() == typeid(ofParameterGroup).name()){
			Gtk::FlowBox* flowBox = flowBoxFromParameterGroup(static_cast<ofParameterGroup&>(*param));

			flowBox->set_margin_left(spacing);
			flowBox->set_margin_right(spacing);

			Gtk::Label* tabTitle = new Gtk::Label(param->getName());
			tabTitle->set_justify(Gtk::Justification::JUSTIFY_LEFT);
			tabTitle->set_margin_bottom(spacing);
			tabTitle->set_margin_top(spacing);

			notebook->append_page(*flowBox, *tabTitle);
		}
	}
	return notebook;
}

Gtk::Widget* widgetFromParameter(ofParameterGroup& group){
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
		return notebookFromParameterGroup(group);
	}else if(!hasChildGroup){
		return expanderFromParameterGroup(group);
	}
}



