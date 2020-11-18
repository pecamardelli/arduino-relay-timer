void printDirectory(File dir, int numTabs, String source)
{  
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      dir.rewindDirectory();
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      printData(source, "\t", false);
    }
    printData(source, entry.name(), false);
    if (entry.isDirectory()) {
      printData(source, "/", true);
      printDirectory(entry, numTabs + 1, source);
    } else {
      // files have sizes, directories do not
      printData(source, "\t\t" + String(entry.size()), true);
    }
    entry.close();
  }
}

