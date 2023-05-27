#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>
#include <RTClib.h>

#define TINGGI_LAYAR 64 // Tinggi layar OLED yang digunakan
#define LEBAR_LAYAR 128 // Lebar layar OLED yang digunakan

Adafruit_SSD1306 oled(LEBAR_LAYAR, TINGGI_LAYAR, &Wire, -1);
RTC_DS1307 rtc;
Servo myservo;

int feedTimeMorning = 8; // Waktu pemberian pakan pagi (jam)
int feedTimeAfternoon = 12; // Waktu pemberian pakan siang (jam)
int feedTimeEvening = 18; // Waktu pemberian pakan sore (jam)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(26);
  myservo.write(0);
  // myservo.write(180);

  if (!rtc.begin()) {
    Serial.println("RTC tidak ditemukan");
    Serial.flush();
    abort();
  }

  // initialize OLED display with I2C address 0x3C
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }

  oled.clearDisplay();
  oled.setTextSize(1);         // Atur ukuran text
  oled.setTextColor(WHITE);    // Atur warna text
  oled.setCursor(20, 20);       // Atur posisi text pada display
  oled.println("SmartFish Aquarium"); // Text yang dicetak
  delay(2000);
  oled.display(); 
  oled.clearDisplay();// menampilkan display OLED

}

void loop() {
  // put your main code here, to run repeatedly:
  DateTime now = rtc.now();
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  oled.setTextSize(1);
  oled.setTextColor(WHITE); 
  oled.setCursor(0,10);
  oled.print(String(now.month()) + "/");
  oled.print(String(now.year() - 2000) + " - ");
  oled.print(String(now.hour()) + ":");
  oled.print(String(now.minute()));
  oled.display();

  // Memeriksa waktu saat ini dengan waktu pemberian pakan
  if (now.hour() == feedTimeMorning && now.minute() == 0 && now.second() == 0) {
    feedFish(); // Memberi makan ikan pada waktu pagi
  } else if (now.hour() == feedTimeAfternoon && now.minute() == 0 && now.second() == 0) {
    feedFish(); // Memberi makan ikan pada waktu siang
  } else if (now.hour() == feedTimeEvening && now.minute() == 0 && now.second() == 0) {
    feedFish(); // Memberi makan ikan pada waktu sore
  }
}

void feedFish() {
  // Aktifkan motor pakan untuk beberapa detik
  myservo.write(90);
  delay(2000); // Durasi motor berputar (2 detik)
  myservo.write(0);
  delay(10000); // Tunggu selama 10 detik sebelum program melanjutkan
}