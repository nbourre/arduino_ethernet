#ifndef Reseau_h
#define Reseau_h

#define ADDR_MAC_ADDR 0

#define MAC_ADDR_SIZE 6
#define IP_ADDR_SIZE  13

#include <SPI.h>
#include <Ethernet.h>

struct MAC
{
  unsigned char addr [MAC_ADDR_SIZE];
};

class Reseau{
  
  public:

    Reseau();
    Reseau(String &ipAddr);
    Reseau(char* ipAddr);


    void Init();
    void Start();
    void Repeat();

    void SetSendData(int id, String &startDate, String &startTime, float ph, float temp, bool agit, char sender);
    
    unsigned char *GetMACAddr();
    IPAddress GetIPAddr();
    EthernetClient &GetEthernetClient();
        
  private:
    void readMac();
    void printMac();

    // Paramètres carte et serveur
    MAC macAddr;
    
    char server[IP_ADDR_SIZE]; 

    char *ipAddr;

    bool isInitialized = false;

    IPAddress ip;
    IPAddress myDns;

    EthernetClient client;
        
    unsigned long beginMicros, endMicros;
    unsigned long byteCount;
    bool printWebData;  // set to false for better speed measurement
    int compteur;
    String compteurString;

    String sendData;

  
};



#endif
