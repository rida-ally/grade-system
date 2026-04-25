#include <Wire.h>
#include <EEPROM.h>

extern "C" void led_animation(); // Assembly function

const int greenLed = 2;
const int whiteLed = 3;
const int orangeLed = 4;
const int redLed = 5;

int marks[3];
const int maxSaved = 3;

void setup() {
  Serial.begin(9600);
  pinMode(greenLed, OUTPUT);
  pinMode(whiteLed, OUTPUT);
  pinMode(orangeLed, OUTPUT);
  pinMode(redLed, OUTPUT);

  float total = 0;
  int count = 0;

  for (int i = 0; i < maxSaved; i++) {
    float p;
    EEPROM.get(i * sizeof(float), p);
    if (p >= 0.01 && p <= 100) {
      total += p;
      count++;
    }
  }

  Serial.println("📘 Welcome to the Grading System!");
  if (count <= 0) {
    Serial.println("No stored results yet.");
  }

  Serial.println("\nEnter 3 marks (0-100) OR type 'avg' to view average:");
}

void loop() {
  Serial.println("\nEnter 3 marks OR type 'avg' to see average:");

  String input;
  while (Serial.available() == 0) {}
  input = Serial.readStringUntil('\n');
  input.trim();

  if (input.equalsIgnoreCase("avg")) {
    float total = 0;
    int count = 0;
    float temp;

    for (int i = 0; i < maxSaved; i++) {
      EEPROM.get(i * sizeof(float), temp);
      if (temp >= 0.01 && temp <= 100) {
        total += temp;
        count++;
      }
    }

    if (count > 0) {
      float avg = total / count;
      Serial.print("📊 Average of stored results: ");
      Serial.print(avg, 1);
      Serial.println("%");
      showGradeLED(avg);
      delay(3000);
      turnOffLEDs();
    } else {
      Serial.println("⚠ No results stored yet!");
    }

    delay(1000);
    return;
  }

  marks[0] = input.toInt();
  for (int i = 1; i < 3; i++) {
    while (Serial.available() == 0) {}
    input = Serial.readStringUntil('\n');
    input.trim();
    marks[i] = input.toInt();
  }

  for (int i = 0; i < 3; i++) {
    Serial.print("Mark ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(marks[i]);
  }

  int sum = marks[0] + marks[1] + marks[2];
  float percentage = (sum / 300.0) * 100;

  String grade;
  int resultLED;
  if (percentage >= 80) {
    grade = "A+";
    resultLED = greenLed;
  } else if (percentage >= 65) {
    grade = "Pass";
    resultLED = whiteLed;
  } else if (percentage >= 40) {
    grade = "Average";
    resultLED = orangeLed;
  } else {
    grade = "Fail";
    resultLED = redLed;
  }

  // Assembly LED animation
  led_animation();

  digitalWrite(resultLED, HIGH);

  Serial.print("📊 Percentage: ");
  Serial.print(percentage, 1);
  Serial.println("%");
  Serial.print("🎓 Grade: ");
  Serial.println(grade);

  for (int i = maxSaved - 1; i > 0; i--) {
    float prev;
    EEPROM.get((i - 1) * sizeof(float), prev);
    EEPROM.put(i * sizeof(float), prev);
  }
  EEPROM.put(0, percentage);

  delay(3000);
  turnOffLEDs();
}

void turnOffLEDs() {
  digitalWrite(greenLed, LOW);
  digitalWrite(whiteLed, LOW);
  digitalWrite(orangeLed, LOW);
  digitalWrite(redLed, LOW);
}

void showGradeLED(float avg) {
  turnOffLEDs();
  if (avg >= 80) digitalWrite(greenLed, HIGH);
  else if (avg >= 65) digitalWrite(whiteLed, HIGH);
  else if (avg >= 40) digitalWrite(orangeLed, HIGH);
  else digitalWrite(redLed, HIGH);
}
