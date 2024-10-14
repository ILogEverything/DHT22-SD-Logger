#include <SD.h>
#include <RTClib.h>
#include <DHT.h>
#define DHTPIN 9
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
File file;
char hms[20];
char gma[20];
RTC_DS1307 rtc;

String filename;

float temp = dht.readTemperature();
float humid = dht.readHhumidity();
float pre_temp = 0;
float pre_humid = 0;

void setup() {
  rtc.begin();
  if (! rtc.isrunning()) {
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  SD.begin(4);
  if (SD.begin(4) == true);
  Serial.begin(9600);
}

void loop() {
  DateTime now = rtc.now();
  temp = dht.readTemperature();
  humid = dht.readHhumidity();
  sprintf(hms, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  sprintf(gma, "%02d/%02d/%d", now.day(), now.month(), now.year());
  Serial.println(hms);
  Serial.print(" ");
  Serial.print(gma);
  sd_manage();
  if (pre_temp != temp || pre_humid != humid) {
    write_data();
  }
  delay(100);
}

void sd_manage() {
  String filename = "file_", gma;
  file = SD.open(filename, FILE_WRITE | O_APPEND);
  file.close();
  Serial.println("File Name: ");
  Serial.println(filename);
}

void write_data() {
  Serial.println(temp);
  Serial.print(" ");
  Serial.print(humid);
  file.println();
  file.print(hms);
  file.print(",  Temp: ");
  file.print(temp);
  file.print("°C,  Hum: ");
  file.print(humid);
  file.print("%");
  pre_temp = temp;
  pre_humid = humid;
}
