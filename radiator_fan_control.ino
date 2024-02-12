#define debug

#define ntc_thresh 500
#define ntc_hyst 10
#define ntc_max 260 // 300~50c, 260~55c 
#define pwm_min 30
#define sensor_avg_weight 10

#define ntc_pin A0
#define pot_pin A1
#define pwm_pin 3
#define turbo_pin 2

int pwm_max, pwm_val;
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
  ntc = ((ntc * (sensor_avg_weight - 1)) + analogRead(ntc_pin)) / sensor_avg_weight;
  pwm_max = map(analogRead(pot_pin), 1023, 0, pwm_min, 255);
  pwm_val = map(ntc, ntc_thresh, ntc_max, pwm_min, pwm_max);
  if (pwm_val < pwm_min)
    pwm_val = pwm_min;
  else if (pwm_val > pwm_max)
    pwm_val = pwm_max;

#ifdef debug
  Serial.print("ntc: ");
  Serial.print(ntc);
  Serial.print(" pwm_max: ");
  Serial.print(pwm_max);
  Serial.print(" pwm_val: ");
  Serial.println(pwm_val);
#endif

  if (ntc < ntc_thresh - ntc_hyst) {
    if (!digitalRead(turbo_pin)) analogWrite(pwm_pin, 255);
    else analogWrite(pwm_pin, pwm_val);
  }
  else if (ntc > ntc_thresh + ntc_hyst) {
    analogWrite(pwm_pin, 0);
  }

  delay(1000);
}
