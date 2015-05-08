#include <SPI.h>
#include <SD.h>

const int chipSelect = 6;
char filename[] = "DATA00.TXT";
unsigned long interval = 50;
unsigned long last_read = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }

  for (int i = 0; i < 100; i++) {
    filename[4] = '0' + i/10;
    filename[5] = '0' + i%10;
    if (!SD.exists(filename)) {
      Serial.print("Logging data to file ");
      Serial.println(filename);
      break;
    }
  }
}

void loop() {
  unsigned long now = millis();
  if (interval < (now - last_read)) {
    write_log(now);
    last_read = now;
  }
}

void write_log(unsigned long now) {
  String dataString = String(now);
  dataString += ",";
    
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }
    
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
}








