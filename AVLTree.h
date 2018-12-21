#pragma once

template <class T> class Node;
template <class T> class AVLTree;

template <typename T>
class AVLNode {
	friend class AVLTree<T>;
	T value;
	AVLNode<T>* left = nullptr;
	AVLNode<T>* right = nullptr;
	int balance = 0;

public:
	AVLNode(T v, AVLNode<T>* l = nullptr, AVLNode<T> *r = nullptr) :value(v), left(l), right(r) {

	}
};

template<typename T>
class AVLTree
{
	AVLNode<T>* root;

	void Delete(AVLNode<T> *& node) {
		if (node != nullptr) {
			Delete(node->left);
			Delete(node->right);

			delete node;
			node = nullptr;
		}
	}
	//right rotation
	void RR(AVLNode<T> *&node) {
		AVLNode<T> *father = node;
		AVLNode<T> *son = node->left;

		father->left = son->right;
		son->right = father;
		node = son;
		
		// B(A(A0,A1),B1)
		// bB = hB1 - max(hA0,hA1) - 1
		// bA = hA1 - hA0
		// bB = hB1 - hA1
		// bA = max(hA1, hB1) + 1 - hA0
		father->balance++;
		if (son->balance < 0)
			father->balance -= son->balance;

		son->balance++;
		if (father->balance > 0)
			son->balance += father->balance;
	}

	void LR(AVLNode<T>*& node) {
		AVLNode<T>*father = node;
		AVLNode<T>*son = node->right;

		father->right = son->left;
		son->left = father;
		node = son;

		father->balance--;
		if (son->balance > 0)
			father->balance -= son->balance;

		son->balance--;
		if (father->balance < 0)
			son->balance += father->balance;
	}
	
	//返回值为true时,其高度改变了
	//分成insert 还是 remove
	//insert时最多rotate两次,remove时只有当son的balance为0时高度才不变
	bool Rebalance(AVLNode<T>*& node, int dbalance, bool isinsert ) {
		node->balance += dbalance;

		switch (node->balance) {
		case -2:{
			int i = node->left->balance;
			if (i == 1)
				LR(node->left);

			RR(node);
			return !isinsert && i;
		}
		case 2: {
			int i = node->right->balance;
			if (node->right->balance == -1)
				RR(node->right);
			LR(node);
			return !isinsert && i;
		}
		case 0:
			return true ^ isinsert ;
		case -1:
		case 1:
			return false ^ isinsert;
		default:
			break;
		}
	}

	bool Add(const T& item, AVLNode<T> *&node) {
		if (node == nullptr) {
			node = new AVLNode<T>(item);
			return true;
		}
		else if (item > node->value) {
			return Add(item, node->right) && Rebalance(node, 1, true);
		}
		else if (item < node->value) {
			return Add(item, node->left) && Rebalance(node, -1, true);
		}

		//已经有这个值了
		return false;
	}

	T Min(AVLNode<T> *node) {
		AVLNode<T> * n = node;
		while (n->left != nullptr)
			n = n->left;
		return n->value;
	}

	T Max(AVLNode<T> *node) {
		AVLNode<T> * n = node;
		while (n->right != nullptr)
			n = n->right;
		return n->value;
	}

	bool Remove(const T &item, AVLNode<T>*&node) {
		if (node == nullptr) {
			return false;
		}
		else if (item < node->value) {
			return Remove(item, node->left) && Rebalance(node, 1, false);
		}
		else if (item > node->value) {
			return Remove(item, node->right) && Rebalance(node, -1, false);
		}
		else {
			if (node->left == nullptr && node->right == nullptr) {
				Delete(node);

				return true;
			}
			else if (node->left != nullptr && node->right != nullptr) {
				AVLNode<T>* curr_node = node;
				T min_v = Min(node->right);

				bool isHChanged = Remove(min_v, node);
				curr_node->value = min_v;

				return isHChanged;
			}
			else {
				AVLNode<T> *node_remove = node;

				if (node->left == nullptr) {
					node = node_remove->right;
					node_remove->right = nullptr;
				}
				else {
					node = node_remove->left;
					node_remove->left = nullptr;
				}

				Delete(node_remove);

				return true;
			}
		}
	}

public:
	AVLTree() {
	}
	~AVLTree() {
		Delete(root);
	}

	T* Search(T item) {
		AVLNode<T> * node = root;

		while (node != nullptr) {
			if (node->value > item)
				node = node->left;
			else if (node->value < item)
				node = node->right;
			else
				return &node->value;
		}

		return nullptr;
	}

	void Insert(T item) {
		Add(item, root);
	}

	void Remove(T item) {
		Remove(item, root);
	}
};

template<typename X, typename Y> class AVLMap;
template<typename X, typename Y> class AVLCouple;

template<typename X, typename Y>
class AVLCouple {
	friend class AVLMap<X,Y>;
	X x;
	Y y;
};

template<typename X, typename Y>
class AVLMap {
	AVLTree<AVLCouple<X, Y>> tree;

};