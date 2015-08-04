#include <SoftwareSerial.h>
#include <EEPROM.h>

#define RxD 10  //CONNECT THIS TO TxD OF BLUETOOTH MODULE
#define TxD 11  //CONNECT THIS TO RxD OF BLUETOOTH MODULE

SoftwareSerial bluetoothSerial(RxD,TxD);

char MESSAGE_BREAK[1] = {1};
char MESSAGE_END[1] = {2};

//char REQUEST = 14;
//char UPDATE = 15;
char RESPONSE = 16;
//char MENU = 17;
char REQUEST_MENU = 17;
char RESPONSE_MENU = 18;
char NULL_CHAR = 255;

//char MESSAGE_BREAK = '`';
//char MESSAGE_END = '~';

//String REQUEST_MENU = "REQUEST`MENU";

char m[20] = {0};
char recvChar[1] = {0};
String message = "";

void setup()
{  
  Serial.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBluetoothConnection();
  
//  char something[] = "R";
//  char somethingelse[] = {85};
//  strcat(m, something);
//  strcat(m, somethingelse);
//  Serial.println(m[0]);
//  Serial.println(m[1]);
  
  //char test[5] = {'h', 'e', 'l', 'l', 'o'};
  //sendMessage(test);
  
  //printMemory();
  sendMenu();
  //handleMessage("UPDATE<>ADD_LIQUOR`Rum`2");
}

void loop()
{
  while(1) {
    
    if(bluetoothSerial.available())
    {
      recvChar[0] = bluetoothSerial.read();
      if (recvChar[0] == MESSAGE_END[0])
      {
        //Serial.println(message);
        handleMessage(m);
        sendMessage(m);
        memset(m, 0, sizeof(m));
      }
      else
      {
        strcat(m, recvChar);
      }
    }
    
  }
}

void setupBluetoothConnection()
{
  bluetoothSerial.begin(9600);
}

void sendMessage(char message[])
{
  for (int i = 0; i < 20; i++)
  {
    //bluetoothSerial.print(message[i]);
    Serial.print(message[i]);
  }
  //bluetoothSerial.print(MESSAGE_END);

//  int length = message.length() + 2;
//  char c[length];
//  
//  (message + MESSAGE_END).toCharArray(c, length);
//  
//  for (int i = 0; i < length; i++)
//  {
//    bluetoothSerial.print(c[i]);
//  }
}

void handleMessage(char message[])
{
  if (message[0] == REQUEST_MENU)
  {
    sendMenu();
  }
//  if (m.startsWith("REQUEST"))
//  {
//    m = m.substring(m.indexOf(MESSAGE_BREAK)+1);
//    if (m.startsWith("MENU"))
//    {
//      sendMenu();
//    }
//  }
//  else if (m.startsWith("UPDATE"))
//  {
//    m = m.substring(m.indexOf("<>")+2);
//    if (m.startsWith("ADD_LIQUOR"))
//    {
//      m = m.substring(m.indexOf(MESSAGE_BREAK)+1);
//      String name = m.substring(0,m.indexOf(MESSAGE_BREAK));
//      int nozzle = m.substring(m.indexOf(MESSAGE_BREAK)+1).toInt();
//    }
//  }
}

void sendMenu()
{
  //String menu1 = String(RESPONSE) + MESSAGE_BREAK + String(MENU);
  
  char menu[180];
  for (int i = 0; i < 8; i++)
  {
    char liquor[20];
    for (int j = 0; j < 20; j++)
    {
      liquor[j] = EEPROM.read(i*20 + j);
    }
    
    if (liquor[0] != NULL_CHAR)
    {
      Serial.println(liquor[0]);
      
      //char pos[1];
      //String(i+1).toCharArray(pos, 1);
      
      //strcat(menu, MESSAGE_BREAK);
      //strcat(menu, pos);
      //strcat(menu, MESSAGE_BREAK);
      //strcat(menu, liquor);
      memset(liquor, 0, sizeof(liquor));
    }
  }
  
  sendMessage(menu);
  
//  String menu = "RESPONSE<>MENU";
//  for (int i = 0; i < 8; i++)
//  {
//    String liquor = "";
//    for (int j = 0; j < 20; j++)
//    {
//      byte n = EEPROM.read(i*20 + j);
//      liquor += n;
//    }
//    
//    if (!liquor.equals(""))
//    {
//      menu += MESSAGE_BREAK + String(i+1) + MESSAGE_BREAK + liquor;
//      liquor = "";
//    }
//    
//    sendMessage(menu);
//  }
  
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
