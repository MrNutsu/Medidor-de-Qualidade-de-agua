# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Inicializar SDK do Raspberry Pi Pico
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)

if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Importar Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

project(Medidor_pH C CXX ASM)

# Inicializar SDK
pico_sdk_init()

# Adicionar executável
add_executable(Medidor_pH Medidor_pH.c tf_cad.c)

pico_set_program_name(Medidor_pH "Medidor_pH")
pico_set_program_version(Medidor_pH "0.1")

# Configuração da saída UART e USB
pico_enable_stdio_uart(Medidor_pH 0)
pico_enable_stdio_usb(Medidor_pH 1)  # Habilita saída USB para Debug

# Adicionar bibliotecas ao projeto
target_link_libraries(Medidor_pH
    pico_stdlib
    hardware_adc
    hardware_uart
    hardware_gpio
    hardware_spi
    fatfs          # Biblioteca FatFs para SD Card
)

# Incluir FatFs (Sistema de Arquivos para Cartão SD)
add_subdirectory(${PICO_SDK_PATH}/lib/fatfs)
target_include_directories(Medidor_pH PRIVATE ${PICO_SDK_PATH}/lib/fatfs)

# Adicionar os diretórios do projeto
target_include_directories(Medidor_pH PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Adicionar arquivos extras necessários
pico_add_extra_outputs(Medidor_pH)
