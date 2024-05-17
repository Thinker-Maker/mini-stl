//
// Created by 董鸿博 on 2024/4/29.
//
#pragma once

#include <cstdlib>
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
    template<typename K, typename V>
    struct rb_tree_node
    {
        rb_tree_node<K,V>* m_left = nullptr;
        rb_tree_node<K,V>* m_right = nullptr;
        rb_tree_node<K, V>* m_parent =nullptr;
        Color m_color = RED;
        std::pair<K, V> m_kv;
        rb_tree_node(const std::pair<K,V>& kv)
            :m_kv(kv)
        {}
    };

    template<typename K, typename V>
    class rb_tree
    {
        typedef rb_tree_node<K, V> Node;
    public:
        rb_tree();
        ~rb_tree();
        bool insert(const std::pair<K, V>& kv);
        Node* find(const K& key);
        void traverse();
        void clear();
        friend bool isvalid_bst(Node* root);
        Node* getRoot();
        bool balanced();
        bool valid();
    private:
        bool dfs(Node* root, int black_nums, int target);
        int height(Node* root);
        void _traverse(Node* root);
        void _clear(Node* root);
        void left_rotate(Node* parent);
        void right_rotate(Node* parent);
        void left_right_rotate(Node* parent);
        void right_left_rotate(Node* parent);
    private:
        Node* m_root = nullptr;
    };

    template<typename K, typename V>
    rb_tree<K,V>::rb_tree()
    {
    }


    template<typename K, typename V>
    rb_tree<K,V>::~rb_tree()
    {
        clear();
        m_root = nullptr;
    }

    template<typename K, typename V>
    bool rb_tree<K,V>::insert(const std::pair<K, V>& kv)
    {
        if(m_root == nullptr){
            m_root = new Node(kv);
            m_root->m_color = BLACK;
            return true;
        }

        Node* parent = nullptr;
        Node* cur = m_root;
        while(cur){
            if(cur->m_kv.first < kv.first){
                parent = cur;
                cur = cur->m_right;
            }
            else if(cur->m_kv.first > kv.first){
                parent = cur;
                cur = cur->m_left;
            }
            else
                return false;
        }

        cur = new Node(kv);
        cur->m_parent = parent;
        if(parent->m_kv.first < kv.first)
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
        return true;
    }

    template<typename K, typename V>
    void rb_tree<K, V>::left_rotate(typename rb_tree<K, V>::Node* parent)
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
    template<typename K, typename V>
    void rb_tree<K, V>::right_rotate(typename rb_tree<K, V>::Node* parent)
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

    template<typename K, typename V>
    void rb_tree<K, V>::left_right_rotate(typename rb_tree<K, V>::Node* parent)
    {
        Node* subL = parent->m_left;
        Node* subLR = subL->m_right;

        left_rotate(subL);
        right_rotate(parent);

    }

    template<typename K, typename V>
    void rb_tree<K, V>::right_left_rotate(typename rb_tree<K, V>::Node* parent)
    {

        Node* subR = parent->m_right;
        Node* subRL = subR->m_left;

        right_rotate(subR);
        left_rotate(parent);


    }

    template<typename K, typename V>
    typename rb_tree<K, V>::Node* rb_tree<K, V>::find(const K& key)
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

    template<typename K, typename V>
    bool rb_tree<K, V>::balanced()
    {
        return height(m_root) != -1;
    }

    template<typename K, typename V>
    int rb_tree<K, V>::height(rb_tree<K, V>::Node* root)
    {
        if(!root)
            return 0;

        int lh = height(root->m_left);
        int rh = height(root->m_right);

        if(lh == -1 || rh == -1 || abs(lh - rh) > 1){
            abort();
            return -1;
        }

        return std::max(lh, rh) + 1;
    }

    template<typename K, typename V>
    bool rb_tree<K, V>::valid()
    {
        int blackNum = 0;
        if(m_root && m_root->m_color == RED)
           assert(false);

        Node* cur = m_root;

        while(cur){
            if(cur->m_color == BLACK)
                blackNum++;
            cur = cur->m_left;
        }
        return dfs(m_root, 0, blackNum);
    }


    template<typename K, typename V>
    bool rb_tree<K,V>::dfs(typename rb_tree<K, V>::Node* root, int black_nums, int target)
    {
        if(!root)
            return true;

        Node* parent = root->m_parent;
        if(root->m_color == RED){
            if(parent->m_color == RED){
                std::cout << "continuous red nodes: " << parent->m_kv.first << " " << root->m_kv.first << std::endl;
                assert(false);
                return false;
            }
        }
        else{
            black_nums++;
        }

        if(root->m_left == nullptr || root->m_right == nullptr){
            if(black_nums != target){
                std::cout << "black_nums != target" << std::endl;
                assert(false);
                return false;
            }

        }

        return dfs(root->m_left, black_nums, target)
            && dfs(root->m_right, black_nums, target);
    }

    template<typename K, typename V>
    void rb_tree<K, V>::traverse(){
        _traverse(m_root);
        std::cout << '\n';
    }
    template<typename K, typename V>
    void rb_tree<K, V>::_traverse(rb_tree<K, V>::Node* root){
        if(!root)
            return;

        _traverse(root->m_left);
        std::cout << root->m_kv.first << ' ' << root->m_kv.second << '\n';
        _traverse(root->m_right);
    }

    template<typename K, typename V>
    void rb_tree<K, V>::clear()
    {
        _clear(m_root);
    }

    template<typename K, typename V>
    void rb_tree<K,V>::_clear(rb_tree<K,V>::Node* root)
    {
        if(!root)
            return;

        _clear(root->m_left);
        _clear(root->m_right);
        delete root;
    }



    template<typename K, typename V>
    typename rb_tree<K,V>::Node* rb_tree<K, V>::getRoot(){
        return m_root;
    }

    template<typename K, typename V>
    bool isvalid_bst(rb_tree_node<K, V>* root)
    {
        if(!root)
            return true;
        rb_tree_node<K,V>* pre = nullptr;
        rb_tree_node<K,V>* cur = root;
        std::stack<rb_tree_node<K,V>*> st;
        while(cur || !st.empty()){
            while(cur){
                st.push(cur);
                cur = cur->m_left;
            }

            cur = st.top();
            st.pop();
            if(pre && pre->m_kv.first >= cur->m_kv.first){
                abort();
                return false;
            }

            pre = cur;
            cur = cur->m_right;
        }
        return true;
    }

    void test_rb_tree()
    {
        std::vector<int> v{8, 3, 1, 10, 6, 4, 7, 14, 13, 4, 2, 6, 1, 3, 5, 15, 7, 16, 14};
        //std::vector<int> v{ 4, 2, 6, 1, 3, 5, 15, 7, 16, 14, 8, 3, 1, 10, 6, 4, 7, 14, 13 };
        rb_tree<int, int> avl;
        for(auto& e : v){
            avl.insert({e,e});
            isvalid_bst(avl.getRoot());
        }
        std::cout << std::endl;
        avl.traverse();

    }

    void test_rb_tree2()
    {
        const int N = 100000;
        std::vector<int> v;
        v.reserve(N);
        srand(time(0));

        for (size_t i = 0; i < N; i++)
        {
            v.push_back(rand()+i);
        }

        rb_tree<int, int> t;
        for (auto e : v)
        {
            t.insert({e, e});
            t.valid();
        }
    }
}




