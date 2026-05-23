// -------- PIN DEFINITIONS --------
#define MQ3_PIN 34   // MQ3 Analog
#define BUZZER 22
#define RELAY 23

// Motor Driver Pins
#define IN1 18
#define IN2 19
#define IN3 21
#define IN4 5
#define ENA 25
#define ENB 26

int alcoholThreshold = 300;  // Adjust after testing

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER, OUTPUT);
  pinMode(RELAY, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // PWM setup
  ledcAttach(ENA, 1000, 8);
  ledcAttach(ENB, 1000, 8);

  digitalWrite(RELAY, HIGH); // Engine ON
}

// -------- MAIN LOOP --------
void loop() {

  int alcoholValue = analogRead(MQ3_PIN);
  Serial.println(alcoholValue);

  // -------- ALCOHOL DETECTED --------
  if (alcoholValue > alcoholThreshold) {

    digitalWrite(BUZZER, HIGH); // Alert ON

    // Gradual speed reduction
    for (int speed = 200; speed >= 0; speed -= 20) {

      ledcWrite(ENA, speed);
      ledcWrite(ENB, speed);

      // Move forward
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

      delay(300);
    }

    // Turn to roadside (LEFT)
    ledcWrite(ENA, 80);   // Left motor slow
    ledcWrite(ENB, 150);  // Right motor fast

    delay(1000);

    // STOP vehicle
    ledcWrite(ENA, 0);
    ledcWrite(ENB, 0);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    // Cut engine
    digitalWrite(RELAY, LOW);

  }

  // -------- NORMAL CONDITION --------
  else {

    digitalWrite(BUZZER, LOW);
    digitalWrite(RELAY, HIGH);

    ledcWrite(ENA, 200);
    ledcWrite(ENB, 200);

    // Move forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }

  delay(500);
}
