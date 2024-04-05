#ifndef STACK_STACK_HPP
#define STACK_STACK_HPP
#include <iostream>
#include <deque>

namespace dhb{
    template<typename T, class Container = std::deque<T>>
    class stack{
    public:
        void push(const T& value);
        void pop();
        T& top();
        const T& top() const;
        bool empty() const;
        size_t size() const;
    private:
       Container _con;
    };

    template<typename T, class Container>
    void stack<T, Container>::push(const T& value){
        _con.push_back(value);
    }

    template<typename T, class Container>
    void stack<T, Container>::pop(){
        _con.pop_back();
    }

    template<typename T, class Container>
    T& stack<T, Container>::top(){
        return _con.back();
    }

    template<typename T, class Container>
    const T& stack<T, Container>::top() const{
        return _con.back();
    }

    template<typename T, class Container>
    bool stack<T, Container>::empty()const{
        return _con.empty();
    }

    template<typename T, class Container>
    size_t stack<T, Container>::size()const{
        return _con.size();
    }

    void testStack(){
        stack<int> st;
        st.push(1);
        st.push(2);
        st.push(3);
        std::cout << st.top() << std::endl;
        st.pop();
        std::cout << st.top() << std::endl;
        std::cout << st.size() << std::endl;
        std::cout << st.empty() << std::endl;
    }

}
#endif //STACK_STACK_HPP
