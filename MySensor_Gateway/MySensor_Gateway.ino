#define LED_GREEN_PIN 6
#define LED_BLUE_PIN 5
#define LED_RED_PIN 4

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_GREEN_PIN,OUTPUT);
  pinMode(LED_BLUE_PIN,OUTPUT);
  pinMode(LED_RED_PIN,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  while(1)
  {
    delay(1000);
    digitalWrite(LED_GREEN_PIN,HIGH);
    digitalWrite(LED_BLUE_PIN,HIGH);
    digitalWrite(LED_RED_PIN,HIGH);
    delay(1000);
    digitalWrite(LED_GREEN_PIN,LOW);
    digitalWrite(LED_BLUE_PIN,LOW);
    digitalWrite(LED_RED_PIN,LOW);
  }
}
