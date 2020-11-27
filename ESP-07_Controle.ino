//Proramação do módulo Wi-Fi do controle versão 01
//ESTADO - TESTES INICIAIS (INCOMPLETO)

#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


WiFiClient controleclient;
PubSubClient APP_Broker(controleclient);

const char* BROKER_TCP = "18.231.155.160";
const int BROKER_PORT = 1234;

String val_ya, val_yb, val_xa;

void setup() {
  conectar_wifi();
}

void loop() {
  reconectar();
  APP_Broker.loop();
  envia_dados_wifi();
}

void conectar_wifi(){
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("Controle");
  while(WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Controle Conectado");
  conectar_broker();
}

void conectar_broker(){
  APP_Broker.setServer(BROKER_TCP, BROKER_PORT);
  delay(500);
  
  if (APP_Broker.connect(BROKER_TCP)) {
      Serial.println("Conectado");
    } else {
        while (!APP_Broker.connected()) {
          Serial.println("Tentando se conectar ao Broker");
          delay(700);
          if (!APP_Broker.connect(BROKER_TCP)) {
            Serial.println("Falha ao Reconectar");
            Serial.println("Tentando Novamente");
            delay(1000);
          } 
        }
    }
}

void reconectar(){
  if(WiFi.status() != WL_CONNECTED){
    conectar_wifi();
  }else
    
  if(!APP_Broker.connected()){
    conectar_broker();
  }
}

void envia_dados_wifi(){
  if(WiFi.status() == WL_CONNECTED){
    val_ya = Serial.read();
    delay(1);
    val_yb = Serial.read();
    delay(1);
    val_xa = Serial.read();
    String payload = val_ya;
           payload += ",";
           payload += val_yb;
           payload += ",";
           payload += val_xa;
    char payloadPWM[payload.length()];
    payload.toCharArray(payloadPWM, payload.length() + 1);
    APP_Broker.publish("PWM", payloadPWM);
    delay(1);
  }
}
