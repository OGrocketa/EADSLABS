//
// Created by Yaraslau Sedach on 12/01/2024.
//

#ifndef AVL_TREE_AVL_TREE_H
#define AVL_TREE_AVL_TREE_H

#include <queue>
#include <iostream>
#include <utility>
#include <vector>
#include<algorithm>
#include <chrono>

template<typename Key,typename Info>
class avl_tree{
private:
    class Node{
    private:
        Node*left;
        Node*right;
        int height;

    public:
        Key key;
        Info info;
        Node(const Key &_key, const Info &_info, Node *left = nullptr, Node *right = nullptr)
                : key(_key), info(_info), left(left), right(right), height(1) {}
                
        friend class avl_tree;

    };

    Node*root = nullptr;
    int size =0;    //Total amount of elements in the tree

    //Helper method for clear()
    void clearPrivate(Node*node){
        if(node){

            clearPrivate(node->left);
            clearPrivate(node->right);

            delete node;
        }
    }

    //Helper method for assignment operator
    Node* assignmentPrivate(const Node* src){
        if(!src) return nullptr;    //check for assigning a nullptr
        
        Node* result = new Node(src->key,src->info);    //Copying a node

        //Recursively traverse the tree
        result->left = assignmentPrivate(src->left);
        result->right = assignmentPrivate(src->right);

        result->height = src->height;
        
        return result;
    }

    //Helper method for insert
    Node* insertPrivate(Node*& node, const Key& key, const Info&info){
        //Insertion part
        if (!node) {
            size++;
            node = new Node(key, info);
            return node;
        }
        Node* found;
        //Searching for the right place part
        if (key < node->key) {
            found = insertPrivate(node->left, key, info);
        } else if (key > node->key) {
            found = insertPrivate(node->right, key, info);
        } else {   //node with given key found -> update info
            node->info = info;
            found = node;
        }
        balance(node);
        return found;
    }

    //Helper method for updating the height
    void updateHeight(Node*node){
        if(node){

            //Calculate the height of the children nodes
            int lHeight = (node->left) ? node->left->height : 0;
            int rHeight = (node->right) ? node->right->height : 0;

            node->height = 1 + std::max(lHeight,rHeight);
        }
    }

    //Helper method for balancing the tree
    void balance(Node*&node){
        updateHeight(node); //just in case update

        int balanceFactor = getBalanceFactor(node);

        if(balanceFactor > 1){  //left heavy node

            if(getBalanceFactor(node->left) <0){    //LR heavy
                leftRotate(node->left);
            }
            rightRotate(node);   //LL heavy
        }

        if(balanceFactor < -1){  //right heavy node

            if(getBalanceFactor(node->right) > 0){    //RL heavy
                rightRotate(node->right);
            }
            leftRotate(node);   //RR heavy
        }

        //balanced
    }

    //Helper method to get balance factor of the given node
    int getBalanceFactor(Node*node){
        if(!node) return 0;
        
        int lHeight = (node->left) ? node->left->height : 0;
        int rHeight = (node->right) ? node->right->height : 0;
        
        return lHeight - rHeight;
    }

    void leftRotate(Node*& toRotate) {
        Node* newRoot = toRotate->right;
        toRotate->right = newRoot->left;
        newRoot->left = toRotate;

        updateHeight(toRotate);
        toRotate = newRoot;
        updateHeight(toRotate);
    }


    void rightRotate(Node*& toRotate){
        Node*newRoot = toRotate->left;
        toRotate->left = newRoot->right;
        newRoot->right = toRotate;

        updateHeight(toRotate);
        toRotate = newRoot;
        updateHeight(toRotate);
        
    }

    //Non-const helper method for finding the node with the given key
    Node* findPrivate(Node*node, const Key& key){
        if(!node) return nullptr;

        if(key == node->key) return node;
        else if(key < node->key) return findPrivate(node->left,key);
        else return findPrivate(node->right,key);
    }
    
    //Const helper method for searching
    Node* findPrivate(Node*node, const Key& key)const{

        if(!node) return nullptr;
        else if(key == node->key) return node;

        if(key < node->key) return findPrivate(node->left, key);
        else return findPrivate(node->right, key);
    }

    //Helper method to finding the node with minimum key value
    Node *findMinValue(Node *node) const
    {
        if (!node->left) return node;   //Found the most left node
        
        return findMinValue(node->left);    //Keep searching
    }

    //Helper method to finding the node with maximum key value
    Node *findMaxValue(Node*node)const{
        if(!node->right)return node;

        return findMaxValue(node->right);
    }

    bool removePrivate(Node *&node, const Key &key)
    {
        if (!node)  return false; // node not found

        if(root->height == 0) return false;//empty tree

        bool removed = false;   //variable will help to track if not was deleted

        //Searching for the required node
        if (node->key > key)
        {
            removed = removePrivate(node->left, key);
        }
        else if (node->key < key)
        {
            removed = removePrivate(node->right, key);
        }
        else    //node found
        {
            if (!node->left || !node->right)    //if node has 1 or no children
            {
                Node *toDel = node->left ? node->left : node->right;

                if (!toDel) // no child
                {
                    toDel = node;
                    node = nullptr;
                }
                else // one child
                {
                    *node = *toDel; // Copy the contents of the child to the current node.
                }

                delete toDel;
                size--;
            }
            else    //Case 2 children nodes
            {
                //find the inorder successor (smallest in the right subtree)
                Node *successor = findMinValue(node->right);

                node->key = successor->key;
                node->info = successor->info;

                //Remove successor node
                removePrivate(node->right, successor->key);
            }

            removed = true;
        }

        balance(node);
        return removed;
    }

    
    
    //Helper method to traverse the tree from left to right
    void traverse(const Node* node, std::function<void (const Key&, const Info&)> fn) const {
        if (node) {
            traverse(node->left, fn);
            fn(node->key, node->info);
            traverse(node->right, fn);
        }
    }
    
    
public:
    //Constructor
    avl_tree(){};

    //Copy Constructor
    avl_tree(const avl_tree& src){
        *this = src;
    };

    //Destructor
    ~avl_tree(){
        clear();
    };

    avl_tree& operator=(const avl_tree& src){
        if(this!=&src){

            clear();

            root = assignmentPrivate(src.root);
            this->size = src.size;
        }
        
        return *this;
    }
    //Const method to find info of the given key in the tree
    const Info &operator[](const Key& key)const{
        Node* node = findPrivate(root,key);

        if(!node)throw std::runtime_error("Key not in the tree");

        return node->info;
    }

    //Non-const method to find info of the given key
    Info &operator[](const Key& key){
        Node* node = findPrivate(root,key);

        if(!node){
            node = insertPrivate(root, key, Info());
        }

        return node->info;
    }

    void insert(const Key& key, const Info& info){
        insertPrivate(root,key,info);
    }
    
    void clear(){
        clearPrivate(root);
        root = nullptr;
        size =0;
    }

    int getSize()const{
        return size;
    }

    //Return 1 if empty
    bool isEmpty()const{
        return size ==0;
    }
    
    bool remove(const Key&key){
        return removePrivate(root,key);
    }

    //Check if the given key is in the tree return 1 if found
    bool find(const Key& key)const{
        if(findPrivate(root,key)) return true;
        return false;
    }

    //Traverse tree left root right
    void traverse(std::function<void (const Key&, const Info&)> fn) const {
        traverse(root, fn);
    }

    //can print up to 10 elements if tree is bigger it will print first 10 elements, prints rows
    //This function is just for more visual testing
    void printTree() const {
        if (!root) {
            std::cout << "The tree is empty." << std::endl;
            return;
        }

        std::queue<Node*> nodesQueue;
        nodesQueue.push(root);
        int count = 0;

        while (!nodesQueue.empty() && count < 10) {
            Node* currentNode = nodesQueue.front();
            nodesQueue.pop();

            std::cout << "Key: " << currentNode->key
                      << ", Info: " << currentNode->info
                      << ", Height: " << currentNode->height << std::endl;

            if (currentNode->left) {
                nodesQueue.push(currentNode->left);
            }
            if (currentNode->right) {
                nodesQueue.push(currentNode->right);
            }

            count++;
        }
    }
};

//Additional task

template <typename Key, typename Info>
std::vector<std::pair<Key, Info>> maxinfo_selector(const avl_tree<Key, Info>& tree, unsigned int cnt) {

    std::vector<std::pair<Key, Info>> res, maxInfo;

    tree.traverse([&](const Key& key, const Info& info) {   //Traverse the tree and add all pairs to the maxInfo vector
        maxInfo.push_back(std::make_pair(key, info));
    });

    // Sort vector in descending order based on the info
    std::sort(maxInfo.begin(), maxInfo.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    // Add cnt elements from the vector to res
    for (int i = 0; i < cnt && i < maxInfo.size(); ++i) {
        res.push_back(maxInfo[i]);
    }

    return res;
}

avl_tree<std::string ,int> count_words(std::istream& is){
    avl_tree<std::string,int> treeRes;

    std::string word;
    while(is >> word){
        treeRes[word]++;  //If the word doesn't exist in the tree, it is added with a count of 1.
                                //If the word already exists, its count is incremented.
    }

    return treeRes;
}

#endif //AVL_TREE_AVL_TREE_H
