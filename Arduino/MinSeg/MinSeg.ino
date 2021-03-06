
#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#define LED 13
#define MOTOR_PIN_F 44
#define MOTOR_PIN_B 45
#define ENCODER_F 18
#define ENCODER_B 19
//#define ENCODER_F 7
//#define ENCODER_B 6

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
double angle;

int time = 0;
int pos = 0;
boolean goingForward = false;

void setup() {
  setupAccelgyro();
  setupPins();
  setupInterrupts();
  Serial.begin(115200);
  Serial.println("Setup done");
}

void setupAccelgyro(){
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    
    accelgyro.initialize();
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void setupPins() {
  pinMode(LED, OUTPUT);
  pinMode(MOTOR_PIN_F, OUTPUT);
  pinMode(MOTOR_PIN_B, OUTPUT);
  pinMode(ENCODER_F, INPUT);
  pinMode(ENCODER_B, INPUT);
}

void setupInterrupts() {
  attachInterrupt(digitalPinToInterrupt(ENCODER_F), changePos, RISING);
}

void loop() {
  if (Serial.available()) {
    pos = readInt();
  }
  getAccelgyroValues();
  //printAccelgyroValues();
  simpleControl();
  angle = atan2(az, ay);
  angle = (angle < 0) ? angle + PI : angle - PI;
  //Serial.println("_________________________________________________________________________________________________________________________________");
  Serial.print("Angle: ");
  //Serial.print('\t');
  Serial.print(angle*180.0/PI);
  Serial.print('\t');
  Serial.print('\t');
  Serial.print("Position: ");
  //Serial.print('\t');
  Serial.println(pos);
  delay(time);
}

/*
   Read an int, doesn't block and reads even if it is not an int
*/
int readInt() {
  String s = "";
  while (Serial.available()) {
    s += (char)Serial.read();
  }
  return s.toInt();
}

void simpleControl(){
  if(abs(angle) > (PI/2)){
    stopMotor();
    return;
  }
  if(angle*180.0/PI > 6){
    goingForward = true;
  } else{
    goingForward = false;
  }
  if(goingForward){
    forward();
    //Serial.println("forward");
  }else{
    backward();
    //Serial.println("backward");
  }
}

void getAccelgyroValues(){
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
}

void printAccelgyroValues(){
        // display tab-separated accel/gyro x/y/z values
        Serial.print("a/g:\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.println(gz);
}

void forward() {
  digitalWrite(MOTOR_PIN_B, LOW);
  digitalWrite(MOTOR_PIN_F, HIGH);
  digitalWrite(LED, HIGH);
}

void forward(int speed) {
  digitalWrite(MOTOR_PIN_B, LOW);
  analogWrite(MOTOR_PIN_F, speed);
  digitalWrite(LED, HIGH);
}

void backward() {
  digitalWrite(MOTOR_PIN_F, LOW);
  digitalWrite(MOTOR_PIN_B, HIGH);
  digitalWrite(LED, LOW);
}

void backward(int speed) {
  digitalWrite(MOTOR_PIN_F, LOW);
  analogWrite(MOTOR_PIN_B, speed);
  digitalWrite(LED, LOW);
}

void stopMotor() {
  digitalWrite(MOTOR_PIN_F, LOW);
  digitalWrite(MOTOR_PIN_B, LOW);
}

void changePos() {
  if ((digitalRead(ENCODER_F) << 1) + digitalRead(ENCODER_B) == 2) {
    ++pos;
  } else {
    --pos;
  }
}
