#include <iostream>
#include <fstream>
#include <algorithm>
#include "BST.h"
#include<string>


using namespace std;


BST::BST() {
	root = NULL;
	count = 0;
}


bool BST::empty() {
	if (count == 0) return true;
	return false;
}


int BST::size() {
	return count;
}


void BST::preOrderPrint() {
	if (root == NULL) return;// handle special case
	else preOrderPrint2(root);// do normal process
	cout << endl;
}


void BST::preOrderPrint2(BTNode *cur) {

	if (cur == NULL) return;
	cur->item.print(cout);
	preOrderPrint2(cur->left);
	preOrderPrint2(cur->right);
}


void BST::inOrderPrint() {
	if (root == NULL) return;// handle special case
	else inOrderPrint2(root);// do normal process
	cout << endl;
}


void BST::inOrderPrint2(BTNode *cur) {

	if (cur == NULL) return;

	inOrderPrint2(cur->left);
	cur->item.print(cout);
	inOrderPrint2(cur->right);
}


void BST::postOrderPrint() {
	if (root == NULL) return;// handle special case
	else postOrderPrint2(root);// do normal process
	cout << endl;
}


void BST::postOrderPrint2(BTNode *cur) {
	if (cur == NULL) return;
	postOrderPrint2(cur->left);
	postOrderPrint2(cur->right);
	cur->item.print(cout);
}



int BST::countNode() {
	int	counter = 0;
	if (root == NULL) return 0;
	countNode2(root, counter);
	return counter;
}


void BST::countNode2(BTNode *cur, int &count) {
	if (cur == NULL) return;
	countNode2(cur->left, count);
	countNode2(cur->right, count);
	count++;
}


bool BST::findGrandsons(type grandFather) {
	if (root == NULL) return false;
	return (fGS2(grandFather, root));
}


bool BST::fGS2(type grandFather, BTNode *cur) {
	if (cur == NULL) return false;
	//if (cur->item == grandFather) {
	if (cur->item.compare2(grandFather)){

		fGS3(cur, 0);// do another TT to find grandsons
		return true;
	}
	if (fGS2(grandFather, cur->left)) return true;
	return fGS2(grandFather, cur->right);
}


void BST::fGS3(BTNode *cur, int level) {
	if (cur == NULL) return;
	if (level == 2) {
		cur->item.print(cout);
		return;  // No need to search downward
	}
	fGS3(cur->left, level + 1);
	fGS3(cur->right, level + 1);
}



void BST::topDownLevelTraversal() {
	BTNode			*cur;
	Queue		    q;


	if (empty()) return; 	// special case
	q.enqueue(root);	// Step 1: enqueue the first node
	while (!q.empty()) { 	// Step 2: do 2 operations inside
		q.dequeue(cur);
		if (cur != NULL) {
			cur->item.print(cout);

			if (cur->left != NULL)
				q.enqueue(cur->left);

			if (cur->right != NULL)
				q.enqueue(cur->right);
		}
	}
}

//insert for BST
bool BST::insert(type newItem) {
	BTNode	*cur = new BTNode(newItem);
	if (!cur) return false;		// special case 1
	if (root == NULL) {
		root = cur;
		count++;
		return true; 			// special case 2
	}
	insert2(root, cur);			// normal
	count++;
	return true;
}


void BST::insert2(BTNode *cur, BTNode *newNode) {
	//if (cur->item > newNode->item) {
	if (cur->item.compare1(newNode->item)){
		if (cur->left == NULL)
			cur->left = newNode;
		else
			insert2(cur->left, newNode);
	}
	else {
		if (cur->right == NULL)
			cur->right = newNode;
		else
			insert2(cur->right, newNode);
	}
}



bool BST::remove(type item) {
	if (root == NULL) return false; 		// special case 1: tree is empty
	return remove2(root, root, item); 		// normal case
}

bool BST::remove2(BTNode *pre, BTNode *cur, type item) {

	// Turn back when the search reaches the end of an external path
	if (cur == NULL) return false;

	// normal case: manage to find the item to be removed
	//if (cur->item == item) {
	if (cur->item.compare2(item)){
		if (cur->left == NULL || cur->right == NULL)
			case2(pre, cur);	// case 2 and case 1: cur has less than 2 sons
		else
			case3(cur);		// case 3, cur has 2 sons
		count--;				// update the counter
		return true;
	}

	// Current node does NOT store the current item -> ask left sub-tree to check
	//if (cur->item > item)
	if (cur->item.compare1(item))
		return remove2(cur, cur->left, item);

	// Item is not in the left subtree, try the right sub-tree instead
	return remove2(cur, cur->right, item);
}


void BST::case2(BTNode *pre, BTNode *cur) {

	// special case: delete root node
	if (pre == cur) {
		if (cur->left != NULL)	// has left son?
			root = cur->left;
		else
			root = cur->right;

		free(cur);
		return;
	}

	if (pre->right == cur) {		// father is right son of grandfather? 
		if (cur->left == NULL)			// father has no left son?
			pre->right = cur->right;			// connect gfather/gson
		else
			pre->right = cur->left;
	}
	else {						// father is left son of grandfather?
		if (cur->left == NULL)			// father has no left son? 
			pre->left = cur->right;				// connect gfather/gson
		else
			pre->left = cur->left;
	}

	free(cur);					// remove item
}


void BST::case3(BTNode *cur) {
	BTNode		*is, *isFather;

	// get the IS and IS_parent of current node
	is = isFather = cur->right;
	while (is->left != NULL) {
		isFather = is;
		is = is->left;
	}

	// copy IS node into current node
	cur->item = is->item;

	// Point IS_Father (grandfather) to IS_Child (grandson)
	if (is == isFather)
		cur->right = is->right;		// case 1: There is no IS_Father    
	else
		isFather->left = is->right;	// case 2: There is IS_Father

	// remove IS Node
	free(is);
}


bool BST::display(int order, int source) {

	if (root == NULL)
	{
		cout << "\nCannot display an empty tree" << endl;
		return false;
	}
	BTNode* cur = root;

	if (source == 1) {
		display1(cur, order);
		cout << endl;
		return true;
	}
	else if (source == 2) {
		ofstream outFile;
		outFile.open("student-info.txt", ios::trunc);
		outFile.close();
		display2(cur, order);
		cout << "\nSuccessfully output the student list into student-info.txt." << endl;
		return true;
	}
	return false;
}

void BST::display1(BTNode* cur, int order) {

	if (order == 1) {
		if (cur == NULL) return;

		display1(cur->left, order);
		cur->item.print(cout);
		display1(cur->right, order);
	}

	if (order == 2) {
		if (cur == NULL) return;
		display1(cur->right, order);
		cur->item.print(cout);
		display1(cur->left, order);

	}


}

void BST::display2(BTNode* cur, int order) {

	if (order == 1) {
		if (cur == NULL) return;
		display2(cur->left, order);
		ofstream outFile;
		outFile.open("student-info.txt", ios::app);
		cur->item.print(outFile);
		outFile.close();
		display2(cur->right, order);
	}

	if (order == 2) {
		if (cur == NULL) return;
		display2(cur->right, order);
		ofstream outFile;
		outFile.open("student-info.txt", ios::app);
		cur->item.print(outFile);
		outFile.close();
		display2(cur->left, order);

	}
}

bool BST::CloneSubtree(BST t1, type item) {
	if (t1.root == NULL) {	//special case 1:empty tree
		cout << "\nCannot clone subtree from an empty tree." << endl;
		return false;
	}
	BTNode* cur = t1.root;
	if (cur->item.compare2(item)) {	//special case 2: the clone item is same with the root's item 
		insert(cur->item);
		CloneSubtree3(cur->left);
		CloneSubtree3(cur->right);
		return true;
	}

	if (CloneSubtree2(cur->left, item)) return true;	//normal case
	else if (CloneSubtree2(cur->right, item)) {
		return true;
	}
	cout << "\nCannot find the clone item from the tree." << endl;
	return false;//special case 3: cannot find the clone item

}

bool BST::CloneSubtree2(BTNode* cur, type item) {
	if (cur == NULL) return false;
	if (cur->item.compare2(item)) {
		insert(cur->item);
		CloneSubtree3(cur->left);
		CloneSubtree3(cur->right);
		return true;
	}
	if (CloneSubtree2(cur->left, item)) return true;
	else if (CloneSubtree2(cur->right, item)) return true;
	return false;
}

void BST::CloneSubtree3(BTNode* cur) {
	if (cur == NULL) return;
	insert(cur->item);
	CloneSubtree3(cur->left);
	CloneSubtree3(cur->right);
}


bool BST::findDuplicateID(int studentID)
{
	if (root == NULL) return false;  // If the tree is empty, no duplicates
	return findDuplicateID2(studentID, root);  // Start the search from the root
}

bool BST::findDuplicateID2(int studentID, BTNode* cur)
{
	if (cur == NULL) return false;  // Special case: empty subtree
	// If the current node's student ID matches the new student's ID
	if (cur->item.id == studentID) {
		return true;  // Duplicate found
	}
	// Recursively check in the left and right subtrees
	if (findDuplicateID2(studentID, cur->left)) return true;
	return findDuplicateID2(studentID, cur->right);
}

bool BST::printLevelNodes()
{
	if (!root) {
		cout << "\nCannot print nodes for each level because the tree is empty." << endl;
		return false; // Special case: the tree is empty
	}

	int height = getHeight(root); // Get the height of the tree
	for (int i = 1; i <= height; ++i) {
		cout << "Level " << i << " nodes: ";
		printGivenLevel(root, i); // Print All nodes at the current level
		cout << endl;
	}
	cout << endl;
	return true;
}

int BST::getHeight(BTNode* node) // Function to calculate the height of the tree
{
	if (!node) return 0;
	int leftHeight = getHeight(node->left);
	int rightHeight = getHeight(node->right);
	return max(leftHeight, rightHeight) + 1; // Return the maximum height
}
void BST::printGivenLevel(BTNode* node, int level) // Function to print all nodes at a given level
{
	if (!node) return;
	if (level == 1) {
		cout << node->item.id << " "; // Print the student ID at first level
	}
	else if (level > 1) {
		printGivenLevel(node->left, level - 1); //Print left the student ID at current level
		printGivenLevel(node->right, level - 1); //Print right the student ID at current level
	}
}

bool BST::printPath() {
	if (!root) {
		cout << "\nCannot print the external paths because the tree is empty." << endl;
		return false; // Special case: the tree is empty
	}

	cout << "Below are all the external paths for the tree:\n" << endl;
	printPathsUtil(root, ""); // Start with an empty path
	cout << endl;
	return true; // Return true after processing
}


void BST::printPathsUtil(BTNode* node, string path)
{
	if (!node) return; // Speical case: null node

	path += to_string (node->item.id) + " "; // change the id to string and combine with empty space

	// If this is a leaf node, print the path
	if (!node->left && !node->right)
	{
		cout << path << endl;
	}
	else {
		// Otherwise, recurse down the left and right subtrees
		if (node->left) {
			printPathsUtil(node->left, path);
		}
		if (node->right) {
			printPathsUtil(node->right, path);
		}
	}
}

bool BST::deepestNodes() {
	if (root == NULL) return false; // Handle empty tree

	int height = getHeight(root);
	if (height < 0) return false; // Edge case for an empty tree

	// Call the recursive function to print nodes at the deepest level
	printDeepestNodes(root, 1, height);

	cout << endl; // End the printout with a newline
	return true;
}

void BST::printDeepestNodes(BTNode* node, int currentLevel, int targetLevel) {
	if (node == NULL) return;

	if (currentLevel == targetLevel) {
		node->item.print(cout); // Print the student ID
	}
	else {
		printDeepestNodes(node->left, currentLevel + 1, targetLevel);
		printDeepestNodes(node->right, currentLevel + 1, targetLevel);
	}
}


