//
// Created by 29217 on 2024/4/2.
//

#ifndef LIST_LIST_HPP
#define LIST_LIST_HPP
#include <iostream>
#include <string>
#include <cassert>
#include <initializer_list>

namespace dhb{
    template<typename T>
    struct ListNode{
        T _data;
        ListNode<T>* _prev;
        ListNode<T>* _next;
        explicit ListNode(const T& val = T())
        :_data(val), _prev(nullptr), _next(nullptr)
        { }
    };

    /**
     *
     * @tparam T
     * @tparam Ptr
     * @tparam Ref
     * 这个类就是浅拷贝的，所以不用担心指针的复制问题。
     * 资源的释放由list类负责，所以不需要担心内存泄漏问题。
     */
    template<typename T, typename Ptr, typename Ref>
    struct ListIterator{
        typedef ListNode<T> Node;
        typedef ListIterator<T, Ptr, Ref> Self;
        Node* _node;
        //会去调用ListNode的构造函数
        ListIterator(Node* node)
        :_node(node)
        { }
        ListIterator(const Self& other)
        :_node(other._node)
        {
        }


        Self& operator++(){
            _node = _node->_next;
            return *this;
        }

        Self operator++(int){
            //ListIterator<T, Ptr, Ref> tmp(*this);//调用默认生成的拷贝构造函数
            Self tmp(_node);
            _node = _node->_next;
            return tmp;
        }

        Self& operator--(){
            _node = _node->_prev;
            return *this;
        }

        Self operator--(int){
            Self tmp(_node);
            _node = _node->_prev;
            return tmp;
        }


        bool operator !=(const Self& other){
            return _node != other._node;
        }
        bool operator ==(const Self& other){
            return _node == other._node;
        }

        //it->
        // T*
        Ptr operator->(){
            return &(_node->_data);
        }

        // T&
        //*it
        Ref operator *(){
           return _node->_data;
        }

    };

    template<typename T>
    class list{
        typedef ListNode<T> Node;
    public:
        typedef ListIterator<T, T*, T&> iterator;
        typedef ListIterator<T, const T*, const T&> const_iterator;
       //ctor & dtor & operator=
       list();
       ~list();
       list(const list<T>& lt);
       template<typename InputIterator>
       list(InputIterator first, InputIterator last);
       list(std::initializer_list<T> il);
       list& operator=(list<T> lt);


       //iterator
       iterator begin();
       iterator end();
       const_iterator begin() const;
       const_iterator end() const;

       //capacity
       size_t size() const;
       bool empty() const;

       //modifiers
       void empty_init();
       void swap(list<T>& lt);
       void push_front(const T& val);
       void pop_front();
       void push_back(const T& val);
       void pop_back();
       void insert(iterator pos, const T & val);
       iterator erase(iterator pos);
       void clear();
    private:
        Node* _head;
        size_t _size;
    };

    template<typename T>
    list<T>::list(){
        empty_init();
    }

    template<typename T>
    list<T>::~list(){
        clear();
        delete _head;
        _head = nullptr;
        _size = 0;
    }

    template<typename T>
    list<T>::list(const list<T>& lt){
        /**
         * empty_init很重要
         */
        empty_init();
        list<T> tmp(lt.begin(), lt.end());
        /**
         * error: swap(*this, tmp);自带传递参数this
         */
         swap(tmp);
    }

    template<typename T>
    template<typename InputIterator>
    list<T>::list(InputIterator first, InputIterator last){
        /**
         * empty_init很重要
         */
        empty_init();
        auto it = first;
        while(it != last){
            push_back(*it);
            ++it;
            /**
             *error:++_size
             * reason: push_back()已经维护了_size
             */

        }
    }
    template<typename T>
    list<T>::list(std::initializer_list<T> il){
        /**
         * empty_init很重要
         */
        empty_init();
        list<T> tmp(il.begin(), il.end());
        /**
         * error: swap(*this, tmp);自带传递参数this
         */
        swap(tmp);
    }
    template<typename T>
    list<T>& list<T>::operator=(list<T> lt){
        /**
         * error: swap(*this, tmp);自带传递参数this
         */
        swap(lt);
        /**
         * 记得返回*this
         */
        return *this;
    }
    template<typename T>
    typename list<T>::iterator list<T>::begin(){
        return  _head->_next;
    }

    template<typename T>
    typename list<T>::iterator list<T>::end(){
        return _head;
    }

    template<typename T>
    typename list<T>::const_iterator list<T>::begin()const{
        return  _head->_next;
    }

    template<typename T>
    typename list<T>::const_iterator list<T>::end()const{
        return _head;
    }
    template<typename T>
    size_t list<T>::size() const{
        return _size;
    }
    template<typename T>
    bool list<T>::empty() const{
        return _head->_next == _head;
    }

    template<typename T>
    void list<T>::empty_init(){
        _head = new Node();
        _size= 0;
        _head->_next = _head;
        _head->_prev = _head;
    }


    template<typename T>
    void list<T>::swap(list<T>& lt){
        std::swap(_head, lt._head);
        std::swap(_size, lt._size);
    }
    template<typename T>
    void list<T>::push_front(const T& val){
        insert(begin(), val);
    }

    template<typename T>
    void list<T>::push_back(const T& val){
        insert(end(), val);
    }

    /**
     *
     * @tparam T
     * @param pos
     * @param val
     * 是Node*在插入删除链接
     */
    template<typename T>
    void list<T>::insert(typename list<T>::iterator pos, const T& val){
        Node* cur = pos._node;
        Node* prev = cur->_prev;
        Node* newNode = new Node(val);
        newNode->_prev = prev;
        newNode->_next = cur;
        prev->_next = newNode;
        cur->_prev = newNode;
        /**
         * 定义一种性质，并且维护这种性质
         */
        _size++;
    }

    template<typename T>
    void list<T>::pop_front(){
        erase(begin());
    }

    template<typename T>
    void list<T>::pop_back(){
        erase(--end());
    }



    template<typename T>
    typename list<T>::iterator list<T>::erase(typename list<T>::iterator pos){
        assert(empty() == false);
        //_node是pos的成员变量
        Node* cur = pos._node;
        Node* prev = cur->_prev;
        Node* next = cur->_next;

        prev->_next = next;
        next->_prev = prev;
        delete cur;
        /**
         * 定义一种性质，并且维护这种性质
         */
        --_size;
        return next;
    }

    template<typename T>
    void list<T>::clear(){
        Node* cur = _head->_next;
        while(cur != _head){
            Node* next = cur->_next;
            delete cur;
            cur = next;
        }
    }

    void testList1(){
        list<int> lt;
        lt.push_back(1);
        lt.push_back(2);
        lt.push_back(3);
        lt.push_back(4);
        lt.push_back(5);
        lt.push_front(0);
        for(auto it = lt.begin(); it != lt.end(); ++it){
            std::cout << *it << " ";
        }
        std::cout << '\n';
        for(auto e : lt){
            std::cout << e << " ";
        }
        std::cout << "size: " << lt.size() << '\n';
        std::cout << '\n';

    }

    void testList2(){
        list<int> lt;
        lt.push_back(1);
        lt.push_back(2);
        lt.push_back(3);
        auto iter = lt.begin();
        ++iter;
        lt.insert(iter, 4);
        for(auto e : lt){
            std::cout << e << " ";
        }
        std::cout << '\n';

        lt.pop_front();
        lt.pop_back();
        for(auto e : lt){
            std::cout << e << " ";
        }
        std::cout << '\n';

        lt.push_back(5);
        for(auto e : lt){
            std::cout << e << " ";
        }
        std::cout << '\n';
        lt.erase(++lt.begin());
        for(auto e : lt) {
            std::cout << e << " ";
        }
        std::cout << '\n';
    }

    void testList3(){
        list<std::string> lt = {"hello", ",", "world", "!"};
        for(auto &e : lt) {
            std::cout << e << " ";
        }
        std::cout << "size: " << lt.size() << '\n';
        std::cout << '\n';
        list<std::string> lt2(lt);
        for(auto &e : lt2) {
            std::cout << e << " ";
        }
        std::cout << "size: " << lt.size() << '\n';
        std::cout << '\n';
        lt2 = {"a", "b", "c"};
        for(auto &e : lt2) {
            std::cout << e << " ";
        }
        std::cout << "size: " << lt2.size() << '\n';
    }

}


#endif //LIST_LIST_HPP
