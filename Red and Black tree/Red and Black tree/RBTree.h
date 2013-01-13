#ifndef STRING
#define STRING std::string
#define C_STRING const char *

#include <iostream>
#include <string>

/*
	Can be used as a key in the Red and Black tree.
*/
class Comparable : public STRING {
public:
	Comparable() : STRING() { }
	Comparable(C_STRING value) : STRING(value) { }

	bool operator < (Comparable& comparable) {
		return compare(comparable.c_str()) < 0;
	}
	bool operator > (Comparable& comparable) {
		return compare(comparable.c_str()) > 0;
		//return lstrcmpW(this->c_str(), comparable.c_str()) > 0;
	}
	bool operator == (Comparable& comparable) {
		return !compare(comparable.c_str());
	}
};

template <class classType, class keyType>
struct Node {
	Node<classType, keyType> *left;
	Node<classType, keyType> *right;
	Node<classType, keyType> *parent;

	classType *data;
	keyType key;

	bool isBlack;
};

template <class classType, class keyType = long>
class RBTree {
	Node<classType, keyType> *_root;

	//Deletes a Node
	//Garbage can never be _root.
	//To qualifie for garbage, a node needs at least a left null child
	void _terminateNode(Node<classType, keyType> *garbage) {
		Node<classType, keyType> *parent = garbage->parent;
		bool leftChild = garbage->parent->left == garbage;


		//Garbage will first be detached before being deleted.
		if(!garbage->isBlack) {
			if(leftChild)
				parent->left = NULL;
			else
				parent->right = NULL;

			delete garbage;
		}
		//Black with red child
		else if(garbage->right != NULL) {
			if(leftChild)
				parent->left = garbage->right;
			else
				parent->right = garbage->right;

			garbage->right->isBlack = !garbage->right->isBlack;
			garbage->right->parent = parent;

			delete garbage;
		}

		    
		//garbage is black.
		else {
			Node<classType, keyType> *sibling;
			
			//Determine if sibling is a leftChild or not.
			leftChild ? sibling = parent->right : sibling = parent->left;

			if(sibling->isBlack) {
				//Black sibling with red nephew.
				if(sibling->left != NULL) {
					//If sibling leftChild //Outer grandchild
					if(!leftChild) {
						sibling->left->isBlack = parent->isBlack;

						_RoR(parent);
					}
					//Inner Grandchid
					else {
						sibling->left->isBlack = parent->isBlack;

						_RoR(sibling);
						_RoL(parent);

						parent->isBlack = true;
					}
					
				}
				else if(sibling->right != NULL) {
					//If sibling leftChild //Outer grandchild
					if(leftChild) {
						sibling->right->isBlack = parent->isBlack;

						_RoL(parent);
					}
					//Inner Grandchid
					else {
						sibling->right->isBlack = parent->isBlack;

						_RoL(sibling);
						_RoR(parent);

						parent->isBlack = true;
					}
				}
				//Black sibling without nephew
				else {
					Node<classType, keyType> *tempParent;
					Node<classType, keyType> *doublyBlack = garbage;

					//Black height can't be restored in one fell swoop.
					while(doublyBlack != NULL) {
						tempParent = doublyBlack->parent;
						doublyBlack == tempParent->left ? 
							sibling = tempParent->right : sibling = tempParent->left;

						leftChild = tempParent->left == doublyBlack;

						//Red sibling //At this stage, a red sibling will always have two children.
						if(!sibling->isBlack) {
							//tempParent->isBlack = false;



							if(!leftChild) {
								Node<classType, keyType> *nephew = sibling->right;
								if(nephew->left != NULL && !nephew->left->isBlack) {
									_RoL(sibling);
									_RoR(tempParent);

									sibling->right->isBlack = true;


								}
								else if(nephew->right != NULL && !nephew->right->isBlack) {
									nephew->isBlack = false;
									nephew->right->isBlack = true;

									//Create condition identical to previous if-statement
									_RoL(nephew);

									_RoL(sibling);
									_RoR(tempParent);

									sibling->right->isBlack = true;
								}
								else {
									sibling->isBlack = true;

									_RoR(tempParent);
									tempParent->left->isBlack = false;
								}
							}
							else {
								Node<classType, keyType> *nephew = sibling->left;
								if(nephew->right != NULL && !nephew->right->isBlack) {
									_RoR(sibling);
									_RoL(tempParent);

									sibling->left->isBlack = true;

								}
								else if(nephew->left != NULL && !nephew->left->isBlack) {
									nephew->isBlack = false;
									nephew->left->isBlack = true;

									//Create condition identical to previous if-statement
									_RoR(nephew);

									_RoR(sibling);
									_RoL(tempParent);

									sibling->left->isBlack = true;
								}
								else {
									sibling->isBlack = true;

									_RoL(tempParent);
									tempParent->right->isBlack = false;
								}
							}

							doublyBlack = NULL;
						}
						//Black sibling
						else if(sibling->left != NULL && !sibling->left->isBlack) {

							//!sibling->left->isBlack;

							//Outer grandchild
							if(!leftChild) {
								sibling->isBlack = tempParent->isBlack;
								_RoR(tempParent);

								sibling->left->isBlack = true;
								sibling->right->isBlack = true;

								//tempParent->isBlack = true;
							}
							//Outer grandchild has precedence over inner grandchild
							else if(sibling->right != NULL && !sibling->right->isBlack) {
								sibling->isBlack = tempParent->isBlack;
								_RoL(tempParent);

								sibling->left->isBlack = true;
								sibling->right->isBlack = true;

							}
							else {
								Node<classType, keyType> *newUncle = sibling->left;
								newUncle->isBlack = tempParent->isBlack;

								_RoR(sibling);
								_RoL(tempParent);


								newUncle->left->isBlack = true;
								newUncle->right->isBlack = true;
							}

							tempParent->isBlack = true;
							doublyBlack = NULL;
						}
						else if(sibling->right != NULL && !sibling->right->isBlack) {
							//tempParent->isBlack = true;



							//Outer grandchild
							if(leftChild) {
								sibling->isBlack = tempParent->isBlack;

								_RoL(tempParent);

								sibling->left->isBlack = true;
								sibling->right->isBlack = true;
							}
							else {
								Node<classType, keyType> *newUncle = sibling->right;
								newUncle->isBlack = tempParent->isBlack;

								_RoL(sibling);
								_RoR(tempParent);

								newUncle->left->isBlack = true;
								newUncle->right->isBlack = true;
							}

							tempParent->isBlack = true;
							doublyBlack = NULL;
						}
						//Red parent
						else if(!tempParent->isBlack){
							tempParent->isBlack = true;
							sibling->isBlack = false;

							doublyBlack = NULL;
						}
						//Propagate doublyblack upward
						else {
							sibling->isBlack = false;

							if(tempParent == _root)
								doublyBlack = NULL;
							else
								doublyBlack = tempParent;
						}

					}
				}
			}
			//Red sibling
			else {
				sibling->isBlack = !sibling->isBlack;

				//Sibling is leftChild
				if(!leftChild) {
					sibling->right->isBlack = !sibling->right->isBlack;
					_RoR(parent);
				}
				else {
					sibling->left->isBlack = !sibling->left->isBlack;
					_RoL(parent);
				}
			}

			if(parent->left == garbage) {
				parent->left = garbage->right;
			}
			else {
				parent->right = garbage->right;
			}

			if(garbage->right != NULL)
				garbage->right->parent = parent;
			delete garbage;
		}
	}

	void _rebalance(Node<classType, keyType> *current, Node<classType, keyType> *parent, Node<classType, keyType> *grandParent) {
		//Violation of rule 3
		if(!current->isBlack && !parent->isBlack) {
							
			bool leftChild = grandParent->left == parent;
			bool leftGrandchild = parent->left == current;

			//Inside grandchild
			if(leftChild != leftGrandchild) {
				grandParent->isBlack = !grandParent->isBlack;
				current->isBlack = !current->isBlack;

				if(leftGrandchild) {
					_RoR(parent);
					_RoL(grandParent);
				}
				else {
					_RoL(parent);
					_RoR(grandParent);
				}
			}
			//Outside grandchild
			else {
				grandParent->isBlack = !grandParent->isBlack;
				parent->isBlack = !parent->isBlack;

				if(leftGrandchild) {
					_RoR(grandParent);
				}
				else {
					_RoL(grandParent);
				}
			}
		}
	}

	void _flip(Node<classType, keyType> *node) {
		if(node != _root) //_root must always be black.
			node->isBlack = !node->isBlack;
		node->left->isBlack = !node->left->isBlack;
		node->right->isBlack = !node->right->isBlack;
	}

	void _RoR(Node<classType, keyType> *node) {
		Node<classType, keyType> *child = node->left;

		if(node != _root) {
			Node<classType, keyType> *greatGrandparent = node->parent;

			//If rightChild
			if(greatGrandparent->left == node) {
				greatGrandparent->left = child;
			}
			else {
				greatGrandparent->right = child;
			}

			child->parent = greatGrandparent;
		}
		else {
			//_root->parent = child;
			_root = child;
			child->parent = NULL;
		}
		

		if(child->right != NULL) {
			node->left = child->right;
			child->right->parent = node;
		}
		else {
			node->left = NULL;
		}

		node->parent = child;
		child->right = node;
	}

	void _RoL(Node<classType, keyType> *node) {
		Node<classType, keyType> *child = node->right;

		if(node != _root) {
			Node<classType, keyType> *greatGrandparent = node->parent;

			//If rightChild
			if(greatGrandparent->right == node) {
				greatGrandparent->right = child;
			}
			else {
				greatGrandparent->left = child;
			}

			child->parent = greatGrandparent;
		}
		else {
			//_root->parent = child;
			_root = child;
			child->parent = NULL;
		}

		
		if(child->left != NULL) {
			node->right = child->left;
			child->left->parent = node;
		} else {
			node->right = NULL;
		}

		node->parent = child;
		child->left = node;
	}

	void _AllocateNode(Node<classType, keyType> *&node, Node<classType, keyType> *parent, classType *data, keyType key) {
		node = new Node<classType, keyType>;

		node->left = NULL;
		node->right = NULL;
		node->parent = parent;

		node->data = data;
		node->key = key;
		node->isBlack = false;
	}

	//To delete an internal node, retrieve a successor node at the bottom of the tree.
	//Those are easier to delete.
	//The successor will replace the current node.
	Node<classType, keyType> *_getSuccessor(Node<classType, keyType> *current) {
		
		if(current->right == NULL) {
			if(current->left != NULL)
				return current->left;
			else
				return current;
		}

		Node<classType, keyType> *parent = current;
		Node<classType, keyType> *grandParent = parent;

		current = current->right;
		
		if(current->left == NULL) {
			return current;
		}


		do {
			grandParent = parent;
			parent = current;
			current = current->left;
		} while(current != NULL);

		return parent;
	}

	//Retrieves Node associated with key.
	Node<classType, keyType> *searchData(keyType key) {
		if(_root == NULL)
			return _root;
		Node<classType, keyType> *current = _root;

		while(current->key != key) {
			if(key < current->key)
				current = current->left;
			else
				current = current->right;
			if(current == NULL) //Key doesnt exist.
				return NULL;
		}

		return current;
	}

	void _terminateSubtree(Node<classType, keyType> *sub_root) {
		if(sub_root == NULL)
			return;

		//cout << endl << sub_root->key << endl;
		_terminateSubtree(sub_root->left);
		_terminateSubtree(sub_root->right);

		delete sub_root->data;
		delete sub_root;

		return;
	}
public:
	RBTree() {
		_root = NULL;
	}
	~RBTree() {
		int count = 0;
		_terminateSubtree(_root);
	}

	//Returns data associated with the key.
	//Returns NULL if no data found.
	classType *getData(keyType key) {
		Node<classType, keyType> *node = searchData(key);

		if(node == NULL)
			return NULL;
		else
			return node->data;
	}

	void insertData(keyType key, classType *data) {
		if(_root == NULL) {
			_AllocateNode(_root, NULL, data, key);
			_root->isBlack = true;
		} 
		else {
			Node<classType, keyType> *current = _root;
			Node<classType, keyType> *parent = current;
			Node<classType, keyType> *grandParent = parent;

			while(true) {
				//Black Node with two red children
				if(current->isBlack 
					&& current->left != NULL && current->right != NULL
					&& !current->left->isBlack && !current->right->isBlack) {
						_flip(current);
						_rebalance(current, parent, grandParent);
				}

				grandParent = parent;
				parent = current;

				if(key < current->key) {
					current = current->left;
					if(current == NULL) {
						_AllocateNode(parent->left, parent, data, key);
						_rebalance(parent->left, parent, grandParent);
						return;
					}
				}
				else if(key > current->key) {
					current = current->right;
					if(current == NULL) {
						_AllocateNode(parent->right, parent, data, key);
						_rebalance(parent->right, parent, grandParent);
						return;
					}
				}
				else {
					return;
				}
			}
		}

		//return NULL; //Duplicate key.
	}
	bool deleteData(keyType key) {
		//cout << "deleteData not supported yet." << endl;
		Node<classType, keyType> *current = _root;
		Node<classType, keyType> *parent;

		current = searchData(key);
		if(current == NULL) {
			return false;
		}

		if(current == _root && (current->left == NULL || current->right == NULL)) {
			if(current->left == NULL && current->right == NULL) {
				delete _root->data;
				delete _root;

				_root = NULL;
			}
			else if(current->left != NULL) {
				current->left->isBlack = true;
				current->left->parent = NULL;

				_root = current->left;

				delete current->data;
				delete current;
			}
			else if(current->right != NULL) {
				current->right->isBlack = true;
				current->right->parent = NULL;

				_root = current->right;

				delete current->data;
				delete current;
			}

			return true;
		}

		parent = current->parent;
		if(current->left == NULL && current->right == NULL) {
			if(current == parent->left) {

				delete current->data;
				_terminateNode(current);
				parent->left = NULL;
			}
			else { //current == parent->right)

				delete current->data;
				_terminateNode(current);
				parent->right = NULL;
			}
		}
		else {
			Node<classType, keyType> *successor = _getSuccessor(current);

			delete current->data;
			current->data = successor->data;

			current->key = successor->key;
			_terminateNode(successor);
		}
		return true;
	}
};

#endif