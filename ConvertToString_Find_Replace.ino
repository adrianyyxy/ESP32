#include <string.h>
 
// converts character array
// to string and returns it

int found;

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
  char esp[] = {0x10, 0x01, 0x36, 0x31, 0x35, 0x30, 0x32, 0x31, 0x30, 0x30, 0x30, 0x30, 0x52, 0x52, 0x30, 0x31, 0x4C, 0x30, 0x31, 0x10, 0x03, 0x1A, 0x6D};
 
  int esp_size = sizeof(esp) / sizeof(char);
  
  String s_a;
 
  s_a = convertToString(esp, esp_size);

  Serial.println(s_a);

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
  
  delay(1000);

}
