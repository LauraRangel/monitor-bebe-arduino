#ifndef RemoteXYApiData_h
#define RemoteXYApiData_h  

#include <inttypes.h> 


#define REMOTEXY_PASSWORD_LENGTH_MAX 26

class CRemoteXYThread;
class CRemoteXYComm;
class CRemoteXYConnectionComm;


class CRemoteXYData {

  public:
  uint8_t confVersion;
  uint8_t *conf;
  uint8_t *var;
  uint8_t *accessPassword;
  uint16_t outputLength;
  uint16_t inputLength;
  uint16_t confLength;
  uint8_t *connect_flag;

  
  CRemoteXYThread * threads;  
  CRemoteXYComm * comms;    
  CRemoteXYConnectionComm * connections;    


  public:
  void init (const void * _conf, void * _var, const char * _accessPassword = NULL) {
  
    uint8_t* p = (uint8_t*)_conf;
    uint8_t b = getConfByte (p++);

    if (b==0xff) {
      inputLength = getConfByte (p++);
      inputLength |= getConfByte (p++)<<8;
      outputLength = getConfByte (p++); 
      outputLength |= getConfByte (p++)<<8;
    }
    else {
      inputLength = b;
      outputLength = getConfByte (p++);    
    }
    confLength = getConfByte (p++);
    confLength |= getConfByte (p++)<<8;
    conf = p;
    confVersion = getConfByte (p);
    var = (uint8_t*)_var;
    uint16_t varLength = outputLength + inputLength;
    connect_flag = var + varLength;
    *connect_flag = 0;   
        
    setPassword (_accessPassword);        
    
    p = var;
    uint16_t i = varLength;
    while (i--) *p++=0;    
    
    
    comms = NULL;
    connections = NULL;    
    threads = NULL;
  }
  
  public:
  void setPassword (const char * _accessPassword) {
    accessPassword = NULL;  
    if (_accessPassword != NULL) {
      accessPassword = (uint8_t*)_accessPassword;
    }   
  }
  
  public:
  uint16_t getReceiveBufferSize () {
    uint16_t receiveBufferSize = inputLength;
    if (accessPassword != NULL) {
      if (receiveBufferSize < REMOTEXY_PASSWORD_LENGTH_MAX) receiveBufferSize = REMOTEXY_PASSWORD_LENGTH_MAX;
    }
    receiveBufferSize +=6;     
    return receiveBufferSize;
  }
  
  public:
  inline uint8_t getConfByte (uint8_t* p) {
    return pgm_read_byte_near (p);                                     
  } 
  
  
};




#endif //RemoteXYData_h