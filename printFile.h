void printFile(String file, String source)
{  
  //Se vuelve a abrir el fichero, esta vez para leer los datos escritos.
  File entry = SD.open(file);
  
  //Si el archivo se ha abierto correctamente se muestran los datos.
  if(entry){
    //Serial.println("Imprimiendo " + logfile + ";");
    //Se implementa un bucle que recorrerá el archivo hasta que no encuentre más
    //información (entry.available()==FALSE).
    while (entry.available()){
     //Se escribe la información que ha sido leída del archivo.
     //Serial.write(entry.read());
      //printData(source, String(entry.read()), false);
      if(source == "serial")
      {
        Serial.write(entry.read());
      }
      else if(source == "telnet")
      {
        client.write(entry.read());
      }
    }    
    //Si todo ha ido bien cierra el archivo para no perder datos.
    entry.close();
  }
  else
  {
    printData(source, "El archivo " + file + " no se abrió correctamente", true);
  }
}

