#ifndef BIN_CONSTS
#define BIN_CONSTS

#define pinDht 10
#define pinServo 7
#define ir1 11
#define ir2 12
#define MAX 200
#define BIOSAMPLES 3

int bin1, bin2, bio = 0;
int temp, humid;
bool cap, ind;
char strTemp1[32],strTemp2[32];
const char * phoneNum = "0999999999";
const char * cmd[] = {"TRASH", "TESTS"};

struct plate {
    static const int flat = 92;
    static const int bio = 20;
    static const int nonbio = 160;
};

struct pinUSonic {
    static const int echo1 = 30;
    static const int trig1 = 31;
    static const int echo2 = 32;
    static const int trig2 = 33;
};

class smsSerial {
  public:
    smsSerial(byte rxPin, byte txPin) : textMessage(""),  _softSer(rxPin, txPin) {
      
    }
    ~smsSerial() {

    }
    void begin(unsigned int baud_rate) {
      _softSer.begin(baud_rate);
    }

    void recv_msg(String &textMessage){
      _softSer.print("AT+ CMGF=1\r");
      delay(50);
      _softSer.print("AT+CNMI=2,2,0,0,0\r");
      delay(50);
      if(_softSer.available()){
        while (_softSer.available())
        {
          textMessage += char(_softSer.read());
        }
        
        delay(100);
        Serial.println(textMessage);
        delay(100);
        if (textMessage.length() >= 53) {
            _softSer.print("AT+CMGD=1,3\r"); //deletes recv read sms
        }
        if(textMessage.indexOf(cmd[0])>=0) {
          textMessage = "";
          send_msg(textMessage, phoneNum);
        } else if(textMessage.indexOf(cmd[1]) > 0){
          textMessage = "";
          send_msg(textMessage, phoneNum);
        }
        textMessage = "";
      }
    }

    void send_msg(String txt, String number){
      _softSer.print("AT+CMGF=1\r");
      delay(100);
      _softSer.println("AT+CMGS=\""+ number +"\"");
      delay(100);
      _softSer.println(txt);
      delay(100);
      _softSer.println((char)26);
      delay(1000);
      _softSer.println();
      delay(2000);
    }
  private:
    SoftwareSerial _softSer;
    String textMessage;
};

#endif
