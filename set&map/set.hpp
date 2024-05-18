//
// Created by 董鸿博 on 2024/5/16.
//

#ifndef SET_MAP_SET_HPP
#define SET_MAP_SET_HPP
#include "rb_tree.hpp"

namespace dhb
{
   template<class K>
   class set
   {
       struct SetKeyOfT
       {
           const K& operator()(const K& key)
           {
               return key;
           }
       };
   public:

       typedef typename rb_tree<K,const K,SetKeyOfT>::const_iterator iterator;
       typedef typename rb_tree<K,const K,SetKeyOfT>::const_iterator const_iterator;

       const_iterator begin() const
       {
          return m_rbt.begin();
       }
       const_iterator end() const
       {
           return m_rbt.end();
       }


       std::pair<iterator, bool> insert(const K& key)
       {
           return m_rbt.insert(key);
       }
   private:
       rb_tree<K, const K, SetKeyOfT> m_rbt;
   };
    void test_set()
    {
        set<int> s;
        s.insert(4);
        s.insert(2);
        s.insert(5);
        s.insert(15);
        s.insert(7);
        s.insert(1);
        s.insert(5);
        s.insert(7);


        set<int>::iterator it = s.begin();
        while (it != s.end())
        {
            //*it += 5;

            std::cout << *it << " ";
            ++it;
        }
        std::cout << std::endl;

        for (auto e : s)
        {
            std::cout << e << " ";
        }
        std::cout << std::endl;

        set<int> copy = s;
        for (auto e : copy)
        {
            std::cout << e << " ";
        }
        std::cout << std::endl;
    }

}

#endif //SET_MAP_SET_HPP
