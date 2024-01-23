class LedControl {
public:
  LedControl(int pin) : ledPin(pin) {
    pinMode(ledPin, OUTPUT);
    isOn = false;
  }

  void ligar() {
    digitalWrite(ledPin, HIGH);
    isOn = true;
  }

  void desligar() {
    digitalWrite(ledPin, LOW);
    isOn = false;
  }

  void espera(unsigned long tempo) {
    unsigned long inicio = millis();
    while (millis() - inicio < tempo) {
      
      }
    }


  bool estaLigado() {
    return isOn;
  }

private:
  int ledPin;
  bool isOn;
};
 
 #define PORTA 27
 
 void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
}

void loop() {
  LedControl meuLed(PORTA);
  Serial.println("Recebendo dados de localização.");
  meuLed.ligar();
  meuLed.espera(1000);
  meuLed.desligar();
  meuLed.espera(3000);
}
