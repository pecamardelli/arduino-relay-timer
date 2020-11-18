void loadSystemData()
{

  // CARGA DE LOS DATOS DEL SISTEMA
  // Se almacenan a lo último de la EEPROM.
  // Los relés se guardan al principio.
 
  eeAddress = EEPROM.length() - sizeof(systemData) - 1;
  EEPROM.get(eeAddress, sys);

  Serial.print("Iniciando ethernet shield. Espera: ");

  // Esperamos 3 segundos antes de iniciar el shield porque no levanta cuando encendemos el arduino.
  for(int i=1;i<4;i++)
  {
    Serial.print(String(i) + ".");
    delay(1000);
  }

  Ethernet.begin(sys.mac, sys.ip, sys.dns, sys.gateway, sys.subnet);  
  Serial.println("hecho.\n");
  
  // Iniciamos el servidor de telnet
  Serial.print("Iniciando servidor: ");
  server.begin();
  Serial.println("hecho.\n");

  relayQuantity = 0;
  eeAddress     = 0;

  // Se cargan todos los relés guardados en la EEPROM en una lista
  // dinámica. De esta forma, pueden crearse relés sin problemas.
  
  do
  {
    aux = (node_t *)malloc(sizeof(node_t));
    
    if (aux == NULL)
    {
        Serial.println("Error al asignar memoria. Se cargaron " + String(relayQuantity) + " reles.");
        break;
    }
    else
    {
      EEPROM.get(eeAddress, aux->relay);
  
      if(aux->relay.type == 200)
      {
        if(first == NULL)
        {
          first       = aux;
          first->next = NULL;
          last        = first;
        }
        else
        {
          last->next  = aux;
          last        = aux;
          last->next  = NULL;
        }
  
        pinMode(aux->relay.pin, OUTPUT);
        digitalWrite(aux->relay.pin, HIGH);
        aux->changeFlag = false;
        aux->overrided  = false;
        
        eeAddress += sizeof(relayData);
        relayQuantity++;
      }
      else
      {
        Serial.println("Lista la carga de datos. " + String(relayQuantity) + " reles cargados.");
        break;
      }
    }
  }
  while(1);
}
