#include <JeeLib.h>

MilliTimer sendTimer, ledTimer;
typedef struct {
  unsigned int ping;
  int value1;
  unsigned long ms;
} Payload;
Payload inData, outData;
byte pendingOutput;
byte led = 13;

static byte produceOutData ()
{
  ++outData.ping;
  
  outData.value1 = analogRead(0);
  
  outData.ms = millis();
  
  return 1;
}

void setup()
{
  rf12_config();
  pinMode(led, OUTPUT);
}

void loop()
{
  if (rf12_recvDone() && rf12_crc == 0 && rf12_len == sizeof inData) {
    memcpy(&inData, (byte*) rf12_data, sizeof inData);
  }
  
  if (sendTimer.poll(5000))
    pendingOutput = produceOutData();
  
  if (pendingOutput && rf12_canSend()) {
    digitalWrite(led, HIGH);
    
    ledTimer.set(100);
    
    rf12_sendStart(0, &outData, sizeof outData);
    pendingOutput = 0;
  }
  
  if (ledTimer.poll())
    digitalWrite(led, LOW);
}
