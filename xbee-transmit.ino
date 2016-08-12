

int LW; //left wheels / joystick
int RW; //Right wheels / joystick
double tempLW, tempRW; //temporary variables used in calculations

//transmitted packet
byte byteLW;
byte byteRW;
byte Digital1;
byte checksum;

//misc variable
int deadband = 5;


void setup() {
 Serial1.begin(9600); //xbee series 1 on Leonardo 
 Serial.begin(9600); //serial monitor used for testing, leonardo only
 }

void loop() {
  reinitVars(); //nulls out temporary variables
  captureData(); //retrieves new data
  packetizeData(); //assembles data to be sent
  transmitData(); //send data off to XBee
  debugData();//prints debug data to local serial console

  delay(20); //delay for consistant data  
}

//reinitialize temporary variables
void reinitVars() {
  Digital1 = 0;
  tempLW = 0;
  tempRW = 0;
}

//retrieve analog data and conform to specs
void captureData() {
  LW = analogRead(A4); //left joystick X axis
  RW = analogRead(A2); //right joystick X axis
  tempLW = map(LW, 0, 1024, 0, 250);
  tempRW = map(RW, 0, 1024, 0, 250);
  //if ((tempLW > (0 - deadband)) && (tempLW < deadband))
  // {tempLW = 0; }
  //if ((tempRW > (0 - deadband)) && (tempRW < deadband))
  // {tempRW = 0; }
}

//assemble the data to be sent
void packetizeData(){


  //convert transmitted data into bytes
  byteLW = byte(tempLW);
  byteRW = byte(tempRW);
  checksum = byteLW + byteRW + byte(Digital1);
}

void transmitData() { //xbee communication
  Serial1.write("S");
  Serial1.write("D");
  Serial1.write("R");
  Serial1.write(byteLW);
  Serial1.write(byteRW);
  Serial1.write(byte(Digital1));
  Serial1.write(checksum);
  delay(20);
}

void debugData(){
  Serial.print("[LW: "); Serial.print(byteLW); Serial.print("]");
  Serial.print("[RW: "); Serial.print(byteRW); Serial.print("]");
  Serial.print("[D1: "); Serial.print(byte(Digital1), BIN); Serial.print("]");
  Serial.print(checksum);
  Serial.println("");
}

