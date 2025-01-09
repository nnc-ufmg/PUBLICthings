#define numero_botoes 4
#define DEBOUNCE_TIME 50  // Tempo mínimo entre leituras válidas (em milissegundos)

int* botoes;
int** matriz_botoes;
int colunas[numero_botoes] = {1,1,1,1};

unsigned long tempoUltimaLeitura[numero_botoes] = {0, 0, 0, 0};  // Último tempo de leitura de cada botão
int estadoAnterior[numero_botoes] = {LOW, LOW, LOW, LOW};  // Estado anterior de cada botão
int lastval[numero_botoes] = {LOW, LOW, LOW, LOW};  // Último valor lido de cada botão
unsigned long tempoUltimaImpressao = 0;

void setup() {
  Serial.begin(9600);

  // Inicializa os pinos
  pinMode(10, OUTPUT); 
  pinMode(9, OUTPUT);

  // Aloca memória para os pinos dos botões
  botoes = (int*)malloc(numero_botoes * sizeof(int));

  botoes[0] = 8;
  botoes[1] = 11;
  botoes[2] = 12;
  botoes[3] = 13;

  // Aloca memória para as linhas (vetor de ponteiros)
  matriz_botoes = (int**)malloc(numero_botoes * sizeof(int));
  if (matriz_botoes == NULL) {
    Serial.println("Erro ao alocar memória para as linhas!");
    return;
  }

  // Aloca memória para cada linha
  for (int i = 0; i < numero_botoes; i++) {
    matriz_botoes[i] = (int*)malloc(colunas[i] * sizeof(int));
    if (matriz_botoes[i] == NULL) {
      Serial.println("Erro ao alocar memória para a linha " + String(i));
      return;
    }
  }

  for(int i = 0; i < numero_botoes;i++){
    for(int j =0; j< colunas[i]; j++)
      matriz_botoes[i][j] = 0;

    }



  }



  // Configura os pinos como entradas com resistores pull-up
  for (int i = 0; i < numero_botoes; i++) {
    pinMode(botoes[i], INPUT_PULLUP);
  }
}

void sendManchester(int bit) {
  if (bit == 0) {
    digitalWrite(10, LOW);
    delay(100); // Half-bit period
  } else {
    digitalWrite(10, HIGH);
    delay(100); // Half-bit period
  }
}

void ler_botao(int pino, int indice) {
  unsigned long tempoAtual = millis();  // Tempo atual em milissegundos
  int val = digitalRead(pino);  // Lê o estado do botão
  
  // Verifica se o tempo desde a última leitura é maior que o debounce
  if ((tempoAtual - tempoUltimaLeitura[indice]) > DEBOUNCE_TIME) {

    // Verifica se o estado do botão mudou (de LOW para HIGH ou HIGH para LOW)
    if (val != lastval[indice]) {
      
      // Se o estado mudou e é um valor HIGH, significa que o botão foi pressionado
      if (val == HIGH) {
        Serial.println("Botão " + String(indice) + " pressionado!");
        Serial.print("Tempo: ");
        Serial.println(tempoAtual);

      
        matriz_botoes[indice] = (int*)realloc(matriz_botoes[indice], colunas[indice] * sizeof(int));

        if (matriz_botoes[indice] == NULL) {
          Serial.println("Erro ao realocar memória para o botão " + String(indice));
          return;
        }

        // Adiciona o horário
        matriz_botoes[indice][colunas[indice]-1 ] = tempoAtual;

        // Aumenta o tamanho da coluna do botão para adicionar um horário
        colunas[indice]++;

        // Transmite uma amostra de dados codificados Manchester (exemplo: 1010)
        int message[] = {1, 0};
        for (int i = 0; i < sizeof(message) / sizeof(message[0]); i++) {
          sendManchester(message[i]);
        }
      }
      
      // Atualiza o valor anterior e o tempo da última leitura
      lastval[indice] = val;
      tempoUltimaLeitura[indice] = tempoAtual;
      
    }
  }
}

// Função para imprimir a matriz
void imprimirMatriz() {
  Serial.println("Imprimindo matriz:");
  for (int i = 0; i < numero_botoes; i++) {
    Serial.print("Botão ");
    Serial.print(i);
    Serial.print(": ");
    for (int j = 0; j < colunas[i] -1; j++) {
      Serial.print(matriz_botoes[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void loop() {
  // Lê os botões
  ler_botao(11, 2);
  ler_botao(12, 1);
  ler_botao(8, 0);
  ler_botao(13, 3);

  // Verifica se passaram 10 segundos desde a última impressão
  if (millis() - tempoUltimaImpressao >= 10000) {
    // Atualiza o tempo da última impressão
    tempoUltimaImpressao = millis();

    // Imprime a matriz
   imprimirMatriz();
}
}
