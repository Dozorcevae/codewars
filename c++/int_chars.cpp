//Если символ в слове встречается 1 раз => '('; >=2 раз => ')'

#include <string>
std::string duplicate_encoder(const std::string& word){
  std::map<char, int> char_cnt;
  std::string lower_word = word;
  std::string res = "";
  
  for(char c: lower_word){
    c = std::tolower(c);
    char_cnt[c]++;
  }
  for(char c: lower_word){
    c = std::tolower(c);
    if(char_cnt[c] > 1){
      res += ')';
    }
    else{
      res += '(';
    }
  }
  return res;
}v
