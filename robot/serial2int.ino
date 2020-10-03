String serial2int() {
  if (serialData.indexOf("A") > -1) {
    Motor1 = serialDatMotor1(serialData);
    Motor1prev = Motor1;
  }
  else {
    Motor1 = Motor1prev;
  }

  if (serialData.indexOf("B") > -1) {
    Motor2 = serialDatMotor2(serialData);
    Motor2prev = Motor2;
  }
  else {
    Motor2 = Motor2prev;
  }

  if (serialData.indexOf("I") > -1) {
    return IrSensor();
    Serial.println("Sensor");
    //return "4324243224325432432624242424242424242";
  }

  if (serialData.indexOf("G") > -1) {

    return MPU6050();
    Serial.println("MPU6050");

  }

  if (serialData.indexOf("E") > -1) {
    Camera = serialDatCamera(serialData);
    Cameraprev = Camera;
  }
  else {
    Camera = Cameraprev;
  }

  return "42";
}

int serialDatMotor1(String data) {
  data.remove(data.indexOf("A"), 1);
  data.remove(data.indexOf("B"));
  return data.toInt();
}

int serialDatMotor2(String data) {
  data.remove(0, data.indexOf("B") + 1);
  data.remove(data.indexOf("C"));
  return data.toInt();
}
int serialDatGrijper(String data) {
  data.remove(0, data.indexOf("C") + 1);
  data.remove(data.indexOf("D"));
  return data.toInt();
}
int serialDatTilt(String data) {
  data.remove(0, data.indexOf("D") + 1);
  data.remove(data.indexOf("E"));
  return data.toInt();
}
int serialDatCamera(String data) {
  data.remove(0, data.indexOf("E") + 1);
  return data.toInt();
}
