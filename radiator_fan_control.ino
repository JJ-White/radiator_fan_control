#define debug

#define ntc_thresh 400
#define pwm_min 100

#define ntc_pin A0
#define pot_pin A1
#define pwm_pin 3
#define turbo_pin 2

int ntc = ntc_thresh;

void setup() {
#ifdef debug
  Serial.begin(115200);
#endif
  pinMode(ntc_pin, INPUT);
  pinMode(pot_pin, INPUT);
  pinMode(pwm_pin, OUTPUT);
  pinMode(turbo_pin, INPUT_PULLUP);
}

void loop() {
  ntc = ((ntc * 4) + analogRead(ntc_pin)) / 5;

#ifdef debug
  Serial.println(ntc);
#endif

  if (ntc < ntc_thresh) {
    if (!digitalRead(turbo_pin)) analogWrite(pwm_pin, 255);
    else analogWrite(pwm_pin, map(analogRead(pot_pin), 1023, 0, pwm_min, 255));
  }
  else {
    analogWrite(pwm_pin, 0);
  }

  delay(1000);
}