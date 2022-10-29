#pragma once

#include <iostream>
#include <utility>
#include<vector>
using std::pair;
using std::cout;
using std::cin;

enum color
{
    RED,
    BLACK
};

template<class T>
struct RBTreeNode
{
    RBTreeNode<T>* _left;
    RBTreeNode<T>* _right;
    RBTreeNode<T>* _parent;

    color _col;
    T _data;

    RBTreeNode(T data) :_left(nullptr), _right(nullptr), _parent(nullptr), _col(RED), T(data) {}
};

template<class K, class T, class KeyOfT>
class RBTree
{
    typedef RBTreeNode<K> Node;

public:

    RBTree() :_root(nullptr) {}

    pair<Node*, bool> insert(const T& data)
    {
        if (_root == nullptr)
        {
            _root = new Node(data);
            _root->_col = BLACK;
            return true;
        }

        KeyOfT kov;
        Node* cur = _root;
        Node* parent = _root;

        while (cur)
        {
            if (kov(data) < kov(cur->_data))
            {
                parent = cur;
                cur = cur->_left;
            }
            else if (kov(data) > kov(cur->_data))
            {
                parent = cur;
                cur = cur->_right;
            }
            else
            {
                cout << "data exist." << std::endl;
            }
        }

        //insert new node
        cur = new Node(kv);
        cur->_col = RED;
        if (kov(parent->_data) < kov(data))
        {
            parent->_right = cur;
            cur->_parent = parent;
        }
        else
        {
            parent->_left = cur;
            cur->_parent = parent;
        }

        while (parent && parent->_col == RED)
        {
            Node* grandParent = parent->_parent;
            if (parent == grandParent->_left)
            {
                Node* uncle = grandParent->_right;
                //if uncle exist and is red color. renew uncle && parent's color
                if (uncle && uncle->_col == RED)
                {
                    parent->_col = BLACK;
                    uncle->_col = BLACK;
                }
                else//if uncle doesn't exist or uncle is black. means unbalance need to rotate
                {
                    if (cur == parent->_left)
                    {
                        RotateR(grandParent);
                        parent->_col = BLACK;
                        grandParent->_col = RED;
                    }
                    else
                    {
                        RotateL(parent);
                        RotateR(grandParent);
                        grandParent->_col = RED;
                        cur->_col = BLACK;
                    }
                }
            }
            else if (parent == grandParent->_right)
            {
                Node* uncle = grandParent->_left;
                //if uncle exist and is red color. renew uncle && parent's color
                if (uncle && uncle->_col == RED)
                {
                    parent->_col = BLACK;
                    uncle->_col = BLACK;
                }
                else//if uncle doesn't exist or uncle is black. means unbalance need to rotate
                {
                    if (cur == parent->_right)
                    {
                        RotateL(grandParent);
                        parent->_col = BLACK;
                        grandParent->_col = RED;
                    }
                    else
                    {
                        RotateR(parent);
                        RotateL(grandParent);
                        grandParent->_col = RED;
                        cur->_col = BLACK;
                    }
                }
            }
            cur = grandParent;
            cur->_col = RED;
            parent = cur->_parent;

            _root->_col = BLACK;
        }
        return true;
    }

    bool isBalance()
    {
        int count = 0;
        Node* cur = _root;
        while (cur)
        {
            if (cur->_col == BLACK)
                count++;
            cur = cur->_left;
        }
        int num = 0;
        return _isBalance(_root, num, count);
    }
    void PreOrder()
    {
        _preOrder(_root);
    }

private:
    Node* _root;
    void _preOrder(Node* root)
    {
        if (!root)
            return;
        _preOrder(root->_left);
        cout << root->_kv.first << "->" << root->_kv.second << std::endl;
        _preOrder(root->_right);
    }
    void RotateR(Node* parent)
    {
        //left side have more nodes, rorate to the right
        Node* subL = parent->_left;
        Node* subLR = subL->_right;
        Node* grandparent = parent->_parent;
        parent->_left = subLR;
        subL->_right = parent;
        parent->_parent = subL;
        if (subLR)
        {
            subLR->_parent = parent;
        }


        if (parent == _root)
        {
            _root = subL;
            _root->_parent = nullptr;
        }
        else
        {
            if (grandparent->_left == parent)
            {
                grandparent->_left = subL;
            }
            else
            {
                grandparent->_right = subL;
            }

            subL->_parent = grandparent;
        }

    }

    void RotateL(Node* parent)
    {
        //right side have more nodes, rorate to the left

        Node* grandparent = parent->_parent;
        Node* subR = parent->_right;
        Node* subRL = subR->_left;
        parent->_right = subRL;
        subR->_left = parent;
        parent->_parent = subR;
        if (subRL)
        {
            subRL->_parent = parent;
        }

        if (parent == _root)
        {
            _root = subR;
            _root->_parent = nullptr;
        }
        else
        {
            if (grandparent->_left == parent)
            {
                grandparent->_left = subR;
            }
            else
            {
                grandparent->_right = subR;
            }

            subR->_parent = grandparent;
        }
    }
    bool _isBalance(Node* root, int BlackNodeCount, int refNodeCount)
    {
        if (!root)
        {
            if (BlackNodeCount == refNodeCount)
                return true;
            return false;
        }

        if (root->_col == BLACK)
            BlackNodeCount++;

        if (root->_parent && root->_col == RED && root->_parent->_col == RED)
            return false;

        return _isBalance(root->_left, BlackNodeCount, refNodeCount) && _isBalance(root->_right, BlackNodeCount, refNodeCount);
    }
};
