## Instruções de Uso do Programa
1. Clone o repositório
    ```bash
    git clone https://github.com/Samlxrd/InterfacesComunicacao.git

2. Navegue até o diretório do projeto:
    ```bash
    cd InterfacesComunicacao

3. Execução do Código
    - Compilar e executar o projeto utilizando o Pico SDK no VS Code. Utilizar o Serial Monitor no VSCode para enviar os caractéres para a placa.
  
## Funcionalidades do Projeto 
1. Modificação da Biblioteca font.h 
  - Adicionar caracteres minúsculos à biblioteca font.h. Use sua criatividade para criar os novos caracteres.

2. Entrada de caracteres via PC
  - Utilize o Serial Monitor do VS Code para digitar os caracteres. 
  - Cada caractere digitado no Serial Monitor deve ser exibido no display SSD1306.

Observação: Apenas um caractere será enviado de cada vez, não é necessário suportar o envio de strings completas. 
  • Quando um número entre 0 e 9 for digitado, um símbolo correspondente ao número deve ser exibido, também, na matriz 5x5 WS2812.

3. Interação com o Botão A  
  - Pressionar o botão A deve alternar o estado do LED RGB Verde (ligado/desligado). 
  - A operação deve ser registrada de duas formas:
    - Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306 
    - Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor. 

4. Interação com o Botão B 
  - Pressionar o botão A deve alternar o estado do LED RGB Azul (ligado/desligado). 
  - A operação deve ser registrada de duas formas: 
    - Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306 
    - Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor. 

## Requisitos do Projeto 
Para o desenvolvimento, devem ser seguidos os seguintes requisitos: 
1. Uso de interrupções: Todas as funcionalidades relacionadas aos botões devem ser implementadas 
utilizando rotinas de interrupção (IRQ).
2. Debouncing: É obrigatório implementar o tratamento do bouncing dos botões via software.
3. Controle de LEDs: O projeto deve incluir o uso de LEDs comuns e LEDs WS2812, demonstrando o domínio de diferentes tipos de controle. 
4. Utilização do Display 128 x 64: A utilização de fontes maiúsculas e minúsculas demonstrará o domínio do uso de bibliotecas, o entendimento do princípio de funcionamento do display, bem como, a utilização do protocolo I2C. 
5. Envio de informação pela UART: Visa observar a compreensão sobre a comunicação serial via UART. 
6. Organização do código: O código deve estar bem estruturado e comentado para facilitar o 
entendimento. 
