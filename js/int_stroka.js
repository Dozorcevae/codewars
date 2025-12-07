//highAndLow("1 2 -3 4 5"); // return "5 -3"

function highAndLow(numbers){
  numbers = numbers.split(' ');
  return `${Math.max(...numbers)} ${Math.min(...numbers)}`;
}
