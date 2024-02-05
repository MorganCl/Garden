// Import required libraries
#ifdef ESP32
  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
#else
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebSrv.h>
#endif
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PZEM004Tv30.h>
#include <ArduinoJson.h>

/* Use software serial for the PZEM
 * Pin 5 Rx (Connects to the Tx pin on the PZEM)
 * Pin 4 Tx (Connects to the Rx pin on the PZEM)
*/
PZEM004Tv30 pzem(12, 14);
// Data wire is connected to GPIO 4
#define ONE_WIRE_BUS 4
#define RelayPin1 16  //D1
#define RelayPin2 5  //D1
#define RelayPin3 0  //D1
#define RelayPin4 2   //D1

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// Variables to store temperature values
String temperatureF = "";
String temperatureC = "";
String temperatures = "0";

// Timer variables
unsigned long lastTime = 0;  
unsigned long last_temp_read = 1000 * 60 * 30;
unsigned long temp_read_delay = 1000 * 60 * 30;
unsigned long timerDelay = 2000;
String pumpStatus = "Inactive";
String heatMatStatus = "Inactive";

String voltage = "0";
String current = "0";
String power = "0";
String pf = "0";
float cost_per_day = 0;
float total_cost = 0;

// Replace with your network credentials
const char* ssid = "NETWORK SSID";
const char* password = "NETWORK PASSWORD";
bool status = false;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDSTemperatureC() {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);

  if(tempC == -127.00) {
    Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  } else {
    Serial.print("Temperature Celsius: ");
    Serial.println(tempC); 
  }
  return String(tempC);
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">

<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/moment.js/2.24.0/moment.min.js"></script>
    <script src="https://cdn.jsdelivr.net/gh/MorganCl/Garden@latest/main.js"></script>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css"
        integrity="sha512-iecdLmaskl7CVkqkXNQ/ZH/XLlvWZOJyj7Yy7tcenmpD1ypASozpmT/E0iPtmFIB46ZmdtAc9eNBvH0H/ZpiBw=="
        crossorigin="anonymous" referrerpolicy="no-referrer" />
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/gh/MorganCl/Garden@latest/main.css" />
</head>

<body>
    <div class="main-content">
        <aside>
            <section>
                <h2>
                    <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor"
                        class="bi bi-geo-alt" viewBox="0 0 16 16">
                        <path
                            d="M12.166 8.94c-.524 1.062-1.234 2.12-1.96 3.07A31.493 31.493 0 0 1 8 14.58a31.481 31.481 0 0 1-2.206-2.57c-.726-.95-1.436-2.008-1.96-3.07C3.304 7.867 3 6.862 3 6a5 5 0 0 1 10 0c0 .862-.305 1.867-.834 2.94zM8 16s6-5.686 6-10A6 6 0 0 0 2 6c0 4.314 6 10 6 10z" />
                        <path d="M8 8a2 2 0 1 1 0-4 2 2 0 0 1 0 4zm0 1a3 3 0 1 0 0-6 3 3 0 0 0 0 6z" />
                    </svg> Kirby Le Soken
                </h2>
                <h2 id="sunrise">07:19</h2>
            </section>
            <section>
                <h3 id="date">8th July</h3>
                <h2 id="sunset">19:32</h2>
            </section>
            <section>
                <div>
                    <h1>
                        <span id="tempActual">18</span><sup>&deg;C</sup>
                    </h1>
                    <h2>
                        <span id="temp"></span><sup>&deg;C</sup>
                    </h2>
                    <h2 id="current_forecast">Sunny</h2>
                </div>
                <div class="weather-container">
                    <div class="hot weather">
                        <span class="sun"></span>
                        <span class="sunx"></span>
                    </div>
                    <div class="cloudy weather">
                        <span class="cloud"></span>
                        <span class="cloudx"></span>
                    </div>
                    <div class="stormy weather">
                        <ul>
                            <li></li>
                            <li></li>
                            <li></li>
                            <li></li>
                            <li></li>
                            <li></li>
                            <li></li>
                            <li></li>
                        </ul>
                        <span class="snowe"></span>
                        <span class="snowex"></span>
                        <span class="stick"></span>
                        <span class="stick2"></span>
                    </div>
                    <div class="breezy weather">
                        <ul>
                            <li></li>
                            <li></li>
                            <li></li>
                            <li></li>
                            <li></li>
                        </ul>
                        <span class="cloudr"></span>
                    </div>
                    <div class="night weather">
                        <span class="moon"></span>
                        <span class="spot1"></span>
                        <span class="spot2"></span>
                        <ul>
                            <li></li>
                            <li></li>
                            <li></li>
                            <li></li>
                            <li></li>
                        </ul>
                    </div>
                </div>
            </section>
        </aside>
        <main>
            <div class="header">
                <h1>
                    Welcome back Morgan!
                </h1>
                <h2>
                    Check out today's climate tracker
                </h2>
            </div>
            <div class="day-graph-container">
                <div class="top-labels">
                </div>
                <div class="day-graph">
                    <canvas id="day-graph-canvas" height="300px"></canvas>
                </div>
            </div>
            <div class="header">
                <h1>Relay</h1>
            </div>
            <div class="card-container">
                <div class="row">
                    <div onclick="togglePump()">
                        <h1>Water</h1>
                        <h2 id="water">Inactive</h2>
                    </div>
                    <div onclick="toggleHeatMat()">
                        <h1>Heat Mat</h1>
                        <h2 id="heat_mat">Inactive</h2>
                    </div>
                </div>
                <div class="row">
                    <div>
                        <h1>Spare</h1>
                        <h2>Inactive</h2>
                    </div>
                    <div>
                        <h1>Spare</h1>
                        <h2>Inactive</h3>
                    </div>
                </div>
            </div>
            <div class="header">
                <h1>Power</h1>
            </div>
            <div class="card-container">
                <div class="row">
                    <div>
                        <h1>Daily Cost</h1>
                        <h2 id="cost"></h2>
                    </div>
                    <div>
                        <h1>Total Cost</h1>
                        <h2 id="total_cost"></h2>
                    </div>
                </div>
                <div class="row">
                    <div>
                        <h1>Voltage</h1>
                        <h2 id="voltage">0</h2>
                    </div>
                    <div>
                        <h1>Current</h1>
                        <h2 id="current">0</h2>
                    </div>
                </div>
                <div class="row">
                    <div>
                        <h1>Power</h1>
                        <h2 id="power">0</h2>
                    </div>
                    <div>
                        <h1>Power Factor</h1>
                        <h2 id="pf">0</h3>
                    </div>
                </div>
            </div>
        </main>
    </div>
</body>

</html>)rawliteral";

void setup(){
  // Serial port for debugging purposes
  Serial.begin(9600);
  Serial.println();
  
  // Start up the DS18B20 library
  sensors.begin();
  pinMode(RelayPin1, OUTPUT);  
  digitalWrite(RelayPin1, HIGH);
  pinMode(RelayPin2, OUTPUT);
  digitalWrite(RelayPin2, HIGH);

  temperatureC = readDSTemperatureC();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", temperatureC.c_str());
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", temperatureC.c_str());
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  server.on("/temperatures", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", (temperatures).c_str());
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  server.on("/pumpc", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", pumpStatus.c_str());
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  
  server.on("/heat_mat", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", heatMatStatus.c_str());
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  server.on("/pumpctog", HTTP_GET, [](AsyncWebServerRequest *request){
    if (pumpStatus == "Pumping") {
      digitalWrite(RelayPin1, HIGH);
      pumpStatus = "Inactive";
    } else {
      digitalWrite(RelayPin1, LOW);
      pumpStatus = "Pumping";
    }
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", pumpStatus.c_str());
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  
  server.on("/heat_mattog", HTTP_GET, [](AsyncWebServerRequest *request){
    if (heatMatStatus == "Heating") {
      digitalWrite(RelayPin2, HIGH);
      heatMatStatus = "Inactive";
    } else {
      digitalWrite(RelayPin2, LOW);
      heatMatStatus = "Heating";
    }
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", heatMatStatus.c_str());
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  
  server.on("/power", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", power.c_str());
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  
  server.on("/current", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", current.c_str());
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  
  server.on("/voltage", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", voltage.c_str());
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  
  server.on("/cost_per_day", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", String(cost_per_day).c_str());
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  server.on("/total_cost", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", String(total_cost).c_str());
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  server.on("/pf", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", pf.c_str());
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  // Start server
  server.begin();
}
 
void loop(){
  if ((millis() - lastTime) > timerDelay) {
    temperatureC = readDSTemperatureC();
    if ((millis() - last_temp_read) > temp_read_delay) {
      temperatures += "," + temperatureC;
      int count = 0;
      for(int i = 0; temperatures[i] != '\0'; i++)
      {
          if(temperatures[i] == ',')
              count++;
      }

      if (count > 12) {
        String temp = temperatures;
        if (temp[0] == 0) {
          temp.remove(2);
          temperatures.remove(temperatures.indexOf("0,"), temp.indexOf(",") + 2);
        } else {
          temperatures.remove(0, temperatures.indexOf(",") + 1);
        }
      }
      Serial.println(temperatures);
      last_temp_read = millis();
    }
    float v = pzem.voltage();
    if( !isnan(v) ){
        Serial.print("Voltage: "); Serial.print(v); Serial.println("V");
        voltage = String(v);
    } else {
        Serial.println("Error reading voltage");
    }

    float c = pzem.current();
    if( !isnan(c) ){
        Serial.print("Current: "); Serial.print(c); Serial.println("A");
        current = String(c);
    } else {
        Serial.println("Error reading current");
    }

    float p = pzem.power();
    if( !isnan(p) ){
        Serial.print("Power: "); Serial.print(p); Serial.println("W");
        power = String(p);
        cost_per_day = ((p / 1000) * 24) * 0.3072;
        total_cost += (((cost_per_day / 24) / 60) / 30);
    } else {
        Serial.println("Error reading power");
    }

    float p_f = pzem.pf();
    if( !isnan(p_f) ){
        Serial.print("PF: "); Serial.println(p_f);
        pf = String(p_f);
    } else {
        Serial.println("Error reading power factor");
    }
    lastTime = millis();
  }
  //if ((millis() - lastPumpTime) > pumpActive && pumpStatus == "Pumping") {
  //  digitalWrite(RelayPin1, HIGH);
  //  pumpStatus = "Inactive";
  //  lastPumpTime = millis();
  //}

}
