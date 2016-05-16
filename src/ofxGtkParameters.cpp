#include "ofxGtkParameters.h"

static int spacing = 15;

string ofxGtkEscape(string& _str){

	std::string str(_str);

	ofStringReplace(str, " ", "_");
	ofStringReplace(str, "<", "_");
	ofStringReplace(str, ">", "_");
	ofStringReplace(str, "{", "_");
	ofStringReplace(str, "}", "_");
	ofStringReplace(str, "[", "_");
	ofStringReplace(str, "]", "_");
	ofStringReplace(str, ",", "_");
	ofStringReplace(str, "(", "_");
	ofStringReplace(str, ")", "_");
	ofStringReplace(str, "/", "_");
	ofStringReplace(str, "\\", "_");
	ofStringReplace(str, ".", "_");

	return str;
}

std::string ofxGtkGetWidgetOscPath(ofAbstractParameter& param){
	auto path = param.getGroupHierarchyNames();
	for(auto& p: path){
		p = ofxGtkEscape(p);
	}

	string str = "/";
	string delimiter = "/";
	for (auto i : path)
		str += i + delimiter;
	str = str.substr(0, str.size() - delimiter.size());
	return str;
}

////////////////////////////////////////////////////


template<>
Gtk::Widget* widgetFromParameter(ofParameter<float>& param){
	return new ofxGtkSpinButton<float>(param);
}

template<>
Gtk::Widget* widgetFromParameter(ofParameter<unsigned>& param){
	return new ofxGtkSpinButton<unsigned>(param);
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

template<>
Gtk::Widget* widgetFromParameter(ofParameter<ofVec2f>& param){
	return new ofxGtkVec<ofVec2f>(param);
}

template<>
Gtk::Widget* widgetFromParameter(ofParameter<ofVec3f>& param){
	return new ofxGtkVec<ofVec3f>(param);
}

template<>
Gtk::Widget* widgetFromParameter(ofParameter<ofVec4f>& param){
	return new ofxGtkVec<ofVec4f>(param);
}

////////////////////////////////////////////////////////////////////////////////

static std::map<std::string, WidgetFromParamFunction> widgetFromParams;
bool bDefaultCreatorsSetup = false;

void addWidgetFromParameterCreator(string typeName, WidgetFromParamFunction creator){
	widgetFromParams[typeName] = creator;
}


void addDefaultCreators(){
	if(bDefaultCreatorsSetup)
		return;

	bDefaultCreatorsSetup = true;

	addParamToWidgetType<bool>();
	addParamToWidgetType<int>();
	addParamToWidgetType<unsigned>();
	addParamToWidgetType<float>();
	addParamToWidgetType<double>();
	addParamToWidgetType<ofColor>();
	addParamToWidgetType<std::string>();
    addParamToWidgetType<ofVec2f>();
    addParamToWidgetType<ofVec3f>();
    addParamToWidgetType<ofVec4f>();
}

/////////////////////////////////////////////////////////////////////////////////

Gtk::Widget* widgetFromParameter(ofAbstractParameter& param){

	addDefaultCreators();

	if(widgetFromParams.find(param.type()) != widgetFromParams.end()){
		return widgetFromParams[param.type()](param);
	}

	ofLogWarning("widgetFromParameter") << "parameter type not registered " << param.type() << " add one with addParamToWidgetType<ParameterType>();";

	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////

void printGroup(ofParameterGroup& group){
	for(auto p: group){
		ofLog() << p->getName();
		if(p->type() == typeid(ofParameterGroup).name()){
			printGroup(static_cast<ofParameterGroup&>(*p));
		}
	}
}

Gtk::Widget* widgetFromParameter(ofParameterGroup& group){
	return new ofxGtkParameters(group);
}

////

ofxGtkParameters::ofxGtkParameters(){

}

ofxGtkParameters::ofxGtkParameters(ofParameterGroup& group){

}

void ofxGtkParameters::set(ofParameterGroup &group){
	remove();

	widgets.clear();

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
		add(notebookFromParameterGroup(group));
	}else if(!hasChildGroup){
		add(expanderFromParameterGroup(group));
	}

	show_all();
}

void ofxGtkParameters::handle(Gtk::Widget *widget){
	widgets.push_back(shared_ptr<Gtk::Widget>(widget));
}

///////////
Gtk::Notebook& ofxGtkParameters::notebookFromParameterGroup(ofParameterGroup& group){
	Gtk::Notebook& notebook = create<Gtk::Notebook>();

	notebook.set_tab_pos(Gtk::PositionType::POS_LEFT);

	for(auto param: group){
		if(param->type() == typeid(ofParameterGroup).name()){
			Gtk::ScrolledWindow& scrolled = create<Gtk::ScrolledWindow>();

			Gtk::FlowBox& flowBox = flowBoxFromParameterGroup(static_cast<ofParameterGroup&>(*param));

			scrolled.add(flowBox);

			flowBox.set_margin_left(spacing);
			flowBox.set_margin_right(spacing);
			flowBox.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
			flowBox.set_selection_mode(Gtk::SelectionMode::SELECTION_NONE);
			//flowBox.set_homogeneous(true);


			Gtk::Label& tabTitle = create<Gtk::Label>(param->getName());

			tabTitle.set_justify(Gtk::Justification::JUSTIFY_LEFT);
			tabTitle.set_margin_bottom(spacing);
			tabTitle.set_margin_top(spacing);

			notebook.append_page(scrolled, tabTitle);
		}
	}
	return notebook;
}

Gtk::Expander& ofxGtkParameters::expanderFromParameterGroup(ofParameterGroup& group){
	Gtk::Expander& expander = create<Gtk::Expander>(group.getName());//new Gtk::Expander(group.getName());
	//expander->set_spacing(spacing);
	expander.set_use_underline(false);
	expander.set_expanded(false);
	Gtk::Alignment& alignment = create<Gtk::Alignment>();
	alignment.set_padding(spacing, 0, spacing, spacing*2);
	auto& child = vBoxFromParameterGroup(group);
	alignment.add(child);
	expander.add(alignment);
	return expander;
}

Gtk::FlowBox& ofxGtkParameters::flowBoxFromParameterGroup(ofParameterGroup& group){
	Gtk::FlowBox& flowBox = create<Gtk::FlowBox>();
	flowBox.set_homogeneous(false);
	flowBox.set_column_spacing(spacing);
	flowBox.set_row_spacing(spacing);
	flowBox.set_selection_mode(Gtk::SELECTION_NONE);
	flowBox.set_valign(Gtk::Align::ALIGN_START);

	for(auto param: group){
		Gtk::FlowBoxChild& child = create<Gtk::FlowBoxChild>();
		child.set_valign(Gtk::Align::ALIGN_START);
		if(param->type() == typeid(ofParameterGroup).name()){
				child.add(frameFromParameterGroup(static_cast<ofParameterGroup&>(*param)));
		}else{
			child.add(labeledWidgetFromParameter(*param));
		}
		flowBox.add(child);
		//flowBox.add(*labeledWidgetFromParameter(*param));
	}
	return flowBox;
}

Gtk::ScrolledWindow& ofxGtkParameters::scrolledWindowFromParameterGroup(ofParameterGroup& group){
	Gtk::ScrolledWindow& window = create<Gtk::ScrolledWindow>();
	window.add(flowBoxFromParameterGroup(group));
	return window;
}

Gtk::Frame& ofxGtkParameters::frameFromParameterGroup(ofParameterGroup &group){
	Gtk::Frame& frame = create<Gtk::Frame>();

	frame.set_label_align(.5, .5);
	//frame.set_shadow_type(Gtk::ShadowType::SHADOW_NONE);

	Gtk::Label& frameTitle = create<Gtk::Label>(group.getName());

	Pango::AttrList attr;
	Pango::Attribute fontWeight = Pango::Attribute::create_attr_weight(Pango::WEIGHT_BOLD);
	attr.insert(fontWeight);
	frameTitle.set_attributes(attr);

	frameTitle.set_margin_bottom(spacing/2);
	frameTitle.set_margin_top(spacing/2);

	frame.set_label_widget(frameTitle);

	Gtk::VBox& vBox = vBoxFromParameterGroup(group);

	vBox.set_margin_bottom(spacing);
	vBox.set_margin_left(spacing);
	vBox.set_margin_right(spacing);
	vBox.set_margin_top(spacing);

	frame.add(vBox);

	return frame;
}

/////////////////////////////////////////////////////////////////////////////////

Gtk::ListBox& ofxGtkParameters::listBoxFromParameterGroup(ofParameterGroup& group){
	Gtk::ListBox& listBox = create<Gtk::ListBox>();

	for(auto param: group){
		Gtk::ListBoxRow& row = create<Gtk::ListBoxRow>();
		row.add(labeledWidgetFromParameter(*param));
		listBox.append(row);
	}

	return listBox;
}

Gtk::VBox& ofxGtkParameters::vBoxFromParameterGroup(ofParameterGroup& group){
	Gtk::VBox& vBox = create<Gtk::VBox>();
	vBox.set_valign(Gtk::Align::ALIGN_START);
	vBox.set_spacing(spacing);

	for(auto param: group){
		vBox.pack_end(labeledWidgetFromParameter(*param));
	}

	return vBox;
}

Gtk::Widget& ofxGtkParameters::labeledWidgetFromParameter(ofAbstractParameter& param){

	if(param.type() == typeid(ofParameterGroup).name()){
		//return frameFromParameterGroup(static_cast<ofParameterGroup&>(param));
		return expanderFromParameterGroup(static_cast<ofParameterGroup&>(param));
	}


	auto path = ofxGtkGetWidgetOscPath(param);

	Gtk::EventBox& eventBox = create<Gtk::EventBox>();
	eventBox.set_events(Gdk::BUTTON_PRESS_MASK);
	eventBox.set_tooltip_text(path+" (right click to copy");
	eventBox.signal_button_press_event().connect([&, path](GdkEventButton* evt){
		ofLog() << "Saved to clipboard (requires xclip) " << path;

		auto refClipboard = Gtk::Clipboard::get();
		refClipboard->clear();
		refClipboard->set_can_store();
		refClipboard->set_text(path);
		refClipboard->store();

		string command = "echo -n \""+path+"\" | xclip -selection clipboard";
		system(command.c_str());

		return true;
	});


	Gtk::Widget* widget = widgetFromParameter(param);

	bool noLabel = false;
	if(widget && dynamic_cast<Gtk::Button*>(widget)){
		noLabel = true;
	}


	Gtk::HBox& hBox = create<Gtk::HBox>();
	hBox.set_valign(Gtk::Align::ALIGN_START);
	hBox.set_spacing(spacing);

	eventBox.add(hBox);

	if(!noLabel){
		Gtk::Label& label = create<Gtk::Label>(param.getName());
		hBox.pack_start(label, false, false);
	}

	if(widget){
		handle(widget);

		if(noLabel)
			hBox.pack_start(*widget, true, true);
		else
			hBox.pack_end(*widget, false, false);
	}

	return eventBox;
}


