#include <cassert>
#include <string>
#include <fstream>   // For std::ifstream
#include <sstream>   // For std::stringstream
#include <filesystem>

#include "avl_tree.h"
//In some tests there is a printTree function which you can uncomment to see how the tree looks like


void test_assignment_operator() {
    avl_tree<int, std::string> tree1, tree2,tree3;

    //Empty to empty
    tree2 = tree1;
    assert(tree2.isEmpty() == true);

    // Non-empty with 1 node to empty
    tree1.insert(1, "One");
    tree2 = tree1;
    assert(tree2.getSize() == 1);

    // Non-empty with multiple nodes to empty
    tree1.insert(2, "Two");
    tree1.insert(3, "Three");
    tree2.clear();
    tree2 = tree1;
    assert(tree2.getSize() == 3);

    //Self-assignment
    tree1 = tree1;  // Self-assignment
    assert(tree1.getSize() == 3);

    // Check if we're really making a deep copy by updating tree 1 to see if tree 2 will change
    tree1.insert(4, "Four");
    assert(tree2.getSize() == 3);

    //Non-empty = empty
    tree1 = tree3;
    assert(tree1.getSize() == 0);

    //Non-empty to non-empty
    tree1.insert(1, "One");
    tree1.insert(2, "Two");
    tree1.insert(3, "Three");
    tree3.insert(5, "Five");
    tree3.insert(6, "Six");
    tree3.insert(7, "Seven");
    tree1 = tree3;
    assert(tree1[5] == "Five");
    assert(tree1[6] == "Six");
    assert(tree1[7] == "Seven");
    assert(tree1.getSize()==3);

}

void test_insert_isEmpty_getSize_clear(){
    avl_tree<int, std::string> tree1;
    //Try to clear an empty tree
    tree1.clear();

    //Check if empty on empty tree
    assert(tree1.isEmpty()==1);

    //Check size on empty tree
    assert(tree1.getSize() == 0);

    //Add elements
    tree1.insert(1,"One");
    tree1.insert(2,"Two");
    tree1.insert(3,"Three");
    tree1.insert(4,"Four");
    tree1.insert(5,"Five");
    tree1.insert(6,"Six");

    //Check if empty again
    assert(tree1.isEmpty()==0);

    //Check size on empty tree
    assert(tree1.getSize() == 6);

//    tree1.printTree();
}

void test_remove(){
    avl_tree<int, std::string> tree1;
    //Try to remove non-existing element from an empty tree
    tree1.remove(1);

    tree1.insert(1,"One");
    //Remove the only element from the tree
    tree1.remove(1);
    assert(tree1.isEmpty());
    assert(tree1.getSize()==0);

    tree1.insert(2,"Two");
    tree1.insert(3,"Three");
    tree1.insert(4,"Four");
    tree1.insert(5,"Five");
    tree1.insert(6,"Six");
    tree1.insert(7,"Seven");
    tree1.insert(8,"Eight");
    tree1.insert(9,"Nine");
    tree1.insert(10,"Ten");

    //Try to remove non-existing element from the tree
    tree1.remove(11);

    //Try to remove an existing element
    tree1.remove(8);
    tree1.remove(7);

    //Let's now see how tree looks like
//    tree1.printTree();

    //from print, we can see that remove is working correctly, and it adjusts the tree while removing elements correctly

}

void test_find(){
    avl_tree<int, std::string> tree1;
    //Try to find an element in an empty tree
    assert(!tree1.find(1));
    tree1.insert(1,"One");
    tree1.insert(2,"Two");
    tree1.insert(3,"Three");
    tree1.insert(4,"Four");
    tree1.insert(5,"Five");
    tree1.insert(6,"Six");
    tree1.insert(7,"Seven");
    tree1.insert(8,"Eight");
    tree1.insert(9,"Nine");
    tree1.insert(10,"Ten");

    //Try to find non-existing element
    assert(!tree1.find(100));

    //Try to find existing element
    assert(tree1.find(6));
}

void tests_task_one(){
    test_assignment_operator();
    test_insert_isEmpty_getSize_clear();
    test_remove();
    test_find();
    std::cout<<"Task 1 tests passed!"<<std::endl;
}

void test_maxinfo_selector() {
    avl_tree<std::string, int> tree;
    tree.insert("One", 1);
    tree.insert("Two", 2);
    tree.insert("Five", 5);
    tree.insert("Twelve", 12);
    tree.insert("Hundred", 100);
    tree.insert("Fifteen", 15);
    tree.insert("Six", 6);
    tree.insert("Twenty", 20);
    tree.insert("Fifty-five", 55);
    tree.insert("Nine", 9);

    // Get the top 3 elements with the highest info values
    std::vector<std::pair<std::string, int>> topElements = maxinfo_selector(tree, 3);

    // Print the results
    std::cout << "Top 3 elements (by info value):" << std::endl;
    for (const auto& elem : topElements) {
        std::cout << "Key: " << elem.first << ", Info: " << elem.second << std::endl;
    }
}

int test_count_words(){
    for (int rep = 0; rep < 5; ++rep)
    {
        std::ifstream is("beagle_voyage.txt");
        if (!is)
        {
            std::cout << "Error opening input file.\n";
            return 1;
        }
        auto start_time = std::chrono::high_resolution_clock::now();
        std::string word;
        avl_tree<std::string, int> wc; // counting word occurrences in the stream
        while (is >> word)
        {
            wc[word]++;
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time = end_time - start_time;
        std::cout << "Ellapsed time: " << time/std::chrono::milliseconds(1)
                  << " ms.\n";
    }
    return 0;
}

void tests_task_two(){
    test_maxinfo_selector();
    test_count_words();

}
int main() {
    tests_task_one();
    tests_task_two();
}
