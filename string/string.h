#ifndef STRING_STRING_H
#define STRING_STRING_H
#include <iostream>
#include <cstring>
#include <vector>
#include <cassert>
using namespace std;

namespace dhb
{
    class string
    {
    public:
        typedef char* iterator;
        typedef const char* const_iterator;
        /**
         *  error: string(const char* str = nullptr);
         *  reason: strlen(str)对nullptr解引用了
         *
         *  error: string(const char* str = '\0');
         *  reason: char(整型)不支持隐式类型转换为nullptr(指针), 意义相近类型才能转换
         **/
        //ctor&dtor
        string(const char* str = "");
        string(const string& str);//不加引用，会引发无穷递归调用
        string& operator=(string str); //copy & swap
        ~string();
        //Iterator
        iterator begin();
        iterator end();
        const_iterator begin()const;
        const_iterator end()const;
        /**
         *  size_t type不需要判断是否小于0，因为size_t是unsigned,不存在负数
         * */
        //Capacity
        size_t size()const;
        size_t capacity()const;
        void reserve(size_t n);
        void resize(size_t n, char ch ='\0');
        void clear();
        //Modifier
        void push_back(char ch);
        void append(const char* str);
        void insert(size_t pos, char ch);
        void insert(size_t pos, const char* str);
        void erase(size_t pos, size_t len = npos);
        string& operator+=(char ch);
        string& operator+=(const char* str);
        //Element Access
        char& operator[](size_t pos);
        const char& operator[](size_t pos)const;
        //string operations
        const char* c_str() const;
        void swap(string& str);
        size_t find(const char* str, size_t pos = 0)const;
        size_t find(char ch, size_t pos = 0)const;
        string substr(size_t pos = 0, int len = npos) const;//default argument 必须从右到左给
    private:
        char* _str = nullptr;//防止copy ctor的Modern写法有错误
        size_t _size;
        size_t _capacity;//_capacity不包括‘\0’
    public:
        static const int npos;
    };
    const int string::npos = -1;

    //ctor & dtor
    inline
    string::string(const char* str)
        : _size(strlen(str))
    {
        _capacity = _size;
        _str = new char[_capacity+1];
        strcpy(_str, str);
    }
    inline
    string::string(const string& str){
        //error: delete _str; 这是拷贝构造，还没构造，_str是野指针，delete _str;野指针问题
        /**
        _str = new char[str._capacity + 1];
        strcpy(_str, str._str);
        _size = str._size;
        _capacity = str._capacity;
         */
         //Modern
         /**error: 此时_str是随机值，直接交换,出函数作用域调用析构函数, delete tmp._str,野指针问题
          * string tmp(str.c_str());
          *  swap(tmp);
          */
          //再给_str赋值后，_str = nullptr,没有问题了
          string tmp(str._str);
          swap(tmp);
    }
    //copy & swap
    //传值传参调用拷贝构造， reason:形参是实参的拷贝，就是拷贝构造
    inline
    string& string::operator=(string str){
        /** error: 没有检查self assignment
        swap(_str, str._str);
        _size = str._size;
        _capacity = str._capacity;
        */
        /**上面其实没有错误, copy & swap写法可以不用检查self assignment, 它是安全的
         * if(this != &str)永远成立，因为this所指的对象和str完全不在一个地方
         * if(this != &str)对于copy & swap是画蛇添足
         */
        //& 类会默认提供这个成员函数
        /**Modern**/
        swap(str);
       //要有返回值
        return *this;
    }
    inline
    string::~string()
    {
        delete[] _str;
        _size = _capacity = 0;
    }
    //Iterator
    //注意iterator & const_iterator受到的类域的封装
    string::iterator string::begin(){
        return _str;
    }
    string::iterator string::end(){
        return _str +_size;
    }
    string::const_iterator string::begin()const{
        return _str;
    }

    string::const_iterator string::end()const{
       return _str + _size;
    }

    //Capacity
    inline
    size_t string::size()const{
        return _size;
    }
    inline
    size_t string::capacity()const{
        return _capacity;
    }
    //Element Access
    inline
    char& string::operator[](size_t pos){
        assert(pos < _size);
        return _str[pos];
    }
    inline
    const char& string::operator[](size_t pos)const{
        assert(pos < _size);
        return _str[pos];
    }
    inline
    void string::reserve(size_t n){//size_t 没有负数
        if(_capacity >= n){
            return;
        }else{
            char* tmp = new char[n+1];
            strcpy(tmp, _str);
            delete _str;
            _str = tmp;
            _capacity = n;
        }

    }
    //Error: void string::resize(size_t n, char ch ='\0')Redefinition of default argument
    inline
    void string::resize(size_t n, char ch ){//size_t 没有负数
        if(n <= _size){
            _str[n] = '\0';
            _size = n;
        }else{
           reserve(n);
           //error: for(int i = _size; i < _capacity; i++){ i是下标这是关键，左闭又开才是个数，左闭右闭合不是个数
           for(int i = _size; i < _capacity; i++){
               _str[i] = ch;
           }
           _size = n;
        }
    }
    void string::clear(){
        resize(0);
    }
    //Modifier
    inline
    void string::push_back(char ch){
        //if(_size == _capacity){
        //    //_capacity先开始为0
        //    reserve(_capacity == 0 ? 4 : 2 * _capacity);
        //}
        //_str[_size++] = ch;
        //_str[_size] = '\0';
        insert(_size, ch);
    }
    inline
    void string::append(const char* str){
        //int len = strlen(str);
        //if(_size + len > _capacity){
        //    int n = (_size + len < 2 * _capacity) ? 2 * _capacity : _size + len;
        //    //error: reserve(n + 1);//加1是考虑'\0' |  reserve会考虑这一点，这是画蛇添足，多此一举
        //    reserve(n);
        //}
        ////error: strcpy(_str + len, str); 是从原位置的terminated null character开始拷贝, 是加_size
        //strcpy(_str + _size, str);
        //_size += len;
        insert(_size, str);
    }

    inline
    void string::insert(size_t pos, char ch){
        assert(pos <= _size);
        if(_size == _capacity){
            reserve(_capacity == 0 ? 4 : 2 * _capacity);
        }

        //size_t 没有负值，警惕 0 - 1下溢
        //要记得_size位置是'\0'
        for(int i = _size + 1; i > pos; i--){
            _str[i] = _str[i - 1];
        }
        _str[pos] = ch;
        _size += 1;
    }
    inline
    void string::insert(size_t pos, const char* str){
        assert(pos <= _size);
        size_t len = strlen(str);
        if(_size + len > _capacity){
            int n = (pos + len  < 2 * _capacity) ? 2 *_capacity : pos + len;
            reserve(n);
        }
        //for(int i = _size + len; i > pos + len - 1; i--){也可以
        for(int i = _size + len; i >= pos + len; i--){
            //error: _str[i] = _str[i - (len - pos + 1)]; //纯属胡写，是从下标为_size处开始搬运
            _str[i] =  _str[i - len];
        }
        strncpy(_str+pos, str, len);
        _size += len;
    }

    inline
    void string::erase(size_t pos, size_t len){
        assert(pos < _size);
        //error: if(pos == npos || pos + len >= _size){ 如果len = npos - 1, pos + len就已溢出了
        if(pos == npos ||  len >= _size - pos){
            _str[pos] = '\0';
            _size = pos;
            return;
        }
        strcpy(_str + pos, _str + pos + len);
        _size -= len;
    }
    inline
    string& string::operator+=(char ch){
        push_back(ch);
        return *this;
    }
    inline
    string& string::operator+=(const char* str){
        append(str);
        return *this;
    }

    //String operations
    inline
    const char* string::c_str() const{
        return _str;
    }
    inline
    void string::swap(string& str){
        ::swap(_str, str._str);
        ::swap(_size, str._size);
        ::swap(_capacity, str._capacity);
    }
    size_t string::find(const char* str, size_t pos)const{
        assert(pos < _size);
        char* ptr = strstr(_str + pos, str);
        if(ptr == nullptr){
            return npos;
        }
        return ptr - _str;

    }
    size_t string::find(char ch, size_t pos)const{
        assert(pos < _size);
        char *ptr = strchr(_str + pos, ch);
        if(ptr == nullptr){
            return npos;
        }
        return ptr - _str;
    }
    string string::substr(size_t pos, int len) const{
        if(len == npos || len >= _size - pos){
            return _str + pos;
        }
        char str[_size + 1];
        memset(str, 0, _size + 1);
        strncpy(str, _str + pos, len);
        return str;

    }
    //overloading << >>
    ostream&
    operator<<(ostream& os, const string& str){
        return os << str.c_str();
    }

    istream&
    operator>>(istream& is, string& str){
        //每次都要从头覆盖
        str.clear();
        //get()方法要了解楚, get(): Get characters
        //error is >> ch; >>默认以' '和'\n'为分隔符， 忽略' ', '\n', 忽略就是不读入
        char ch = is.get();
        //buf缓冲数组解决频繁扩容造成的消耗
        char buf[128];
        size_t i = 0;//计数器
        while(ch != ' ' && ch != '\n'){
            //+=要想起来
            //str += ch;
            buf[i++] = ch;
            if(i == 127){
                buf[i] = '\0';
                str += buf;
                i = 0;
            }
            ch = is.get();
        }
        //收尾工作: buf没有满，但是还是有数据添加到str
        if(i != 0){
            //不加buf[i] = '\0'会越界
            buf[i] = '\0';
            str += buf;
        }
        return is;
    }

    //test push_back & reserve
    void test_string1(){
         string str("hello, world");
         cout << str.c_str() << endl;
         cout << str.size() << endl;
         cout << str.capacity() << endl;
         cout << "-------------" << endl;
         str += '!';
         cout << str.c_str() << endl;
         cout << str.size() << endl;
         cout << str.capacity() << endl;
        cout << "-------------" << endl;
         str.reserve(1);
         cout << str.c_str() << endl;
        cout << str.size() << endl;
        cout << str.capacity() << endl;
    }
    //test append & resize;
    void test_string2(){
        string str("hello");
        str += ", world!";
        cout << str.c_str() << endl;
        cout << str.size() << ' ' << strlen(str.c_str()) << endl;
        cout << str.capacity() << endl;

        cout << "-------------" << endl;
        str.resize(5);
        cout << str.c_str() << endl;
        cout << str.size() << endl;
        cout << str.capacity() << endl;
        cout << "-------------" << endl;
        str.resize(13, 'h');
        cout << str.c_str() << endl;
        cout << str.size() << ' ' << strlen(str.c_str()) << endl;
        cout << str.capacity() << endl;
    }
    //test insert & erase
    void test_string3(){
        string str(",world!");
        cout << str.c_str() << endl;
        cout << str.size() << endl;
        cout << str.capacity() << endl;
        cout << "-------------" << endl;
        str.insert(0, "hello");
        cout << str.c_str() << endl;
        cout << str.size() << endl;
        cout << str.capacity() << endl;
        cout << "-------------" << endl;
        str.insert(6, ' ');
        cout << str.c_str() << endl;
        cout << str.size() << endl;
        cout << str.capacity() << endl;
        cout << "-------------" << endl;
        str.erase(6, 3);
        cout << str.c_str() << endl;
        cout << str.size() << endl;
        cout << str.capacity() << endl;
        cout << "-------------" << endl;
        str.erase(6);
        cout << str.c_str() << endl;
        cout << str.size() << endl;
        cout << str.capacity() << endl;
        cout << "-------------" << endl;
    }
    //test copy ctor & operator=
    void test_string4(){
        string str1("hello, world!");
        cout << str1.c_str() << endl;
        cout << str1.size() << endl;
        cout << str1.capacity() << endl;
        cout << "-------------" << endl;

        string str2(str1);
        cout << str2.c_str() << endl;
        cout << str2.size() << endl;
        cout << str2.capacity() << endl;
        cout << "-------------" << endl;

        str2 = "ByteDance";
        cout << str2.c_str() << endl;
        cout << str2.size() << endl;
        cout << str2.capacity() << endl;
        cout << "-------------" << endl;

        str2 = str2;
        cout << str2.c_str() << endl;
        cout << str2.size() << endl;
        cout << str2.capacity() << endl;
        cout << "-------------" << endl;
    }

    //test iterator & const iteratot & find & substr
    void test_string5(){
        const string str("ByteDance");
        auto it = str.begin();
        while(it != str.end()){
            cout << *it;
            ++it;
        }
        cout << endl;
        for(auto ch : str){
            cout << ch;
        }
        cout << endl;
        if(str.find('B') != string::npos){
            cout << "str.find('B'): " << str.find('B') << endl;
            cout << str.substr(str.find('B'), 4) << endl;
        }
        if(str.find("Dance") != string::npos){
            cout << "str.find('Dance'): " << str.find("Dance") << endl;
            cout << str.substr(str.find("Dance")) << endl;
        }
    }

    //test operator>>
    void test_string6(){
        string str("Byte");
        cout << str << endl;
        cin >> str;
        cout << str << endl;
    }
}

#endif //STRING_STRING_H
