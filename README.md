# ESP32 Bluetooth Präsentations-Controller

Ein kabelloser Presenter, gebaut mit einem ESP32, OLED-Display und Tasten, der über Bluetooth Low Energy (BLE) Tastatur-Befehle (Pfeil Links/Rechts, Home) an ein iPad oder einen PC sendet.

## ⚙️ Komponenten (Hardware)

* **ESP32 Dev Module**
* **0.96 Zoll OLED Display** (SSD1306, I2C)
* 3x Taster (Links, Rechts, Bestätigen)

## 🔌 Verdrahtung (Schaltplan)

Verdrahten Sie die Komponenten wie folgt. Es werden interne Pull-up-Widerstände verwendet, d.h. die Taster ziehen den Pin auf GND.

| Komponente | Pin (ESP32) |
| :--- | :--- |
| OLED SCL | **GPIO 22** |
| OLED SDA | **GPIO 21** |
| Taster Links | **GPIO 18** |
| Taster Rechts | **GPIO 19** |
| Taster Bestätigen | **GPIO 23** |

## 💻 Software & Installation

1.  **Arduino IDE** installieren.
2.  Das **ESP32 Board-Paket** installieren und auf Version 3.0.0 oder höher aktualisieren.
3.  Installieren Sie folgende Bibliotheken über den Bibliotheksverwalter:
    * `Adafruit GFX Library`
    * `Adafruit SSD1306`
    * `ESP32-BLE-Keyboard` (von T-vK)
4.  Laden Sie die Datei `ESP32_Presentation_Remote.ino` auf Ihren ESP32 hoch.

## 🗺️ Menü-Funktionen

Der Controller bietet folgende Aktionen, die über die Taster Links/Rechts ausgewählt und mit Bestätigen ausgelöst werden:

| Anzeige | Bluetooth-Befehl | Funktion |
| :--- | :--- | :--- |
| Nächste Folie | Pfeil Rechts (`KEY_RIGHT_ARROW`) | Springt zur nächsten Folie. |
| Vorherige Folie | Pfeil Links (`KEY_LEFT_ARROW`) | Springt zur vorherigen Folie. |
| Zur ersten Folie | Home (`KEY_HOME`) | Geht zurück zur Startfolie der Präsentation. |
