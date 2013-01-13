#include "RBTree.h"
#include <iostream>


int main() {
	RBTree<int, Comparable> tree;

	tree.insertData("Number 5", new int(5));
	tree.insertData("Number 7", new int(7));
	tree.insertData("Number 9", new int(9));
	tree.insertData("Number 1000", new int(1000));

	std::cout << *tree.getData("Number 9") << std::endl;

	tree.deleteData("Number 9");

	std::cout << (int)(tree.getData("Number 9") == NULL) << std::endl;

	return 0;
}