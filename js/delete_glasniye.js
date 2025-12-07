//удалить все гласные из строк

function disemvowel(str) {
  return str.replace(/[aeoiuAEOIU]/g, '');
}

//replace - замена букв из набора [aeoiuAEOIU] на '' пустую строку
// g - глобальный поиск, а не до первого совпадения
