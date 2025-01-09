#define numero_botoes 4
#define numero_botoesOUT 2
#define TamanhoMaxDaTabela 10000
#define BlinkTimeInterval 1000
#define DEBOUNCE_TIME 50  // Tempo mínimo entre leituras válidas (em milissegundos)

int botoes[numero_botoes]={8,11,12,13};
int botoesOUT[numero_botoesOUT]={9,10};
int* TABELAEVENTOS_botoes;
unsigned long* TABELAEVENTOS_tempo;
int countTabela =0;
unsigned long tempoUltimaLeitura[numero_botoes] = {0, 0, 0, 0};  // Último tempo de leitura de cada botão
unsigned long tempoAtual;
unsigned long tempoBLINK=0;
bool bBlinkState=false;

int posImprimirMatriz=0;

unsigned long tempoUltimaImpressao = 0;

void setup() {
  Serial.begin(9600)

  // Vamos programar os BotoesOut
  for (int i=0; i< numero_botoesOUT;i++)
    pinMode(botoesOUT[i], OUTPUT);
    
  // Vamos programar os interrupts para todos os canais
  for (int i=0; i< numero_botoes;i++)
  {
    pinMode(botoes[i], INPUT);
    attachInterrupt(digitalPinToInterrupt(botoes[i]), sharedISR, RISING);
  }

  // Aloca memória para a TABELA
  TABELAEVENTOS_botoes=malloc(TamanhoMaxDaTabela * sizeof(int));
  TABELAEVENTOS_tempo=malloc(TamanhoMaxDaTabela * sizeof(unsigned long));

}

void sharedISR() {
  tempoAtual = millis();
  for (int i=0; i< numero_botoes;i++)
  if (digitalRead(botoes[i]))
    if ((tempoAtual-tempoUltimaLeitura[i])>DEBOUNCE_TIME)
    {
      tempoUltimaLeitura[i]=tempoAtual;
      TABELAEVENTOS_botoes[countTabela]=i;
      TABELAEVENTOS_tempo[countTabela]=tempoAtual;
      countTabela++;
    }
}



// Função para imprimir a matriz
void imprimirMatriz() {
  Serial.println("Imprimindo matriz:");
  for (int i = 0; i < countTabela; i++) {
    Serial.print("Botão ");
    Serial.print(TABELAEVENTOS_botoes[i]);
    Serial.print(": "+TABELAEVENTOS_tempo[i]);
    Serial.println();
  }
}

void loop() {
  if((tempoBLINK+BlinkTimeInterval)<millis());
  {
    tempoBLINK=millis();
    digitalWrite(botoesOUT[1],bBlinkState);
    bBlinkState=!bBlinkState;
  }


  if ((posImprimirMatriz+10)<countTabela)
  {
    posImprimirMatriz=countTabela;
   imprimirMatriz();
  }



}

