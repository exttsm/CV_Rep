#include <WiFi.h>
#include <esp_wifi.h>

const char* ssid = "routerPC";
const char* password = "antianti";

// This is the callback function that triggers every time a Wi-Fi packet with CSI data is received
void wifi_csi_rx_cb(void *ctx, wifi_csi_info_t *info) {
    if (!info || !info->buf) {
        return;
    }

    // Only print the raw CSI subcarrier data array for Python to parse
    for (int i = 0; i < info->len; i++) {
        Serial.print(info->buf[i]);
        
        // Add a comma after every number except the very last one
        if (i < info->len - 1) {
            Serial.print(",");
        }
    }
    // Send a single newline to mark the end of this packet's array
    Serial.println();
}

void setup() {
    // Start the serial communication so we can see the output on our computer
    Serial.begin(115200);
    Serial.println("Starting ESP32 CSI Collector...");
    
    // Set Wi-Fi to Station mode and connect to the router
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println(".");
    }
    Serial.println("\nConnected!");

    // Define the configuration for the CSI data we want to extract
    wifi_csi_config_t csi_config = {
        .lltf_en           = true,
        .htltf_en          = true,
        .stbc_htltf2_en    = true,
        .ltf_merge_en      = true,
        .channel_filter_en = false,
        .manu_scale        = false,
        .shift             = false
    };
    
    // Apply the configuration, register the callback function, and turn CSI on
    esp_err_t conf_res = esp_wifi_set_csi_config(&csi_config);
    esp_err_t cb_res = esp_wifi_set_csi_rx_cb(&wifi_csi_rx_cb, NULL);
    esp_err_t en_res = esp_wifi_set_csi(true);
    
    if (conf_res == ESP_OK && cb_res == ESP_OK && en_res == ESP_OK) {
        Serial.println("CSI extraction successfully activated! Waiting for packets...");
    } else {
        Serial.println("Failed to activate CSI.");
    }
}

void loop() {
    // The main loop is intentionally left empty. 
    // The Wi-Fi driver handles everything in the background and triggers the callback automatically.
    delay(10000);
}