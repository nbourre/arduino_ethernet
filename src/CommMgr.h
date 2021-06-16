///
/// \file 		CommMgr.h
/// \brief 		CommMgr application module
/// \author 	Me
///

#ifndef CommMgr_h
#define CommMgr_h

#define MQTT_SERVER "10.79.83.205" // TEST

////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////
#include "Reseau.h"
#include "PubSubClient.h"

class CommMgr {
    public :
        CommMgr(char * ipAddr);

        void Init();

        void Task();
        void PublishData(const char * topic, const uint8_t * payload, unsigned int plength);
        

    private:
        bool isInitialized = false;
        char moduleName[32];    ///< Name of the device
        char * mqttServerIP;
        int mqttServerPort = 1883;

        PubSubClient mqttClient;        
        EthernetClient ethClient;
        Reseau network;
};

#endif