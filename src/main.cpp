// To Do's
// - time-server approach
// WiFi reconnect configurieren

#include <Arduino.h>
#include <M5StickCPlus.h>
#include <WiFi.h>
#include <secrets.h>

char ssid[] = SECRET_CASTEL_SSID;
char password[] = SECRET_CASTEL_PASS;

#define CONNECTION_TIMEOUT 10

IPAddress local_IP(192, 168, 1, 48);
IPAddress primaryDNS(8, 8, 8, 8);    // optional
IPAddress secondaryDNS(8, 8, 4, 4);  // optional
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 254, 1);

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

void setup() {
    M5.begin();
    M5.Lcd.setRotation(3);

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

void loop() {
    // Wenn Sie die Button-Klasse verwenden, rufen Sie immer Update auf, um den
    // Status zu aktualisieren M5.update();
}