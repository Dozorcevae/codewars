#pragma once
#include <cstddef>
#include <vector>
#include <stdexcept>
#include <utility>
#include <functional>

/*
    BST<Key, Data> — двоичное дерево поиска.

    Требования варианта 8:
    - шаблонный класс (Key, Data)
    - рекурсивные: find/insert/erase
    - обход Lt -> Rt -> t (postorder)
    - predecessor(k): максимальный ключ < k
    - итератор (min -> max): *, ++, ==
    - show(): визуализация (право-узел-лево)
*/

template<typename Key, typename Data, typename Compare = std::less<Key>>
class BST {
private:
    struct Node {
        Key key;
        Data data;
        Node* left;
        Node* right;

        Node(const Key& k, const Data& d)
            : key(k), data(d), left(nullptr), right(nullptr) {}
    };

    Node* root_ = nullptr;
    std::size_t size_ = 0;
    Compare cmp_{};

private:
    // --- Сравнение полезности с помощью Compare ---
    bool less(const Key& a, const Key& b) const { return cmp_(a, b); }
    bool eq(const Key& a, const Key& b) const { return !less(a,b) && !less(b,a); }

    static void clearRec(Node* t) {
        if (!t) return;
        clearRec(t->left);
        clearRec(t->right);
        delete t;
    }

    Node* findRec(Node* t, const Key& k) const {
        if (!t) return nullptr;
        if (eq(k, t->key)) return t;
        if (less(k, t->key)) return findRec(t->left, k);
        return findRec(t->right, k);
    }

    Node* insertRec(Node* t, const Key& k, const Data& d, bool& inserted) {
        if (!t) {
            inserted = true;
            return new Node(k, d);
        }
        if (eq(k, t->key)) {
            // Политика: ключи уникальны, при повторной вставке обновляем data.
            t->data = d;
            inserted = false;
            return t;
        }
        if (less(k, t->key)) {
            t->left = insertRec(t->left, k, d, inserted);
        } else {
            t->right = insertRec(t->right, k, d, inserted);
        }
        return t;
    }

    static Node* minNode(Node* t) {
        while (t && t->left) t = t->left;
        return t;
    }

    Node* eraseRec(Node* t, const Key& k, bool& deleted) {
        if (!t) { deleted = false; return nullptr; }

        if (less(k, t->key)) {
            t->left = eraseRec(t->left, k, deleted);
            return t;
        }
        if (less(t->key, k)) {
            t->right = eraseRec(t->right, k, deleted);
            return t;
        }

        // нашли узел
        deleted = true;

        if (!t->left) {
            Node* r = t->right;
            delete t;
            return r;
        }
        if (!t->right) {
            Node* l = t->left;
            delete t;
            return l;
        }

        // два потомка: замена преемником (min в правом поддереве)
        Node* succ = minNode(t->right);
        t->key = succ->key;
        t->data = succ->data;

        bool dummy = false;
        t->right = eraseRec(t->right, succ->key, dummy);
        return t;
    }

    template<typename Fn>
    static void postorderRec(Node* t, Fn&& fn) {
        if (!t) return;
        postorderRec(t->left, fn);
        postorderRec(t->right, fn);
        fn(t);
    }

    bool predecessorRec(Node* t, const Key& k, Node*& best) const {
        if (!t) return best != nullptr;

        if (less(t->key, k)) {
            best = t;
            return predecessorRec(t->right, k, best);
        }
        return predecessorRec(t->left, k, best);
    }

    static void showRec(Node* t, int level, void(*emit)(const Key&, int)) {
        if (!t) return;
        showRec(t->right, level + 1, emit);
        emit(t->key, level);
        showRec(t->left, level + 1, emit);
    }

public:
    BST() = default;
    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    ~BST() { clear(); }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    void clear() {
        clearRec(root_);
        root_ = nullptr;
        size_ = 0;
    }

    // find: true/false и data через out
    bool find(const Key& k, Data& outData) const {
        Node* t = findRec(root_, k);
        if (!t) return false;
        outData = t->data;
        return true;
    }

    // insert: true если новый узел, false если обновили существующий
    bool insert(const Key& k, const Data& d) {
        bool inserted = false;
        root_ = insertRec(root_, k, d, inserted);
        if (inserted) ++size_;
        return inserted;
    }

    // erase: true если удалили, false если ключ не найден
    bool erase(const Key& k) {
        bool deleted = false;
        root_ = eraseRec(root_, k, deleted);
        if (deleted) --size_;
        return deleted;
    }

    // Постфиксный обход Lt->Rt->t: применить fn к каждому узлу
    template<typename Fn>
    void traverse_postorder(Fn&& fn) const {
        postorderRec(root_, [&](Node* t){ fn(t->key, t->data); });
    }

    // predecessor: max key < k
    bool predecessor(const Key& k, Key& outKey, Data& outData) const {
        Node* best = nullptr;
        bool ok = predecessorRec(root_, k, best);
        if (!ok) return false;
        outKey = best->key;
        outData = best->data;
        return true;
    }

    // show: отдаём callback, чтобы внешне решать как печатать
    // emit(key, level) — level используется для отступов
    void show(void(*emit)(const Key&, int)) const {
        showRec(root_, 0, emit);
    }

    // ---- Iterator: inorder min -> max ----
    class Iterator {
        const BST* tree_ = nullptr;
        Node* cur_ = nullptr;
        std::vector<Node*> stack_;

        void pushLeft(Node* t) {
            while (t) {
                stack_.push_back(t);
                t = t->left;
            }
        }

    public:
        Iterator() = default;

        Iterator(const BST* tree, bool makeBegin) : tree_(tree) {
            if (!tree_) return;
            if (makeBegin) {
                pushLeft(tree_->root_);
                cur_ = stack_.empty() ? nullptr : stack_.back();
            } else {
                cur_ = nullptr;
            }
        }

        // * — доступ к данным текущего узла
        Data& operator*() const {
            if (!cur_) throw std::logic_error("Iterator dereference on end()");
            return cur_->data;
        }

        // Доп: доступ к ключу (для тестов/печати)
        const Key& key() const {
            if (!cur_) throw std::logic_error("Iterator key() on end()");
            return cur_->key;
        }

        // ++ — следующий по ключу
        Iterator& operator++() {
            if (!cur_) return *this;

            Node* t = stack_.back();
            stack_.pop_back();

            if (t->right) pushLeft(t->right);

            cur_ = stack_.empty() ? nullptr : stack_.back();
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return tree_ == other.tree_ && cur_ == other.cur_;
        }
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    Iterator begin() const { return Iterator(this, true); }
    Iterator end() const { return Iterator(this, false); }
};
