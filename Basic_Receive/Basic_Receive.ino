#include <JeeLib.h>

typedef struct {
  unsigned int ping;
  int value1;
  unsigned long ms;
} Payload;
Payload inData, outData;

void consumeInData ()
{
  Serial.print(inData.ping);
  Serial.print(",");
  Serial.print(inData.value1);
  Serial.print(",");
  Serial.print(inData.ms);
  Serial.println(";");
}

void setup ()
{
  rf12_config();
  Serial.begin(57600);
}

void loop ()
{
  if (rf12_recvDone() && rf12_crc == 0 && rf12_len == sizeof inData) {
    memcpy(&inData, (byte*) rf12_data, sizeof inData);
    
    if (RF12_WANTS_ACK)
    {
      Serial.println(" -> ACK");
      rf12_sendStart(RF12_ACK_REPLY, 0, 0);
    }
    
    consumeInData();
  }
}

