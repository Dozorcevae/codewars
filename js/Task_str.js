//Task: 'world'  =>  'dlrow'

function solution(str){
  
  //let - локальная переменная
  let CharArr = str.split(''); //split  - разделяет строку на массив символов "World" = ['w', 'o', 'r', 'l', 'd']
  let ReversedArray = CharArr.reverse(); //реверс - понятно что делает с массивом
  let ResStr = ReversedArray.join(''); //объединение назад в строку
  
  return ResStr;
}

//Другое решение:
function solution(str){
  return str.split('').reverse().join('');  
}
//И еще
const solution = str => str.split('').reverse().join('');м
