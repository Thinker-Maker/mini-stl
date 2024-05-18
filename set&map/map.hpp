//
// Created by 董鸿博 on 2024/5/16.
//

#ifndef SET_MAP_MAP_HPP
#define SET_MAP_MAP_HPP
#include "rb_tree.hpp"

namespace dhb
{
    template<typename K, typename T>
    class map
    {
        struct MapKeyOfT
        {
            const K& operator()(const std::pair<K, T>& kv)
            {
                return kv.first ;
            }
        };
    public:
        typedef typename rb_tree<K, std::pair<const K, T>, MapKeyOfT>::iterator iterator;
        typedef typename rb_tree<K, std::pair<const K, T>, MapKeyOfT>::const_iterator const_iterator;

        iterator begin()
        {
            return m_rbt.begin();
        }
        iterator end()
        {
            return m_rbt.end();
        }


        std::pair<iterator, bool> insert(const std::pair<K, T>& kv)
        {
            return m_rbt.insert(kv);
        }
        T& operator[](const K& key)
        {
            std::pair<iterator, bool> ret = insert({key, T()});
            return ret.first->second;
        }

    private:
        rb_tree<K, std::pair<const K, T>, MapKeyOfT> m_rbt;
    };

    void test_map1()
    {
        map<std::string, int> m;
        m.insert({"ƻ",1});
        m.insert({ "㽶",1 });
        m.insert({ "",1 });
        m.insert({ "ƻ",3 });

        map<std::string, int>::iterator it = m.begin();
        while (it != m.end())
        {
            //it->first += 'x';
            it->second += 1;

            //std::cout << it.operator->()->first << ":" << it->second << std::endl;
            std::cout << it->first << ":" << it->second << std::endl;
            ++it;
        }
        std::cout << std::endl;
    }

    void test_map2()
    {
        std::string arr[] = { "ƻ", "", "ƻ", "", "ƻ", "ƻ", "",
                         "ƻ", "㽶", "ƻ", "㽶","ƻ","ݮ", "ƻ","ݮ" };
        map<std::string, int> countMap;
        for (auto& e : arr)
        {
            countMap[e]++;
        }

        for (auto& kv : countMap)
        {
            std::cout << kv.first << ":" << kv.second << std::endl;
        }
        std::cout << std::endl;
    }
}

#endif //SET_MAP_MAP_HPP
