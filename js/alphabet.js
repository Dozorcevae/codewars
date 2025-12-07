//Input = "The sunset sets at twelve o' clock."
//Output = "20 8 5 19 21 14 19 5 20 19 5 20 19 1 20 20 23 5 12 22 5 15 3 12 15 3 11"
// а = 1, b = 2 и так далее

function alphabetPosition(text) {
  text = text.toLowerCase();
  
  let res = [];
  
  for (let i = 0; i < text.length; i++){
    let char = text[i];
    
    if (char >= 'a' && char <= 'z'){
      res.push(char.charCodeAt(0) - 'a'.charCodeAt(0) + 1);
    }
  }
  return res.join(' ');
}
