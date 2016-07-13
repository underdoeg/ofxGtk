#ifndef OFXGTKPARAMS_H
#define OFXGTKPARAMS_H

#include <gtkmm.h>
#include "ofxGtkWidgets.h"
#include "ofMain.h"
#include <type_traits>

template<typename Type>
Gtk::Widget* widgetFromParameter(Type& param){
	ofLogError("widgetFromParameter") <<  "you must provide an overrided version of this function with your parameter type (" << typeid(Type).name() << "). Like this:";
	ofLogError("widgetFromParameter") <<  "template<>";
	ofLogError("widgetFromParameter") <<  "Gtk::Widget* widgetFromParameter(YourType& type){....}";
	return nullptr;
}

Gtk::Widget* widgetFromParameter(ofParameterGroup& group);
Gtk::Widget* widgetFromParameter(ofAbstractParameter& param);

/*
Gtk::Expander* expanderFromParameterGroup(ofParameterGroup& group);
Gtk::VBox* vBoxFromParameterGroup(ofParameterGroup& group);
Gtk::Frame* frameFromParameterGroup(ofParameterGroup& group);
*/

using WidgetFromParamFunction = std::function<Gtk::Widget*(ofAbstractParameter& param)>;

void addWidgetFromParameterCreator(std::string typeName, WidgetFromParamFunction creator);

template<typename Param>
Gtk::Widget* genericWidgetFromParamCreator(ofAbstractParameter& param){
	return widgetFromParameter((Param&)param);
}

template<typename Type>
void addParamToWidgetType(){
	//ofLog() << typeid(Type).name() << " " << std::is_base_of<ofAbstractParameter, Type>::value;
	if(std::is_base_of<ofAbstractParameter, Type>::value)
		addWidgetFromParameterCreator(typeid(Type).name(), &genericWidgetFromParamCreator<Type>);
	else
		addWidgetFromParameterCreator(typeid(ofParameter<Type>).name(), &genericWidgetFromParamCreator<ofParameter<Type>>);
}


///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class ofxGtkParameters: public Gtk::ScrolledWindow{
public:
	ofxGtkParameters();
	ofxGtkParameters(ofParameterGroup &group);

	void set(ofParameterGroup& group);

private:
	template<typename Type>
	Type& create(){
		Type* ret = new Type();
		handle(ret);
		return *ret;
	}

	template<typename Type, typename ...Args>
	Type& create(Args... args){
		Type* ret = new Type(args...);
		handle(ret);
		return *ret;
	}

	void handle(Gtk::Widget* widget);

	//
	Gtk::Notebook& notebookFromParameterGroup(ofParameterGroup &group);
	Gtk::Expander& expanderFromParameterGroup(ofParameterGroup &group);
	Gtk::FlowBox& flowBoxFromParameterGroup(ofParameterGroup &group);
	Gtk::Frame& frameFromParameterGroup(ofParameterGroup &group);
	Gtk::Widget& labeledWidgetFromParameter(ofAbstractParameter &param);
	Gtk::ScrolledWindow &scrolledWindowFromParameterGroup(ofParameterGroup &group);
	Gtk::ListBox &listBoxFromParameterGroup(ofParameterGroup &group);
	Gtk::VBox &vBoxFromParameterGroup(ofParameterGroup &group);

	//
	std::vector<shared_ptr<Gtk::Widget>> widgets;
};

#endif // OFXGTKPARAMS_H
