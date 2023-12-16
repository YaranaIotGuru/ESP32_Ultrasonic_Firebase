#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>

#define FIREBASE_HOST "" // Firebase project name address
#define FIREBASE_AUTH "" // Firebase secret key
#define WIFI_SSID "Yarana" // Your WiFi name
#define WIFI_PASSWORD "12112001" // WiFi password
#define TRIG_PIN 12 // Replace with your TRIG pin number
#define ECHO_PIN 14 // Replace with your ECHO pin number

String fireStatus = "";                    // LED status received from Firebase
int led = 2;                               // Define the LED pin

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(led, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);   // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);

  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  // Connect to Firebase
  Firebase.setString("LED_STATUS", "OFF");      // Send initial LED status
}

void loop() {
  // Read distance from ultrasonic sensor
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * 0.0343) / 2;

  // Send ultrasonic sensor data to Firebase
  Firebase.setInt("ULTRASONIC_DISTANCE", distance);

   Serial.println("Distance : " + String(distance));

  fireStatus = Firebase.getString("LED_STATUS");  // Get LED status input from Firebase

  if (fireStatus == "1") {
    Serial.println("Led Turned ON");
    digitalWrite(led, HIGH);                    // Turn on the LED
  } else if (fireStatus == "0") {
    Serial.println("Led Turned OFF");
    digitalWrite(led, LOW);                     // Turn off the LED
  } else {
    Serial.println("Wrong Credential! Please send ON/OFF");
  }

  delay(1000); // Add delay as needed
}
