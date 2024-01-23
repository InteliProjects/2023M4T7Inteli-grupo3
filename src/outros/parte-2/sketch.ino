#define botaoPin 25 // pino de entrada para o botão
#define ledV 26 // pino em que se encontra o led
int estadoAnterior = LOW;  // variável de estado do botão. Começa como não pressionado.
bool botaoPressionado = false; // variável boleana de controle
unsigned long ultimoDebounceTime = 0;
unsigned long debounceDelay = 50;

void buttonPressed(int estadoBotao);

void setup() {
  pinMode(botaoPin, INPUT);
  pinMode(ledV, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int estadoBotao = digitalRead(botaoPin);
  buttonPressed(estadoBotao);
}

void buttonPressed(int estadoBotao) {
  if ((millis() - ultimoDebounceTime) > debounceDelay) {
    if (estadoBotao == HIGH && estadoAnterior == LOW && !botaoPressionado) {
      // O botão foi pressionado
      Serial.println("Botão pressionado");
      botaoPressionado = true;
      digitalWrite(ledV, HIGH);
    }
  
    if (estadoBotao == HIGH) {
      botaoPressionado = false;
      digitalWrite(ledV, LOW);
    }
  
    estadoAnterior = estadoBotao;
    ultimoDebounceTime = millis();
  }
}
