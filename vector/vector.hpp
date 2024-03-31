#ifndef VECTOR_VECTOR_HPP
#define VECTOR_VECTOR_HPP
#include <cassert>
#include <iostream>
#include <initializer_list>

namespace dhb{
    template<typename T>
    class vector{
    public:
        typedef T* iterator;
        typedef const T* const_iterator;

        //ctor & dtor
        vector() = default;
        vector(const vector<T>& vec);
        /**Clang-Tidy: Constructors that are callable with a single argument
          *must be marked explicit to avoid unintentional implicit conversions
          * 重载原因：
         * explicit vector(size_t n, const T& val = T());会和迭代器区间构造函数冲突，所以要重载
          */
         explicit vector(size_t n, const T& val = T());
         explicit vector(int n, const T& val = T());
        /**
         *
         * @tparam InputIterator
         * @param first
         * @param last
         * 函数模板下套了一个函数模板，可以接受任意类型的输入迭代器
         * 类模板成员可以是函数模板
         */
        template<typename InputIterator>
        vector(InputIterator first, InputIterator last);
        vector(std::initializer_list<T> il);
        vector& operator=(vector<T> vec); // pass by value
        ~vector();

        //Iterator
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

        //Capacity
        size_t size() const;
        size_t capacity() const;
        bool empty() const;
        void reserve(size_t n);
        void resize(size_t n, const T& val = T());

        //Element access
        T& operator[](size_t pos);
        const T& operator[](size_t pos)const;

        //Modifier
        void swap(vector<T>& vec);
        void push_back(const T& val);
        void pop_back();
        iterator insert(iterator pos, const T& val);
        iterator erase(iterator pos);

    private:
        //c++10设置默认值
        iterator _start = nullptr;
        iterator _finish = nullptr;
        iterator _end_of_storage = nullptr;
    };

    /**
     * Remember:  在类外定义类成员函数一定要受到类域的限制,包括iterator这种受类域限制的名称
     */
    //ctor & dtor
    template<typename T>
    vector<T>::vector(const vector<T>& vec){
        /**
         * error: reserve(vec.size());
         * reason: 扩的应该是vec.capacity()
         */
        //reserve(vec.capacity());
        /**
         * error: memcpy(_start, vec._start, vec.size() * sizeof(T));
         * _finish = _start + vec.size();
         * memcpy是字节序拷贝，原封不动地拷贝，如果对象数组中的对象是string就有问题
        */

        //for(const auto& e : vec){
        //    push_back(e);
        //}
        vector<T> tmp(vec.begin(), vec.end());
        swap(tmp);
    }

    template<typename T>
    vector<T>::vector(size_t n, const T& val ){
        resize(n, val);
    }

    template<typename T>
    vector<T>::vector(int  n, const T& val ){
        resize(n, val);
    }


    /**
     *
     * @tparam InputIterator
     * @param first
     * @param last
     * 类模板下套了一个函数模板，可以接受任意类型的输入迭代器
     * 类模板成员可以是函数模板
     */
    template<typename T>
    template<typename InputIterator>
    vector<T>::vector(InputIterator first, InputIterator last){
        auto it = first;
        while(it != last){
            push_back(*it);
            ++it;
        }
    }
    /**
     *template<typename T>
     *vector<T>::vector(const std::initializer_list<T>& il){
     * 元素的类型本身就是const T, 所以不需要const_cast
     * 也没必要引用接受参数
     */
    template<typename T>
    vector<T>::vector(std::initializer_list<T> il){
        //vector tmp(il.begin(), il.end());
        //swap(tmp);
        reserve(il.size());
        for(auto& e : il){
            push_back(e);
        }
    }

    //pass by value
    template<typename T>
    vector<T>& vector<T>::operator=(vector<T> vec){
        swap(vec);
        return *this;
    }

    template<typename T>
    vector<T>::~vector(){
        /**
         * error: delete _start;
         * 自定义类型只调用了一次析构函数
         */
        delete[] _start;
        _start = _finish = _end_of_storage = nullptr;
    }

    //Iterator
    /**
     *以下迭代器前加typename是为了解决dependent name的问题
     */
    template<typename T>
    typename vector<T>::iterator vector<T>::begin() {
        return _start;
    }

    template<typename T>
    typename vector<T>::iterator vector<T>::end(){
        return _finish;
    }

    template<typename T>
    typename vector<T>::const_iterator vector<T>::begin() const{
        return _start;
    }

    template<typename T>
    typename vector<T>::const_iterator vector<T>::end() const{
        return _finish;
    }


    //Capacity
    template<typename T>
    size_t vector<T>::size() const{
        return _finish - _start;
    }

    template<typename T>
    size_t vector<T>::capacity() const{
        return _end_of_storage - _start;
    }


    template<typename T>
    bool vector<T>::empty() const{
        return  _start == _finish;
    }

    template<typename T>
    void vector<T>::reserve(size_t n){
        if(n > capacity()){
            size_t oldSize = size();
            T* tmp = new T[n];
            /**
             * error: memcpy(tmp, _start, oldSize);
             * reason: memcpy是按照字节数拷贝的
             */
            /**
             * error: memcpy(tmp, _start, oldSize * sizeof(T));
             * reason: memcpy是逐字节拷贝，会导致对象数组中的对象是string就有问题，
             *         引发前拷贝问题
             */
             /**调用operator=, 进行深拷贝**/
             for(size_t i = 0; i < oldSize; ++i){
                 tmp[i] = _start[i];
             }
             delete[] _start;
            /**
             * error: delete _start;
             * 自定义类型只调用了一次析构函数
             */
            _start = tmp;
            /**
             * error: _finish = _start + size();
             * reason: _start已经被delete而且重新被赋值为tmp
             */
            _finish = _start + oldSize;
            _end_of_storage = _start + n;
        }
    }

    template<typename T>
    void vector<T>::resize(size_t n, const T& val ){
        if(n <= size()){
            _finish = _start + n;
        }else{
            size_t oldSize = size();
            reserve(n);
            /**
             * _finish = _start + n 差点忘了
             */
            _finish = _start + n;
            for(auto it = begin() + oldSize, last = end(); it != last; ++it){
                *it = val;
            }
        }
    }


    //Element access
    template<typename T>
    T& vector<T>::operator[](size_t pos){
        assert(pos < size());
        return _start[pos];
    }

    template<typename T>
    const T& vector<T>::operator[](size_t pos)const{
        assert(pos < size());
        return _start[pos];
    }

    //Modifier
    template<typename T>
    void vector<T>::swap(vector<T>& vec){
        std::swap(_start, vec._start);
        std::swap(_finish, vec._finish);
        std::swap(_end_of_storage, vec._end_of_storage);
    }

    template<typename T>
    void vector<T>::push_back(const T& val){
        //if(size() == capacity()){
        //    reserve(capacity() == 0 ? 4 : 2 * capacity());
        //}
        //*_finish = val;
        //++_finish;
        insert(end(), val);
    }

    template<typename T>
    void vector<T>::pop_back(){
        /**
         * emtpy()就要狠狠地惩罚
         */
        //assert(!empty());
        //--_finish;
        erase(end() - 1);
    }
    template<typename T>
    typename vector<T>::iterator vector<T>::insert(typename vector<T>::iterator pos, const T& val){
        assert(begin() <= pos && pos <= end());
        if(size() == capacity()){
            /**
             * error: reserve(capacity() == 0 ? 4 : 2 * capacity());
             * 扩容会导致迭代器失效
             */
            size_t offset = pos - _start;
            reserve(capacity() == 0 ? 4 : 2 * capacity());
            pos = _start + offset;
        }


        auto iter = _finish;
        /**
         * _finish不处理有问题，因为添加了元素后，_finish应该往后移动一位
         */
        ++_finish;
        while(iter != pos){
            *iter = *(iter - 1);
            --iter;
        }
        *pos = val;
        return pos;
    }

    template<typename T>
    typename vector<T>::iterator vector<T>::erase(typename vector<T>::iterator pos){
        assert(begin() <= pos && pos < end());
        auto iter = pos;
        while(iter != _finish){
            *iter = *(iter + 1);
            ++iter;
        }
        /**
         * _finish不处理有问题，因为删除了元素后，_finish应该往前移动一位
         */
        --_finish;
        return pos;
    }

    /**
     * test vector(const vector&)
     * test Iterator
     * test size() capacity()
     * test reserve() resize()
     * test operator[]
     * test push_back()  pop_back()
     */
     template<typename T>
     void print(const vector<T>& vec){
        for(const auto& e : vec){
            std::cout << e << ' ';
        }
        std::cout << '\n';
     }

     void test_vector1(){
         vector<double> vec;
         vec.push_back(1.1);
         vec.push_back(1.2);
         vec.push_back(1.3);
         vec.push_back(1.4);
         vec.push_back(1.5);
         std::cout << vec.size() << '\n';
         std::cout << vec.capacity() << '\n';
         print(vec);
         vec.resize(10, 2.2);
         std::cout << vec.size() << '\n';
         std::cout << vec.capacity() << '\n';
         print(vec);
         vector<double> copy_vec(vec);
         std::cout << copy_vec.size() << '\n';
         std::cout << copy_vec.capacity() << '\n';
         print(copy_vec);
         copy_vec.pop_back();
         copy_vec.pop_back();
         copy_vec.pop_back();
         std::cout << copy_vec.size() << '\n';
         std::cout << copy_vec.capacity() << '\n';
         print(copy_vec);
     }
     void test_vector2(){
         vector<std::string> vec;
         vec.push_back("hello");
         vec.push_back("world");
         vec.push_back("!");
         vector<std::string> copy_vec(1, "new");
         copy_vec = vec;
         print(vec);
         print(copy_vec);
         copy_vec.pop_back();
         print(copy_vec);
         vector<int> v1(2, 4);
         print(v1);
     }
     void test_vector3(){
        vector<int> v1 = {1,2, 3, 4, 5};
        print(v1);
        v1.insert(v1.begin(), 6);
        v1.insert(v1.end(), 7);
        v1.insert(v1.begin()+ 2, 8);
        print(v1);
        v1.erase(v1.begin());
        v1.erase(v1.end() - 1);
        print(v1);
     }
}
#endif //VECTOR_VECTOR_HPP
