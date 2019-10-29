#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

// bWF0ZXVzZW5nX2VjQGFsdS51ZmMuYnI=
// bloyT0pyM21MZEdF

using namespace std;

string mail_username="mateuseng_ec@alu.ufc.br";
string mail_password="nZ2OJr3mLdGE";

char* buf = (char*)malloc(100);

const char* _b64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
 
void a3_to_a4(unsigned char * a4, unsigned char * a3) {
  a4[0] = (a3[0] & 0xfc) >> 2;
  a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
  a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
  a4[3] = (a3[2] & 0x3f);
}
 
int base64_encode(char *output, const char *input, int inputLen) {
  int i = 0, j = 0;
  int encLen = 0;
  unsigned char a3[3];
  unsigned char a4[4];
  while(inputLen--) {
    a3[i++] = *(input++);
    if(i == 3) {
      a3_to_a4(a4, a3);
      for(i = 0; i < 4; i++) {
        output[encLen++] = _b64_alphabet[a4[i]];
      }
      i = 0;
    }
  }
  if(i) {
    for(j = i; j < 3; j++) {
      a3[j] = '\0';
    }
    a3_to_a4(a4, a3);
    for(j = 0; j < i + 1; j++) {
      output[encLen++] = _b64_alphabet[a4[j]];
    }
    while((i++ < 3)) {
      output[encLen++] = '=';
    }
  }
  output[encLen] = '\0';
  return encLen;
}
 
int main(){
 
  base64_encode(buf, mail_username.c_str(), mail_username.length());
  cout << buf << endl;
  base64_encode(buf, mail_password.c_str(), mail_password.length());
  cout << buf << endl;

  free(buf); 

  return 0;

}