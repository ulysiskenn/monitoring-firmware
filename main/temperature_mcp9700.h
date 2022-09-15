const int TEMP_PIN_MCP9700 = PE_3;
float temperature_MCP9700 = 0;
void initMCP7000()
{
  pinMode(TEMP_PIN_MCP9700, OUTPUT);
}

void readMCP97000()
{
  float value_temperature = analogRead(TEMP_PIN_MCP9700);

  // Convert the analog reading (which goes from 0 - 4096) to a voltage (0 - 3.3v):
  float voltage_temperature = value_temperature * (3.3/ 4096);
  temperature_MCP9700 = voltage_temperature - 0.500;
  temperature_MCP9700 = temperature_MCP9700 / 0.01;

}
