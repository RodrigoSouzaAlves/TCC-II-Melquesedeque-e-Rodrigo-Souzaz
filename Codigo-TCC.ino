#include <DHT.h>

#define sensor_umidade_solo01 A0
#define sensor_umidade_solo02 A1
#define sensor_umidade_solo03 A2
#define sensor_dht 2
#define DHTTYPE DHT11
#define sensor_ldr A3
#define rele 3
#define pino_led_vermelho 5
#define pino_led_amarelo 6
#define pino_led_verde 7

DHT dht(sensor_dht, DHTTYPE);
 
int solo01;
int solo02;
int solo03;
int media_da_umidade;
float umidade_ambiente;
float temperatura;
int valor_ldr;
 
void setup()
{
  Serial.begin(9600);
  pinMode(sensor_umidade_solo01, INPUT);
  pinMode(sensor_umidade_solo02, INPUT);
  pinMode(sensor_umidade_solo03, INPUT);
  pinMode(sensor_dht, INPUT);
  pinMode(sensor_ldr, INPUT);
  pinMode(rele, OUTPUT);
  pinMode(pino_led_vermelho, OUTPUT);
  pinMode(pino_led_amarelo, OUTPUT);
  pinMode(pino_led_verde, OUTPUT);
  Serial.println("*****Painel de Controle******");
  Serial.println("-------------------------------");
  dht.begin();
}
 
void loop()
{
  //Le o valor do pino A0 do sensor
  solo01 = analogRead(sensor_umidade_solo01);
  solo02 = analogRead(sensor_umidade_solo02);
  solo03 = analogRead(sensor_umidade_solo03);
  valor_ldr = analogRead(sensor_ldr);
  umidade_ambiente = dht.readHumidity();
  temperatura = dht.readTemperature();
  
  //Calcula a media da umidade dos três sensores
  media_da_umidade = (solo01+solo02+solo03)/3;
  

  //Solo com umidade Ideal
  if (media_da_umidade > 0 && media_da_umidade < 500)
  {
    Serial.println();
    Serial.println("-------------------------------");
    Serial.println("Status: Plantação Irrigada!!");
    apagaleds();
    desligaRele();
    digitalWrite(pino_led_verde, HIGH);
  }

  //Irrigação em nivel moderado
  if(media_da_umidade > 600 && media_da_umidade <800)
  {
    Serial.println("Status: Irrigação em nivel moderado!!");
    Serial.println("Irrigação: Ativada!!");
    apagaleds();
    digitalWrite(pino_led_amarelo, HIGH);
  }
  
  //Condição não ideal para a irrigação
  if ((media_da_umidade > 700 && media_da_umidade < 800)&& (umidade_ambiente > 0 && umidade_ambiente < 90) && (valor_ldr < 600) && (temperatura < 10 && temperatura > 40))
  {
    Serial.println("Status: Condição não ideal para Irrigação");
    Serial.println("Irrigação: Desligada!!");
    apagaleds();
    desligaRele();
    digitalWrite(pino_led_amarelo, HIGH);
  }
 
  //Condição Ideal para Irrigação
  if ((media_da_umidade > 700 && media_da_umidade < 950) && (umidade_ambiente > 0 && umidade_ambiente < 90) && (valor_ldr > 600) && (temperatura > 10 && temperatura < 40) )
  {
    Serial.println("Status: Condição Ideal para Irrigação");
    Serial.println("Irrigação: Ativada!!");
    apagaleds();
    desligaRele();
    digitalWrite(pino_led_amarelo, HIGH);
    digitalWrite(rele, LOW);
  }

  
  //Plantação precisa de Irrigação mesmo que não seja o caso ideal.
  if(media_da_umidade > 950){
    Serial.println("Status: Plantação Necessita de Agua");
    Serial.println("Irrigação: Ativada!!");
    apagaleds();
    desligaRele();
    digitalWrite(pino_led_vermelho, HIGH);
    digitalWrite(rele, LOW);    
  }
  
  //Mostra o valor das portas analogicas no serial monitor
  Serial.print("Umidade sensor de solo 01: ");
  Serial.println(solo01);
  Serial.print("Umidade sensor de solo 02: ");
  Serial.println(solo02);
  Serial.print("Umidade sensor de solo 03: ");
  Serial.println(solo03);
  Serial.print("Média da Umidade---------: ");
  Serial.println(media_da_umidade);
  Serial.print("Luminosidade em----------: ");
  Serial.println(valor_ldr);

  if(isnan(umidade_ambiente) || isnan(temperatura))
  {
    Serial.println("Falha ao capitar dados com o Sensor DHT-11");
  }else{
      Serial.print("Umidade Ambiente em--: ");
      Serial.print(umidade_ambiente);
      Serial.print(" %");
      Serial.println();
      Serial.print("Temperatura ambiente-: ");
      Serial.print(temperatura);
      Serial.print(" Cº");
      Serial.println();
      Serial.println("-------------------------------");

   }
  delay(10000);
}
//Função para apagar todos os led's
void apagaleds()
{
  digitalWrite(pino_led_vermelho, LOW);
  digitalWrite(pino_led_amarelo, LOW);
  digitalWrite(pino_led_verde, LOW);
}

void desligaRele()
{
  digitalWrite(rele, HIGH);
}
