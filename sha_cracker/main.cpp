#include <iostream>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <fstream>

std::string sha256(const std::string& str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), hash);

    std::ostringstream oss;
    for (unsigned char byte : hash) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return oss.str();
}

char intToChar(int i) {

  if(i<26){
    return static_cast<char>('a'+i);
  }
  else {
    return static_cast<char>((i-26)+'0');
  }

}

int main() {

  std::ofstream output;
  output.open("output.txt");

  int buffer1,buffer2,buffer3,buffer4,buffer5 = 0;

  for(int i = 0; i < 36^5; i++) {

    std::string toHash = "";

    toHash += intToChar(buffer1);
    toHash += intToChar(buffer2);
    toHash += intToChar(buffer3);
    toHash += intToChar(buffer4);
    toHash += intToChar(buffer5);

    output << toHash << " : " << sha256(toHash) << std::endl;

    buffer5++;


    if(buffer5 < 36){
      continue;}
    else{
      buffer5 = 0;
      buffer4++;}

    if(buffer4 < 36){
      continue;}
    else{
      buffer4 = 0;
      buffer3++;}

    if(buffer3 < 36){
      continue;}
    else{
      buffer3 = 0;
      buffer2++;}

    if(buffer2 < 36){
      continue;}
    else{
      buffer2 = 0;
      buffer1++;}

    if(buffer1 < 36){
      continue;}
    else{
      break;}

  }


  output.close();

  std::cout  << "done" << std::endl;
  std::cout << sha256("zz");
  return 0;
}

