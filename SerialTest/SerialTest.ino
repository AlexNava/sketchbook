const int INBUFSIZE = 64;
const int OUTBUFSIZE = 64;

char inBuf[INBUFSIZE] = "";
char inBufPtr = 0;

bool dequeueSerial()
{
  while (Serial.available() > 0)
  {
    if (inBufPtr < INBUFSIZE - 1)
    {
      Serial.readBytes(inBuf + inBufPtr, 1);
      if (inBuf[inBufPtr] == '\r')
      {
        // ignore it
      }
      else if (inBuf[inBufPtr] == '\n')
      {
        inBuf[inBufPtr] = '\0';
        inBufPtr = 0;
      }
      else
        inBufPtr++;
    }
    else  // inBufPtr == INBUFSIZE - 1, accept only newline
    {
      Serial.readBytes(inBuf + inBufPtr, 1);
      if (inBuf[inBufPtr] == '\n')
      {
        inBuf[inBufPtr] = '\0';
        inBufPtr = 0;
      }
    }
  }
  return (inBufPtr == 0);
}

void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);
  // initialize serial communication
  Serial.begin(115200);
}

void loop() {
  // Keep looping here until the init string is received on the serial port
  // meanwhile, keep sending a keepalive string
  bool bInit = false;
  while (!bInit)
  {
    Serial.write("YO");
    delay(1000);

    if (dequeueSerial()
      && (inBuf[0] != 0)
      && (strcmp(inBuf, "GO") == 0))
      bInit = true;
  }

  // Wait for queries and setup
  bool bSetup = false;
  while (!bSetup)
  {
    int sensorValue = analogRead(A0);
    // print out the value you read:
    Serial.println(sensorValue);
    delay(1);
  }
}
