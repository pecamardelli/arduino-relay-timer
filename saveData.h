void saveData()
{
  File f;
  String fileName;

  if(!SD.exists(sysFolder))
  {
    SD.mkdir(sysFolder);
  }
  
  fileName = sysFolder + "hostname.txt";
  
  SD.remove(fileName);
  f = SD.open(fileName, FILE_WRITE);
  f.print(hostName);
  f.close();

  for(int i=0;i<4;i++)
  {
    fileName = sysFolder + "r" + String(i) + "enab.txt";
    SD.remove(fileName);
    f = SD.open(fileName, FILE_WRITE);
    f.print(relayEnabled[i]);
    f.close();

    fileName = sysFolder + "r" + String(i) + "desc.txt";
    SD.remove(fileName);
    f = SD.open(fileName, FILE_WRITE);
    f.print(relayDesc[i]);
    f.close();

    fileName = sysFolder + "r" + String(i) + "starth.txt";
    SD.remove(fileName);
    f = SD.open(fileName, FILE_WRITE);
    f.print(relayStartHour[i]);
    f.close();

    fileName = sysFolder + "r" + String(i) + "startm.txt";
    SD.remove(fileName);
    f = SD.open(fileName, FILE_WRITE);
    f.print(relayStartMin[i]);
    f.close();

    fileName = sysFolder + "r" + String(i) + "endh.txt";
    SD.remove(fileName);
    f = SD.open(fileName, FILE_WRITE);
    f.print(relayEndHour[i]);
    f.close();
    
    fileName = sysFolder + "r" + String(i) + "endm.txt";
    SD.remove(fileName);
    f = SD.open(fileName, FILE_WRITE);
    f.print(relayEndMin[i]);
    f.close();
  }  
}
