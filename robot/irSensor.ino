
String IrSensor(){
  String DataIr;
  pcf8574.digitalWrite(0, LOW);
  pcf8574.digitalWrite(1, LOW);
  pcf8574.digitalWrite(2, LOW);
  DataIr = DataIr + String(analogRead(A0)) + ";";

  pcf8574.digitalWrite(0, HIGH);
  pcf8574.digitalWrite(1, LOW);
  pcf8574.digitalWrite(2, LOW);
  DataIr = DataIr + String(analogRead(A0)) + ";";

  pcf8574.digitalWrite(0, LOW);
  pcf8574.digitalWrite(1, HIGH);
  pcf8574.digitalWrite(2, LOW);
  DataIr = DataIr + String(analogRead(A0)) + ";";

  pcf8574.digitalWrite(0, HIGH);
  pcf8574.digitalWrite(1, HIGH);
  pcf8574.digitalWrite(2, LOW);
  DataIr = DataIr + String(analogRead(A0)) + ";";

  pcf8574.digitalWrite(0, LOW);
  pcf8574.digitalWrite(1, LOW);
  pcf8574.digitalWrite(2, HIGH);
  DataIr = DataIr + String(analogRead(A0)) + ";";

  pcf8574.digitalWrite(0, HIGH);
  pcf8574.digitalWrite(1, LOW);
  pcf8574.digitalWrite(2, HIGH);
  DataIr = DataIr + String(analogRead(A0)) + ";";

  pcf8574.digitalWrite(0, LOW);
  pcf8574.digitalWrite(1, HIGH);
  pcf8574.digitalWrite(2, HIGH);
  DataIr = DataIr + String(analogRead(A0)) + ";";

  pcf8574.digitalWrite(0, HIGH);
  pcf8574.digitalWrite(1, HIGH);
  pcf8574.digitalWrite(2, HIGH);
  DataIr = DataIr + String(analogRead(A0)) + ";";
  delay(1);

return DataIr;
}
