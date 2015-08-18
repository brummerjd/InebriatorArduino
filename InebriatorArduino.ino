#include <SoftwareSerial.h>
#include <EEPROM.h>

#define RxD 10  //CONNECT THIS TO TxD OF BLUETOOTH MODULE
#define TxD 11  //CONNECT THIS TO RxD OF BLUETOOTH MODULE

SoftwareSerial bluetoothSerial(RxD,TxD);

char MESSAGE_BREAK = 1;
char MESSAGE_END = 2;

char REQUEST_DRINK = 17;
char RESPONSE_MAKING_DRINK = 18;
char NULL_CHAR = 255;

char m[60] = {0};
char recvChar = 0;
byte m_length = 0;

void setup()
{  
  Serial.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBluetoothConnection();
  
//  char drink[25] = { REQUEST_DRINK, '1', MESSAGE_BREAK, '0', MESSAGE_BREAK,
//    '1', '.', '5', MESSAGE_BREAK, '0', MESSAGE_BREAK, '0', MESSAGE_BREAK, '0',
//    MESSAGE_BREAK, '0', '.', '2', '5', MESSAGE_BREAK, '0', MESSAGE_END };
//  handleMessage(drink);
}

void loop()
{
  while(1) {
    
    if(bluetoothSerial.available())
    {
      recvChar = bluetoothSerial.read();
      Serial.println(m_length);
      // Append char to received message; there has to be a better way of doing this...
      m[m_length] = recvChar;
      m_length++;
        
      if (recvChar == MESSAGE_END)
      {
        // Show what's happening in Serial window
        Serial.print("Received: ");
        printMessage(m);
        
        handleMessage(m);
        
        // Reset to receive next message
        memset(m, 0, sizeof(m));
        m_length = 0;
      }
    }
  }
}

void setupBluetoothConnection()
{
  bluetoothSerial.begin(9600);
}

void sendMessage(char message[], byte length)
{
  Serial.print("Send: ");
  for (byte i = 0; i < length; i++)
  {
    bluetoothSerial.print(message[i]);
    Serial.print(message[i]);
  }
  bluetoothSerial.print(MESSAGE_END);
  Serial.println();
}

void handleMessage(char message[])
{
  printMessage(message);
  if (message[0] == REQUEST_DRINK)
  {
    Serial.println("Making drink");
    char response[] = {RESPONSE_MAKING_DRINK};
    sendMessage(response, 1);
    
    mixDrink(message+1);
  }
}

void mixDrink(char drink[])
{
  float times[8];
  byte drink_num = 0;
  byte pointer = 0;
  byte start_of_prev_drink = 0;
  
  while(1)
  {
    if (drink[pointer] == MESSAGE_BREAK || drink[pointer] == MESSAGE_END)
    {
      char time_arr[pointer - start_of_prev_drink];
      memcpy(time_arr, &drink[start_of_prev_drink], pointer - start_of_prev_drink);
      times[drink_num] = atof(time_arr);
      
      start_of_prev_drink = pointer + 1;
      drink_num++;
      
      if (drink[pointer] == MESSAGE_END) { break; }
    }
    
    pointer++;
  }
  
  Serial.println("Drink recipe: ");
  for (byte i = 0; i < 8; i++)
  {
    Serial.println(times[i]);
  }
}

void printMessage(char message[])
{
  byte i = 0;
  while (1)
  {
    if (message[i] == MESSAGE_END) { break; }
    Serial.print(message[i]);
    i++;
  }
  Serial.println();
}
