/*
Given a non-empty array of integers, return the result of multiplying the values together in order. Example:

[1, 2, 3, 4] => 1 * 2 * 3 * 4 = 24 
Метод массива reduce() позволяет превратить массив в любое другое значение с помощью переданной функции-колбэка и начального значения. 
Функция-колбэк будет вызвана для каждого элемента массива, и всегда должна возвращать результат.
*/
const grow = x => x.reduce((a, b) => a * b);
