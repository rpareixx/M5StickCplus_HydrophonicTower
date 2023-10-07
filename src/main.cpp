// To Do's
// - time-server approach
// WiFi reconnect configurieren

#include <Arduino.h>
#include <GitHubFsOTA.h>
#include <GitHubOTA.h>
#include <M5StickCPlus.h>
#include <WiFi.h>
#include <secrets.h>

#include "LittleFS.h"

// This string should correspond to github tag used for Releasing (via. Github
// Actions)
#define VERSION "0.0.2"

#define RELEASE_URL                                                       \
    "https://github.com/rpareixx/M5StickCplus_HydrophonicTower/releases/" \
    "latest"

#define DELAY_MS 500

char ssid[] = SECRET_CASTEL_SSID;
char password[] = SECRET_CASTEL_PASS;

#define CONNECTION_TIMEOUT 10

#define LED_BUILTIN 10

GitHubOTA OsOta(VERSION, RELEASE_URL, "firmware.bin", true);
GitHubFsOTA FsOta(VERSION, RELEASE_URL, "filesystem.bin", true);

IPAddress local_IP(192, 168, 1, 48);
IPAddress primaryDNS(8, 8, 8, 8);    // optional
IPAddress secondaryDNS(8, 8, 4, 4);  // optional
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 254, 1);

void setup_wifi();
void listRoot();
void get_network_info();

void setup() {
    M5.begin();
    M5.Lcd.setRotation(3);

    pinMode(LED_BUILTIN, OUTPUT);

    Serial.printf("Current firmware version: %s\n", VERSION);
    Serial.printf("Current filesystem version: %s\n", VERSION);

    LittleFS.begin();
    listRoot();

    setup_wifi();

    // Chech for updates
    FsOta.handle();
    OsOta.handle();
}

void loop() {
    // Wenn Sie die Button-Klasse verwenden, rufen Sie immer Update auf, um den
    // Status zu aktualisieren M5.update();

    // Your code goes here
    digitalWrite(LED_BUILTIN,
                 HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(DELAY_MS);     // wait for a second
    digitalWrite(LED_BUILTIN,
                 LOW);  // turn the LED off by making the voltage LOW
    delay(DELAY_MS);    // wait for a second
}

void listRoot() {
    Serial.printf("Listing root directory\r\n");

    File root = LittleFS.open("/", "r");
    File file = root.openNextFile();

    while (file) {
        Serial.printf("  FILE: %s\r\n", file.name());
        file = root.openNextFile();
    }
}

void setup_wifi() {
    // Configures static IP address
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("STA Failed to configure");
    }

    WiFi.mode(WIFI_STA);  // Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    int timeout_counter = 0;

    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.print(".");
        timeout_counter++;
        if (timeout_counter >= CONNECTION_TIMEOUT * 5) {
            ESP.restart();
        }
    }

    // Display Wifi Status in terminal
    get_network_info();

    // Display Wifi Status on M5Stick Display
    M5.Lcd.println("\nConnected to the WiFi network");
    M5.Lcd.print("Local ESP32 IP: ");
    M5.Lcd.println(WiFi.localIP());
}

void get_network_info() {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("[*] Network information for ");
        Serial.println(ssid);

        Serial.println("[+] BSSID : " + WiFi.BSSIDstr());
        Serial.print("[+] Gateway IP : ");
        Serial.println(WiFi.gatewayIP());
        Serial.print("[+] Subnet Mask : ");
        Serial.println(WiFi.subnetMask());
        Serial.println((String) "[+] RSSI : " + WiFi.RSSI() + " dB");
        Serial.print("[+] ESP32 IP : ");
        Serial.println(WiFi.localIP());
    }
}