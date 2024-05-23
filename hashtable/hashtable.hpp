//
// Created by 董鸿博 on 2024/5/23.
//

#ifndef HASHTABLE_HASHTABLE_HPP
#define HASHTABLE_HASHTABLE_HPP
#include <iostream>
#include <string>
#include <utility>
#include <vector>
namespace open_address
{
    enum State
    {
        OCCUPY,
        EMPTY,
        DELETE,
    };

    template<typename K, typename V>
    struct HashData
    {
        std::pair<K, V> m_kv;
        State m_state;
        HashData()
            : m_state(EMPTY)
        {}
    };

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


    template<typename K, typename V, typename Hash = HashFunc<K>>
    class hashtable
    {
    public:
        hashtable();
        bool insert(const std::pair<K, V>& kv);
        bool erase(const K& key);
        HashData<K, V>* find(const K& key);

    private:
        std::vector<HashData<K, V>> m_table;
        size_t m_num;
    };

    template<typename K, typename V, typename Hash>
    hashtable<K, V, Hash>::hashtable()
        :m_num(0)
    {
       m_table.resize(10);
    }

    template<typename K, typename V, typename Hash>
    bool hashtable<K, V, Hash>::insert(const std::pair<K, V> &kv)
    {
        if(find(kv.first))
            return false;


        if(10 * m_num / m_table.size()  >= 7)
        {
            hashtable<K, V, Hash> new_hashtable;
            new_hashtable.m_table.resize(m_table.size() * 2);
            for(const auto& data : m_table)
            {
                if(data.m_state == OCCUPY)
                    new_hashtable.insert(data.m_kv);
            }
            m_table.swap(new_hashtable.m_table);
            m_num = new_hashtable.m_num;
        }

        Hash hash;
        size_t hashi = hash(kv.first) % m_table.size();
        while(m_table[hashi].m_state == OCCUPY)
        {
            hashi++;
            hashi %= m_table.size();
        }

        m_table[hashi].m_kv = kv;
        m_table[hashi].m_state = OCCUPY;
        m_num++;
        return true;


    }

    template<typename K, typename V, typename Hash>
    bool hashtable<K, V, Hash>::erase(const K &key)
    {
        auto ret = find(key);
        if(!ret)
            return false;

        ret->m_state = DELETE;
        m_num--;
        return true;
    }

    template<typename K, typename V, typename Hash>
    HashData<K,V>* hashtable<K, V, Hash>::find(const K& key)
    {
        Hash hash;
        size_t hashi = hash(key) % m_table.size();

        while(m_table[hashi].m_state != EMPTY)
        {
            if(m_table[hashi].m_state == OCCUPY && m_table[hashi].m_kv.first == key)
                return &m_table[hashi];
            hashi++;
            hashi %= m_table.size();
        }
        return nullptr;

    }

    void TestHT1()
    {
        int a[] = { 10001,11,55,24,19,12,31 };
        hashtable<int, int> ht;
        for (auto e : a)
        {
            ht.insert(std::make_pair(e, e));
        }

        std::cout << ht.find(55) << std::endl;
        std::cout << ht.find(31) << std::endl;

        ht.erase(55);
        std::cout << ht.find(55) << std::endl;
        std::cout << ht.find(31) << std::endl;
    }

    void TestHT2()
    {
        int a[] = { 10001,11,55,24,19,12,31 };
        hashtable<int, int> ht;
        for (auto e : a)
        {
            ht.insert(std::make_pair(e, e));
        }

        ht.insert(std::make_pair(32, 32));
        ht.insert(std::make_pair(32, 32));
    }

    struct Person
    {
        //std::string _id;

        std::string _name;
        int _age;
        std::string school;
    };


    // key不支持强转整形取模，那么就要自己提供转换成整形仿函数
    void TestHT3()
    {
        hashtable<Person, int> xxht;

        //hashtable<std::string, int, StringHashFunc> ht;
        hashtable<std::string, int> ht;
        ht.insert(std::make_pair("sort", 1));
        ht.insert(std::make_pair("left", 1));
        ht.insert(std::make_pair("insert", 1));

        std::cout << HashFunc<std::string>()("bacd") << std::endl;
        std::cout << HashFunc<std::string>()("abcd") << std::endl;
        std::cout << HashFunc<std::string>()("aadd") << std::endl;
    }




}

#endif //HASHTABLE_HASHTABLE_HPP
