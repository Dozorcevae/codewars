#pragma once

#include <cstdint>
#include <cstddef>
#include <cmath>
#include <optional>
#include <string>
#include <vector>
#include <ostream>
#include <random>

// Коды ошибок (в стиле МУ/примеров, где ловят catch(int))
#define E_INVALID_KEY   101
#define E_NOT_FOUND     102
#define E_TABLE_FULL    103

// -------------------- UTF-8 -> UTF-32 (минимально) --------------------
// Нужно, чтобы корректно обработать заглавные буквы кириллицы при вводе как UTF-8.

inline std::u32string utf8_to_u32(const std::string& s)
{
    std::u32string out;
    out.reserve(s.size());

    size_t i = 0;
    while (i < s.size())
    {
        unsigned char c = static_cast<unsigned char>(s[i]);
        char32_t cp = 0;
        size_t len = 0;

        if ((c & 0x80) == 0) { cp = c; len = 1; }
        else if ((c & 0xE0) == 0xC0) { cp = c & 0x1F; len = 2; }
        else if ((c & 0xF0) == 0xE0) { cp = c & 0x0F; len = 3; }
        else if ((c & 0xF8) == 0xF0) { cp = c & 0x07; len = 4; }
        else {
            // битая строка
            throw E_INVALID_KEY;
        }

        if (i + len > s.size()) throw E_INVALID_KEY;

        for (size_t j = 1; j < len; ++j)
        {
            unsigned char cc = static_cast<unsigned char>(s[i + j]);
            if ((cc & 0xC0) != 0x80) throw E_INVALID_KEY;
            cp = (cp << 6) | (cc & 0x3F);
        }

        out.push_back(cp);
        i += len;
    }

    return out;
}

inline std::string u32_to_utf8(const std::u32string& s)
{
    std::string out;
    for (char32_t cp : s)
    {
        if (cp <= 0x7F)
        {
            out.push_back(static_cast<char>(cp));
        }
        else if (cp <= 0x7FF)
        {
            out.push_back(static_cast<char>(0xC0 | ((cp >> 6) & 0x1F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        else if (cp <= 0xFFFF)
        {
            out.push_back(static_cast<char>(0xE0 | ((cp >> 12) & 0x0F)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        else
        {
            out.push_back(static_cast<char>(0xF0 | ((cp >> 18) & 0x07)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
    }
    return out;
}

// -------------------- Вариант 8: k -> k' по схеме Горнера --------------------
// k: строка из ЗАГЛАВНЫХ букв кириллицы.
// k': натуральное значение, формируемое по правилу Горнера в основании |алфавит|.

struct CyrillicUpperHorner
{
    // Русский алфавит (33 буквы) в верхнем регистре: А..Я + Ё.
    // Порядок: АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ
    static inline const std::u32string kAlphabet = U"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

    static uint32_t digit(char32_t cp)
    {
        // Возвращаем 1..33, если символ из алфавита, иначе бросаем ошибку.
        for (size_t i = 0; i < kAlphabet.size(); ++i)
        {
            if (kAlphabet[i] == cp) return static_cast<uint32_t>(i + 1);
        }
        throw E_INVALID_KEY;
    }

    static uint64_t to_nat(const std::string& key_utf8)
    {
        const std::u32string u = utf8_to_u32(key_utf8);
        if (u.empty()) throw E_INVALID_KEY;

        constexpr uint64_t base = 33ULL; // размер алфавита
        uint64_t acc = 0;
        for (char32_t cp : u)
        {
            const uint64_t d = static_cast<uint64_t>(digit(cp));
            // Горнер: acc = acc * base + d
            acc = acc * base + d; // переполнение допустимо (по модулю 2^64)
        }
        return acc;
    }

    // Генерация случайного ключа для экспериментов (качество χ² и т.п.)
    static std::string gen_key(std::mt19937_64& rng, size_t min_len = 1, size_t max_len = 12)
    {
        std::uniform_int_distribution<size_t> len_dist(min_len, max_len);
        std::uniform_int_distribution<size_t> ch_dist(0, kAlphabet.size() - 1);

        const size_t len = len_dist(rng);
        std::u32string u;
        u.reserve(len);
        for (size_t i = 0; i < len; ++i)
        {
            u.push_back(kAlphabet[ch_dist(rng)]);
        }
        return u32_to_utf8(u);
    }
};

// -------------------- Хеш-таблица (открытая адресация, линейное зондирование) --------------------

template <class Key, class T, class KeyToNat>
class HashTableOA
{
private:
    enum class State : uint8_t { Free, Busy, Deleted };

    struct Cell
    {
        State state = State::Free;
        std::optional<Key> key;
        std::optional<T> value;
    };

    std::vector<Cell> table_;
    size_t size_ = 0;

    // метрики "текущей операции"
    size_t last_base_hash_ = 0; // h(k)
    size_t last_index_ = 0;     // индекс, на котором завершилась операция (нашли/вставили/остановились)
    size_t last_probes_ = 0;    // число проб (зондирований)

    // Константа для мультипликативного хеширования (A ~ (sqrt(5)-1)/2),
    // представлена в фиксированной точке: floor(A * 2^64).
    // Известна как 0x9E3779B97F4A7C15.
    static constexpr uint64_t kMul = 11400714819323198485ULL;

private:
    void reset_op_metrics()
    {
        last_base_hash_ = 0;
        last_index_ = 0;
        last_probes_ = 0;
    }

    size_t base_hash(const Key& key) const
    {
        const uint64_t k_nat = KeyToNat::to_nat(key);
        // frac(k*A) ~= (k * floor(A*2^64) mod 2^64) / 2^64
        const uint64_t x = k_nat * kMul; // переполнение = mod 2^64
        // h = floor(m * (x / 2^64)) == floor((m*x) / 2^64)
        const __uint128_t prod = static_cast<__uint128_t>(x) * static_cast<__uint128_t>(table_.size());
        return static_cast<size_t>(prod >> 64);
    }

    size_t probe_hash(size_t h0, size_t i) const
    {
        // линейное зондирование: h(k,i) = (h(k) + i) mod m
        return (h0 + i) % table_.size();
    }

public:
    // Конструктор: max_items — предельное число элементов.
    // Для открытой адресации рекомендуют α=0.5, значит m = ceil(max_items / 0.5) = 2*max_items.
    explicit HashTableOA(size_t max_items)
    {
        if (max_items == 0) max_items = 1;
        const size_t m = max_items * 2; // α=0.5
        table_.assign(m, Cell{});
    }

    size_t Size() const { return size_; }
    size_t Capacity() const { return table_.size(); }
    bool Empty() const { return size_ == 0; }

    void Clear()
    {
        for (auto& c : table_)
        {
            c.state = State::Free;
            c.key.reset();
            c.value.reset();
        }
        size_ = 0;
        reset_op_metrics();
    }

    // Метрики текущей операции
    size_t LastHash() const { return last_base_hash_; }
    size_t LastIndex() const { return last_index_; }
    size_t CountProbe() const { return last_probes_; }

    // Вставка (FALSE если ключ уже есть или таблица переполнена)
    bool Insert(const Key& key, const T& value)
    {
        reset_op_metrics();
        const size_t m = table_.size();
        const size_t h0 = base_hash(key);
        last_base_hash_ = h0;

        size_t pos = static_cast<size_t>(-1);
        for (size_t i = 0; i < m; ++i)
        {
            const size_t j = probe_hash(h0, i);
            last_index_ = j;
            last_probes_ = i + 1;

            Cell& c = table_[j];
            if (c.state == State::Busy)
            {
                if (c.key.has_value() && c.key.value() == key)
                {
                    return false; // дубликат
                }
            }
            else if (c.state == State::Deleted)
            {
                if (pos == static_cast<size_t>(-1)) pos = j;
            }
            else // Free
            {
                if (pos == static_cast<size_t>(-1)) pos = j;
                break; // как в псевдокоде: до free
            }
        }

        if (pos == static_cast<size_t>(-1))
        {
            // таблица полностью занята (busy), вставлять некуда
            return false;
        }

        // вставляем в pos
        Cell& dst = table_[pos];
        dst.state = State::Busy;
        dst.key = key;
        dst.value = value;
        size_++;

        last_index_ = pos; // индекс фактической вставки
        return true;
    }

    // Удаление (TRUE если удалили)
    bool Delete(const Key& key)
    {
        reset_op_metrics();
        const size_t m = table_.size();
        const size_t h0 = base_hash(key);
        last_base_hash_ = h0;

        for (size_t i = 0; i < m; ++i)
        {
            const size_t j = probe_hash(h0, i);
            last_index_ = j;
            last_probes_ = i + 1;

            Cell& c = table_[j];
            if (c.state == State::Busy)
            {
                if (c.key.has_value() && c.key.value() == key)
                {
                    c.state = State::Deleted;
                    c.key.reset();
                    c.value.reset();
                    size_--;
                    return true;
                }
            }
            else if (c.state == State::Free)
            {
                // stop на free
                return false;
            }
        }
        return false;
    }

    // Поиск: возвращает ссылку на значение, иначе бросает E_NOT_FOUND
    T& Search(const Key& key)
    {
        reset_op_metrics();
        const size_t m = table_.size();
        const size_t h0 = base_hash(key);
        last_base_hash_ = h0;

        for (size_t i = 0; i < m; ++i)
        {
            const size_t j = probe_hash(h0, i);
            last_index_ = j;
            last_probes_ = i + 1;

            Cell& c = table_[j];
            if (c.state == State::Busy)
            {
                if (c.key.has_value() && c.key.value() == key)
                {
                    return *c.value;
                }
            }
            else if (c.state == State::Free)
            {
                break;
            }
        }
        throw E_NOT_FOUND;
    }

    const T& Search(const Key& key) const
    {
        return const_cast<HashTableOA*>(this)->Search(key);
    }

    // Вывод структуры таблицы: индекс, состояние (f/b/d), ключ, значение
    void Show(std::ostream& os) const
    {
        os << "Index\tState\tKey\tValue\n";
        for (size_t i = 0; i < table_.size(); ++i)
        {
            const Cell& c = table_[i];
            char st = '?';
            switch (c.state)
            {
                case State::Free: st = 'f'; break;
                case State::Busy: st = 'b'; break;
                case State::Deleted: st = 'd'; break;
            }
            os << i << "\t" << st;
            if (c.state == State::Busy)
            {
                os << "\t" << *c.key << "\t" << *c.value;
            }
            os << "\n";
        }
    }

    // -------------------- Итератор (ctor, *, ++, ==) --------------------
    class Iterator
    {
    private:
        HashTableOA* ht_ = nullptr;
        size_t idx_ = 0;

        void skip_to_busy()
        {
            if (!ht_) return;
            while (idx_ < ht_->table_.size())
            {
                if (ht_->table_[idx_].state == State::Busy) return;
                ++idx_;
            }
        }

    public:
        Iterator(HashTableOA* ht, size_t idx) : ht_(ht), idx_(idx)
        {
            skip_to_busy();
        }

        Iterator& operator++()
        {
            ++idx_;
            skip_to_busy();
            return *this;
        }

        bool operator==(const Iterator& other) const
        {
            return ht_ == other.ht_ && idx_ == other.idx_;
        }

        bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }

        // Возвращаем копию пары (ключ, значение)
        std::pair<Key, T> operator*() const
        {
            const Cell& c = ht_->table_.at(idx_);
            return { *c.key, *c.value };
        }
    };

    Iterator Begin() { return Iterator(this, 0); }
    Iterator End() { return Iterator(this, table_.size()); }

    // Служебно: посчитать индекс h(k) без выполнения операции (для χ²)
    size_t HashIndexForKey(const Key& key) const
    {
        return base_hash(key);
    }
};
