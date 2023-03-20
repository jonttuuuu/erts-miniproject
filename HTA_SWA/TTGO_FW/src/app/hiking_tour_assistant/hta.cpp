/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */
#include "config.h"
#include "quickglui/quickglui.h"

#include "hta.h"
#include "gui/mainbar/mainbar.h"
#include "gui/widget_styles.h"
#include "hardware/motion.h"
#include "hardware/motor.h"
#include "hardware/ble/hikeBLE.h"

#include "hikeRecorder.h"

/* INCLUDE APP WIDGETS RTC HERE */

#ifdef NATIVE_64BIT

#else
    #ifdef M5PAPER
    #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 )
        #include <TTGO.h>
    #endif
#endif

// App icon must have an size of 64x64 pixel with an alpha channel
// Use https://lvgl.io/tools/imageconverter to convert your images and set "true color with alpha"
LV_IMG_DECLARE(move_64px);
LV_IMG_DECLARE(trash_32px);
LV_IMG_DECLARE(play_32px);
LV_FONT_DECLARE(Ubuntu_16px);
LV_FONT_DECLARE(Ubuntu_32px);

#define YES "Yes"
#define NO  "No"

// App objects
static SynchronizedApplication htaApp;
static JsonConfig config("hta.json");

// Options 
static String stepLenght;

/* Widgets */
// Buttons
static Button HikeStartStopButton;

// Legends
static Label HikeSessionIdLegend;
static Label HikeStepcountLegend;
static Label HikeDistanceLegend;

// Values
static Label HikeSessionId;
static Label HikeStepcount;
static Label HikeDistance;

// Hike recorder object
static HikeRecorder * hikeRecorder;
bool HikeRecording = false;

// Styles
static Style big, small;

// Default message box callback
static lv_event_cb_t default_msgbox_cb;

// Pages
static void build_main_page();
static void refresh_main_page();
static void build_settings_page();

// Callbacks
static void hta_activate_cb();
static void hta_delete_recordings_cb(lv_obj_t * obj, lv_event_t event);

void hta_app_setup() {
    //   Create and register new application
    //   params: name, icon, auto add "refresh" button (this app will use synchronize function of the SynchronizedApplication class).
    //   Also, you can configure count of the required pages in the next two params (to have more app screens).

    htaApp.init("HikeTracker", &move_64px, true, 1, 1);

    // Register application tile in the mainbar
    mainbar_add_tile_activate_cb( htaApp.mainTileId(), hta_activate_cb );

    // Build and configure application pages
    build_main_page();
    build_settings_page();

    // Create hike recorder object
    hikeRecorder = new HikeRecorder();

    // Sync Button callback
    htaApp.synchronizeActionHandler([](SyncRequestSource source) {
        log_w("Hike Sync button callback");

        lv_obj_t * mbox1 = lv_msgbox_create(lv_scr_act(), NULL);
        lv_msgbox_set_text(mbox1, "Syncing...");
        //lv_msgbox_add_btns(mbox1, btns);
        lv_obj_set_width(mbox1, 200);
        lv_msgbox_start_auto_close(mbox1, 1000);

        // Save default callback
        default_msgbox_cb = lv_obj_get_event_cb(mbox1);
        lv_obj_set_event_cb(mbox1, [](lv_obj_t * obj, lv_event_t event) {
            if (event == LV_EVENT_CLICKED) {
                lv_obj_del(obj);
            }
        });
        lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0);

        NimBLEServer *pServer = NimBLEDevice::getServer();

        // get the hike service 
        NimBLEService *pService = pServer->getServiceByUUID(HIKE_SERVICE_UUID);

        // get the hike characteristic
        NimBLECharacteristic *pCharacteristic = pService->getCharacteristic(HIKE_CHARACTERISTIC_UUID);

        // get the hike logs as string from hike recorder
        char * hikeLogsAsString = hikeRecorder->getLogsAsString();

        log_w("Hike logs: %s", hikeLogsAsString ? hikeLogsAsString : "NULL");

        if(hikeLogsAsString == NULL) {
            log_w("No hike logs to sync");
            return;
        }

        // add null termination 
        size_t len = strlen(hikeLogsAsString);
        pCharacteristic->setValue((uint8_t*)hikeLogsAsString, len);

        delete hikeLogsAsString;
    });

    //Hike Recording button callback
    htaApp.mainPage().addButtonLeftTop(play_32px, [](Widget btn) {
        log_w("Hike Recording button callback");

        // create message box that indicates that recording is started (hide it automatically after 2 seconds)

        lv_obj_t * mbox2 = lv_msgbox_create(lv_scr_act(), NULL);
        if(HikeRecording) {
            lv_msgbox_set_text(mbox2, "Recording stopped");
            HikeRecording = false;
            hikeRecorder->stopSession();
        } else {
            lv_msgbox_set_text(mbox2, "Recording started");
            HikeRecording = true;
            hikeRecorder->startSession(atoi(stepLenght.c_str()));
        }
        lv_obj_set_width(mbox2, 200);
        lv_msgbox_start_auto_close(mbox2, 1000);
        lv_obj_align(mbox2, NULL, LV_ALIGN_CENTER, 0, 0);
        refresh_main_page();
    });

    // Delete Hike Recordings Button callback
    htaApp.mainPage().addAppButton(trash_32px, [](Widget btn) {
        log_w("Delete Hike Recordings button callback");
        static const char * btns[] ={YES, NO, ""};

        lv_obj_t * mbox3 = lv_msgbox_create(lv_scr_act(), NULL);
        lv_msgbox_set_text(mbox3, "Delete all Hike Recordings?");
        lv_msgbox_add_btns(mbox3, btns);
        lv_obj_set_width(mbox3, 200);

        // Save default callback
        default_msgbox_cb = lv_obj_get_event_cb(mbox3);
        lv_obj_set_event_cb(mbox3, hta_delete_recordings_cb);
        lv_obj_align(mbox3, NULL, LV_ALIGN_CENTER, 0, 0);
    });

    refresh_main_page();
}

void build_main_page()
{
    // Styles
    big = Style::Create(ws_get_mainbar_style(), true);
    big.textFont(&Ubuntu_32px).textOpacity(LV_OPA_80);

    small = Style::Create(ws_get_mainbar_style(), true);
    small.textFont(&Ubuntu_16px).textOpacity(LV_OPA_80);

    AppPage& screen = htaApp.mainPage(); // This is parent for all main screen widgets

    // Legend for Session ID
    HikeSessionIdLegend = Label(&screen);
    HikeSessionIdLegend.text("session")
        .alignText(LV_LABEL_ALIGN_LEFT)
        .style(small, true)
        .alignInParentCenter(0, -60);

    // Session ID
    HikeSessionId = Label(&screen);
    HikeSessionId.text("0")
        .alignText(LV_LABEL_ALIGN_CENTER)
        .style(small, true)
        .align(HikeSessionIdLegend, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    // Legend For Step Count 
    HikeStepcountLegend = Label(&screen);
    HikeStepcountLegend.text("steps")
        .alignText(LV_LABEL_ALIGN_LEFT)
        .style(small, true)
        .alignInParentCenter(0, -30);

    // StepCount
    HikeStepcount = Label(&screen);
    HikeStepcount.text("0")
        .alignText(LV_LABEL_ALIGN_CENTER)
        .style(small, true)
        .align(HikeStepcountLegend, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    
    // Legend for Distance
    HikeDistanceLegend = Label(&screen);
    HikeDistanceLegend.text("distance")
        .alignText(LV_LABEL_ALIGN_LEFT)
        .style(small, true)
        .alignInParentCenter(0, 0);

    // Distance
    HikeDistance = Label(&screen);
    HikeDistance.text("0 m")
        .alignText(LV_LABEL_ALIGN_CENTER)
        .style(small, true)
        .align(HikeDistanceLegend, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
}

void refresh_main_page()
{
    char buff[32];
    HikeSession * session;

    // update session object
    hikeRecorder->updateStepcount();

    session = hikeRecorder->getCurrentSession();

    if(session != nullptr) {
        //// Session ID
        snprintf( buff, sizeof( buff ), "%d", hikeRecorder->getCurrentSession()->getId());
        HikeSessionId.text(buff).realign();

        // steps
        snprintf( buff, sizeof( buff ), "%d", hikeRecorder->getCurrentSession()->getStepcount());
        HikeStepcount.text(buff).realign();

        ////// Distance
        snprintf( buff, sizeof( buff ), "%d m", hikeRecorder->getCurrentSession()->getDistance());
        HikeDistance.text(buff).realign();
    }
}

void hta_activate_cb()
{
    refresh_main_page();
}

static void hta_delete_recordings_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        const char *answer = lv_msgbox_get_active_btn_text(obj);
        if ( strcmp( answer, YES) == 0 ) {
            log_w("Delete Hike Recordings");
            hikeRecorder->deleteLogs();
        }
    }
    default_msgbox_cb(obj, event);
}

void build_settings_page()
{
    // Create full options list and attach items to variables

    // Step Lenght
    config.addString("Step length", 3, "50").setDigitsMode(true,"0123456789").assign(&stepLenght); // cm

    htaApp.useConfig(config, true); // true - auto create settings page widgets
}
