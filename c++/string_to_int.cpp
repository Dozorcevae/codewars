//You ask a small girl,"How old are you?" She always says, "x years old", where x is a random number between 0 and 9.

//Write a program that returns the girl's age (0-9) as an integer.

int get_age(const string& she_said) {
  return std::stoi(she_said);
}

//("5 years old"), Equals(5)
