int aanelkaarplakken(char charreceived) {
  if (charreceived != ';') {
    serialData2 = serialData2 + charreceived;
  }
  else {
    commando = serialData2;
    serialData2 = "";
    return 1;
  }
}
