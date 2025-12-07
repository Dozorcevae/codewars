//filter_list([1,2,'a','b']) == [1,2]

function filter_list(l) {
  // Используем метод filter для фильтрации элементов
  return l.filter(item => typeof item === 'number');
}
//другие примеры искользования: 


//Фильтрация чётных чисел: 
const numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
const evenNumbers = numbers.filter(num => num % 2 === 0);
console.log(evenNumbers); // Вывод: [2, 4, 6, 8, 10]


//Фильтрация элементов, присутствующих в другом массиве:
const arr1 = [1, 2, 3, 4, 5];
const arr2 = [2, 4];
const filteredArr = arr1.filter(num => arr2.includes(num));
console.log(filteredArr); // Вывод: [2, 4]

//имя = слово из 4-х символов:
function friend(friends){
  return friends.filter(name => name.length == 4);
}
