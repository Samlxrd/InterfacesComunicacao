#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/numeros.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "ws2812.pio.h"

// Definição de Constantes
#define LED_RED 13
#define LED_BLUE 12
#define LED_GREEN 11
#define WS2812_PIN 7
#define A_BUTTON 5
#define B_BUTTON 6
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

void setup();
static void gpio_irq_handler(uint gpio, uint32_t events);
static inline void put_pixel(uint32_t pixel_grb);
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
void set_led(bool *frame, uint8_t r, uint8_t g, uint8_t b);

// Variável global para armazenar a cor para matrix de led 5x5 (Entre 0 e 255 para intensidade)
uint8_t red_value = 10;  // Intensidade do vermelho
uint8_t green_value = 0; // Intensidade do verde
uint8_t blue_value = 0;  // Intensidade do azul

uint32_t last_time; // Variável auxiliar para controle do debounce
ssd1306_t ssd;      // Inicializa a estrutura do display
bool cor = true;

int main()
{
  setup();

  // Inicializando a Matriz de Led (WS2812)
  PIO pio = pio0;
  int sm = 0;
  uint offset = pio_add_program(pio, &ws2812_program);
  ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);

  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd);                                         // Configura o display
  ssd1306_send_data(&ssd);                                      // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  // Habilitando interrupção da gpio nos botões A e B.
  gpio_set_irq_enabled_with_callback(A_BUTTON, GPIO_IRQ_EDGE_FALL, 1, &gpio_irq_handler);
  gpio_set_irq_enabled(B_BUTTON, GPIO_IRQ_EDGE_FALL, true);

  while (true)
  {
    if (stdio_usb_connected())
    { // Certifica-se de que o USB está conectado
      char c;
      int num;
      if (scanf(" %c", &c) == 1)
      { // Lê caractere da entrada padrão
        printf("Recebido: '%c'", c);

        // Intervalo para 0-9 em ascii
        if (c > 47 && c < 58) {
          num = c - 48; // Convertendo ASCII para índice [0-9]
          set_led(numeros[num], red_value, green_value, blue_value);
        }

        ssd1306_fill(&ssd, !cor); // Limpa o display
        ssd1306_draw_char(&ssd, c, 8, 10);
        ssd1306_send_data(&ssd);
      }
    }
    sleep_ms(40);
  }
}

void setup()
{
  stdio_init_all();

  gpio_init(LED_RED); // Inicializando LED vermelho.
  gpio_set_dir(LED_RED, GPIO_OUT);

  gpio_init(LED_BLUE); // Inicializando LED azul.
  gpio_set_dir(LED_BLUE, GPIO_OUT);

  gpio_init(LED_GREEN); // Inicializando LED verde.
  gpio_set_dir(LED_GREEN, GPIO_OUT);

  gpio_init(A_BUTTON); // Inicializando Botão A.
  gpio_set_dir(A_BUTTON, GPIO_IN);
  gpio_pull_up(A_BUTTON);

  gpio_init(B_BUTTON); // Inicializando Botão B.
  gpio_set_dir(B_BUTTON, GPIO_IN);
  gpio_pull_up(B_BUTTON);

  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA);                     // Pull up the data line
  gpio_pull_up(I2C_SCL);                     // Pull up the clock line
}

static inline void put_pixel(uint32_t pixel_grb) {
  pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void set_led(bool *frame, uint8_t r, uint8_t g, uint8_t b) {
  // Define a cor com base nos parâmetros fornecidos
  uint32_t color = urgb_u32(r, g, b);

  // Define todos os LEDs com a cor especificada
  for (int i = 0; i < WS2812_LED_COUNT; i++)
  {
    if (frame[i]) {
      put_pixel(color); // Liga o LED com um no buffer
    } else {
      put_pixel(0); // Desliga os LEDs com zero no buffer
    }
  }
}

// Rotina da Interrupção
static void gpio_irq_handler(uint gpio, uint32_t events)
{
  uint32_t current_time = to_us_since_boot(get_absolute_time());
  if (current_time - last_time > 200000) // Delay de 200ms para debounce
  {
    char *estado;
    last_time = current_time;

    switch (gpio)
    {
    case A_BUTTON:
      gpio_put(LED_GREEN, !gpio_get(LED_GREEN)); // Alterna estado do led verde
      estado = (gpio_get(LED_GREEN) ? "Ligado" : "Desligado");

      // Exibindo mensagem informativa sobre o estado do LED no Display SSD1306 e no Serial Monitor
      printf("Estado do LED Verde alterado para: %s.\n", estado);
      ssd1306_draw_string(&ssd, "LED Verde ", 8, 10);
      ssd1306_draw_string(&ssd, estado, 8, 30);
      ssd1306_send_data(&ssd);
      break;
    case B_BUTTON:
      gpio_put(LED_BLUE, !gpio_get(LED_BLUE)); // Alterna estado do led azul
      estado = (gpio_get(LED_BLUE) ? "Ligado" : "Desligado");

      // Exibindo mensagem informativa sobre o estado do LED no Display SSD1306 e no Serial Monitor
      printf("Estado do LED Azul alterado para: %s.\n", estado);
      ssd1306_draw_string(&ssd, "LED Azul ", 8, 10);
      ssd1306_draw_string(&ssd, estado, 8, 30);
      ssd1306_send_data(&ssd);
      break;

    default:
      break;
    }
  }
}