#include <iostream>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <thread>



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

std::string calculateChunk(int lowerbound, int upperbound) {

  std::string output = "";

  int repetitions = (upperbound - lowerbound)*36^4;
  
  int buffer2,buffer3,buffer4,buffer5 = 0;
  int buffer1 = lowerbound;
  
  for(int i = 0; i < repetitions; i++) {

    std::string toHash = "";
    
    toHash += intToChar(buffer1) + "b1";
    toHash += intToChar(buffer2);
    toHash += intToChar(buffer3);
    toHash += intToChar(buffer4);
    toHash += intToChar(buffer5);

    output += toHash;
    output += " : ";
    output += sha256(toHash);
    output += "\n";

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
    
    if(buffer1 < upperbound){
      continue;}
    else{
      break;}

  }
  
  return output;
}

int main() {
  
  std::ofstream output;
  output.open("output.txt");
  
  output << calculateChunk(1,2);
  
  output.close();  

  std::cout  << "done" << std::endl;
  std::cout << sha256("zz");
  return 0;
}
