//
// Created by 董鸿博 on 2024/4/29.
//
#pragma once

#include <cstdlib>
#include <string>
#include <cassert>
#include <ctime>
#include <iostream>
#include <utility>
#include <stack>
#include <vector>

namespace dhb
{
    enum Color
    {
        RED,
        BLACK,
    };
    template<typename T>
    struct rb_tree_node
    {
        rb_tree_node<T>* m_left = nullptr;
        rb_tree_node<T>* m_right = nullptr;
        rb_tree_node<T>* m_parent =nullptr;
        Color m_color = RED;
        T m_data;
        rb_tree_node(const T& data)
            :m_data(data)
        {}
    };

    template<typename T, typename Ptr, typename Ref>
    struct rb_tree_iterator
    {
        typedef rb_tree_node<T> Node;
        typedef rb_tree_iterator<T, Ptr, Ref> Self;
        typedef rb_tree_iterator<T, T*, T&> iterator;

    public:
        Node* m_node;
    public:
        rb_tree_iterator(Node* node)
            :m_node(node)
        {
        }
        rb_tree_iterator(const iterator& it)
            :m_node(it.m_node)
        {
        }

    public:
        Ref operator*()
        {
           return m_node->m_data;
        }
        Ptr operator->()
        {
            return &m_node->m_data;
        }
        bool operator!=(const Self& it)
        {
            return m_node != it.m_node;
        }

        Self& operator++()
        {
            if(m_node->m_right){
                Node* leftmost = m_node->m_right;
                while(leftmost->m_left)
                    leftmost = leftmost->m_left;
                m_node = leftmost;
            }
            else{
                Node* parent = m_node->m_parent;
                Node* cur = m_node;
                while(parent && parent->m_right == cur){
                    cur = parent;
                    parent = cur->m_parent;
                }
                m_node = parent;
            }
            return *this;
        }
        Self& operator--()
        {
            if(m_node->m_left){
                Node* rightmost = m_node->m_left;
                while(rightmost->m_right)
                    rightmost = rightmost->m_right;
                m_node = rightmost;
            }
            else{
               Node* parent = m_node->m_parent;
               Node* cur = m_node;
               while(parent && parent->m_left == cur){
                   cur = parent;
                   parent = cur->m_parent;
               }
               m_node = parent;
            }
            return *this;
        }

    };

    template<typename K, typename T, typename KeyOfT>
    class rb_tree
    {
        typedef rb_tree_node<T> Node;
    public:
        typedef rb_tree_iterator<T, T*, T&> iterator;
        typedef rb_tree_iterator<T, const T*, const T&> const_iterator;
        iterator begin()
        {
            Node* cur = m_root;
            while(cur && cur->m_left)
                cur = cur->m_left;
            return cur;
        }
        iterator end()
        {
            return nullptr;
        }
        const_iterator begin() const
        {
            Node* cur = m_root;
            while(cur && cur->m_left)
                cur = cur->m_left;
            return cur;
        }
        const_iterator end() const
        {
            return nullptr;
        }

        rb_tree();
        rb_tree(const rb_tree<K,T,KeyOfT>& t)
        {
            m_root = copy(t.m_root);
        }
        rb_tree<K,T,KeyOfT>& operator=(rb_tree<K,T,KeyOfT> t)
        {
            std::swap(m_root, t.m_root);
            return *this;
        }
        ~rb_tree();
        std::pair<iterator, bool> insert(const T& data);
        Node* find(const K& key);
        void clear();
    private:
        Node* copy(Node* root)
        {
            if(root == nullptr)
                return nullptr;

            Node* new_node = new Node(root->m_data);
            new_node->m_color = root->m_color;

            new_node->m_left = copy(root->m_left);
            //反向链接
            if(new_node->m_left)
               new_node->m_left->m_parent = new_node;

            new_node->m_right = copy(root->m_right);
            if(new_node->m_right)
                new_node->m_right->m_parent = new_node;

            return new_node;
        }
        void _clear(Node* root);
        void left_rotate(Node* parent);
        void right_rotate(Node* parent);
        void left_right_rotate(Node* parent);
        void right_left_rotate(Node* parent);
    private:
        Node* m_root = nullptr;
        KeyOfT m_kot;
    };

    template<typename K, typename T, typename KeyOfT>
    rb_tree<K,T,KeyOfT>::rb_tree()
    {
    }


    template<typename K, typename T, typename KeyOfT>
    rb_tree<K,T,KeyOfT>::~rb_tree()
    {
        clear();
        m_root = nullptr;
    }

    template<typename K, typename T, typename KeyOfT>
    std::pair<typename rb_tree<K,T,KeyOfT>::iterator, bool> rb_tree<K,T,KeyOfT>::insert(const T& data)
    {
        if(m_root == nullptr){
            m_root = new Node(data);
            m_root->m_color = BLACK;
            return {m_root,true};
        }

        Node* parent = nullptr;
        Node* cur = m_root;
        while(cur){
            if(m_kot(cur->m_data) < m_kot(data)){
                parent = cur;
                cur = cur->m_right;
            }
            else if(m_kot(cur->m_data) > m_kot(data)){
                parent = cur;
                cur = cur->m_left;
            }
            else
                return {cur, false};
        }

        cur = new Node(data);
        Node* newnode = cur;
        cur->m_parent = parent;
        if(m_kot(parent->m_data) < m_kot(data))
            parent->m_right = cur;
        else
            parent->m_left = cur;

        while(parent && parent->m_color == RED){
            Node* grandparent = parent->m_parent;
            if(grandparent->m_left == parent){
                Node* uncle = grandparent->m_right;
                if(uncle && uncle->m_color == RED){
                    grandparent->m_color = RED;
                    parent->m_color = BLACK;
                    uncle->m_color = BLACK;

                    cur = grandparent;
                    parent =grandparent->m_parent;
                }
                else {
                    if(parent->m_left == cur){
                        right_rotate(grandparent);
                        parent->m_color = BLACK;
                        grandparent->m_color = RED;
                        break;
                    }
                    else if(parent->m_right== cur){
                        left_right_rotate(grandparent);
                        grandparent->m_color = RED;
                        cur->m_color = BLACK;
                        break;
                    }
                    else{
                        assert(false);
                    }

                }
            }
            else{
                Node* uncle = grandparent->m_left;
                if(uncle && uncle->m_color == RED){
                    grandparent->m_color = RED;
                    parent->m_color = BLACK;
                    uncle->m_color = BLACK;

                    cur = grandparent;
                    parent = cur->m_parent;
                }
                else{
                    if(parent->m_right == cur){
                        left_rotate(grandparent);
                        grandparent->m_color = RED;
                        parent->m_color= BLACK;
                        break;
                    }
                    else if(parent->m_left == cur){
                        right_left_rotate(grandparent);
                        grandparent->m_color = RED;
                        cur->m_color = BLACK;
                        break;
                    }
                    else{
                        assert(false);
                    }
                }
            }
        }
        m_root->m_color = BLACK;
        return {newnode,true};
    }

    template<typename K, typename T, typename KeyOfT>
    void rb_tree<K,T,KeyOfT>::left_rotate(typename rb_tree<K, T, KeyOfT>::Node* parent)
    {
        Node* subR = parent->m_right;
        Node* subRL = subR->m_left;

        parent->m_right = subRL;
        if(subRL)
            subRL->m_parent = parent;

        subR->m_left = parent;
        Node* pparent = parent->m_parent;
        parent->m_parent = subR;

        subR->m_parent = pparent;
        if(parent == m_root){
            m_root = subR;
            m_root->m_parent = nullptr;
        }
        else{
            if(parent == pparent->m_left)
                pparent->m_left = subR;
            else
                pparent->m_right = subR;
        }

    }
    template<typename K, typename T, typename KeyOfT>
    void rb_tree<K,T,KeyOfT>::right_rotate(typename rb_tree<K, T, KeyOfT>::Node* parent)
    {
        Node* subL = parent->m_left;
        Node* subLR = subL->m_right;

        parent->m_left = subLR;
        if(subLR)
            subLR->m_parent = parent;

        Node* pparent = parent->m_parent;
        subL->m_right = parent;
        parent->m_parent = subL;

        subL->m_parent = pparent;
        if(parent == m_root){
            m_root = subL;
            m_root->m_parent = nullptr;
        }
        else {
            if(parent == pparent->m_left)
                pparent->m_left = subL;
            else
                pparent->m_right = subL;
        }
    }

    template<typename K, typename T, typename KeyOfT>
    void rb_tree<K,T,KeyOfT>::left_right_rotate(typename rb_tree<K, T, KeyOfT>::Node* parent)
    {
        Node* subL = parent->m_left;
        Node* subLR = subL->m_right;

        left_rotate(subL);
        right_rotate(parent);

    }

    template<typename K, typename T, typename KeyOfT>
    void rb_tree<K,T,KeyOfT>::right_left_rotate(typename rb_tree<K, T, KeyOfT>::Node* parent)
    {

        Node* subR = parent->m_right;
        Node* subRL = subR->m_left;

        right_rotate(subR);
        left_rotate(parent);


    }

    template<typename K, typename T, typename KeyOfT>
    typename rb_tree<K,T,KeyOfT>::Node* rb_tree<K,T,KeyOfT>::find(const K& key)
    {
        Node* cur = m_root;
        while(cur){
            if(cur->m_kv.first < key)
                cur = cur->m_right;
            else if(cur->m_kv.first > key)
                cur = cur->m_left;
            else
                return cur;

        }
        return nullptr;
    }


    template<typename K, typename T, typename KeyOfT>
    void rb_tree<K,T,KeyOfT>::clear()
    {
        _clear(m_root);
    }

    template<typename K, typename T, typename KeyOfT>
    void rb_tree<K,T,KeyOfT>::_clear(typename rb_tree<K,T,KeyOfT>::Node* root)
    {
        if(!root)
            return;

        _clear(root->m_left);
        _clear(root->m_right);
        delete root;
    }








}




