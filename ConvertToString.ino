#include <string.h>
 
// converts character array
// to string and returns it
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
  char a[] = { 'C', 'O', 'D', 'E' };
 
  int a_size = sizeof(a) / sizeof(char);
 
  String s_a = convertToString(a, a_size);

  Serial.println(s_a);
  delay(500);

}
