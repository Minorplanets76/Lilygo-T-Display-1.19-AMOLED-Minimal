/**
 * Minimal Test case C.Chad 2024
 */
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
#include <lvgl.h>
#include <AceButton.h>


using namespace ace_button;

LilyGo_Class amoled;
lv_obj_t *label1;
lv_obj_t *testButtonLabel;
lv_obj_t *labelSlider;
lv_obj_t *touchTest;
uint8_t btnPin = 0;
AceButton button(btnPin);
uint8_t rotation = 1;
const char *format_string = "#0000ff X:%d#\n #990000 Y:%d#\n #3d3d3d Size:%s# ";

static void btn_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t *testButtonLabel = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(testButtonLabel, "Button: %d", cnt);
    }
}

static void slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);

    /*Refresh the text*/
    lv_label_set_text_fmt(labelSlider, "%" LV_PRId32, lv_slider_get_value(slider));
    lv_obj_align_to(labelSlider, slider, LV_ALIGN_OUT_TOP_MID, 0, -15); /*Align top of the slider*/
}

void handleEvent(AceButton * /* button */, uint8_t eventType,
                 uint8_t /* buttonState */,lv_obj_t *touchTest)
{
    switch (eventType) {
    case AceButton::kEventPressed:
        lv_label_set_text(touchTest,"Home Pressed");
        break;
    default: break;
    }
}

   
void lv_example_get_started_1(void)
{
    lv_obj_t *testButton = lv_btn_create(lv_scr_act());                /*Add a button the current screen*/
    lv_obj_set_pos(testButton, 20, 100);                                /*Set its position*/
    lv_obj_set_size(testButton, 200, 100);                              /*Set its size*/
    lv_obj_add_event_cb(testButton, btn_event_cb, LV_EVENT_ALL, NULL); /*Assign a callback to the button*/

    lv_obj_t *testButtonLabel = lv_label_create(testButton); /*Add a label to the button*/
    lv_label_set_text(testButtonLabel, "Button");     /*Set the labels text*/
    lv_obj_set_style_text_font(testButtonLabel, &lv_font_montserrat_28, 0);
    lv_obj_center(testButtonLabel);

    lv_obj_t *testHeading = lv_label_create(lv_scr_act());
    lv_label_set_text(testHeading, "LVGL TEST");
    lv_obj_set_style_text_font(testHeading, &lv_font_montserrat_40, 0);
    lv_obj_set_pos(testHeading, 10, 15);

        /*Create a slider in the center of the display*/
    lv_obj_t *slider = lv_slider_create(lv_scr_act());
    lv_obj_set_width(slider, 200);                                              /*Set the width*/
    lv_obj_center(slider);
                                                   
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL); /*Assign an event function*/

    /*Create a label above the slider*/
    labelSlider = lv_label_create(lv_scr_act());
    lv_label_set_text(labelSlider, "0");
    lv_obj_align_to(labelSlider, slider, LV_ALIGN_OUT_TOP_MID, 0, -15); /*Align top of the slider*/
    lv_obj_set_style_text_font(labelSlider, &lv_font_montserrat_28, 0);

    /*Touch Test*/
    touchTest = lv_label_create(lv_scr_act());
    lv_obj_align(touchTest, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_obj_set_style_text_font(touchTest, &lv_font_montserrat_28, 0);
    lv_label_set_text(touchTest, "Touch Test");
    amoled.setHomeButtonCallback([](void *ptr) {
        Serial.println("Home key pressed!");
        static uint32_t checkMs = 0;
        if (millis() > checkMs) {
            lv_label_set_text(touchTest, "Home Pressed");
        }
        checkMs = millis() + 200;
        lv_timer_create([](lv_timer_t *t) {
            lv_label_set_text(touchTest, "Touch Test");
            lv_timer_del(t);
        }, 2000, NULL);
    }, NULL);

    label1 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
    lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
    lv_label_set_text_fmt(label1, format_string, 0, 0, amoled.getName());
    lv_obj_set_width(label1, 150);  /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 100);
}



void setup()
{
    Serial.begin(115200);

    bool rslt = false;
    // Automatically determine the access device
    rslt = amoled.begin();

    if (!rslt) {
        while (1) {
            Serial.println("The board model cannot be detected, please raise the Core Debug Level to an error");
            delay(1000);
        }
    }

    beginLvglHelper(amoled);
    amoled.setRotation(rotation);
    lv_disp_drv_t *drv = lv_disp_get_default()->driver;
    drv->hor_res = amoled.width();
    drv->ver_res = amoled.height();
    lv_disp_drv_update(lv_disp_get_default(), drv);
    
    lv_example_get_started_1();

}


void loop()
{
    lv_indev_t *indev = lv_indev_get_next(NULL);
    lv_point_t  point;
    if ( indev->proc.state == LV_INDEV_STATE_PRESSED ) {
        lv_indev_get_point(indev, &point);
        lv_label_set_text_fmt(label1, format_string, point.x, point.y,  amoled.getName());
    }
    lv_task_handler();
    delay(5);
}
