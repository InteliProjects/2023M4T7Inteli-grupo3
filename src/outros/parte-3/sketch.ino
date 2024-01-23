// Define os pinos do LED RGB
#define RED_PIN 14
#define GREEN_PIN 27
#define BLUE_PIN 26

unsigned long previousMillis = 0;
const long interval = 3000; // Intervalo de 3 segundos entre as cores
int status = 0; // Inicialmente, define o status como 0 (vermelho)
// Define os pinos do LED RGB
#define RED_PIN 14
#define GREEN_PIN 27
#define BLUE_PIN 26

unsigned long previousMillis = 0; // Variável para armazenar o tempo anterior
const long interval = 3000; // Intervalo de 3 segundos entre as cores
int status = 0; // Variável para controlar o status, inicialmente definida como 0 (vermelho)

void setup() {
  Serial.begin(9600);

// Aqui configura os pinos dos leds como saídas
  pinMode(RED_PIN, OUTPUT); 
  pinMode(GREEN_PIN, OUTPUT); 
  pinMode(BLUE_PIN, OUTPUT); 
}

// Função para atualizar o LED com base no status da classificação de risco de cada paciente no pronto atendimento
void atualizarLED(int status) {
  switch (status) {
    case 0: // Cor do led Vermelho para pacientes com casos graves (emergência)
      digitalWrite(RED_PIN, HIGH); 
      digitalWrite(GREEN_PIN, LOW); 
      digitalWrite(BLUE_PIN, LOW); 
      break;
    case 1: // Cor do led Amarelo (Vermelho + Verde) para pacientes com casos urgente
      digitalWrite(RED_PIN, HIGH); 
      digitalWrite(GREEN_PIN, HIGH); 
      digitalWrite(BLUE_PIN, LOW); 
      break;
    case 2: // Cor do led Verde para pacientes com casos pouco urgentes
      digitalWrite(RED_PIN, LOW); 
      digitalWrite(GREEN_PIN, HIGH); 
      digitalWrite(BLUE_PIN, LOW); 
      break;
  }
}

void loop() {
  unsigned long currentMillis = millis(); // Obtém o tempo atual em milissegundos

  if (currentMillis - previousMillis >= interval) { // Verifica se passou o intervalo
    previousMillis = currentMillis; // Atualiza o tempo anterior

    atualizarLED(status); // Chama a função para atualizar o LED com base no status da classificação de risco

    if (status == 0) {
      status = 1; // Muda para amarelo
    } else if (status == 1) {
      status = 2; // Muda para verde
    } else {
      status = 0; // Muda de volta para vermelho
    }
  }
}
