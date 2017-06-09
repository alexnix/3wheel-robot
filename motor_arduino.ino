#include <SoftwareSerial.h>
SoftwareSerial hc05(8, 10); // RX, TX
int power_1 = 9;
int power_2 = 13;
int power_3 = 11;

void motor1_ahead() {
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
}

void motor2_ahead() {
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
}

void motor3_ahead() {
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
}

void motor1_back() {
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
}

void motor2_back() {
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
}

void motor3_back() {
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  pinMode(power_1, OUTPUT);
  pinMode(power_2, OUTPUT);
  pinMode(power_3, OUTPUT);

  motor1_ahead();
  motor2_ahead();
  motor3_ahead();

  //  for(int p = 0; p <= 250; p+=10) {
  //    analogWrite(power_1, p);
  //  }

  analogWrite(power_1, 0);
  //  delay(1000);
  analogWrite(power_2, 0);
  //  delay(1000);
  analogWrite(power_3, 0);
  //  delay(1000);
  Serial.begin(9600);
  hc05.begin(9600);
}
char command[20];
int p = 0;

void exec(char *command) {
  char buf[4];
  int p;

  // Read PWM value for motor 1
  memcpy(buf, &command[0], 4 * sizeof(char));
  p = atoi(buf);
  if (p > 0) {
    motor1_ahead();
    analogWrite(power_1, p);
  }
  else {
    motor1_back();
    analogWrite(power_1, p * -1);
  }

  // Read PWM value for motor 2
  memcpy(buf, &command[5], 4 * sizeof(char));
  p = atoi(buf);
  if (p > 0) {
    motor2_ahead();
    analogWrite(power_2, p);
  }
  else {
    motor2_back();
    analogWrite(power_2, p * -1);
  }

  // Read PWM value for motor 3
  memcpy(buf, &command[10], 4 * sizeof(char));
  p = atoi(buf);
  if (p > 0) {
    motor3_ahead();
    analogWrite(power_3, p);
  }
  else {
    motor3_back();
    analogWrite(power_3, p * -1);
  }
}

void loop() {

  if (hc05.available() > 0) { // Checks whether data is comming from the serial port
    char c = hc05.read();
    if ( c != '\n' && c != 0 ) {
      command[p++] = c;
    } else if (c == '\n') {
      command[p] = '\0';
      exec(command);
      p = 0;
    }
  }

}
