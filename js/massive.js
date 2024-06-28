// Example : n=5 --> [5,4,3,2,1]
const reverseSeq = n => {
  var ResArr = [];
  var ints = n;
  for(let i = 0; i < n; i++){
    ResArr[i] = ints;
    ints --;
  }
  return ResArr;
};
