//Вам будет предоставлен массив чисел. Вы должны отсортировать нечетные числа в порядке возрастания, оставляя четные числа в их исходных позициях.
#include <vector>
#include <algorithm>
#include <iostream>

class Kata
{
public:
    std::vector<int> sortArray(std::vector<int> array)
    {
        // отделяем нечетные числа
        std::vector<int> odd_nums;
        for (int num : array) {
            if (num % 2 != 0) {
                odd_nums.push_back(num);
            }
        }

        // сортировОчка
        std::sort(odd_nums.begin(), odd_nums.end());

        // добавляем нечетные числа не трогая позиции четных
        int odd_index = 0;
        for (int i = 0; i < array.size(); ++i) {
            if (array[i] % 2 != 0) {
                array[i] = odd_nums[odd_index];
                odd_index++;
            }
        }

        return array;
    }
};
