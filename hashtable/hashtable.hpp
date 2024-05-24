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

namespace hash_bucket
{
    template<typename K, typename V>
    struct HashNode
    {
        std::pair<K, V> m_kv;
        HashNode* m_next;
        HashNode(const std::pair<K, V>& kv)
            : m_kv(kv),
              m_next(nullptr)
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
        typedef HashNode<K, V> Node;
    public:
        hashtable();
        ~hashtable();
        bool insert(const std::pair<K, V>& kv);
        Node* find(const K& key);
        bool erase(const K& key);
    private:
        std::vector<Node*>  m_table;
        size_t m_num;
    };

    template<typename K, typename V, typename Hash>
    hashtable<K, V, Hash>::hashtable()
    {
        m_table.resize(10, nullptr);
        m_num = 0;
    }

    template<typename K, typename V, typename Hash>
    hashtable<K, V, Hash>::~hashtable()
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


    template<typename K, typename V, typename Hash>
    bool hashtable<K, V, Hash>::insert(const std::pair<K, V>& kv)
    {
        if(find(kv.first))
            return false;

        Hash hash;
        if(m_num == m_table.size()){
            std::vector<Node*> new_table(m_num * 2, nullptr);
            for(size_t i = 0; i < m_table.size(); i++){
                Node* cur = m_table[i];
                while(cur){
                    Node* next = cur->m_next;
                    size_t hashi = hash(cur->m_kv.first) % new_table.size();
                    cur->m_next = new_table[hashi];
                    new_table[hashi] = cur;
                    cur = next;
                }
                m_table[i] = nullptr;
            }
            m_table.swap(new_table);
        }
        size_t hashi = hash(kv.first) % m_table.size();
        Node* cur = new Node(kv);
        cur->m_next = m_table[hashi];
        m_table[hashi] = cur;
        m_num++;

        return true;
    }
    template<typename K, typename V, typename Hash>
    bool hashtable<K, V, Hash>::erase(const K& key)
    {

        Hash hash;
        size_t hashi = hash(key) % m_table.size();
        Node* cur = m_table[hashi];
        Node* prev = nullptr;
        while(cur){
            if(cur->m_kv.first == key){
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

    template<typename K, typename V, typename Hash>
    typename hashtable<K, V, Hash>::Node* hashtable<K, V, Hash>::find(const K& key)
    {
        Hash hash;
        size_t hashi = hash(key) % m_table.size();
        Node* cur = m_table[hashi];
        while(cur)
        {
            if(cur->m_kv.first == key)
                return cur;
            cur = cur->m_next;
        }
        return nullptr;
    }

    void TestHT1()
    {
    	int a[] = { 10001,11,55,24,19,12,31,4,34,44};
    	hashtable<int, int> ht;
    	for (auto e : a)
    	{
    		ht.insert(std::make_pair(e, e));
    	}

    	ht.insert(std::make_pair(32, 32));
    	ht.insert(std::make_pair(32, 32));

    	ht.erase(31);
    	ht.erase(11);
    }

    void TestHT2()
    {
    	hashtable<std::string, int> ht;
    	ht.insert(std::make_pair("sort", 1));
    	ht.insert(std::make_pair("left", 1));
    	ht.insert(std::make_pair("insert", 1));
    }


}

#endif //HASHTABLE_HASHTABLE_HPP
