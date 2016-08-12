
//Network related ram
bool sdrFound = false;
bool sFound = false;
bool dFound = false;
bool rFound = false;

//recieved ram
byte byteLW;
byte byteRW;
byte Digital1;
byte checksum;
int Lspeed;
int Rspeed;

//define ram
//byte Bad_Tx;
//word No_Tx =0;
//used for timeouts
unsigned long currentTime;
unsigned long timeOfLastGoodPacket = 0;
  int E1 = 5; //Left Speed Control PWM pin
  int E2 = 6; //Right Speed Control PWM pin
  int M1 = 4; //Left direction Control pin
  int M2 = 7; //Right direction Control pin
  int MD1; //left motor direction
  int MD2; //right motor direction

//setup serial coms for Dfrobot Romeo v2 (Leonardo) using xbee s1
void setup() {

  int i;
  for (i = 4; i<=7; i++) pinMode (i, OUTPUT);
  Serial.begin(9600); //local usb for serial monitor debug
  Serial1.begin(9600); //serial port to recieve xbee data
  allStop(); //make sure all motors are off.
}

void loop() {
  currentTime = millis();
  processSerial();
  timeout();
  delay(10);
}

void allStop(){ //turn off all motors
  digitalWrite(E1, LOW);
  digitalWrite(E2, LOW);
}

void processSerial(){
  unsigned char inputBufferTemp;
  byte checksumTest;

  //Wait for serial1
  if (Serial1.available() > 0){
    if (!sFound) {
      inputBufferTemp = Serial1.read();
      if(inputBufferTemp == 0x53) {sFound = true;}
      else {sFound = false; dFound = false; rFound = false; sdrFound = false; }
    }
      
    if (!dFound) {
      inputBufferTemp = Serial1.read();
      if (inputBufferTemp == 0x44) { dFound = true;}
      else {sFound = false; dFound = false; rFound = false; sdrFound = false; }
    }

    if (!rFound) {
      inputBufferTemp = Serial1.read();
      if (inputBufferTemp == 0x52) { rFound = true; sdrFound = true; Serial.println("sdrFound");}
      else {sFound = false; dFound = false; rFound = false; sdrFound = false; }
    }

    if (sdrFound && (Serial1.available() > 3 )) {
      //store bytes in the appropriate variables
      byteLW = Serial1.read();
      byteRW = Serial1.read();
      Digital1 = Serial1.read();
      checksum = Serial1.read();
      Serial.println(byteLW); Serial.println(byteRW);

      //clear flags
      sdrFound = false;
      sFound = false;
      dFound = false;
      rFound = false;

      //calculate checksum
      checksumTest = byteLW + byteRW + byte(Digital1);
      //compare our calculatedchecksum to the expected
      if (checksumTest != checksum) {
        return;
      }

      //we found a good packet, set current time
      timeOfLastGoodPacket = currentTime;

      // driving info
      Lspeed = map(byteLW, 0, 250, -255, 255); //change joystick readings
      Rspeed = map(byteRW, 0, 250, -255, 255);
      MD1 = 0; MD2 = 0; // 0= LOW (forward on mine)
      if (Lspeed < 3) MD1 = 1; //joystick reverse, change motor direction. 1=HIGH (reverse on mine)
      if (Rspeed < 3) MD2 = 1; 
      Lspeed = abs(Lspeed); //change negative speeds, to positive
      Rspeed = abs(Rspeed);

      
      
      if (Lspeed <= 5) Lspeed = 0;//anything below 5 is centered joystick
      if (Rspeed <= 5) Rspeed = 0;

      analogWrite (E1, Lspeed); //motor 1 speed
      digitalWrite (M1, MD1);  //motor 1 direction
      analogWrite (E2, Rspeed);  //motor 2 speed
      digitalWrite (M2, MD2);  //motor 2 direction
      
      Serial.print("LW ");Serial.println(Lspeed);Serial.println(MD1);
      Serial.print("RW ");Serial.println(Rspeed);Serial.println(MD2);
      }

      
    }
  }


void timeout() {
  if (currentTime > (timeOfLastGoodPacket + 1000)) {
    allStop();
    timeOfLastGoodPacket = currentTime;
  }
}

