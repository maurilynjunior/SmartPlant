#include <DHT.h>
#include <BlynkSimpleEsp8266.h>


#define DHTPIN 2     // pino de dados do sensor
#define DHTTYPE DHT11   // DHT 11

#define VPIN_BUTTON_1    V12 

char auth[] = "U9OT75btWBCwKJwJ4FE8LzqTyi-7UgYj";
char ssid[] = "Familia_Lima";
char pass[] = "fvp0673mp";

DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

double T, P;
char status;

void setup() {
  Serial.begin(9600);
/*
  WifiManager wm;

  bool res;
  
  res = wm.autoConnect("SmartPlant","12345678");

  if(!res){
    Serial.println("Falha na conexão!");
    delay(3000);
    ESP.restart();
  } else{
    Serial.println("Conectado com sucesso!");
  }
*/
  pinMode(A0, INPUT);
  // Delay pra não dar problema na inicialização
  delay(1500); 
  
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();

}

void loop() {
  LeituraUmidade();
  LeituraDHT();

}

void LeituraDHT() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Sem dados do sensor DHT!");
    return;
  }

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  
  Serial.print("Temp: "); Serial.println(t);
  Serial.print("UmAr: "); Serial.println(h);
  
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}

void LeituraUmidade(){
  int ValorADC;
  float Umidade;

  ValorADC = analogRead(A0);

  Umidade = map(ValorADC, 300, 1024, 100, 0);

  Blynk.virtualWrite(V2, Umidade);

  if(Umidade < 30){
    Blynk.logEvent("baixaumidade","Umidade do solo baixa!");
    WidgetLED LED(V3);
    LED.on();
  } else{
    WidgetLED LED(V3);
    LED.off();
  }

  Serial.print("vADC: "); Serial.println(analogRead(A0));
  Serial.print("UmSo: "); Serial.println(Umidade);
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
}