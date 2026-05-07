#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <time.h>
#include <HTTPClient.h>

// ----------------------
// Pinos dos sensores
// ----------------------
#define DHT_PIN 15
#define POT_PIN 34
#define PIR_PIN 27

// ----------------------
// Wi-Fi do Wokwi
// ----------------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ----------------------
// MQTT
// ----------------------
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "cardioia/paciente001/sinais";

// ----------------------
// Objetos
// ----------------------
WiFiClient espClient;
PubSubClient client(espClient);
DHTesp dht;

// ----------------------
// Controle de tempo
// ----------------------
unsigned long ultimoEnvio = 0;
const unsigned long intervaloLeitura = 5000;

// ----------------------
// NTP para timestamp datetime
// ----------------------
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -3 * 3600; // Brasil UTC-3
const int daylightOffset_sec = 0;

// ----------------------
// Estrutura da leitura
// ----------------------
struct Leitura {
  int bpm;
  float temperatura;
  bool movimento;
  String timestamp;
};

// ----------------------
// Fila offline simulada
// ----------------------
const int LIMITE_FILA = 20;
Leitura filaOffline[LIMITE_FILA];
int totalOffline = 0;

// ----------------------
// Simulação de conexão lógica
// true = online
// false = offline
// ----------------------
bool sistemaOnline = true;

// ----------------------
// Conectar Wi-Fi
// ----------------------
void conectarWiFi() {
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// ----------------------
// Conectar MQTT
// ----------------------
void conectarMQTT() {
  while (!client.connected() && sistemaOnline) {
    Serial.println("Conectando ao broker MQTT...");

    String clientId = "CardioIA-ESP32-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado ao MQTT!");
    } else {
      Serial.print("Falha MQTT. Codigo: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// ----------------------
// Timestamp em formato datetime
// ----------------------
String obterTimestamp() {
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    return "1970-01-01T00:00:00";
  }

  char buffer[25];
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &timeinfo);

  return String(buffer);
}

// ----------------------
// Montar JSON no formato esperado pela API
// ----------------------
String montarJSON(Leitura leitura) {
  String json = "{";
  json += "\"bpm\":";
  json += String(leitura.bpm);
  json += ",";
  json += "\"temperatura\":";
  json += String(leitura.temperatura, 1);
  json += ",";
  json += "\"movimento\":";
  json += leitura.movimento ? "true" : "false";
  json += ",";
  json += "\"timestamp\":\"";
  json += leitura.timestamp;
  json += "\"";
  json += "}";

  return json;
}

// ----------------------
// Publicar MQTT
// ----------------------
void publicarMQTT(Leitura leitura) {
  String mensagem = montarJSON(leitura);

  Serial.println("Enviando JSON para MQTT/API:");
  Serial.println(mensagem);

  client.publish(mqtt_topic, mensagem.c_str());
}

// ----------------------
// Armazenar offline
// ----------------------
void armazenarOffline(Leitura leitura) {
  if (totalOffline < LIMITE_FILA) {
    filaOffline[totalOffline] = leitura;
    totalOffline++;

    Serial.println("Sistema offline. Leitura armazenada localmente.");
    Serial.print("Leituras pendentes: ");
    Serial.println(totalOffline);
  } else {
    Serial.println("Fila cheia. Descartando leitura mais antiga.");

    for (int i = 1; i < LIMITE_FILA; i++) {
      filaOffline[i - 1] = filaOffline[i];
    }

    filaOffline[LIMITE_FILA - 1] = leitura;
  }
}

// ----------------------
// Sincronizar offline
// ----------------------
void sincronizarOffline() {
  if (totalOffline == 0) {
    return;
  }

  Serial.println("Sincronizando leituras offline...");

  for (int i = 0; i < totalOffline; i++) {
    publicarMQTT(filaOffline[i]);
    delay(300);
  }

  totalOffline = 0;
  Serial.println("Sincronizacao concluida.");
}

// ----------------------
// Ler sensores
// ----------------------
Leitura lerSensores() {
  TempAndHumidity dadosDHT = dht.getTempAndHumidity();

  int valorPot = analogRead(POT_PIN);
  int bpm = map(valorPot, 0, 4095, 40, 160);

  bool movimento = digitalRead(PIR_PIN) == HIGH;

  Leitura leitura;
  leitura.bpm = bpm;
  leitura.temperatura = dadosDHT.temperature;
  leitura.movimento = movimento;
  leitura.timestamp = obterTimestamp();

  return leitura;
}

// ----------------------
// Setup
// ----------------------
void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);

  dht.setup(DHT_PIN, DHTesp::DHT22);

  conectarWiFi();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  client.setServer(mqtt_server, mqtt_port);

  Serial.println("Sistema CardioIA iniciado.");
}

// ----------------------
// Loop principal
// ----------------------
void loop() {
  if (sistemaOnline) {
    if (WiFi.status() != WL_CONNECTED) {
      conectarWiFi();
    }

    if (!client.connected()) {
      conectarMQTT();
    }

    client.loop();
  }

  unsigned long agora = millis();

  if (agora - ultimoEnvio >= intervaloLeitura) {
    ultimoEnvio = agora;

    Leitura leituraAtual = lerSensores();

    Serial.println("----------------------------");
    Serial.println("Nova leitura capturada:");
    Serial.print("BPM: ");
    Serial.println(leituraAtual.bpm);

    Serial.print("Temperatura: ");
    Serial.print(leituraAtual.temperatura);
    Serial.println(" C");

    Serial.print("Movimento: ");
    Serial.println(leituraAtual.movimento ? "true" : "false");

    Serial.print("Timestamp: ");
    Serial.println(leituraAtual.timestamp);

    if (sistemaOnline) {
      sincronizarOffline();
      publicarMQTT(leituraAtual);
    } else {
      armazenarOffline(leituraAtual);
    }
  }
}