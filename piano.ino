#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

const char *defaultSSID = "ESP8266";
const char *defaultPassword = "11111111";

char ssid[32];
char password[32];

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
  loadSettings();

  WiFi.begin(ssid, password);
  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 10000; // 10 секунд

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("STA IP-адрес: ");
  Serial.println(WiFi.localIP());

  WiFi.mode(WIFI_AP_STA);

  WiFi.softAP(defaultSSID, defaultPassword);
  Serial.print("AP IP-адрес: ");
  Serial.println(WiFi.softAPIP());
  
  // Определение маршрутов
  server.on("/", handleRoot);
  server.on("/settings", HTTP_GET, handleSettings);
  server.on("/saveSettings", HTTP_POST, handleSaveSettings);
  server.on("/new_ip", HTTP_GET, handleNewIP);
  server.on("/getIP", HTTP_GET, getNewIP);
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
  String html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style> * { box-sizing: border-box; padding: 0; margin: 0; } body { padding: 50px; display: flex; flex-direction: column; align-items: center; justify-content: flex-start; gap: 30px; } .settings { display: flex; justify-content: center; align-items: center; gap: 30px; } .settings_btn { width: 100px; height: 40px; display: flex; justify-content: center; align-items: center; background-color: #2f2f2f; color: #fff; border-radius: 5px; font-size: 20px; font-weight: 600; text-decoration: none; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); transition: 0.3s all; } .settings_btn:hover { background-color: #000; } h1 { text-align: center; } .song { display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 30px; } .song_btn { width: 230px; height: 40px; border: none; border-radius: 5px; font-size: 25px; font-weight: 600; background-color: #2f2f2f; color: #fff; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); cursor: pointer; } .song_btn:active { background-color: #000; } </style></head><body><div class='settings'><a class='settings_btn' href='/settings'>Wi-Fi</a><a class='settings_btn' href='/new_ip'>New IP</a></div><h1>Piano</h1><div class='song'><button id='cant_stop' class='song_btn'>Can't stop</button><button id='thunderstruck' class='song_btn'>Thunderstruck</button><button id='happy_birthday' class='song_btn'>Happy Birthday</button><button id='undertale' class='song_btn'>Undertale</button><button id='crazy_frog' class='song_btn'>Crazy frog</button></div><script> btns = document.querySelectorAll('.btn').forEach((a) => { a.addEventListener('mousedown', () => { fetch('/start_note?note=' + a.id); a.classList.add('active'); }); a.addEventListener('mouseup', () => { fetch('/stop_note'); a.classList.remove('active'); }); }); cant_stop = document.getElementById('cant_stop'); cant_stop.addEventListener('click', () => { fetch('/play_song?song=' + cant_stop.id); }); thunderstruck = document.getElementById('thunderstruck'); thunderstruck.addEventListener('click', () => { fetch('/play_song?song=' + thunderstruck.id); }); happy_birthday = document.getElementById('happy_birthday'); happy_birthday.addEventListener('click', () => { fetch('/play_song?song=' + happy_birthday.id); }); undertale = document.getElementById('undertale'); undertale.addEventListener('click', () => { fetch('/play_song?song=' + undertale.id); }); crazy_frog = document.getElementById('crazy_frog'); crazy_frog.addEventListener('click', () => { fetch('/play_song?song=' + crazy_frog.id); }); </script></body></html>";
  
  server.send(200, "text/html", html);
}

void handleSettings() {
  String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style> * { box-sizing: border-box; padding: 0; margin: 0; } body { padding: 30px 100px; display: flex; flex-direction: column; align-items: center; justify-content: flex-start; gap: 40px; } h1 { text-align: center; } .form { width: 230px; height: 230px; padding: 15px; color: #fff; background-color: #000; border-radius: 5px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); display: flex; flex-direction: column; justify-content: space-between; align-items: center; } .wrapper { width: 100%; display: flex; flex-direction: column; justify-content: center; align-items: flex-start; gap: 5px; } label { font-size: 20px; font-weight: 600; } .input { width: 100%; height: 30px; border: none; border-radius: 5px; padding: 0 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .submit { height: 30px; background-color: #fff; font-size: 15px; font-weight: 600; color: #000; cursor: pointer; transition: 0.3s all; } .submit:hover { background-color: #000; color: #fff; } .back { width: 230px; height: 40px; display: flex; justify-content: center; align-items: center; background-color: #2f2f2f; color: #fff; border-radius: 5px; font-size: 20px; font-weight: 600; text-decoration: none; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); transition: 0.3s all; } .back:hover { background-color: #000; } </style></head><body><a href='/' class='back'>Go Back</a><h1>Wi-Fi Settings</h1><form action='/saveSettings' method='post' class='form'><div class='wrapper'><label for='ssid'>SSID:</label><input type='text' name='ssid' id='ssid' class='input'></div><div class='wrapper'><label for='ssid'>PASSWORD:</label><input type='password' name='password' id='password' class='input'></div><input type='submit' value='Submit' class='input submit'></form></body></html>";
  server.send(200, "text/html", output);
}

void handleSaveSettings() {
  if (server.hasArg("ssid") && server.hasArg("password")) {
    String newSSID = server.arg("ssid");
    String newPassword = server.arg("password");

    newSSID.toCharArray(ssid, 32);
    newPassword.toCharArray(password, 32);

    EEPROM.begin(512);
    for (int i = 0; i < 32; ++i) {
      EEPROM.write(i, ssid[i]);
      EEPROM.write(32 + i, password[i]);
    }
    EEPROM.commit();
    EEPROM.end();

    String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Settings Saved</title></head><body><h1>Settings Saved</h1><a href='/'>Go Back</a></body></html>";
    server.send(200, "text/html", output);

    ESP.restart();
  } else {
    server.send(400, "text/html", "Invalid Request");
  }
}

void loadSettings() {
  EEPROM.begin(512);
  for (int i = 0; i < 32; ++i) {
    ssid[i] = char(EEPROM.read(i));
    password[i] = char(EEPROM.read(32 + i));
  }
  EEPROM.end();

  if (strlen(ssid) == 0) {
    strcpy(ssid, defaultSSID);
  }
  if (strlen(password) == 0) {
    strcpy(password, defaultPassword);
  }
}

void handleNewIP() {
  String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style> * { box-sizing: border-box; padding: 0; margin: 0; } body { padding: 30px 100px; display: flex; flex-direction: column; align-items: center; justify-content: flex-start; gap: 40px; } h1 { text-align: center; } .back { width: 230px; height: 40px; display: flex; justify-content: center; align-items: center; background-color: #2f2f2f; color: #fff; border-radius: 5px; font-size: 20px; font-weight: 600; text-decoration: none; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); transition: 0.3s all; } .back:hover { background-color: #000; } </style></head><body><a href='/' class='back'>Go Back</a><h1>New IP Address</h1><a href='/' class='back' id='ip'>New IP: </a><script> function getIP() { fetch('/getIP').then(response => response.json()).then(data => { const ip = data.ip; const ip_link = document.getElementById('ip'); ip_link.innerText = 'New IP: ' + String(ip); ip_link.setAttribute('href', 'http://' + String(ip)); }); } function load() { getIP(); } window.onload = load(); </script></body></html>";
  server.send(200, "text/html", output);
}

void getNewIP() {
  String ip = WiFi.localIP().toString();
  String output = "{\"ip\":\"" + ip + "\"}";
  server.send(200, "application/json", output);
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

