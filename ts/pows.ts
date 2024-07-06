//n = 0
// answ = [2^0, 2^1, ... , 2^n]

export function powersOfTwo(n:number):number[]{
  let res :number[] = []; 
  for(let i = 0; i <= n ; i++){
    res.push(Math.pow(2, i));
  }
  return res;
}
