/**
 * Minimal Test case Minorplanets 2025
 */
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
#include <lvgl.h>
#include "ui/ui.h"

LilyGo_Class amoled;
lv_obj_t *label1;
lv_obj_t *testButtonLabel;
lv_obj_t *labelSlider;
lv_obj_t *touchTest;
uint8_t btnPin = 0;
uint8_t rotation = 1;
const char *format_string = "#0000ff X:%d#\n #990000 Y:%d#\n #3d3d3d Size:%s# ";


void timer_cb(lv_timer_t * timer)
{
    // code to be executed when the timer expires
    lv_label_set_text_fmt(ui_PPM_Label3, "Charging:%s\nUSB PlugIn:%s\nCHG state:\n%s\nBattery Voltage:\n%u mV\n USB Voltage:\n%u mV\n SYS Voltage:\n%u mV\n",
        amoled.BQ.isCharging() ? "#00ff00 YES" : "#ff0000 NO",
        amoled.BQ.isVbusIn() ? "#00ff00 YES" : "#ff0000 NO",
        amoled.BQ.getChargeStatusString(),
        amoled.BQ.getBattVoltage(),
        amoled.BQ.getVbusVoltage(),
        amoled.BQ.getSystemVoltage()
       );
}



void lv_example_get_started_1(void)
{
    

    /*Touch Test*/
    touchTest = lv_label_create(lv_scr_act());
    lv_obj_align(touchTest, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_obj_set_style_text_font(touchTest, &lv_font_montserrat_28, 0);
    lv_label_set_text(touchTest, "Touch Test");

    amoled.setHomeButtonCallback([](void *ptr) {
        Serial.println("Home key pressed!");
        static uint32_t checkMs = 0;
        static uint8_t lastBri =0;

        if (millis() > checkMs) {
            _ui_screen_change(&ui_Main, LV_SCR_LOAD_ANIM_FADE_IN, 500, 0, &ui_Main_screen_init);

            lv_label_set_text(touchTest, "Home Pressed");
            
            
            if (amoled.getBrightness()) {
                lastBri = amoled.getBrightness();
                amoled.setBrightness(0);  //turn screen "off"
            } else {
                amoled.setBrightness(lastBri);
            }
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
    
    ui_init();

    //PPM init
    amoled.BQ.setChargeTargetVoltage(4208);
    amoled.BQ.setPrechargeCurr(64);
    amoled.BQ.setChargerConstantCurr(832);
    amoled.BQ.enableMeasure();
    amoled.BQ.enableCharge();

    //SD Card Init
    bool mounted = amoled.installSD();

    //Time
    struct tm timeinfo;
    time_t now = amoled.hwClockRead();
    
    struct timeval tv;
    tv.tv_sec = now;
    tv.tv_usec = 0;
    settimeofday(&tv, NULL);
    

    lv_example_get_started_1();
    
    lv_timer_t * timer = lv_timer_create(timer_cb,5000,NULL); 
    
}


void loop()
{

    lv_task_handler();
    lv_indev_t *indev = lv_indev_get_next(NULL);
    lv_point_t  point;
    if ( indev->proc.state == LV_INDEV_STATE_PRESSED ) {
        lv_indev_get_point(indev, &point);
        lv_label_set_text_fmt(label1, format_string, point.x, point.y,  amoled.getName());
    }
}
