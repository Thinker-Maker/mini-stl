//
// Created by 董鸿博 on 2024/4/9.
//

#ifndef PRIORITY_QUEUE_PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_PRIORITY_QUEUE_HPP
#include <iostream>
#include <vector>
#include <string>
namespace dhb{
    template<typename T>
    struct greater
    {
        bool operator()(const T& x, const T& y)
        {
            return x > y;
        }
    };

    template<typename T>
    struct less
    {
       bool operator()(const T& x, const T& y)
       {
            return x < y;
       }
    };

    template<typename T>
    struct less<T*>
    {
        bool operator()(const T* x, const T* y)
        {
            return *x < *y;
        }
    };
    template<typename T>
    struct greater<T*>
    {
       bool operator()(const T* x, const T* y)
       {
           return *x > *y;
       }
    };


    template<typename T, class Container = std::vector<T>, class Compare = less<T> >
    class priority_queue{
    public:
        void shiftUp(size_t child);
        void shiftDown(size_t parent);
        void push(const T& val);
        void pop();
        const T& top()const;
        bool empty() const;
        size_t size() const;
    private:
        Container _con;
    };

    template<typename T, class Container, class Compare>
    void priority_queue<T, Container, Compare>::shiftUp(size_t child)
    {
        Compare cmp;
        size_t parent = (child - 1) / 2;
        while(child > 0)
        {
            if (cmp(_con[parent], _con[child]))
            {
               std::swap(_con[parent], _con[child]);
               child = parent;
               parent = (child - 1) / 2;
            }
            else
            {
                break;
            }
        }

    }
    template<typename T, class Container, class Compare>
    void priority_queue<T, Container, Compare>::shiftDown(size_t parent)
    {
        Compare cmp;
        size_t child = 2 * parent + 1;
        size_t sz = _con.size();

        while(child < sz)
        {
            if (child + 1 < sz && cmp(_con[child], _con[child+1]))
            {
               ++child;
            }

            if (cmp(_con[parent], _con[child]))
            {
               std::swap(_con[parent], _con[child]);
               parent = child;
               child = 2 * parent;
            }
            else
            {
                break;
            }
        }
    }
    template<typename T, class Container, class Compare>
    void priority_queue<T, Container, Compare>::push(const T& val)
    {
        _con.push_back(val);
        shiftUp(_con.size() - 1);
    }

    template<typename T, class Container, class Compare>
    void priority_queue<T, Container, Compare>::pop()
    {
        std::swap(_con[0], _con[_con.size() - 1]);
        _con.pop_back();
        shiftDown(0);
    }

    template<typename T, class Container, class Compare>
    bool priority_queue<T, Container, Compare>::empty() const
    {
        return _con.empty();
    }

    template<typename T, class Container, class Compare>
    size_t priority_queue<T, Container, Compare>::size() const
    {
        return _con.size();
    }

    template<typename T, class Container, class Compare>
    const T& priority_queue<T, Container, Compare>::top() const
    {
        return _con[0];
    }

    void test_priority_queue1()
    {
       priority_queue<int> pq;
       pq.push(3);
       pq.push(4);
       pq.push(1);
       pq.push(0);
       pq.push(2);
       pq.push(5);
       pq.push(6);
       while(!pq.empty())
       {
           std::cout << pq.top() << ' ';
           pq.pop();
       }
       std::cout << '\n';
    }

    void test_priority_queue2()
    {
       priority_queue<int, std::vector<int>, greater<int> > pq;
       pq.push(3);
       pq.push(4);
       pq.push(1);
       pq.push(0);
       pq.push(2);
       pq.push(5);
       pq.push(6);
       while(!pq.empty())
       {
           std::cout << pq.top() << ' ';
           pq.pop();
       }
       std::cout << '\n';

    }

    void test_priority_queue3()
    {
        priority_queue<std::string> pq;
        pq.push("banana");
        pq.push("apple");
        pq.push("pear");
        pq.push("zoo");
        pq.push("yellow");
        pq.push("blue");
        while(!pq.empty())
        {
            std::cout << pq.top() << ' ';
            pq.pop();
        }
        std::cout << '\n';

    }

    void test_priority_queue4()
    {
        std::string *p2 = new std::string("pear");
        std::string *p3 = new std::string("zoo");
        std::string *p1 = new std::string("apple");

        priority_queue<std::string*, std::vector<std::string*>, greater<std::string*> > pq;
        pq.push(p3);
        pq.push(p1);
        pq.push(p2);
        while(!pq.empty())
        {
            std::cout << *pq.top() << ' ';
            pq.pop();
        }
        std::cout << '\n';

        delete p1;
        delete p2;
        delete p3;
        p1=p2=p3=nullptr;
    }
}

#endif //PRIORITY_QUEUE_PRIORITY_QUEUE_HPP
