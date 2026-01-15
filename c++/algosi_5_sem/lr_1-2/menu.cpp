#include "hash_table_oa.h"
#include <iostream>
#include <sstream>
#include <clocale>

using std::cin;
using std::cout;
using std::endl;
using std::string;

using Value = string; // можно заменить на int/struct при необходимости

static void print_menu()
{
    cout << "\nАТД \"Хеш-таблица (открытая адресация, линейное зондирование)\"\n";
    cout << "1  - Размер (число элементов)\n";
    cout << "2  - Емкость (m)\n";
    cout << "3  - Пустая ли таблица\n";
    cout << "4  - Очистить таблицу\n";
    cout << "5  - Вставка (ключ, значение)\n";
    cout << "6  - Удаление (ключ)\n";
    cout << "7  - Поиск (ключ)\n";
    cout << "8  - Показать таблицу (f/b/d)\n";
    cout << "9  - Показать метрики последней операции (h, index, probes)\n";
    cout << "10 - Обход итератором\n";
    cout << "99 - Выход\n";
}

static string read_key()
{
    cout << "Ключ (ЗАГЛАВНЫЕ кириллицы): ";
    string k;
    cin >> k;
    return k;
}

static Value read_value()
{
    cout << "Значение: ";
    Value v;
    cin >> v;
    return v;
}

int main()
{
    std::setlocale(LC_ALL, "");

    // Чтобы емкость была небольшой (до 10), берем max_items=5 => m=10 (alpha=0.5)
    const size_t max_items = 5;
    HashTableOA<string, Value, CyrillicUpperHorner> ht(max_items);

    print_menu();

    while (true)
    {
        cout << "\n>> ";
        int cmd = 0;
        if (!(cin >> cmd)) return 0;

        std::ostringstream out;

        try
        {
            switch (cmd)
            {
                case 1:
                    out << "Size = " << ht.Size() << endl;
                    break;
                case 2:
                    out << "Capacity(m) = " << ht.Capacity() << endl;
                    break;
                case 3:
                    out << (ht.Empty() ? "Таблица пустая" : "Таблица НЕ пустая") << endl;
                    break;
                case 4:
                    ht.Clear();
                    out << "Очищено" << endl;
                    break;
                case 5:
                {
                    const string k = read_key();
                    const Value v = read_value();
                    const bool ok = ht.Insert(k, v);
                    out << (ok ? "Вставка: OK" : "Вставка: FALSE (дубликат или нет места)") << endl;
                    out << "index=" << ht.LastIndex() << ", probes=" << ht.CountProbe() << ", h=" << ht.LastHash() << endl;
                    break;
                }
                case 6:
                {
                    const string k = read_key();
                    const bool ok = ht.Delete(k);
                    out << (ok ? "Удаление: OK" : "Удаление: FALSE (не найдено)") << endl;
                    out << "index=" << ht.LastIndex() << ", probes=" << ht.CountProbe() << ", h=" << ht.LastHash() << endl;
                    break;
                }
                case 7:
                {
                    const string k = read_key();
                    Value& v = ht.Search(k);
                    out << "Найдено значение: " << v << endl;
                    out << "index=" << ht.LastIndex() << ", probes=" << ht.CountProbe() << ", h=" << ht.LastHash() << endl;
                    break;
                }
                case 8:
                    ht.Show(out);
                    break;
                case 9:
                    out << "h=" << ht.LastHash() << ", index=" << ht.LastIndex() << ", probes=" << ht.CountProbe() << endl;
                    break;
                case 10:
                {
                    out << "Элементы (обход итератором):\n";
                    for (auto it = ht.Begin(); it != ht.End(); ++it)
                    {
                        auto kv = *it;
                        out << kv.first << " -> " << kv.second << endl;
                    }
                    break;
                }
                case 99:
                    return 0;
                default:
                    out << "Неизвестная команда" << endl;
                    break;
            }
        }
        catch (int code)
        {
            out << "Ошибка (код " << code << "): ";
            switch (code)
            {
                case E_INVALID_KEY: out << "Некорректный ключ (нужны заглавные кириллические буквы)"; break;
                case E_NOT_FOUND: out << "Ключ не найден"; break;
                case E_TABLE_FULL: out << "Таблица переполнена"; break;
                default: out << "Неизвестная ошибка"; break;
            }
            out << endl;
            out << "h=" << ht.LastHash() << ", index=" << ht.LastIndex() << ", probes=" << ht.CountProbe() << endl;
        }

        cout << out.str();
        print_menu();
    }
}
