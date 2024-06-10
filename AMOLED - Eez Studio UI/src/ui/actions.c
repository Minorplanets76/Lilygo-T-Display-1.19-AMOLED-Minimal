#include "ui.h"

void action_btn_event_cb(lv_event_t * e)
{
	    static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_label_set_text_fmt(objects.button_label, "Button: %d", cnt);
    
}
extern void action_slider_change(lv_event_t * e)
{
    

    /*Refresh the text*/
    lv_label_set_text(objects.slider_text,   lv_slider_get_value(objects.slider));
    
}