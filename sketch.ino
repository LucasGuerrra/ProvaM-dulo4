#include "Button.h"//importa um arquivo com uma classe feita para retirar o bouncing do botão

const uint8_t led_verde = 2; // Pino utilizado para controle do led verde
const uint8_t led_vermelho = 1; // Pino utilizado para controle do led vermelho
const uint8_t led_amarelo = 9; // Pino utilizado para controle do led amarelo

const uint8_t botaoPino = 18;  // Numero do pino para o botão

const uint8_t ldrPino = 4;  // Número do pino para o sensor de luz
int limite = 600; //Limite de brilho no sensor para ser considerado dia

//Define o botão como um objeto
Button btn(botaoPino);

//Função para facilitar desligar e ligar as cores certas do semáforo
void semaforo(bool verde, bool amarelo, bool vermelho){
  digitalWrite(led_verde, verde);
  digitalWrite(led_amarelo, amarelo);
  digitalWrite(led_vermelho, vermelho);
}

//Função para quando o sensor detecta mais que o limite de luz
void modoDia(long tempo){
  while(1){    
    long intervalo = millis() - tempo;//Momento em que inicia
    int luz = analogRead(ldrPino);//Le o sensor de luz

    //Caso o sensor detecte que ficou de noite durante o processo, ele interrompe
    if(luz < limite){
      break;
    }
    

    if(intervalo < 3000){//Se o intervalo entre o inicio e agora for menor q 3 segundo ligue o led verde
      semaforo(HIGH, LOW, LOW);
    } else if(intervalo >= 3000 && intervalo < 5000){//Se for maior que 3 e menor que 5 ligue o amarelo
      semaforo(LOW, HIGH, LOW);
    } else if(intervalo >= 5000 && intervalo < 10000){//Caso seja maior que 5 e menor que 10 ligue o vermelho
      semaforo(LOW, LOW, HIGH);
      //Caso o botão seja apertado enquanto o intervalo é maior que 5000 e menor que 10000 ele liga o verde
      btn.update();
      if (btn.wasClicked()) {
        delay(1000);
        semaforo(HIGH, LOW, LOW);
        delay(3000);
      }
    } else{//Caso passe de 10 segundos de intervalo ele finaliza 
      break;
    }
  }
}

//Função para quando o sensor detecta menos que o limite de luz
void modoNoite(){
  while(1){
    //lendo o sensor de luz
    int luz = analogRead(ldrPino);

    //Se ocorrer de ficar mais claro durante o processo então ele sai do loop
    if(luz > limite){
      break;
    }

    //Faz o led brilhar amarelo
    semaforo(LOW,HIGH,LOW);
    delay(1000);

    //Confere novamente se não mudou o estado da luz
    luz = analogRead(ldrPino);

    if(luz > limite){
      break;
    }
    //desliga o led amarelo
    semaforo(LOW,LOW,LOW);
    delay(1000);    
  }
}

void setup() {
  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_verde,OUTPUT);
  pinMode(led_amarelo,OUTPUT);
  pinMode(led_vermelho,OUTPUT);

  // Inicialização das entradas
  pinMode(botaoPino, INPUT); // Definir o pino do botão como input

  //Setando todos os leds para apagados
  digitalWrite(led_verde, LOW);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_vermelho, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600
}

void loop() {
  int ldrstatus=analogRead(ldrPino);// Confere o valor atual de luz
  
  if(ldrstatus<=limite){// Se for menor que o limite ele inicia o modo noite
    modoNoite();
  }else{//Caso seja maior que o limite ele inicia o modo dia
    modoDia(millis());
  }
}