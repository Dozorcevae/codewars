//39 --> 3 (because 3*9 = 27, 2*7 = 14, 1*4 = 4 and 4 has only one digit, there are 3 multiplications)


function persistence(num) {
  let cnt = 0;
  
  while(num.toString().length > 1){
    
    num = num.toString().split('').reduce((acc, dight) => acc*dight);
    cnt++;
  }
  return cnt;
}
