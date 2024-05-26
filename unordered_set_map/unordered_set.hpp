//
// Created by 董鸿博 on 2024/5/24.
//

#ifndef UNORDERED_SET_MAP_UNORDERED_SET_HPP
#define UNORDERED_SET_MAP_UNORDERED_SET_HPP
#include "hashtable.hpp"

namespace dhb
{
    template<typename K, typename Hash = HashFunc<K>>
    class unordered_set
    {
    public:
        struct SetKeyOfT
        {
            const K& operator()(const K& key){
                return key;
            }
        };
    public:
        typedef typename hashtable<K, const K, SetKeyOfT, Hash>::const_iterator  iterator;
        typedef typename hashtable<K, const K, SetKeyOfT, Hash>::const_iterator  const_iterator;

        const_iterator begin() const{
            return m_ht.begin();
        }
        const_iterator end() const{
           return m_ht.end();
        }

        iterator find(const K& key){
            return m_ht.find(key);
        }

        std::pair<iterator, bool> insert(const K& key){
            return m_ht.insert(key);
        }
    private:
        hashtable<K, const K, SetKeyOfT, Hash> m_ht;
    };

    void Func(const unordered_set<int>& s)
    {
        unordered_set<int>::iterator it = s.begin();
        while (it != s.end())
        {
            //*it = 1;
            std::cout << *it << " ";
            ++it;
        }
        std::cout << std::endl;
    }

    void test_unordered_set()
    {
        unordered_set<int> s;
        s.insert(31);
        s.insert(11);
        s.insert(5);
        s.insert(15);
        s.insert(25);

        unordered_set<int>::iterator it = s.begin();
        while (it != s.end())
        {
            //*it = 1;
            std::cout << *it << " ";
            ++it;
        }
        std::cout << std::endl;

        for (auto e : s)
        {
            std::cout << e << " ";
        }
        std::cout << std::endl;
    }
}

#endif //UNORDERED_SET_MAP_UNORDERED_SET_HPP
