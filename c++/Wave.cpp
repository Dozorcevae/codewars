//wave("hello") => {"Hello", "hEllo", "heLlo", "helLo", "hellO"}


std::vector<std::string> wave(std::string y){
  std::vector<std::string> result;
  for(auto it = y.begin(); it != y.end(); ++it){
    if(isalpha(*it)){
      *it = toupper(*it);
      result.push_back(y);
      *it = tolower(*it);
    }
  }
  return result;
}v
