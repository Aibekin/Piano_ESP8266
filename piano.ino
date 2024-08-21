#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//const char* ssid = "ECI";
//const char* password = "freefromauthority";

const char* ssid = "Redmi Note 12";
const char* password = "12345678";

ESP8266WebServer server(80);

const int piezoPin = D4;
const int BTN1 = D0;
const int BTN2 = D1;
const int BTN3 = D2;
const int BTN4 = D3;
const int BTN5 = D5;
const int BTN6 = D6;
const int BTN7 = D7;
const float C = 261.63;
const float CS = 277.18;
const float D = 293.66;
const float DS = 311.13;
const float E = 329.63;
const float F = 349.23;
const float FS = 369.99;
const float G = 392.00;
const float GS = 415.30;
const float A = 440.00;
const float AS = 466.16;
const float B = 493.88;
const float B_s = 246.94;

void setup() {
  pinMode(piezoPin, OUTPUT);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);
  pinMode(BTN5, INPUT_PULLUP);
  pinMode(BTN6, INPUT_PULLUP);
  pinMode(BTN7, INPUT_PULLUP);
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
  server.on("/play_song", handlePlaySong);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  if (digitalRead(BTN1) == LOW) {
    handleStartNote("C");
  } else if (digitalRead(BTN2) == LOW) {
    handleStartNote("D");
  } else if (digitalRead(BTN3) == LOW) {
    handleStartNote("E");
  } else if (digitalRead(BTN4) == LOW) {
    handleStartNote("F");
  } else if (digitalRead(BTN5) == LOW) {
    handleStartNote("G");
  } else if (digitalRead(BTN6) == LOW) {
    handleStartNote("A");
  } else if (digitalRead(BTN7) == LOW) {
    handleStartNote("B");
  } else {
    noTone(piezoPin);
  }
  server.handleClient();
}

void handleRoot() {
  // Создание кнопок для нот
  String html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style> * { box-sizing: border-box; padding: 0; margin: 0; } body { padding: 50px; display: flex; flex-direction: column; align-items: center; justify-content: flex-start; gap: 30px; } h1 { text-align: center; } .song { display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 30px; } .song_btn { width: 230px; height: 40px; border: none; border-radius: 5px; font-size: 25px; font-weight: 600; background-color: #2f2f2f; color: #fff; cursor: pointer; } .song_btn:active { background-color: #000; } </style></head><body><h1>Play songs</h1><div class='song'><button id='cant_stop' class='song_btn'>Can't stop</button><button id='thunderstruck' class='song_btn'>Thunderstruck</button><button id='happy_birthday' class='song_btn'>Happy Birthday</button><button id='undertale' class='song_btn'>Undertale</button><button id='crazy_frog' class='song_btn'>Crazy frog</button></div><script> cant_stop = document.getElementById('cant_stop'); cant_stop.addEventListener('click', () => { fetch('/play_song?song=' + cant_stop.id); }); thunderstruck = document.getElementById('thunderstruck'); thunderstruck.addEventListener('click', () => { fetch('/play_song?song=' + thunderstruck.id); }); happy_birthday = document.getElementById('happy_birthday'); happy_birthday.addEventListener('click', () => { fetch('/play_song?song=' + happy_birthday.id); }); undertale = document.getElementById('undertale'); undertale.addEventListener('click', () => { fetch('/play_song?song=' + undertale.id); }); crazy_frog = document.getElementById('crazy_frog'); crazy_frog.addEventListener('click', () => { fetch('/play_song?song=' + crazy_frog.id); }); </script></body></html>";
  
  server.send(200, "text/html", html);
}

void handleStartNote(String note) {
  float frequency = 0;

  if (note == "C") {
    frequency = C;  // Нота C (До)
  }
  else if (note == "D") {
    frequency = D;  // Нота D (Ре)
  }
  else if (note == "E") {
    frequency = E;  // Нота E (Ми)
  }
  else if (note == "F") {
    frequency = F;  // Нота F (Фа)
  }
  else if (note == "G") {
    frequency = G;  // Нота G (Соль)
  }
  else if (note == "A") {
    frequency = A;  // Нота A (Ля)
  }
  else if (note == "B") {
    frequency = B;  // Нота B (Си)
  }
  
  // Диезы
  else if (note == "CS") {
    frequency = CS;  // Нота CS (До#)
  }
  else if (note == "DS") {
    frequency = DS;  // Нота DS (Ре#)
  }
  else if (note == "FS") {
    frequency = FS;  // Нота FS (Фа#)
  }
  else if (note == "GS") {
    frequency = GS;  // Нота GS (Соль#)
  }
  else if (note == "AS") {
    frequency = AS;  // Нота AS (Ля#)
  }
  
  if (frequency > 0) {
    tone(piezoPin, frequency);  // Играть ноту до тех пор, пока не будет вызвано stopNote
  }
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
  } else if (song == "crazy_frog") {
    playCrazyFrog();
  }

  server.send(200, "text/plain", "Playing " + song);
}

void playCrazyFrog() {
  float melody[] = {
    F, GS, F, F, AS, F, DS,
    F, 523.26, F, F, 554.36, 523.26, AS,
    F, 523.26, 698.46, F, DS, DS, C, G, F,
    698.46, 698.46
  };
  int noteDurations[] = {
    3, 4, 6, 10, 6, 5, 5,
    3, 4, 6, 10, 6, 5, 5,
    5, 6, 6, 8, 6, 10, 6, 5, 2,
    2, 2
  };
  for (int i = 0; i < 3; i++) {
    for (int thisNote = 0; thisNote < 25; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(piezoPin, melody[thisNote], noteDuration);
      delay(noteDuration * 1.3);
    } 
  }
}

void playUndertale() {
  float melody[] = {
    D, D, 587.32, A, GS, G, F, D, F, G,
    C, C, 587.32, A, GS, G, F, D, F, G,
    B_s, B_s, 587.32, A, GS, G, F, D, F, G,
    233.08, 233.08, 587.32, A, GS, G, F, D, F, G,
  };
  int noteDurations[] = {
    8, 8, 4, 3, 4, 4, 4, 8, 8, 8,
    8, 8, 4, 3, 4, 4, 4, 8, 8, 8,
    8, 8, 4, 3, 4, 4, 4, 8, 8, 8,
    8, 8, 4, 3, 4, 4, 4, 8, 8, 8,
  }; // Пример длительностей

  for (int i = 0; i < 3; i++) {
    for (int thisNote = 0; thisNote < 40; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(piezoPin, melody[thisNote], noteDuration);
      delay(noteDuration * 1.3);
    } 
  }
}

// Пример функций для воспроизведения мелодий
void playHappyBirthday() {
  float melody[] = {
    C, C, D, C, F, E,
    C, C, D, C, G, F,
    C, C, 523.26, A, F, E, D,
    AS, AS, A, F, G, F,
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
  float melody[] = {
    B_s,
    DS, B_s, FS, B_s, DS, B_s, FS, B_s,
    DS, B_s, FS, B_s, DS, B_s, FS, B_s,
    DS, B_s, FS, B_s, DS, B_s, FS, B_s,
    DS, B_s, FS, B_s, DS, B_s, FS, B_s,
    E, B_s, G, B_s, E, B_s, G, B_s,
    E, B_s, G, B_s, E, B_s, G, B_s,
    E, B_s, G, B_s, E, B_s, G, B_s,
    E, B_s, G, B_s, E, B_s, G, B_s,
    DS, B_s, FS, B_s, DS, B_s, FS, B_s,
    DS, B_s, FS, B_s, DS, B_s, FS, B_s,
    DS, B_s, FS, B_s, DS, B_s, FS, B_s,
    DS, B_s, FS, B_s, DS, B_s, FS, B_s,
    E, B_s, G, B_s, E, B_s, G, B_s,
    E, B_s, G, B_s, E, B_s, G, B_s,
    E, B_s, G, B_s, E, B_s, G, B_s,
    E, B_s, G, B_s, E, B_s, G, B_s,
    B, B_s, A, B_s, GS, B_s, A, B_s,
    GS, B_s, FS, B_s, GS, B_s, E, B_s,
    FS, B_s, DS, B_s, E, B_s, DS, B_s,
    E, B_s, DS, B_s, E, B_s, DS, B_s,
    B, B_s, A, B_s, GS, B_s, A, B_s,
    GS, B_s, FS, B_s, GS, B_s, E, B_s,
    FS, B_s, DS, B_s, E, B_s, DS, B_s,
    E, B_s, DS, B_s, E, B_s, DS, B_s,
  };
  int noteDurations[128];
  for (int i = 0; i < 193; i++) {
    noteDurations[i] = 12;
  }

  for (int thisNote = 0; thisNote < 193; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(piezoPin, melody[thisNote], noteDuration);
    delay(noteDuration * 1.3);
  }
}

void playCantStop() {
  float melody[] = {E, E, 587.32, 659.26, E, E, E, 587.32, 659.26, E, E, E, D, 587.32, 659.26, D, D, D, 587.32, 659.26, D, D, D, B_s, 587.32, 659.26, B_s, B_s, B_s, 587.32, 659.26, B_s, B_s, B_s, C, 587.32, 659.26, C, C, C, 587.32, 659.26, C, C};
  int noteDurations[] = {8, 8, 4, 4, 8, 8, 8, 4, 4, 8, 8, 8, 4, 4, 4, 8, 8, 8, 4, 4, 8, 8, 8, 4, 4, 4, 8, 8, 8, 4, 4, 8, 8, 8, 4, 4, 4, 8, 8, 8, 4, 4, 8, 4};

  for (int i = 0; i < 3; i++) {
    for (int thisNote = 0; thisNote < 44; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(piezoPin, melody[thisNote], noteDuration);
      delay(noteDuration * 1.3);
    } 
  }
}

