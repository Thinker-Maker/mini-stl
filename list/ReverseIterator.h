//
// Created by 董鸿博 on 2024/4/13.
//

#ifndef LIST_REVERSEITERATOR_H
#define LIST_REVERSEITERATOR_H

namespace dhb {
    template<typename iterator, typename Ptr, typename Ref>
    struct ReverseIterator {
        typedef ReverseIterator<iterator, Ptr, Ref> Self;
        iterator _cur;

        ReverseIterator(const iterator &it)
                : _cur(it) {}

        //Self operator++() {
        Self& operator++() {
            //error: non-const lvalue reference to type 'Self' (aka 'ReverseIterator<dhb::ListIterator<A, A *, A &>, A *, A &>')
            //cannot bind to a value of unrelated type 'Self' (aka 'ListIterator<A, A *, A &>')
            //return (--_cur);
            --_cur;
            return *this;
        }


        //Self operator--() {
        Self& operator--() {
            //error: non-const lvalue reference to type 'Self' (aka 'ReverseIterator<dhb::ListIterator<A, A *, A &>, A *, A &>')
            //cannot bind to a value of unrelated type 'Self' (aka 'ListIterator<A, A *, A &>')
            //return (++_cur);
            ++_cur;
            return *this;
        }

        Self operator++(int)
        {
            iterator tmp = _cur;
            --_cur;
            return tmp;
        }

        Self operator--(int)
        {
            iterator tmp = _cur;
            ++_cur;
            return tmp;
        }

        bool operator==(const Self &other) {
            return _cur == other._cur;
        }

        bool operator!=(const Self &other) {
            return _cur != other._cur;
        }

        Ref operator*() {
            iterator tmp(_cur);
            return *(--tmp);
        }

        Ptr operator->() {
            //iterator tmp(_cur);
            //return (--tmp).operator->();
            //另一种写法
            return &(operator*());
        }
    };
}

#endif //LIST_REVERSEITERATOR_H
