// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.0
// LVGL version: 8.3.11
// Project name: SquareLine_Project_Example_UI

#include "ui.h"
#include "LilyGo_AMOLED.h"
extern LilyGo_Class amoled;

static bool colon;
void btn_event_cb(lv_event_t * e)
{
	    static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_label_set_text_fmt(ui_Main_Label2, "Button: %d", cnt);
    
}

void setPPMlabel(lv_event_t * e)
{
	// Your code here
        lv_label_set_text_fmt(ui_PPM_Label3, "Charging:%s\nUSB PlugIn:%s\nCHG state:\n%s\nBattery Voltage:\n%u mV\n USB Voltage:\n%u mV\n SYS Voltage:\n%u mV\n",
                amoled.BQ.isCharging() ? "#00ff00 YES" : "#ff0000 NO",
                amoled.BQ.isVbusIn() ? "#00ff00 YES" : "#ff0000 NO",
                amoled.BQ.getChargeStatusString(),
                amoled.BQ.getBattVoltage(),
                amoled.BQ.getVbusVoltage(),
                amoled.BQ.getSystemVoltage()
               );
}

void sdcard_screen_loaded(lv_event_t * e)
{
	// Your code here
        uint32_t size = SD.cardSize() / (1024 * 1024);
        uint32_t totalBytes = SD.totalBytes() / (1024 * 1024);
        uint32_t usedBytes = SD.usedBytes() / (1024 * 1024);

        lv_label_set_text_fmt(ui_SDcard_Label6,
                "#00ff00 Success!#\nSD Card Size: \n%uMB\nTotal space: \n%uMB\nUsed space: \n%uMB",
                size, totalBytes, usedBytes);
}

void update_time_and_date_to_screen(lv_event_t * e)
{
	// Your code here
        struct tm timeinfo;
        
        amoled.getDateTime(&timeinfo);
        lv_label_set_text_fmt(ui_RTCtimeLabel,
                "%d:%02d %s",
                timeinfo.tm_hour % 12 == 0 ? 12 : timeinfo.tm_hour % 12, timeinfo.tm_min,
                timeinfo.tm_hour >= 12 ? "PM" : "AM");
                colon = !colon;
        lv_calendar_set_today_date(ui_RTC_Calendar,
                timeinfo.tm_year + 1900,
                timeinfo.tm_mon + 1,
                timeinfo.tm_mday);
        lv_calendar_set_showed_date(ui_RTC_Calendar,
                timeinfo.tm_year + 1900,
                timeinfo.tm_mon + 1);

        lv_label_set_text_fmt(ui_RTC_Date_Label,
                "%d/%02d/%04d",
                timeinfo.tm_mday,
                timeinfo.tm_mon + 1,
                timeinfo.tm_year + 1900);

}

void set_time_manually(lv_event_t * e)
{
    // Get the current time from the RTC
    struct tm timeinfo;
    amoled.getDateTime(&timeinfo);

    // Update the hours and minutes according to the hour, minute and AM/PM rollers
    timeinfo.tm_hour = lv_roller_get_selected(ui_Set_Time_Roller_Hour)+1;
    timeinfo.tm_min = lv_roller_get_selected(ui_Set_Time_Roller_Minute);
    if (lv_roller_get_selected(ui_Set_Time_Roller_AMPM) == 1) { // PM
        if (timeinfo.tm_hour != 12) {
            timeinfo.tm_hour += 12;
        }
    } else { // AM
        if (timeinfo.tm_hour == 12) {
            timeinfo.tm_hour = 0;
        }
    }

    // Update the RTC time
    amoled.setDateTime(timeinfo.tm_year,
                       timeinfo.tm_mon + 1,
                       timeinfo.tm_mday,
                       timeinfo.tm_hour,
                       timeinfo.tm_min,
                       timeinfo.tm_sec);
                       _ui_screen_change(&ui_RTC, 
                                LV_SCR_LOAD_ANIM_FADE_IN, 500, 0,
                                &ui_RTC_screen_init);
    // Update system time
    mktime(&timeinfo);
    struct timeval tv;
    tv.tv_sec = mktime(&timeinfo);
    tv.tv_usec = 0;
    settimeofday(&tv, NULL);
}

void Set_Date_Manually(lv_event_t * e)
{
	// Your code here
        // Get the current time from the RTC
    struct tm timeinfo;
    amoled.getDateTime(&timeinfo);

    // Update the year, month and day according to the year, month and day rollers
    timeinfo.tm_year = lv_roller_get_selected(ui_Set_Time_Roller_Year) + 125;
    timeinfo.tm_mon = lv_roller_get_selected(ui_Set_Time_Roller_Month) + 1;
    timeinfo.tm_mday = lv_roller_get_selected(ui_Set_Time_Roller_Day) + 1;

    // Update the RTC time
    amoled.setDateTime(timeinfo.tm_year,
                       timeinfo.tm_mon,
                       timeinfo.tm_mday,
                       timeinfo.tm_hour,
                       timeinfo.tm_min,
                       timeinfo.tm_sec);

    // Update the system time
    mktime(&timeinfo);
    struct timeval tv;
    tv.tv_sec = mktime(&timeinfo);
    tv.tv_usec = 0;
    settimeofday(&tv, NULL);
}

void update_day_roller(int month,int year)
{
        // Update the Day roller options
        lv_roller_set_options(ui_Set_Time_Roller_Day, "0", LV_ROLLER_MODE_NORMAL); // clear existing options
        
        std::string options;
        int max_days;
        switch (month) {
        case 1: // January
                max_days = 31;
                break;
        case 2: // February
                if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
                max_days = 29; // leap year
                } else {
                max_days = 28;
                }
                break;
        case 3: // March
                max_days = 31;
                break;
        case 4: // April
                max_days = 30;
                break;
        case 5: // May
                max_days = 31;
                break;
        case 6: // June
                max_days = 30;
                break;
        case 7: // July
                max_days = 31;
                break;
        case 8: // August
                max_days = 31;
                break;
        case 9: // September
                max_days = 30;
                break;
        case 10: // October
                max_days = 31;
                break;
        case 11: // November
                max_days = 30;
                break;
        case 12: // December
                max_days = 31;
                break;
        default:
                // handle invalid month
                break;
        }

        for (int i = 1; i <= max_days; i++) {
        options += std::to_string(i) + "\n";
        }

        lv_roller_set_options(ui_Set_Time_Roller_Day, options.c_str(),LV_ROLLER_MODE_NORMAL);
}

void select_time_and_date_rollers(lv_event_t * e)
{
	// Your code here
        struct tm timeinfo;
        amoled.getDateTime(&timeinfo);        
        update_day_roller(timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
        lv_roller_set_selected(ui_Set_Time_Roller_Hour, 
                timeinfo.tm_hour % 12 - 1, LV_ANIM_ON);
        lv_roller_set_selected(ui_Set_Time_Roller_Minute, 
                timeinfo.tm_min, LV_ANIM_ON);
        lv_roller_set_selected(ui_Set_Time_Roller_AMPM, 
                timeinfo.tm_hour >= 12 ? 1 : 0, LV_ANIM_ON);
        lv_roller_set_selected(ui_Set_Time_Roller_Day,
                timeinfo.tm_mday - 1, LV_ANIM_ON);
        lv_roller_set_selected(ui_Set_Time_Roller_Month,
                timeinfo.tm_mon , LV_ANIM_ON);
        lv_roller_set_selected(ui_Set_Time_Roller_Year,
                timeinfo.tm_year - 125, LV_ANIM_ON);
}

void date_changed(lv_event_t * e)
{
	int month = lv_roller_get_selected(ui_Set_Time_Roller_Month) + 1;
        int year = lv_roller_get_selected(ui_Set_Time_Roller_Year) + 2025;
        update_day_roller(month, year);
}

void main_screen_button_long_pressed(lv_event_t * e)
{
	// Your code here
        lv_label_set_text_fmt(ui_Main_Label2,"Sleep...");
        lv_refr_now(lv_disp_get_default());
        delay(1000);
        esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
        esp_sleep_enable_timer_wakeup(5000000);
        esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_ON);
        esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_ON);
        esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_ON);
        esp_light_sleep_start();
        lv_label_set_text_fmt(ui_Main_Label2,"Awake!");
        lv_refr_now(lv_disp_get_default());
}
