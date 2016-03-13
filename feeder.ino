#include <Keypad.h>


const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char mode_key = '1';  // mode marker
int stop_signal = 0;  // indicates stop mode
long current_delta = 0;  // delay counter
//int feed_time_delta = 1000 * 60 * 60;  // feed every 1 hour
long feed_time_delta = 30000; 

void setup() {
  // relay init
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  Serial.begin(9600);

}

void feed(int while_pressed) {
  digitalWrite(13, HIGH);
  if (while_pressed == 0) {
    delay(5000);
    digitalWrite(13, LOW);
  }
}

void stop_feeder() {
  digitalWrite(13, LOW);
}

int night() {
  int value = 0;
  value = analogRead(A0);
  Serial.print("night :");
  Serial.println(value);
  if (value > 180) {
    return 0;
  }
  else {
    return 1;
  }
}

void loop() {
  
  char key = keypad.getKey();
  if (key != NO_KEY) {
    // TODO update indicators
    stop_feeder();
    current_delta = 0;
    mode_key = key;
    Serial.println(mode_key);
  }
  Serial.println(mode_key);
  stop_signal = 0;
  if (mode_key == '0') {
    stop_signal = 1;
    // TODO pulse indicators
    Serial.println("stop");
  }
  
  if (stop_signal != 1 && night() != 1) {
    Serial.println("working");
    if (mode_key == '*') {
      feed(1);
    }

    else {
      Serial.println(current_delta);
     
      if (feed_time_delta < current_delta) {
        Serial.println("feed!");
        feed(0);
        current_delta = 0;
      }
      current_delta = current_delta + 510;
    }
    delay(200);
  } 

  else {
    current_delta = 0;
    delay(200); // feeder stopped or it is too dark to feed
  }

}
