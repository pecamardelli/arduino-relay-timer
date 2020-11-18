String getDate()
{
  DateTime now = RTC.now();              // Obtiene la fecha y hora del RTC

  return  String(now.hour()) + ":" +
          String(now.minute()) + ":" +
          String(now.second()) + " " +
          dias[now.dayOfTheWeek()] + " " +
          String(now.day()) + "/" +
          String(now.month()) + "/" +
          String(now.year());
}
