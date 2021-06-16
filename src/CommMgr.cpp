#include <Arduino.h>
#include "CommMgr.h"
#include "Reseau.h"
#include "PubSubClient.h"
#include "Ethernet.h"

CommMgr::CommMgr(char * ipAddr) {
    network = Reseau(ipAddr);
}

void CommMgr::Init() {
    isInitialized = false;

    network.Init();
    if (Serial) {
        Serial.println ("Initiating communication device...");
    }
    delay(1000);
    network.Start();

    
    mqttServerIP = MQTT_SERVER;
    mqttClient.setClient(network.GetEthernetClient() );


    isInitialized = true;
}

void CommMgr::Task() {

    if (!isInitialized) return;
}