#include <string.h>
 
// converts character array
// to string and returns it

int found;

String addComma(String dataS){
  String originals[] = {"01234","56789","LA","HO","VES0","2410"};
  String replaces[] = {"98765","43210", "HO", "LA", "BESO", "4820,,,"};
  
  Serial.println("Start");
  for(int k = 0; k < (sizeof(originals) / sizeof(originals[0])); k++){
    dataS.replace(originals[k], replaces[k]);
  }
  Serial.println("End");

  return dataS;
}

String convertToString(char* a, int size)
{
    int i;
    String s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

void setup() {
  // put your setup code here, to run once
  Serial.begin(115200); //Computer Serial
}

void loop() {
  // put your main code here, to run repeatedly:
  //char esp[] = {0x10, 0x01, 0x36, 0x31, 0x35, 0x30, 0x32, 0x31, 0x30, 0x30, 0x30, 0x30, 0x52, 0x52, 0x30, 0x31, 0x4C, 0x30, 0x31, 0x10, 0x03, 0x1A, 0x6D};
  char esp[] = {'0','1','2','3','4','5','6','7','8','9','L','A','H','O','V','E','S','0','2','4','1','0'};
 
  int esp_size = sizeof(esp) / sizeof(char);
  
  String s_a;
 
  s_a = convertToString(esp, esp_size);
  Serial.println(s_a);
/*
  String s_b;

  s_b = addComma(s_a);

  Serial.println(s_b);
*/
  
  //Serial.println(s_a);

  found=s_a.indexOf("RR0");

  if (found!=-1){
    Serial.print("it was found at: ");
    Serial.print(found);
    Serial.println("");  
  }

  String s_b;

  s_b = s_a;

  s_b.replace("RR0","Hello");
  Serial.println(s_b);

  
  
  delay(5000);

}
