#define debug

#define ntc_thresh 500
#define ntc_hyst 10
#define ntc_max 280 // 300~50c, 260~55c 
#define pwm_min 30
#define sensor_avg_weight 10

#define ntc_pin A0
#define pot_pin A1
#define pwm_pin 3
#define turbo_pin 2

// PWM steps chosen manually to avoid resonances
int pwm_steps[] = {pwm_min, 60, 90, 120, 150};
int s;
int ntc = ntc_thresh;

void setup() {
#ifdef debug
  Serial.begin(115200);
#endif
  pinMode(ntc_pin, INPUT);
  pinMode(pot_pin, INPUT);
  pinMode(pwm_pin, OUTPUT);
  pinMode(turbo_pin, INPUT_PULLUP);

  // Clear some dust
  analogWrite(pwm_pin, 255);
  delay(5000);
}

void loop() {
//  analogWrite(pwm_pin, map(analogRead(pot_pin), 1023, 0, pwm_min, 255));
//  Serial.print(" pwm: ");
//  Serial.println(map(analogRead(pot_pin), 1023, 0, pwm_min, 255));
//  delay(500);
//  return;

  ntc = ((ntc * (sensor_avg_weight - 1)) + analogRead(ntc_pin)) / sensor_avg_weight;
  s = map(ntc, ntc_thresh, ntc_max, 0, 4);
  if (s < 0) s = 0;
  else if (s > 4) s = 4;

#ifdef debug
  Serial.print("ntc: ");
  Serial.print(ntc);
  Serial.print(" pwm_max: ");
  Serial.print(pwm_steps[4]);
  Serial.print(" pwm: ");
  Serial.println(pwm_steps[s]);
#endif

  if (ntc < ntc_thresh - ntc_hyst) {
    if (!digitalRead(turbo_pin)) analogWrite(pwm_pin, 255);
    else analogWrite(pwm_pin, pwm_steps[s]);
  }
  else if (ntc > ntc_thresh + ntc_hyst) {
    analogWrite(pwm_pin, 0);
  }

  delay(1000);
}
