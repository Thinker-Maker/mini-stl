//
// Created by 董鸿博 on 2024/4/29.
//

#ifndef BSK_BST_HPP
#define BSK_BST_HPP
#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <cassert>
using std::cout;
using std::vector;
using std::stack;

namespace dhb
{
    template<typename K>
    struct bst_node
    {
        bst_node<K>* left = nullptr;
        bst_node<K>* right = nullptr;
        K m_key;
        bst_node(const K& key)
            : m_key(key)
        {}
    };

    template<typename K>
    class bst
    {
       typedef bst_node<K> Node;
    public:
        bst();
        ~bst();
        bool insert(const K& key);
        bool erase(const K& key);
        Node* find(const K& key);
        void traverse();
        void clear();
        friend bool isvalid_bst(Node* root);
        Node* getRoot();
    private:
        void _traverse(Node* root);
        void _clear(Node* root);
    private:
        Node* m_root = nullptr;
    };

    template<typename K>
    bst<K>::bst()
    {
    }


    template<typename K>
    bst<K>::~bst()
    {
       clear();
       m_root = nullptr;
    }

    template<typename K>
    bool bst<K>::insert(const K& key)
    {
        if(m_root == nullptr){
            m_root = new Node(key);
            return true;
        }

        Node* parent = nullptr;
        Node* cur = m_root;
        while(cur){
            if(cur->m_key < key){
                parent = cur;
                cur = cur->right;
            }
            else if(cur->m_key > key){
                parent = cur;
                cur = cur->left;
            }
            else
                abort();
        }

        cur = new Node(key);
        if(parent->m_key < key)
            parent->right = cur;
        else
            parent->left = cur;
        return true;

    }

    template<typename K>
    bool bst<K>::erase(const K& key)
    {
        Node* parent = nullptr;
        Node* cur = m_root;
        while(cur)
        {
            if(cur->m_key < key){
                parent = cur;
                cur = cur->right;
            }
            else if(cur->m_key > key){
                parent = cur;
                cur = cur->left;
            }
            else{
                if(cur->left == nullptr){ //包含了left和right同时为nullptr的情况
                    if(cur == m_root){
                        m_root = m_root->right;
                        delete cur;
                        return true;
                    }

                    if(cur == parent->left)
                        parent->left = cur->right;
                    else
                        parent->right = cur->right;
                    delete cur;
                    return true;
                }
                else if(cur->right == nullptr){
                    if(cur == m_root){
                        m_root = m_root->left;
                        delete cur;
                        return true;
                    }

                    if(cur == parent->left)
                        parent->left = cur->left;
                    else
                        parent->right = cur->left;
                    return true;
                }
                else{
                    Node* rmin_parent = cur;
                    Node* rmin = cur->right;
                    while(rmin->left){
                        rmin_parent = rmin;
                        rmin = rmin->left;
                    }

                    cur->m_key = rmin->m_key;
                    if(rmin_parent == cur)
                        rmin_parent->right = rmin->right;
                    else
                        rmin_parent->left = rmin->right;
                    delete rmin;
                    return true;
                }

            }
        }

        abort();
        return false;
    }

    template<typename K>
    typename bst<K>::Node* bst<K>::find(const K& key)
    {
        Node* cur = m_root;
        while(cur){
            if(cur->key < key)
                cur = cur->right;
            else if(cur->key > key)
                cur = cur->left;
            else
                return cur;

        }
        return nullptr;
    }

    template<typename K>
    void bst<K>::traverse(){
        _traverse(m_root);
        cout << '\n';
    }
    template<typename K>
    void bst<K>::_traverse(bst<K>::Node* root){
        if(!root)
            return;

        _traverse(root->left);
        cout << root->m_key << ' ';
        _traverse(root->right);
    }

    template<typename K>
    void bst<K>::clear()
    {
       _clear(m_root);
    }

    template<typename K>
    void bst<K>::_clear(bst<K>::Node* root)
    {
        if(!root)
            return;

        _clear(root->left);
        _clear(root->right);
        delete root;
    }


    template<typename K>
    bst<K>::Node* bst<K>::getRoot(){
        return m_root;
    }

    template<typename K>
    bool isvalid_bst(bst_node<K>* root)
    {
        if(!root)
            return true;
        bst_node<K>* pre = nullptr;
        bst_node<K>* cur = root;
        stack<bst_node<K>*> st;
        while(cur || !st.empty()){
            while(cur){
                st.push(cur);
                cur = cur->left;
            }

            cur = st.top();
            st.pop();
            if(pre && pre->m_key >= cur->m_key){
                abort();
                return false;
            }

            pre = cur;
            cur = cur->right;
        }
        return true;
    }

    void test_bst()
    {
        vector<int> v{8, 3, 1, 10, 6, 4, 7, 14, 13};
        bst<int> bstree;
        for(auto& e : v){
            bstree.insert(e);
            isvalid_bst(bstree.getRoot());
        }
        bstree.traverse();

        for(auto& e : v){
            cout << e << ' ';
            bstree.erase(e);
            isvalid_bst(bstree.getRoot());
        }

    }
}



#endif //BSK_BST_HPP
