const int FOWARD_PIN      = PE_0;
const int REVERSE_PIN     = PE_1;
const int TEMP_PIN_AD8364 = PE_2;

float vswr = 0;
float power_forward_dbm =0;
float power_reverse_dbm = 0;
float temperature_AD8364 = 0;
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
float voltage_temperature = 0;
void initAD8364 ()
{
  pinMode(FOWARD_PIN, OUTPUT);
  pinMode(REVERSE_PIN, OUTPUT);
  pinMode(TEMP_PIN_AD8364, OUTPUT);
}

void readPowerAD8364()
{
  float slope = 0.04933333;
  float intercept = -60.20;
  int read_forward = analogRead(FOWARD_PIN);
  int read_reverse = analogRead(REVERSE_PIN);

  float voltage_forward = mapfloat(read_forward, 0, 4095, 0, 3.5);
  float voltage_reverse = mapfloat(read_reverse, 0, 4095, 0, 3.5);

  //delay(500);
  float power_forward = (voltage_forward / slope ) + intercept;
  float power_reverse = (voltage_reverse / slope ) + intercept;

  float error_forward = mapfloat(voltage_forward, 0, 3.5, -2, 2);
  float error_reverse = mapfloat(voltage_reverse, 0, 3.5, -2, 2);
  
  power_forward_dbm = power_forward  - error_forward;
  power_reverse_dbm = power_reverse - error_reverse;

  float power_forward_mW = pow(10, (power_forward_dbm/10));
  float power_reverse_mW = pow(10, (power_reverse_dbm/10)); 

  vswr = (1 + sqrt((power_reverse_mW / power_forward_mW))) / (1 - sqrt((power_reverse_mW / power_forward_mW)));

}

void readTemperatureAD8364()
{
  int read_temperature = analogRead(TEMP_PIN_AD8364);
  voltage_temperature = read_temperature * (3.3/ 4096);
  temperature_AD8364 = voltage_temperature / 0.028;
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
