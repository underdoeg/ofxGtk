#ifndef OFXGTKWRAPPER_H
#define OFXGTKWRAPPER_H

#include <gtkmm.h>
#include "ofMain.h"

template<class WidgetType>
class ofxGtkWrapper{
public:
	WidgetType& widget(){
		if(!m_widget){
			//ofLogFatalError("ofxGtkHelper") << "widget not yet set";
			setWidget(new WidgetType());
		}
		WidgetType* ret = m_widget.operator->();
		return *ret;
	}

	operator WidgetType&() { return widget(); }

protected:
	void setWidget(WidgetType* w){
		Gtk::manage(w);
		m_widget = Glib::RefPtr<WidgetType>(w);
	}

	Glib::RefPtr<WidgetType> m_widget;
};

#endif // OFXGTKWRAPPER_H
