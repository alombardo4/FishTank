int mode;
int r;
int g;
int b;
int brightness;
int checksum;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
    mode = Serial.parseInt();
    r = Serial.parseInt();
    g = Serial.parseInt();
    b = Serial.parseInt();
    brightness = Serial.parseInt();
    checksum = Serial.parseInt();

    if(mode + r + g + b + brightness == checksum) {
      Serial.print(mode);
      Serial.print(",");
      Serial.print(r);
      Serial.print(",");
      Serial.print(g);
      Serial.print(",");
      Serial.print(b);
      Serial.print(",");
      Serial.println(brightness);
    } else {
      Serial.println("err");
      Serial.print(mode);
      Serial.print(",");
      Serial.print(r);
      Serial.print(",");
      Serial.print(g);
      Serial.print(",");
      Serial.print(b);
      Serial.print(",");
      Serial.println(brightness);
    }
  }
  delay(10);
}
