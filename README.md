# Medidor de pH e Turbidez para Raspberry Pi Pico

## 📌 Sobre o Projeto
Este projeto tem como objetivo desenvolver um sistema de monitoramento da qualidade da água utilizando o **Raspberry Pi Pico** e sensores de **pH** e **turbidez**. O sistema coleta dados, verifica se estão dentro dos limites aceitáveis, e os envia para um servidor remoto, além de armazená-los em um cartão SD para análise posterior.

🚨 **Importante:** Este é o projeto final da **1ª Fase do Embarcatech**. A escolha do **Raspberry Pi Pico** foi feita pois a placa fornecida para o desenvolvimento é a **BitDogLab**, uma placa desenvolvida e criada pela **Unicamp**. Além disso, o projeto é **totalmente teórico** e **não foi testado em campo**, impossibilitando a certeza de seu funcionamento real.

## 🎯 Funcionalidades
- 📡 **Leitura de sensores:** Mede o nível de pH e turbidez da água utilizando conversão AD (Analógico-Digital).
- ⚠ **Alertas:** Emite avisos no terminal e pisca um LED se os valores estiverem fora dos limites definidos.
- 📤 **Envio de dados:** Transmite as leituras para um servidor via comunicação serial/UART.
- 💾 **Armazenamento local:** Salva as medições em um cartão SD para análise posterior.
- 📶 **Configuração de rede:** Permite inserir credenciais Wi-Fi e endereço do servidor diretamente pelo terminal.

## 🛠️ Tecnologias Utilizadas
- **Linguagem:** C
- **Plataforma:** Raspberry Pi Pico
- **Bibliotecas:**
  - `pico/stdlib.h` (Utilitários para Raspberry Pi Pico)
  - `hardware/adc.h` (Interface ADC para leitura de sensores)
  - `hardware/uart.h` (Comunicação UART)
  - `ff.h` (FatFs para manipulação de arquivos no SD Card)

## ⚙️ Como Funciona
1. Inicializa os sensores e a comunicação UART.
2. Solicita ao usuário as credenciais Wi-Fi e o endereço do servidor.
3. Realiza leituras periódicas dos sensores.
4. Verifica se os valores estão dentro dos limites aceitáveis:
   - Se o **pH** for menor que 6.5 → ⚠ **Alerta de acidez!**
   - Se a **turbidez** for maior que 5.0 NTU → ⚠ **Alerta de água turva!**
5. Envia os dados para um servidor remoto.
6. Armazena os dados em um cartão SD.
7. Caso os valores estejam fora dos limites, um LED de alerta pisca.
8. Aguarda 5 segundos e repete o processo.

## 🚀 Como Compilar e Executar
### 📥 Requisitos
Antes de compilar o projeto, certifique-se de que possui:
- **Raspberry Pi Pico SDK** instalado e configurado.
- **CMake e Ninja** instalados.
- **Toolchain para ARM Cortex-M0+** (como o GCC ARM Embedded).

### 🏗️ Passo a Passo para Compilar
1. **Clone o repositório:**
   ```sh
   git clone https://github.com/seu-usuario/medidor-ph-turbidez.git
   cd medidor-ph-turbidez
   ```

2. **Configure o SDK:**
   ```sh
   setx PICO_SDK_PATH "C:\Users\SeuUsuario\.pico-sdk"
   ```

3. **Crie a pasta de build e gere os arquivos:**
   ```sh
   mkdir build && cd build
   cmake -G "Ninja" ..
   ```

4. **Compile o código:**
   ```sh
   ninja
   ```

5. **Grave no Raspberry Pi Pico:**
   - Conecte o **Pico** ao PC segurando o botão **BOOTSEL**.
   - Arraste o arquivo `.uf2` gerado para a unidade **RPI-RP2**.

## 🔌 Hardware Utilizado
### 📟 Componentes e Custo Estimado (18/02/2025)
| Componentes | Quantidade | Valor Médio (R$) |
|------------|------------|-----------------|
| Raspberry Pi Pico | 1 unidade | 45,50 |
| Sensor DFRobot SEN0161 | 1 unidade | 170,00 |
| Sensor DFRobot SEN0189 | 1 unidade | 57,00 |
| Conversor ADC | 1 unidade | 20,00 |
| Módulo RTC DS3231 | 1 unidade | 24,50 |
| ESP01 | 1 unidade | 10,00 |
| Cartão MicroSD + Leitor | 1 unidade | 40,00 |
| Fonte de Alimentação 5V | 1 unidade | 30,00 |
| Módulo LM2596 | 1 unidade | 8,00 |
| Placa de Circuito (PBC) | 1 unidade | 4,50 |
| Caixa de Proteção | 1 unidade | 50,00 |
| **Total Estimado:** | | **459,50** |

### 📌 Pinagem Utilizada
A tabela abaixo apresenta detalhadamente a alocação de cada componente aos pinos GPIO do microcontrolador.

| Componentes | Pinos no Raspberry | Tipo de Conexão |
|------------|--------------------|----------------|
| Sensor DFRobot SEN161 | GP26 (ADC0) | Entrada Analógica |
| Sensor DFRobot SEN0189 | GP27 (ADC1) | Entrada Analógica |
| Conversor ADC | GP4 (SDA), GP5 (SCL) | I²C |
| Módulo RTC DS3231 | GP6 (SDA), GP7 (SCL) | I²C |
| Módulo Wi-Fi ESP01 | GP0 (TX), GP1 (RX) | UART |
| Leitor SD SPI | GP10 (MOSI), GP11 (MISO), GP12 (SCK), GP13 (CS) | SPI |
| Fonte de Alimentação | VBUS (5V)/VSYS (3.3V) | Alimentação |
| Módulo de Tensão | Entrada (5V)/Saída (3.3V) | Alimentação |
| GND (Terra) | GND | Comum |

## 📊 Exemplo de Saída no Terminal
```sh
pH Value: 6.2
Turbidity: 6.8 NTU
ALERTA: pH abaixo do limite!
ALERTA: Água turva detectada!
Enviando dados para o servidor...
Salvando dados no SD...
```

## 📌 Possíveis Melhorias
- 📡 **Integração com Wi-Fi** para envio automático dos dados.
- 📊 **Dashboard Web** para visualização em tempo real.
- 🔋 **Otimização de consumo de energia** para uso em campo.

## 🤝 Contribuições
Contribuições são bem-vindas! Se encontrar bugs ou quiser sugerir melhorias:
1. Faça um **fork** do repositório.
2. Crie uma **branch** para sua feature/bugfix.
3. Envie um **pull request**.


