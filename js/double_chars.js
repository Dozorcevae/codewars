function doubleChar(str) {
  return str.split('').map(char => char + char).join('');
}
/*map(char => char + char): Применяет функцию ко всем элементам массива, 
удваивая каждый символ. Например, ['S', 't', 'r', 'i', 'n', 'g'].map(char => char + char)
превращается в ['SS', 'tt', 'rr', 'ii', 'nn', 'gg']*/
