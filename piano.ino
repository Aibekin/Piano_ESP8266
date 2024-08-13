#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

ESP8266WebServer server(80);

const int piezoPin = D0;

void setup() {
  pinMode(piezoPin, OUTPUT);
  Serial.begin(115200);
  
  // Подключение к Wi-Fi
  WiFi.softAP(ssid, password);
  
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  
  // Определение маршрутов
  server.on("/", handleRoot);
  server.on("/start_note", handleStartNote);
  server.on("/stop_note", handleStopNote);
  server.on("/play_song", handlePlaySong);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  // Массив кнопок с нотами, включая диезы и бемоли
  String notes[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "Bb", "Db", "Eb", "Gb", "Ab"};
  
  // Создание кнопок для нот
  String html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style> * { box-sizing: border-box; padding: 0; margin: 0; } body { padding: 50px; display: flex; flex-direction: column; align-items: center; justify-content: flex-start; } h1 { text-align: center; } .piano { width: 280px; height: 80px; position: relative; } .harmonic { width: 280px; height: 120px; display: flex; } .diez2 { position: absolute; z-index: 1000; top: 0; right: 25px; display: flex; gap: 10px; } .diez1 { position: absolute; z-index: 1000; top: 0; left: 25px; display: flex; gap: 10px; } .btn { width: 40px; height: 120px; background-color: #fff; border: 1px solid #000; cursor: pointer; display: flex; flex-direction: column; justify-content: flex-end; align-items: center; padding-bottom: 15px; } .btn_diez { width: 30px; height: 70px; background-color: #000; color: #fff; } </style></head><body><h1>Piano</h1><div class='piano'><div class='harmonic'><button class='btn' id='C'>C</button><button class='btn' id='D'>D</button><button class='btn' id='E'>E</button><button class='btn' id='F'>F</button><button class='btn' id='G'>G</button><button class='btn' id='A'>A</button><button class='btn' id='B'>B</button></div><div class='diez1'><button class='btn btn_diez' id='C#'>C#</button><button class='btn btn_diez' id='D#'>D#</button></div><div class='diez2'><button class='btn btn_diez' id='F#'>F#</button><button class='btn btn_diez' id='G#'>G#</button><button class='btn btn_diez' id='A#'>A#</button></div></div><script> btns = document.querySelectorAll('.btn').forEach((a) => { a.addEventListener('mousedown', () => { fetch('/start_note?note=' + a.id); }); a.addEventListener('mouseup', () => { fetch('/stop_note'); }); }); </script></body></html>";
  
  server.send(200, "text/html", html);
}

void handleStartNote() {
  String note = server.arg("note");
  int frequency = 0;

  // Основные ноты
  if (note == "C") frequency = 261;  // Нота C (До)
  else if (note == "D") frequency = 293;  // Нота D (Ре)
  else if (note == "E") frequency = 329;  // Нота E (Ми)
  else if (note == "F") frequency = 349;  // Нота F (Фа)
  else if (note == "G") frequency = 392;  // Нота G (Соль)
  else if (note == "A") frequency = 440;  // Нота A (Ля)
  else if (note == "B") frequency = 493;  // Нота B (Си)
  
  // Диезы
  else if (note == "C#") frequency = 277;  // Нота C# (До#)
  else if (note == "D#") frequency = 311;  // Нота D# (Ре#)
  else if (note == "F#") frequency = 370;  // Нота F# (Фа#)
  else if (note == "G#") frequency = 415;  // Нота G# (Соль#)
  else if (note == "A#") frequency = 466;  // Нота A# (Ля#)
  
  if (frequency > 0) {
    tone(piezoPin, frequency);  // Играть ноту до тех пор, пока не будет вызвано stopNote
  }

  server.send(200, "text/plain", "Note " + note + " started");
}

void handleStopNote() {
  noTone(piezoPin);  // Останавливает воспроизведение ноты
  server.send(200, "text/plain", "Note stopped");
}

void handlePlaySong() {
  String song = server.arg("song");

  if (song == "creep") {
    playCreep();
  } else if (song == "imperial_march") {
    playImperialMarch();
  } else if (song == "cant_stop") {
    playCantStop();
  }

  server.send(200, "text/plain", "Playing " + song);
}

// Пример функций для воспроизведения мелодий
void playCreep() {
  int melody[] = {261, 293, 329, 349, 392, 440, 493};
  int noteDurations[] = {4, 4, 4, 4, 4, 4, 4}; // Пример длительностей

  for (int thisNote = 0; thisNote < 7; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(piezoPin, melody[thisNote], noteDuration);
    delay(noteDuration * 1.3);
  }
}

void playImperialMarch() {
  int melody[] = {392, 392, 392, 311, 466, 392, 311, 466, 392};
  int noteDurations[] = {4, 4, 4, 3, 8, 4, 3, 8, 4};

  for (int thisNote = 0; thisNote < 9; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(piezoPin, melody[thisNote], noteDuration);
    delay(noteDuration * 1.3);
  }
}

void playCantStop() {
  int melody[] = {440, 466, 392, 440, 466, 392, 392};
  int noteDurations[] = {4, 4, 4, 4, 4, 4, 4};

  for (int thisNote = 0; thisNote < 7; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(piezoPin, melody[thisNote], noteDuration);
    delay(noteDuration * 1.3);
  }
}

