//
// Created by 董鸿博 on 2024/5/24.
//

#ifndef UNORDERED_SET_MAP_UNORDERED_MAP_HPP
#define UNORDERED_SET_MAP_UNORDERED_MAP_HPP
#include "hashtable.hpp"

namespace dhb
{
    template<typename K, typename T, typename Hash = HashFunc<K>>
    class unordered_map
    {
    public:
        struct MapKeyOfT
        {
            const K& operator()(const std::pair<const K, T>& kv){
                return kv.first;
            }
        };
    public:
        typedef typename hashtable<K, std::pair<const K, T>, MapKeyOfT, Hash>::iterator iterator;
        typedef typename hashtable<K, std::pair<const K, T>, MapKeyOfT, Hash>::const_iterator const_iterator;
        iterator begin(){
            return m_ht.begin();
        }
        iterator end(){
            return m_ht.end();
        }

        std::pair<iterator, bool> insert(const std::pair<const K, T>& kv){
            m_ht.insert(kv);
        }

        iterator find(const K& key){
            return m_ht.find(key);
        }

        T& operator[](const K& key){
            std::pair<iterator, bool> ret = m_ht.insert({key, T()});
            return ret.first->second;
        }
    private:
        hashtable<K, std::pair<const K, T>, MapKeyOfT, Hash> m_ht;
    };
    void test_unordered_map()
    {
        std::string arr[] = { "苹果", "西瓜", "苹果", "西瓜", "苹果", "苹果", "西瓜",
                         "苹果", "香蕉", "苹果", "香蕉","苹果","草莓", "苹果","草莓" };
        unordered_map<std::string, int> countMap;
        for (auto& e : arr)
        {
            countMap[e]++;
        }
        countMap.insert({"火龙果", 3});

        unordered_map<std::string, int>::iterator it = countMap.begin();
        while (it != countMap.end())
        {
            //it->first += 'x'; // key不能修改
            it->second += 1;  // value可以修改
            std::cout << it->first << ":" << it->second << std::endl;
            ++it;
        }
        std::cout << std::endl;

        for (auto& kv : countMap)
        {
            std::cout << kv.first << ":" << kv.second << std::endl;
        }
        std::cout << std::endl;
    }
}

#endif //UNORDERED_SET_MAP_UNORDERED_MAP_HPP
