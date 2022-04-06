
#include <Wire.h>
#include <Arduino.h>
#include <MD5.h>
#include "SparkFun_External_EEPROM.h"
#include "string.h"
ExternalEEPROM myMem;
byte inputText[] = "M.T=MegaSega:Version=2.0.3";

//M.T=PCB:Version=2.0.2 eski
//M.T=PCB:Version=2.0.3 eski
//M.T=Vega+:Version=0.4.3
//M.T=Giga:Version=0.0.2
//M.T=MegaSega:Version=2.0.3
//M.T=MegaSega:Version=2.0.3
void writeToEEPROM()
{

  char inputText_as_char[sizeof(inputText)];
  // BYTE ARRAY TO CHAR ARRAY
  for (unsigned int i = 0; i < sizeof(inputText); i++)
  {
    inputText_as_char[i] = (char)inputText[i];
  }

  // MD5 HASH OF INPUT STR
  unsigned char *hash = MD5::make_hash(inputText_as_char);
  char *md5str = MD5::make_digest(hash, 16);
  free(hash);
  Serial.print("Calculated MD5 = ");
  Serial.println(md5str);

  // PREPARE FINAL STR (STR:MD5 HASH)
  String final_text = String((char *)inputText);
  final_text += ":";
  final_text += String((char *)md5str);

  Serial.print("Final string = ");
  Serial.println(final_text);

  byte plain[final_text.length() + 1];

  //STR TO BYTE ARRAY
  final_text.getBytes(plain, final_text.length() + 1);

  free(md5str);

  for (size_t i = 0; i < sizeof(plain) + 1; i++)
  {
    myMem.put(i, plain[i]); //(location, data)
    Serial.print("Writed ");
    Serial.print(i);
    Serial.print(". Adress. Value of adress = ");
    Serial.println(plain[i]);
  }
  myMem.put(sizeof(plain) + 1, '\n'); //(location, data)
}

void readFromEEPROM()
{

  //Read from EEPROM until endln
  String result;
  int counter = 0;
  Serial.print("Reading : ");
  while (true)
  {
    byte myValue3;
    myMem.get(counter, myValue3);

    if (myValue3 == 10)
      break;
    result = result + myValue3 + ",";
    counter++;

    Serial.print("Readed ");
    Serial.print(counter);
    Serial.print(". Adress. Value of adress = ");
    Serial.println(myValue3);
  }
  Serial.print("Readed string from EEPROM: ");
  Serial.println(result);

  int comma_counter = 0;
  for (int i = 0; i < result.length(); i++)
  {
    if (result.substring(i, i + 1) == ",")
      comma_counter++;
  }
  Serial.print("Comma counter : ");
  Serial.println(comma_counter);

  byte cipher[comma_counter - 1];

  int parse_counter = 0;
  int word_counter = 0;
  int begin_counter = 0;
  for (size_t i = 0; i < result.length(); i++)
  {
    if (result.substring(i, i + 1) == ",")
    {
      cipher[word_counter] = result.substring(begin_counter, i).toInt();
      begin_counter = i + 1;
      word_counter++;
    }
    else
    {
      parse_counter++;
    }
  }

  String myString = String((char *)cipher);
  Serial.println(myString);
  // String outputStr =
  // Serial.println(outputStr);

  // // check if string contains :
  // int index = outputStr.lastIndexOf(":");
  // if (index > 1)
  // {
  //   String keyword = outputStr.substring(0, index);

  //   String hash_from_eeprom = outputStr.substring(index + 1, outputStr.length());

  //   int n = keyword.length();

  //   // declaring character array
  //   char char_array[n + 1];

  //   // copying the contents of the
  //   // string to char array
  //   strcpy(char_array, keyword.c_str());

  //   // MD5 HASH OF INPUT STR
  //   unsigned char *hash = MD5::make_hash(char_array);
  //   //generate the digest (hex encoding) of our hash
  //   char *md5str = MD5::make_digest(hash, 16);
  //   free(hash);
  //   Serial.println("ss");
  //   Serial.println(md5str);
  // Serial.println("asdas");
  // Serial.println(hash_from_eeprom);
  // Serial.println("dd");
  // if (md5str == hash_from_eeprom)
  // {
  //   //Parse String
  //   String machineType, version;
  //   machineType = outputStr.substring(0, outputStr.indexOf(":"));
  //   machineType.remove(0, machineType.indexOf("=") + 1);
  //   Serial.println(machineType);

  //   outputStr.remove(0, outputStr.indexOf(":") + 1);

  //   Serial.println(outputStr);

  //   version = outputStr.substring(0, outputStr.indexOf(":"));
  //   version.remove(0, version.indexOf("=") + 1);
  //   Serial.println(version);
  // }
  // else
  // {
  //   Serial.println("Error");
  // }
}

void setup()
{
  while (!Serial)
    ;
  Serial.begin(115200);
  delay(10);

  Wire.begin();

  if (myMem.begin() == false)
  {
    Serial.println("No memory detected. Freezing.");
    while (true)
    {
      delay(500);
      Serial.println(myMem.begin());
    }
  }
  Serial.println("Memory Detected.");

  for (size_t i = 0; i < 255; i++)
  {
    myMem.put(i, 31);
  }

  writeToEEPROM();
  delay(1000);
  readFromEEPROM();

  // for (size_t i = 0; i < 255; i++)
  // {
  //   myMem.put(i, 0);
  // }
}

void loop()
{
}