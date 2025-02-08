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

int main()
{
  setup();

  // Inicializando a Matriz de Led (WS2812)
  PIO pio = pio0;
  int sm = 0;
  uint offset = pio_add_program(pio, &ws2812_program);
  ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);
 
  ssd1306_t ssd; // Inicializa a estrutura do display
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd); // Configura o display
  ssd1306_send_data(&ssd); // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  bool cor = true;
  while (true)
  {
    sleep_ms(1000);
  }
}

void setup() {
  stdio_init_all();

  gpio_init(LED_RED); // Inicializando LED vermelho.
  gpio_set_dir(LED_RED, GPIO_OUT);

  gpio_init(LED_BLUE);  // Inicializando LED azul.
  gpio_set_dir(LED_BLUE, GPIO_OUT);

  gpio_init(LED_GREEN); // Inicializando LED verde.
  gpio_set_dir(LED_GREEN, GPIO_OUT);

  gpio_init(A_BUTTON); // Inicializando Botão A.
  gpio_set_dir(A_BUTTON, GPIO_IN);
  gpio_pull_up(A_BUTTON);

  gpio_init(B_BUTTON);  // Inicializando Botão B.
  gpio_set_dir(B_BUTTON, GPIO_IN);
  gpio_pull_up(B_BUTTON);

  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line

}

static inline void put_pixel(uint32_t pixel_grb)
{
  pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void set_led(bool *frame, uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < WS2812_LED_COUNT; i++)
    {
      if (frame[i])
      {
        put_pixel(color); // Liga o LED com um no buffer
      }
      else
      {
        put_pixel(0); // Desliga os LEDs com zero no buffer
      }
    }
}

// Rotina da Interrupção
static void gpio_irq_handler(uint gpio, uint32_t events){};