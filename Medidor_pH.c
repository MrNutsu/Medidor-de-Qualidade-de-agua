#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/uart.h"

#define PH_SENSOR_PIN 26
#define TURBIDITY_SENSOR_PIN 27
#define PH_THRESHOLD 6.5
#define TURB_THRESHOLD 5.0
#define BAUD_RATE 115200
#define ESP_TX_PIN 0
#define ESP_RX_PIN 1
#define MAX_INPUT_LENGTH 100

float phValue = 7.0;
float turbidity = 0.0;
char ssid[MAX_INPUT_LENGTH];
char password[MAX_INPUT_LENGTH];
char server_address[MAX_INPUT_LENGTH];

void safe_input(char *buffer, int max_length, const char *prompt) {
    printf("%s", prompt);
    fgets(buffer, max_length, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline
}

void init_sensors() {
    adc_init();
    adc_gpio_init(PH_SENSOR_PIN);
    adc_gpio_init(TURBIDITY_SENSOR_PIN);
    adc_select_input(0);
}

float read_adc(uint adc_channel) {
    adc_select_input(adc_channel);
    uint16_t raw = adc_read();
    return (3.3 * raw) / 4095.0;
}

void calibrate_ph_sensor(float *offset, float *slope) {
    // Placeholder for pH sensor calibration
    // In a real implementation, you'd use known pH buffer solutions
    *offset = 0.5;
    *slope = 3.5;
}

void read_sensors() {
    float ph_offset = 0.5;
    float ph_slope = 3.5;
    
    calibrate_ph_sensor(&ph_offset, &ph_slope);
    
    float ph_voltage = read_adc(0);
    phValue = (ph_voltage * ph_slope) + ph_offset;
    
    float turb_voltage = read_adc(1);
    turbidity = turb_voltage * 10.0;
}

void check_water_quality() {
    printf("pH Value: %.2f\n", phValue);
    printf("Turbidity: %.2f NTU\n", turbidity);
    
    if (phValue < PH_THRESHOLD) {
        printf("ALERT: pH below threshold!\n");
    }
    if (turbidity > TURB_THRESHOLD) {
        printf("ALERT: Turbid water detected!\n");
    }
}

void send_data_to_server() {
    if (strlen(server_address) == 0) {
        printf("Error: Server address not set\n");
        return;
    }
    
    char command[200];
    snprintf(command, sizeof(command), 
             "GET /receber_dados?ph=%.2f&turb=%.2f HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n", 
             phValue, turbidity, server_address);
    
    uart_puts(uart1, command);
    // Add error checking for UART transmission
}

void init_uart() {
    uart_init(uart1, BAUD_RATE);
    gpio_set_function(ESP_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(ESP_RX_PIN, GPIO_FUNC_UART);
}

void blink_led_alert() {
    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    if (phValue < PH_THRESHOLD || turbidity > TURB_THRESHOLD) {
        for (int i = 0; i < 5; i++) {
            gpio_put(LED_PIN, 1);
            sleep_ms(500);
            gpio_put(LED_PIN, 0);
            sleep_ms(500);
        }
    }
}

void get_network_credentials() {
    safe_input(ssid, sizeof(ssid), "Enter Wi-Fi SSID: ");
    safe_input(password, sizeof(password), "Enter Wi-Fi password: ");
    safe_input(server_address, sizeof(server_address), "Enter server address: ");
}

int main() {
    stdio_init_all();
    get_network_credentials();
    init_sensors();
    init_uart();
    
    while (1) {
        read_sensors();
        check_water_quality();
        send_data_to_server();
        blink_led_alert();
        sleep_ms(5000);
    }
    
    return 0;
}
