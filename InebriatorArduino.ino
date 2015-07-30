#include <SoftwareSerial.h>
#include <EEPROM.h>

#define RxD 10
#define TxD 11

SoftwareSerial bluetoothSerial(RxD,TxD);

//char MESSAGE_BREAK = 1;
//char MESSAGE_END = 2;

//char REQUEST = 14;
//char UPDATE = 15;
char RESPONSE = 16;
char MENU = 17;

char MESSAGE_BREAK = '`';
char MESSAGE_END = '~';

String REQUEST_MENU = "REQUEST`MENU";

String message = "";

void setup()
{  
  Serial.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBluetoothConnection();
  
  printMemory();
  //sendMenu();
  //handleMessage("UPDATE<>ADD_LIQUOR`Rum`2");
}

void loop()
{
  char recvChar;
  while(1) {
    
    if(bluetoothSerial.available())
    {
      recvChar = bluetoothSerial.read();
      if (recvChar == MESSAGE_END)
      {
        Serial.println(message);
        handleMessage(message);
        //sendMessage(message);
        message = "";
      }
      else
      {
        message += recvChar;
      }
    }
    
  }
}

void setupBluetoothConnection()
{
  bluetoothSerial.begin(9600);
}

void sendMessage(String message)
{
  int length = message.length() + 2;
  char c[length];
  
  (message + MESSAGE_END).toCharArray(c, length);
  
  for (int i = 0; i < length; i++)
  {
    bluetoothSerial.print(c[i]);
  }
}

void handleMessage(String m)
{
  if (m.startsWith("REQUEST"))
  {
    m = m.substring(m.indexOf(MESSAGE_BREAK)+1);
    if (m.startsWith("MENU"))
    {
      sendMenu();
    }
  }
  else if (m.startsWith("UPDATE"))
  {
    m = m.substring(m.indexOf("<>")+2);
    if (m.startsWith("ADD_LIQUOR"))
    {
      m = m.substring(m.indexOf(MESSAGE_BREAK)+1);
      String name = m.substring(0,m.indexOf(MESSAGE_BREAK));
      int nozzle = m.substring(m.indexOf(MESSAGE_BREAK)+1).toInt();
    }
  }
}

void sendMenu()
{
  //String menu1 = String(RESPONSE) + MESSAGE_BREAK + String(MENU);
  String menu = "RESPONSE<>MENU";
  for (int i = 0; i < 8; i++)
  {
    String liquor = "";
    for (int j = 0; j < 20; j++)
    {
      byte n = EEPROM.read(i*20 + j);
      liquor += n;
    }
    
    if (!liquor.equals(""))
    {
      menu += MESSAGE_BREAK + String(i+1) + MESSAGE_BREAK + liquor;
      liquor = "";
    }
    
    sendMessage(menu);
  }
  
  //Serial.println(menu);
    
  //sendMessage(menu);
}

void printMemory()
{
//  String l = "Liquor 1";
//  int length = l.length() + 1;
//  char c1[length];
//  
//  l.toCharArray(c1, length);
//  for (int i = 0; i < length; i++)
//  {
//    EEPROM.write(i, c1[i]);
//  }
  
  for (int i = 0; i < 1024; i++)
  {
    if (i % 100 == 0)
    {
      Serial.println();
    }
    //Serial.print(str(EEPROM.read(i)));
    Serial.print(EEPROM.read(i));
  }
}
