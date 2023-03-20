#include "hikeBLE.h"
#include "hardware/callback.h"
#include "hardware/pmu.h"
#include "SPIFFS.h"

static BLECharacteristic *hikeCharacteristic;

void setup_hike_ble() {
    // get the ble server
    NimBLEServer *pServer = blectl_get_ble_server();
    NimBLEAdvertising *pAdvertising = blectl_get_ble_advertising();    
    
    // create hike service 
    NimBLEService *service = pServer->createService(HIKE_SERVICE_UUID);

    // set charasteristics
    hikeCharacteristic = service->createCharacteristic(
        HIKE_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
    );   
    
    //start service
    bool res = service->start();    // start advertising
    if(res) {
        log_i("setup_hike_ble: service started");
    }
    else {
        log_e("setup_hike_ble: service start failed");
    }

    // start advertising hike service
    pAdvertising->addServiceUUID(service->getUUID());

    // set advertise name
    pAdvertising->setName("HikeLogs");
    
    // get the characteristic data
    service->getCharacteristic(HIKE_CHARACTERISTIC_UUID)->setValue("No Logs");
};
