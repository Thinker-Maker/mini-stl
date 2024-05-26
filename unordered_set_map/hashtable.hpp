//
// Created by 董鸿博 on 2024/5/23.
//

#ifndef HASHTABLE_HASHTABLE_HPP
#define HASHTABLE_HASHTABLE_HPP
#include <iostream>
#include <string>
#include <utility>
#include <vector>

template<typename T>
struct HashFunc
{
    size_t operator()(const T& data)
    {
        return (size_t)data;
    }
};

template<>
struct HashFunc<std::string>
{
    size_t operator()(const std::string& str)
    {
        size_t hash = 0;
        for (auto ch : str)
        {
            hash *= 131;
            hash += ch;
        }
        return hash;
    }
};

namespace dhb
{
    template<typename T>
    struct HashNode
    {
        T m_data;
        HashNode* m_next;
        HashNode(const T& data)
            : m_data(data),
              m_next(nullptr)
        {}

    };

    template<typename K, typename T, typename KeyOfT, typename Hash>
    class hashtable;

    template<typename T, typename Ptr, typename Ref, typename K, typename KeyOfT, typename Hash>
    struct hashtable_iterator
    {
        typedef HashNode<T> Node;
        typedef hashtable_iterator<T, Ptr, Ref, K, KeyOfT, Hash> Self;
        typedef hashtable_iterator<T, T*, T&, K, KeyOfT, Hash> iterator;

        //points to const object
        const hashtable<K, T, KeyOfT, Hash>* m_pht;
        Node* m_node;

        hashtable_iterator(Node* node, const hashtable<K, T, KeyOfT, Hash>* pht)
            : m_node(node),
              m_pht(pht)
        {}

        hashtable_iterator(const iterator& it)
            : m_node(it.m_node),
              m_pht(it.m_pht)
        {}

        Self& operator++(){
            if(m_node->m_next){
                m_node = m_node->m_next;
            }
            else{
                KeyOfT kot;
                Hash hash;
                size_t hashi = hash(kot(m_node->m_data)) % (m_pht->m_table.size());
                hashi++;
                for(; hashi < m_pht->m_table.size(); hashi++)
                    if(m_pht->m_table[hashi])
                        break;
                if(hashi == m_pht->m_table.size())
                    m_node = nullptr;
                else
                    m_node = m_pht->m_table[hashi];
            }
            return *this;
        }

        bool operator!=(const Self& it){
            return m_node != it.m_node;
        }

        Ref operator*(){
            return m_node->m_data;
        }

        Ptr operator->(){
            return &m_node->m_data;
        }


    };

    template<typename K, typename T, typename KeyOfT, typename Hash>
    class hashtable
    {
        typedef HashNode<T> Node;
        friend struct hashtable_iterator<T, T*, T&, K, KeyOfT, Hash>;
        friend struct hashtable_iterator<T, const T*, const T&, K, KeyOfT, Hash>;
    public:
        typedef hashtable_iterator<T, T*, T&, K, KeyOfT, Hash> iterator;
        typedef hashtable_iterator<T, const T*, const T&, K, KeyOfT, Hash> const_iterator;

        iterator begin(){
            for(size_t i = 0; i < m_table.size(); i++)
                if(m_table[i])
                    return iterator(m_table[i], this);

            return end();
        }
        const_iterator begin()const{
           for(size_t i = 0; i < m_table.size(); i++)
               if(m_table[i])
                   return const_iterator(m_table[i], this);
            return end();
        }
        iterator end(){
            return iterator(nullptr, this);
        }
        const_iterator end()const{
            return const_iterator (nullptr, this);
        }

        hashtable();
        ~hashtable();
        std::pair<iterator, bool> insert(const T& data);
        iterator find(const K& key);
        bool erase(const K& key);
    private:
        std::vector<Node*>  m_table;
        size_t m_num;
    };

    template<typename K, typename T, typename KeyOfT, typename Hash>
    hashtable<K, T, KeyOfT, Hash>::hashtable()
    {
        m_table.resize(10, nullptr);
        m_num = 0;
    }

    template<typename K, typename T, typename KeyOfT, typename Hash>
    hashtable<K, T, KeyOfT, Hash>::~hashtable()
    {
        for(size_t i = 0; i < m_table.size(); i++){
            Node* cur = m_table[i];
            while(cur){
                Node* next = cur->m_next;
                delete cur;
                cur = next;
            }
            m_table[i] = nullptr;
        }
    }


    template<typename K, typename T, typename KeyOfT, typename Hash>
    std::pair<typename hashtable<K, T, KeyOfT, Hash>::iterator, bool> hashtable<K, T, KeyOfT, Hash>::insert(const T& data)
    {
        KeyOfT kot;
        auto ret = find(kot(data));
        if(ret != end())
            return { ret , false};

        Hash hash;
        if(m_num == m_table.size()){
            std::vector<Node*> new_table(m_table.size() * 2, nullptr);
            for(size_t i = 0; i < m_table.size(); i++){
                Node* cur = m_table[i];
                while(cur){
                    Node* next = cur->m_next;
                    size_t hashi = hash(kot(cur->m_data)) % new_table.size();
                    cur->m_next = new_table[hashi];
                    new_table[hashi] = cur;
                    cur = next;
                }
                m_table[i] = nullptr;
            }
            m_table.swap(new_table);
        }
        size_t hashi = hash(kot(data)) % m_table.size();
        Node* cur = new Node(data);
        cur->m_next = m_table[hashi];
        m_table[hashi] = cur;
        m_num++;

        return {iterator(cur, this), true};
    }
    template<typename K, typename T, typename KeyOfT, typename Hash>
    bool hashtable<K, T, KeyOfT, Hash>::erase(const K& key)
    {

        KeyOfT kot;
        Hash hash;
        size_t hashi = hash(key) % m_table.size();
        Node* cur = m_table[hashi];
        Node* prev = nullptr;
        while(cur){
            if(kot(cur->m_data) == key){
                if(prev == nullptr){
                    m_table[hashi] = nullptr;
                }
                else{
                    prev->m_next = cur->m_next;
                }
                delete cur;
                m_num--;
                return true;
            }
            else{
                prev = cur;
                cur = cur->m_next;
            }
        }
        return false;
    }

    template<typename K, typename T, typename KeyOfT, typename Hash>
    typename hashtable<K, T, KeyOfT, Hash>::iterator hashtable<K, T, KeyOfT, Hash>::find(const K& key)
    {
        KeyOfT kot;
        Hash hash;
        size_t hashi = hash(key) % m_table.size();
        Node* cur = m_table[hashi];
        while(cur)
        {
            if(kot(cur->m_data) == key)
                return iterator(cur, this);
            cur = cur->m_next;
        }
        return iterator(nullptr, this);
    }

}

#endif //HASHTABLE_HASHTABLE_HPP
