// Classe Button com lógica de clique
class Button {
  public:
    // Construtor
    Button(int pin) {
      buttonPin = pin;
      pinMode(buttonPin, INPUT_PULLUP);
      lastState = HIGH;
      currentState = HIGH;
      lastDebounceTime = 0;
      debounceDelay = 50; // Tempo de debounce em milissegundos
      clicked = false;
    }

    // Função para atualizar o estado do botão
    void update() {
      int reading = digitalRead(buttonPin);

      if (reading != lastReading) {
        lastDebounceTime = millis();
      }

      if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != currentState) {
          currentState = reading;

          if (currentState == LOW) {
            clicked = true;
          }
        }
      }

      lastReading = reading;
    }

    // Verifica se o botão foi clicado
    bool wasClicked() {
      if (clicked) {
        clicked = false;
        return true;
      }
      return false;
    }

  private:
    int buttonPin;
    int currentState;
    int lastState;
    int lastReading;
    unsigned long lastDebounceTime;
    unsigned long debounceDelay;
    bool clicked;
    int teste;
};