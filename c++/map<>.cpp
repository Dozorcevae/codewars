/*
The main idea is to count all the occurring characters in a string. If you have a string like aba, then the result should be {'a': 2, 'b': 1}.

What if the string is empty? Then the result should be empty object literal, {}.

("aba"), Equals(m{{ 'a', 2 }, { 'b', 1 }}
*/

#include <map>
#include <string>

std::map<char, unsigned> count(const std::string& string) {
  std::map<char, unsigned> answer;
  for(char a: string){
    answer[a]++;
  }
  
    return answer;
}
