//9119 through the function, 811181 (81-1-1-81)

function squareDigits(num){
  return +num.toString().split('').map(n => n*n).join('');
}
