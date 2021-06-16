#include <Arduino.h>
#include <EEPROM.h>

#include <Wire.h>
#include "Reseau.h"
#include "CommMgr.h"

////////////////////////////////////////////////////////////////////////////////
// Data types
////////////////////////////////////////////////////////////////////////////////

///
/// \enum 	ApplicationStateTy
/// \brief 	Supported application states.
///
/// Identifies the different application states supported by this module.
typedef enum
{
	APP_SM_INIT		= 0,		///< Application state - Initialization state.
	APP_SM_NORMAL,				///< Application state - Normal operation state.
} ApplicationStateTy;

////////////////////////////////////////////////////////////////////////////////
// PROTOTYPES
////////////////////////////////////////////////////////////////////////////////
void ApplicationTask();

////////////////////////////////////////////////////////////////////////////////
// Local variables
////////////////////////////////////////////////////////////////////////////////
ApplicationStateTy applicationState;
CommMgr commMgr ("8.8.8.8");

long cT; // current time when loop start
long dT; // delta time
long pT; // previous time

void setup() {
  cT = millis();
  dT = 0;
  pT = cT;

  applicationState = APP_SM_INIT;

  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Wire.begin();

  commMgr.Init();

  Serial.println ("Setup completed!");

  applicationState = APP_SM_NORMAL;
  
}

void loop() {
  cT = millis();
  dT = cT - pT;
  pT = cT;
  
  switch (applicationState) {
    case APP_SM_NORMAL:
      ApplicationTask();
      break;
    default:
      applicationState = APP_SM_NORMAL;
      break;
  }     
}

void ApplicationTask() {
  commMgr.Task();
}
