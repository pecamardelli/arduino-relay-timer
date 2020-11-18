void deleteFile(String file, String source)
{
  if(SD.remove(file))
  {
    printData(source, "Archivo eliminado correctamente.", true);
  }
  else
  {
    printData(source, "Error al eliminar el archivo " + file + ".", true);
  }
}
