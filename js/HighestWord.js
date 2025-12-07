//дано предложение "Бла блабла блаа бла а"
//вернуть слово которое в суммем по ASKII самое большое, в примере это блабла

function high(x){
  
  const words = x.split(' ');
  let Highest = 0;
  let HighestWord = '';
  
  words.forEach(word => {
    let score = 0;
    for(let i = 0; i < word.length; i++){
      score += word.charCodeAt(i) - 96;
    }
    
    if(score > Highest){
      Highest = score;
      HighestWord = word;
    }
  });
  
  return HighestWord;
}
