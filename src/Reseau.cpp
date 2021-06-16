#include "Arduino.h"
#include <EEPROM.h>
#include "Reseau.h"

#include <SPI.h>


Reseau::Reseau() {
  ipAddr = "192.168.1.25";

  //Init(defaultAddr);
}

Reseau::Reseau(char* ipAddr) {
  this->ipAddr = ipAddr;
  String ip = String(ipAddr);
}

Reseau::Reseau(String &ipAddr){
  ipAddr.toCharArray (this->ipAddr, ipAddr.length());
}

void Reseau::Init(){

  readMac();

  if (macAddr.addr[0] != 0xDE) {
    Serial.println("Initializing MAC address");
    macAddr.addr[0]  = 0xDE; 
    macAddr.addr[1]  = 0xAD; 
    macAddr.addr[2]  = 0xBE; 
    macAddr.addr[3]  = 0xEF; 
    macAddr.addr[4]  = random(1, 255); 
    macAddr.addr[5]  = random(1, 255); 

    EEPROM.put(ADDR_MAC_ADDR, macAddr);
  } else {
    Serial.println("Reading MAC address");
    EEPROM.get(ADDR_MAC_ADDR, macAddr);
    printMac();
  }

  String serverTemp = ipAddr;
  
  for (int i = 0 ; i< 12; i++){        
      server[i]= serverTemp[i];          
  }

  byteCount = 0;
  printWebData = true;  // set to false for better speed measurement
  compteur = 0;
  compteurString="";

  isInitialized = true;

  printMac();
}

void Reseau::readMac() {
  EEPROM.get(ADDR_MAC_ADDR, macAddr);
}

void Reseau::printMac() {
  if (!isInitialized) {
    Serial.println ("Network not initialized");
    return;
  }

  char hexVal[2];

  for (int i = 0; i < MAC_ADDR_SIZE; i++) {
    sprintf (hexVal, "%02X", macAddr.addr[i]);
    Serial.print(hexVal);
    
    if (i < MAC_ADDR_SIZE - 1)
      Serial.print('-');
  }

  Serial.println();
}

IPAddress Reseau::GetIPAddr() {
  return ip;
}

unsigned char * Reseau::GetMACAddr() {
  return macAddr.addr;
}

EthernetClient & Reseau::GetEthernetClient() {
  return this->client;
}

void Reseau::Start() {

  if (!isInitialized) return;

  // Start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP...");

  if (Ethernet.begin(macAddr.addr) == 0) {

    Serial.println("Failed to configure Ethernet using DHCP");

    if (Ethernet.hardwareStatus() == EthernetNoHardware) {

      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");

    } else if (Ethernet.linkStatus() == LinkOFF) {

      Serial.println("Ethernet cable is not connected.");

    }

    // Configuration SANS DHCP si non fonctionnel:
    Ethernet.begin(macAddr.addr, ip, myDns);

  }

  printMac();
  Serial.print("Bioreactor IP : ");
  Serial.println(Ethernet.localIP());

} // fin void Start()

void Reseau::SetSendData(int id, String &startDate, String &startTime, float ph, float temp, bool agit, char sender){
  if (!isInitialized) return;

  sendData = "";
  sendData += String(id);
  sendData += ',';
  sendData += startDate;
  sendData += "   ";
  sendData += startTime;
  sendData += ',';
  sendData += String(ph);
  sendData += ',';
  sendData += String(temp);
  sendData += ',';
  if(agit){
    sendData += '1';
  }else {
    sendData += '0';
  }
  sendData += ',';
  sendData += sender;
  sendData += ',';
  sendData += '!';

//      Serial.println("DATA TO SEND");
//      Serial.println(sendData);
//      Serial.println("END OF DATA TO SEND");

  
  
}
  
void Reseau::Repeat(){
  if (!isInitialized) return;

    // RÉCEPTION DE LA RÉPONSE DU SERVEUR

    int len = client.available();
  
    if (len > 0) {
        byte buffer[80];
        if (len > 80) len = 80;
        client.read(buffer, len);
        
        if (printWebData) {
          //Serial.write(buffer, len); 
        }
    
        byteCount = byteCount + len;

        Serial.println("RECEIVED: ");
        for(int i = 0; i < len; i++){
          Serial.print(char(buffer[i]));
        }
        
        Serial.println();
  
    }


      // DÉCONNECTION DU SERVEUR
      if (client.connected()) { 
          endMicros = micros();
          Serial.println();
          Serial.println("Déconnection du serveur...");
          client.stop();
          Serial.println();

      }
  
  } // fin void Repeat()

  
