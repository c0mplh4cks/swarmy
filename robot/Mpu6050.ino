
String MPU6050() {
  String MPU6050Data;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  MPU6050Data = MPU6050Data + String(g.gyro.x) + ";";
  MPU6050Data = MPU6050Data + String(g.gyro.y) + ";";
  MPU6050Data = MPU6050Data + String(g.gyro.z) + ";";

  return MPU6050Data;
}
