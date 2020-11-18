void loadSystemData()
{

  // CARGA DE LOS DATOS DEL SISTEMA
  // Se almacenan a lo último de la EEPROM.
  // Los relés se guardan al principio.
 
  eeAddress = EEPROM.length() - sizeof(systemData) - 1;
  EEPROM.get(eeAddress, sys);

  Serial.print("Iniciando ethernet shield: ");
  Ethernet.begin(sys.mac, sys.ip, sys.dns, sys.gateway, sys.subnet);
  Serial.println("hecho.\n");

  int pos       = 0;
  relayQuantity = 0;
  eeAddress     = 0;

  // Se cargan todos los relés guardados en la EEPROM en una lista
  // dinámica. De esta forma, pueden crearse relés sin problemas.

  // Desde el byte 0 hasta el 100 está el "sistema de archivos" que consiste
  // en dos bytes que guardan un int, este int es la dirección de memoria
  // donde se guarda la info del rele. Si es igual a 0, 
  for(int i=0;i<50;i++)
  {
    EEPROM.get(eeAddress, pos);
  }
  
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
