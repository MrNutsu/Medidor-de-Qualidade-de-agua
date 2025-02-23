#include <stdio.h>
#include "pico/stdlib.h" // 
#include "hardware/adc.h" //biblioteca para leitura dos sensores analógicos
#include "hardware/uart.h" //biblioteca para comunicação serial
#include "ff.h"  // Biblioteca FatFs para manipulação do SD Card

//definições de constantes - pinos dos sensores de pH e turbidez, limiares de alerta, taxa de transmissão serial, pinos de comunicação UART com o módulo ESP01
#define PH_SENSOR_PIN 26
#define TURBIDITY_SENSOR_PIN 27
#define PH_THRESHOLD 6.5
#define TURB_THRESHOLD 5.0
#define BAUD_RATE 115200
#define ESP_TX_PIN 0
#define ESP_RX_PIN 1

//variáveis globais para armazenar os valores dos sensores de pH e turbidez, credenciais da rede Wi-Fi e endereço do servidor
float phValue = 7.0;
float turbidity = 0.0;
char ssid[50];
char password[50];
char server_address[100];

// função para inicializar os sensores de pH e turbidez
void init_sensors() {
    adc_init();
    adc_gpio_init(PH_SENSOR_PIN);
    adc_gpio_init(TURBIDITY_SENSOR_PIN);
    adc_select_input(0);
}

//função para ler os valores dos sensores de pH e turbidez
float read_adc(uint adc_channel) {
    adc_select_input(adc_channel);
    uint16_t raw = adc_read();
    return (3.3 * raw) / 4095.0;
}

//conversão dos valores para digital
void read_sensors() {
    phValue = (read_adc(0) * 3.5) + 0.5;
    turbidity = (read_adc(1) * 10.0);
}

//função para verificar a qualidade da água - pH e turbidez - e exibir alertas
void check_water_quality() {
    printf("pH Value: %.2f\n", phValue);
    printf("Turbidity: %.2f NTU\n", turbidity);
    
    if (phValue < PH_THRESHOLD) {
        printf("ALERTA: pH abaixo do limite!\n");
    }
    if (turbidity > TURB_THRESHOLD) {
        printf("ALERTA: Água turva detectada!\n");
    }
}

//função para enviar os dados ao servidor via módulo ESP01
void send_data_to_server() {
    char command[200];
    sprintf(command, "GET /receber_dados?ph=%.2f&turb=%.2f HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n", phValue, turbidity, server_address);
    uart_puts(uart1, command);
}

//função para inicializar a comunicação UART com o módulo ESP01
void init_uart() {
    uart_init(uart1, BAUD_RATE);
    gpio_set_function(ESP_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(ESP_RX_PIN, GPIO_FUNC_UART);
}

//função para piscar um LED em caso de alerta - pH abaixo do limite ou água turva
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

//função para obter as credenciais da rede Wi-Fi e o endereço do servidor - envio de dados
void get_network_credentials() {
    printf("Digite o SSID da rede Wi-Fi: ");
    scanf("%49s", ssid);
    printf("Digite a senha da rede Wi-Fi: ");
    scanf("%49s", password);
    printf("Digite o endereço do servidor: ");
    scanf("%99s", server_address);
}

int main() {
    stdio_init_all();
    get_network_credentials();
    init_sensors();
    init_uart();
    
    //loop infinito - leitura dos sensores, verificação da qualidade da água, envio dos dados ao servidor e alerta visual
    while (1) {
        read_sensors();
        check_water_quality();
        send_data_to_server();
        blink_led_alert();
        sleep_ms(5000); //delay para evitar envio de dados em excesso
    }
}
