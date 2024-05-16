//
// Created by 董鸿博 on 2024/4/29.
//

#ifndef AVL_AVL_HPP
#define AVL_AVL_HPP
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <iostream>
#include <utility>
#include <stack>
#include <vector>

namespace dhb
{
    template<typename K, typename V>
    struct avl_tree_node
    {
        avl_tree_node<K,V>* m_left = nullptr;
        avl_tree_node<K,V>* m_right = nullptr;
        avl_tree_node<K, V>* m_parent =nullptr;
        int m_bf = 0;
        std::pair<K, V> m_kv;
        avl_tree_node(const std::pair<K,V>& kv)
            :m_kv(kv)
        {}
    };

    template<typename K, typename V>
    class avl_tree
    {
        typedef avl_tree_node<K, V> Node;
    public:
        avl_tree();
        ~avl_tree();
        bool insert(const std::pair<K, V>& kv);
        Node* find(const K& key);
        void traverse();
        void clear();
        friend bool isvalid_bst(Node* root);
        Node* getRoot();
        bool balanced();
    private:
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
    avl_tree<K,V>::avl_tree()
    {
    }


    template<typename K, typename V>
    avl_tree<K,V>::~avl_tree()
    {
        clear();
        m_root = nullptr;
    }

    template<typename K, typename V>
    bool avl_tree<K,V>::insert(const std::pair<K, V>& kv)
    {
        if(m_root == nullptr){
            m_root = new Node(kv);
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
        //`std::cout << cur->m_kv.first << std::endl;
        cur->m_parent = parent;
        if(parent->m_kv.first < kv.first)
            parent->m_right = cur;
        else
            parent->m_left = cur;
        while(parent){
            if(cur == parent->m_left)
                parent->m_bf--;
            else
                parent->m_bf++;

            if(parent->m_bf == 2 || parent->m_bf == -2){
                if(parent->m_bf == 2 && cur->m_bf == 1)
                    left_rotate(parent);
                else if(parent->m_bf == -2 && cur->m_bf == -1)
                    right_rotate(parent);
                else if(parent->m_bf == 2 && cur->m_bf == -1)
                    right_left_rotate(parent);
                else if(parent->m_bf == -2 && cur->m_bf == 1)
                    left_right_rotate(parent);
                break;
            }
            else if(parent->m_bf == 1 || parent->m_bf == -1){
                cur = parent;
                parent = cur->m_parent;
            }
            else if(parent->m_bf == 0){
                break;
            }
            else{
                abort();
            }

        }
        return true;

    }

    template<typename K, typename V>
    void avl_tree<K, V>::left_rotate(typename avl_tree<K, V>::Node* parent)
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

        parent->m_bf = subR->m_bf = 0;
    }
    template<typename K, typename V>
    void avl_tree<K, V>::right_rotate(typename avl_tree<K, V>::Node* parent)
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

        parent->m_bf = subL->m_bf = 0;
    }
    template<typename K, typename V>
    void avl_tree<K, V>::left_right_rotate(typename avl_tree<K, V>::Node* parent)
    {
        Node* subL = parent->m_left;
        Node* subLR = subL->m_right;
        int bf = subLR->m_bf;

        left_rotate(subL);
        right_rotate(parent);

        if(bf == 0){
           //do-nothing
        }
        else if(bf == 1){
            parent->m_bf = 0;
            subL->m_bf = -1;
            subLR->m_bf = 0;
        }
        else if(bf == -1){
            parent->m_bf = 1;
            subL->m_bf = 0;
            subLR->m_bf = 0;
        }
        else{
            abort();
        }
    }
    template<typename K, typename V>
    void avl_tree<K, V>::right_left_rotate(typename avl_tree<K, V>::Node* parent)
    {

        Node* subR = parent->m_right;
        Node* subRL = subR->m_left;
        int bf = subRL->m_bf;

        right_rotate(subR);
        left_rotate(parent);
        if(bf == 0){
            //do_nothihg
        }
        else if(bf == 1){
            parent->m_bf = -1;
            subR->m_bf = 0;
            subRL->m_bf = 0;
        }
        else if(bf == -1){
            parent->m_bf = 0;
            subR->m_bf = 1;
            subRL->m_bf = 0;
        }
        else{
            abort();
        }


    }

    template<typename K, typename V>
    typename avl_tree<K, V>::Node* avl_tree<K, V>::find(const K& key)
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
    bool avl_tree<K, V>::balanced()
    {
        return height(m_root) != -1;
    }

    template<typename K, typename V>
    int avl_tree<K, V>::height(avl_tree<K, V>::Node* root)
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
    void avl_tree<K, V>::traverse(){
        _traverse(m_root);
        std::cout << '\n';
    }
    template<typename K, typename V>
    void avl_tree<K, V>::_traverse(avl_tree<K, V>::Node* root){
        if(!root)
            return;

        _traverse(root->m_left);
        std::cout << root->m_kv.first << ' ' << root->m_kv.second << '\n';
        _traverse(root->m_right);
    }

    template<typename K, typename V>
    void avl_tree<K, V>::clear()
    {
        _clear(m_root);
    }

    template<typename K, typename V>
    void avl_tree<K,V>::_clear(avl_tree<K,V>::Node* root)
    {
        if(!root)
            return;

        _clear(root->m_left);
        _clear(root->m_right);
        delete root;
    }



    template<typename K, typename V>
    typename avl_tree<K,V>::Node* avl_tree<K, V>::getRoot(){
        return m_root;
    }

    template<typename K, typename V>
    bool isvalid_bst(avl_tree_node<K, V>* root)
    {
        if(!root)
            return true;
        avl_tree_node<K,V>* pre = nullptr;
        avl_tree_node<K,V>* cur = root;
        std::stack<avl_tree_node<K,V>*> st;
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

    void test_avl_tree()
    {
        //std::vector<int> v{8, 3, 1, 10, 6, 4, 7, 14, 13};
        std::vector<int> v{ 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
        avl_tree<int, int> avl;
        for(auto& e : v){
            avl.insert({e,e});
            isvalid_bst(avl.getRoot());
            std::cout << avl.balanced() << ' ';
        }
        std::cout << std::endl;
        avl.traverse();

    }

    void test_avl_tree2()
    {
        const int N = 60000;
        std::vector<int> v;
        v.reserve(N);
        srand(time(0));

        for (size_t i = 0; i < N; i++)
        {
            v.push_back(rand()+i);
        }

        avl_tree<int, int> t;
        for (auto e : v)
        {
            t.insert({e, e});
            t.balanced();
        }
    }
}




#endif //AVL_AVL_HPP
