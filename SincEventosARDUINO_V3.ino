#define numero_botoes 2
#define TamanhoMaxDaTabela 100
#define DEBOUNCE_TIME 50
#define BlinkTimeInterval 1000
#define numero_botoesOUT 2

int botoes[numero_botoes] = {3, 2};  // Pinos dos botões
int botoesOUT[numero_botoesOUT] = {9, 10};  // Pinos de saída para LED
int* TABELAEVENTOS_botoes;
unsigned long* TABELAEVENTOS_tempo;
int countTabela = 0;
unsigned long tempoUltimaLeitura[numero_botoes] = {0, 0};  
unsigned long tempoAtual;
unsigned long tempoBLINK = 0;
bool bBlinkState = false;

int posImprimirMatriz = 0;

void setup() {
  Serial.begin(9600);

  // Programar os pinos de saída para o LED
  for (int i = 0; i < numero_botoesOUT; i++) {
    pinMode(botoesOUT[i], OUTPUT);
  }

  // Configura os pinos dos botões como entradas
  for (int i = 0; i < numero_botoes; i++) {
    pinMode(botoes[i], INPUT);
  }

  // Configura interrupções para os botões
  for (int i = 0; i < numero_botoes; i++) {
    attachInterrupt(digitalPinToInterrupt(botoes[i]), sharedISR, RISING);
  }

  // Aloca memória para a TABELA
  TABELAEVENTOS_botoes = (int*)malloc(TamanhoMaxDaTabela * sizeof(int));
  TABELAEVENTOS_tempo = (unsigned long*)malloc(TamanhoMaxDaTabela * sizeof(unsigned long));

  pinMode(11, INPUT);  // Botão de impressão
}

void sharedISR() {
  tempoAtual = millis();
  for (int i = 0; i < numero_botoes; i++) {
    if (digitalRead(botoes[i]) == HIGH) {
      if ((tempoAtual - tempoUltimaLeitura[i]) > DEBOUNCE_TIME) {
        Serial.print("O botão ");
        Serial.print(i);
        Serial.println(" foi pressionado");
        tempoUltimaLeitura[i] = tempoAtual;
        TABELAEVENTOS_botoes[countTabela] = i;
        TABELAEVENTOS_tempo[countTabela] = tempoAtual;
        countTabela++;
      }
    }
  }
}

void imprimirMatriz() {
  Serial.println("Imprimindo tabela:");
  for (int i = 0; i < countTabela; i++) {
    Serial.print("Botão ");
    Serial.print(TABELAEVENTOS_botoes[i]);
    Serial.print(" Pressionado em: ");
    Serial.println(TABELAEVENTOS_tempo[i]);
  }
}

void loop() {
  // Controle do LED com intervalo de blink
  if ((tempoBLINK + BlinkTimeInterval) < millis()) {
    tempoBLINK = millis();
    digitalWrite(botoesOUT[1], bBlinkState);  // Controla o LED no pino 10
    bBlinkState = !bBlinkState;
  }

  // Imprimir a tabela quando o botão 11 for pressionado
  if (digitalRead(11) == HIGH) {
    imprimirMatriz();
  }
}