#include <BleKeyboard.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- PIN Definitionen ---
const int PIN_BUTTON_LEFT = 18;  // Vorherige Folie / Menü nach links
const int PIN_BUTTON_RIGHT = 19; // Nächste Folie / Menü nach rechts
const int PIN_BUTTON_SELECT = 23; // Bestätigen / Aktion ausführen

// --- OLED Display Definitionen ---
#define SCREEN_WIDTH 128    // OLED Breite in Pixel
#define SCREEN_HEIGHT 64    // OLED Höhe in Pixel
#define OLED_RESET -1       // Reset-Pin für SSD1306, -1 wenn geteilt mit ESP32-Reset
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- BLE Keyboard Initialisierung ---
// Der Name, unter dem das Gerät auf dem iPad erscheint
BleKeyboard bleKeyboard("ESP32 Presenter", "Espressif", 100);

// --- Menü Variablen ---
const char* menuItems[] = {"N\x84chste Folie", "Vorherige Folie", "Zur ersten Folie"};
const int numMenuItems = 3;
int currentMenuIndex = 0; // Aktuell ausgewählter Menüpunkt
int lastButtonStateLeft = HIGH;
int lastButtonStateRight = HIGH;
int lastButtonStateSelect = HIGH;

// Setup Funktion
void setup() {
  Serial.begin(115200);

  // Taster Pins als Eingabe mit Pull-up Widerstand
  pinMode(PIN_BUTTON_LEFT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SELECT, INPUT_PULLUP);

  // OLED Display Initialisierung (I2C)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // I2C Adresse 0x3C für 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Endlosschleife bei Fehler
  }

  // Bluetooth starten
  bleKeyboard.begin();
  displayStatus("Verbinde...", "Bluetooth HID");
}

// Haupt-Loop
void loop() {
  // 1. Verbindung prüfen
  if (bleKeyboard.isConnected()) {
    // 2. Taster-Eingabe verarbeiten
    handleInput();
    // 3. Menü auf dem Display anzeigen
    displayMenu();
  } else {
    displayStatus("Warte auf", "Verbindung...");
  }
}

// --- Funktionen ---

// Funktion zur Anzeige des Verbindungsstatus
void displayStatus(const char* line1, const char* line2) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("--- PR\x84SENTER ---");

  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println(line1);
  display.setCursor(0, 40);
  display.println(line2);

  display.display();
  delay(100);
}

// Funktion zur Anzeige des aktuellen Menüs
void displayMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println(">> W\x84hle Aktion:");

  // Pfeile und Text für den aktuellen Menüpunkt anzeigen
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print(">> ");
  display.println(menuItems[currentMenuIndex]);

  display.setTextSize(1);
  display.setCursor(0, 45);
  display.print("L/R: Wechsel | SEL: Best\x84tigen");

  display.display();
}

// Funktion zur Verarbeitung der Taster-Eingaben (mit einfacher Entprellung)
void handleInput() {
  // 1. Taster LINKS (Vorherige Folie / Menü nach links)
  int buttonStateLeft = digitalRead(PIN_BUTTON_LEFT);
  if (buttonStateLeft != lastButtonStateLeft && buttonStateLeft == LOW) {
    currentMenuIndex--; // Menüpunkt dekrementieren
    if (currentMenuIndex < 0) {
      currentMenuIndex = numMenuItems - 1; // Zirkulär zurück zum Ende
    }
  }
  lastButtonStateLeft = buttonStateLeft;

  // 2. Taster RECHTS (Nächste Folie / Menü nach rechts)
  int buttonStateRight = digitalRead(PIN_BUTTON_RIGHT);
  if (buttonStateRight != lastButtonStateRight && buttonStateRight == LOW) {
    currentMenuIndex++; // Menüpunkt inkrementieren
    if (currentMenuIndex >= numMenuItems) {
      currentMenuIndex = 0; // Zirkulär zurück zum Anfang
    }
  }
  lastButtonStateRight = buttonStateRight;

  // 3. Taster SELECT (Bestätigen)
  int buttonStateSelect = digitalRead(PIN_BUTTON_SELECT);
  if (buttonStateSelect != lastButtonStateSelect && buttonStateSelect == LOW) {
    // Aktion basierend auf dem ausgewählten Menüpunkt ausführen
    performAction(currentMenuIndex);
    delay(200); // Kurze Pause, um Mehrfachauslösung zu vermeiden
  }
  lastButtonStateSelect = buttonStateSelect;

  delay(50); // Kleiner Delay für Entprellung und CPU-Schonung
}

// Funktion zum Senden des Bluetooth-Befehls
void performAction(int index) {
  displayStatus("Aktion:", menuItems[index]);
  delay(1000); // Zeigt die Aktion 1 Sekunde lang an

  switch (index) {
    case 0: // Nächste Folie
      Serial.println("Sende: Pfeil RECHTS");
      bleKeyboard.write(KEY_RIGHT_ARROW);
      break;
    case 1: // Vorherige Folie
      Serial.println("Sende: Pfeil LINKS");
      bleKeyboard.write(KEY_LEFT_ARROW);
      break;
    case 2: // Zur ersten Folie (HOME Taste)
      Serial.println("Sende: HOME");
      bleKeyboard.write(KEY_HOME);
      break;
  }
}