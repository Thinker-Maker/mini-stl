#ifndef QUEUE_QUEUE_HPP
#define QUEUE_QUEUE_HPP
#include <iostream>
#include <deque>

namespace dhb{
    template<typename T, class Container = std::deque<T>>
    class queue{
    public:
        void push(const T& value);
        void pop();
        size_t size() const;
        bool empty() const;
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;
    private:
       Container _con;
    };

    template<typename T, class Container>
    void queue<T, Container>::push(const T& value){
        _con.push_back(value);
    }

    template<typename T, class Container>
    void queue<T, Container>::pop(){
        _con.pop_front();
    }

    template<typename T, class Container>
    size_t queue<T, Container>::size()const{
        return _con.size();
    }

    template<typename T, class Container>
    bool queue<T, Container>::empty() const{
        return _con.empty();
    }

    template<typename T, class Container>
    T& queue<T, Container>::front(){
        return _con.front();
    }

    template<typename T, class Container>
    const T& queue<T, Container>::front() const{
        return _con.front();
    }

    template<typename T, class Container>
    T& queue<T, Container>::back(){
        return _con.back();
    }

    template<typename T, class Container>
    const T& queue<T, Container>::back() const{
        return _con.back();
    }


    void test_queue(){
        dhb::queue<int> q;
        q.push(1);
        q.push(2);
        q.push(3);
        std::cout << q.front() << std::endl;
        std::cout << q.back() << std::endl;
        q.pop();
        std::cout << q.front() << std::endl;
        std::cout << q.back() << std::endl;
        std::cout << q.size() << std::endl;
        std::cout << q.empty() << std::endl;
    }


}

#endif //QUEUE_QUEUE_HPP
