// 5 --> 1 + 1/4 + 1/7 + 1/10 + 1/13 --> "1.57" (округление с точностью 2 знака)

function SeriesSum(n) {
  let sum = 0;
  if(n == 0) return "0.00";
  
  for(let i = 0; i < n; i++){
    sum += 1/(1+3*i);
    }
  return sum.toFixed(2);
  }

// toFixed(2) выводит приблеженное до 2 знака (2.05, например)
