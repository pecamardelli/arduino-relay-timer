String arrayToString(byte array[], unsigned int len)
{
  char buffer[20] = "";
  
  for (unsigned int i = 0; i < len; i++) {
      byte nib1 = (array[i] >> 4) & 0x0F;
      byte nib2 = (array[i] >> 0) & 0x0F;
      buffer[i*3+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
      buffer[i*3+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
      
      if(i < len-1) {
        buffer[i*3+2] = ':';
      }
  }
  
  buffer[len*3] = '\0';
  return String(buffer);
}
