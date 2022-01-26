#define debug

#define ntc_thresh 450
#define pwm_min 30
#define sensor_avg_weight 30

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
  ntc = ((ntc * (sensor_avg_weight - 1)) + analogRead(ntc_pin)) / sensor_avg_weight;

#ifdef debug
  Serial.print("pwm: ");
  Serial.print(map(analogRead(pot_pin), 1023, 0, pwm_min, 255));
  Serial.print(" ntc: ");
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
