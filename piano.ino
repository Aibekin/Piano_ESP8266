#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define C 261.63
#define C1 277.18
#define D 293.66
#define D1 311.13
#define E 329.63
#define F 349.23
#define F1 369.99
#define G 392.00
#define G1 415.30
#define A 440.00
#define A1 466.16
#define B 493.88

const char* ssid = "Redmi Note 12";
const char* password = "12345678";

ESP8266WebServer server(80);

const int piezoPin = D0;
const int LED1 = D1;
const int LED2 = D2;
const int LED3 = D3;

void setup() {
  pinMode(piezoPin, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  Serial.begin(115200);
  
  // Подключение к Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
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
  String notes[] = {"C", "C1", "D", "D1", "E", "F", "F1", "G", "G1", "A", "A1", "B"};
  
  // Создание кнопок для нот
  String html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style> * { box-sizing: border-box; padding: 0; margin: 0; } body { padding: 50px; display: flex; flex-direction: column; align-items: center; justify-content: flex-start; gap: 30px; } h1 { text-align: center; } .piano { width: 490px; height: 200px; position: relative; } .harmonic { width: 490px; height: 200px; display: flex; } .diez2 { position: absolute; z-index: 1000; top: 0; right: 45px; display: flex; gap: 20px; } .diez1 { position: absolute; z-index: 1000; top: 0; left: 45px; display: flex; gap: 20px; } .btn { width: 70px; height: 200px; background-color: #fff; border: 1px solid #000; cursor: pointer; display: flex; flex-direction: column; justify-content: flex-end; align-items: center; padding-bottom: 15px; transition: 0.3s all; } .active { background-color: #ccc; } .btn_diez { width: 50px; height: 100px; background-color: #2f2f2f; color: #fff; } .btn_diez.active { background-color: #000; } .song { display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 30px; } .song_btn { width: 230px; height: 40px; border: none; border-radius: 5px; font-size: 25px; font-weight: 600; background-color: #2f2f2f; color: #fff; } .song_btn:active { background-color: #000; } </style></head><body><h1>Piano</h1><div class='piano'><div class='harmonic'><button class='btn' id='C'>C</button><button class='btn' id='D'>D</button><button class='btn' id='E'>E</button><button class='btn' id='F'>F</button><button class='btn' id='G'>G</button><button class='btn' id='A'>A</button><button class='btn' id='B'>B</button></div><div class='diez1'><button class='btn btn_diez' id='C1'>C#</button><button class='btn btn_diez' id='D1'>D#</button></div><div class='diez2'><button class='btn btn_diez' id='F1'>F#</button><button class='btn btn_diez' id='G1'>G#</button><button class='btn btn_diez' id='A1'>A#</button></div></div><div class='song'><button id='cant_stop' class='song_btn'>Can't stop</button><button id='thunderstruck' class='song_btn'>Thunderstruck</button><button id='happy_birthday' class='song_btn'>Happy Birthday</button><button id='undertale' class='song_btn'>Undertale</button></div><script> btns = document.querySelectorAll('.btn').forEach((a) => { a.addEventListener('mousedown', () => { fetch('/start_note?note=' + a.id); a.classList.add('active'); }); a.addEventListener('mouseup', () => { fetch('/stop_note'); a.classList.remove('active'); }); }); cant_stop = document.getElementById('cant_stop'); cant_stop.addEventListener('click', () => { fetch('/play_song?song=' + cant_stop.id); }); thunderstruck = document.getElementById('thunderstruck'); thunderstruck.addEventListener('click', () => { fetch('/play_song?song=' + thunderstruck.id); }); happy_birthday = document.getElementById('happy_birthday'); happy_birthday.addEventListener('click', () => { fetch('/play_song?song=' + happy_birthday.id); }); undertale = document.getElementById('undertale'); undertale.addEventListener('click', () => { fetch('/play_song?song=' + undertale.id); }); </script></body></html>";
  
  server.send(200, "text/html", html);
}

void Note(int index) {
  if (index == 1) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
  } else if (index == 2) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
  } else if (index == 3) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
  } else if (index == 4) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
  } else if (index == 5) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
  } else if (index == 6) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
  } else if (index == 7) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
  }
}

void handleStartNote() {
  String note = server.arg("note");
  float frequency = 0;

  // Основные ноты
  if (note == "C") {
    frequency = C;  // Нота C (До)
    Note(1);
  }
  else if (note == "D") {
    frequency = D;  // Нота D (Ре)
    Note(3);
  }
  else if (note == "E") {
    frequency = E;  // Нота E (Ми)
    Note(5);
  }
  else if (note == "F") {
    frequency = F;  // Нота F (Фа)
    Note(6);
  }
  else if (note == "G") {
    frequency = G;  // Нота G (Соль)
    Note(1);
  }
  else if (note == "A") {
    frequency = A;  // Нота A (Ля)
    Note(5);
  }
  else if (note == "B") {
    frequency = B;  // Нота B (Си)
    Note(7);
  }
  
  // Диезы
  else if (note == "C1") {
    frequency = C1;  // Нота C1 (До#)
    Note(2);
  }
  else if (note == "D1") {
    frequency = D1;  // Нота D1 (Ре#)
    Note(4);
  }
  else if (note == "F1") {
    frequency = F1;  // Нота F1 (Фа#)
    Note(7);
  }
  else if (note == "G1") {
    frequency = G1;  // Нота G1 (Соль#)
    Note(4);
  }
  else if (note == "A1") {
    frequency = A1;  // Нота A1 (Ля#)
    Note(6);
  }
  
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

  if (song == "happy_birthday") {
    playHappyBirthday();
  } else if (song == "thunderstruck") {
    playThunderstruck();
  } else if (song == "cant_stop") {
    playCantStop();
  } else if (song == "undertale") {
    playUndertale();
  }

  server.send(200, "text/plain", "Playing " + song);
}

void playUndertale() {
  int melody[] = {
    D, D, 587.32, A, G1, G, F, D, F, G,
    C, C, 587.32, A, G1, G, F, D, F, G,
    246.94, 246.94, 587.32, A, G1, G, F, D, F, G,
    233.08, 233.08, 587.32, A, G1, G, F, D, F, G,
  };
  int noteDurations[] = {
    8, 8, 4, 3, 4, 4, 4, 8, 8, 8,
    8, 8, 4, 3, 4, 4, 4, 8, 8, 8,
    8, 8, 4, 3, 4, 4, 4, 8, 8, 8,
    8, 8, 4, 3, 4, 4, 4, 8, 8, 8,
  }; // Пример длительностей

  for (int i = 0; i < 4; i++) {
    for (int thisNote = 0; thisNote < 40; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(piezoPin, melody[thisNote], noteDuration);
      delay(noteDuration * 1.3);
    } 
  }
}

// Пример функций для воспроизведения мелодий
void playHappyBirthday() {
  int melody[] = {
    C, C, D, C, F, E,
    C, C, D, C, G, F,
    C, C, 523.26, A, F, E, D,
    A1, A1, A, F, G, F,
  };
  int noteDurations[] = {
    3, 4, 2, 2, 2, 2,
    3, 4, 2, 2, 2, 2,
    3, 4, 2, 2, 2, 2, 1,
    3, 4, 2, 2, 2, 2,
  }; // Пример длительностей

  for (int thisNote = 0; thisNote < 25; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(piezoPin, melody[thisNote], noteDuration);
    delay(noteDuration * 1.3);
  } 
}

void playThunderstruck() {
  int melody[] = {
    D1, 246.94, F1, 246.94, D1, 246.94, F1, 246.94,
    D1, 246.94, F1, 246.94, D1, 246.94, F1, 246.94,
    D1, 246.94, F1, 246.94, D1, 246.94, F1, 246.94,
    D1, 246.94, F1, 246.94, D1, 246.94, F1, 246.94,
    E, 246.94, G, 246.94, E, 246.94, G, 246.94,
    E, 246.94, G, 246.94, E, 246.94, G, 246.94,
    E, 246.94, G, 246.94, E, 246.94, G, 246.94,
    E, 246.94, G, 246.94, E, 246.94, G, 246.94,
    D1, 246.94, F1, 246.94, D1, 246.94, F1, 246.94,
    D1, 246.94, F1, 246.94, D1, 246.94, F1, 246.94,
    D1, 246.94, F1, 246.94, D1, 246.94, F1, 246.94,
    D1, 246.94, F1, 246.94, D1, 246.94, F1, 246.94,
    E, 246.94, G, 246.94, E, 246.94, G, 246.94,
    E, 246.94, G, 246.94, E, 246.94, G, 246.94,
    E, 246.94, G, 246.94, E, 246.94, G, 246.94,
    E, 246.94, G, 246.94, E, 246.94, G, 246.94,
    B, 246.94, A, 246.94, G1, 246.94, A, 246.94,
    G1, 246.94, F1, 246.94, G1, 246.94, E, 246.94,
    F1, 246.94, D1, 246.94, E, 246.94, D1, 246.94,
    E, 246.94, D1, 246.94, E, 246.94, D1, 246.94,
    B, 246.94, A, 246.94, G1, 246.94, A, 246.94,
    G1, 246.94, F1, 246.94, G1, 246.94, E, 246.94,
    F1, 246.94, D1, 246.94, E, 246.94, D1, 246.94,
    E, 246.94, D1, 246.94, E, 246.94, D1, 246.94,
  };
  int noteDurations[128];
  for (int i = 0; i < 192; i++) {
    noteDurations[i] = 12;
  }

  for (int thisNote = 0; thisNote < 192; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(piezoPin, melody[thisNote], noteDuration);
    delay(noteDuration * 1.3);
  }
}

void playCantStop() {
  float melody[] = {E, E, 587.32, 659.26, E, E, E, 587.32, 659.26, E, E, E, D, 587.32, 659.26, D, D, D, 587.32, 659.26, D, D, D, 246.94, 587.32, 659.26, 246.94, 246.94, 246.94, 587.32, 659.26, 246.94, 246.94, 246.94, C, 587.32, 659.26, C, C, C, 587.32, 659.26, C, C};
  int noteDurations[] = {8, 8, 4, 4, 8, 8, 8, 4, 4, 8, 8, 8, 4, 4, 4, 8, 8, 8, 4, 4, 8, 8, 8, 4, 4, 4, 8, 8, 8, 4, 4, 8, 8, 8, 4, 4, 4, 8, 8, 8, 4, 4, 8, 4};

  for (int i = 0; i < 4; i++) {
    for (int thisNote = 0; thisNote < 44; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(piezoPin, melody[thisNote], noteDuration);
      delay(noteDuration * 1.3);
    } 
  }
}

