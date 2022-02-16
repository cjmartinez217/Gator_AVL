#include <iostream>
#include <vector>
#include <queue>
#include <iterator>
#include <sstream>
#include <regex>
using namespace std;


struct Node
{
	string name;
	int id;
	Node* left;
	Node* right;
	int height;

	Node()
	{
		name = "";
		id = 0;
		left = nullptr;
		right = nullptr;
		height = 0;
	}

	Node(string name, int id)
	{
		// overloaded constructor
		this->name = name;
		this->id = id;
		left = nullptr;
		right = nullptr;
		height = 0;
	}


	int getHeight(Node* node)
	{
		// return height of the passed in node
		return height;
	}
};

class AVLTree
{
	// right rotate
	Node* rotateRight(Node* node)
	{
		Node* middle = node->left;
		if (middle->right != nullptr)
			node->left = middle->right;
		else
			node->left = nullptr;
		middle->right = node;

		updateHeight(node);

		return middle;
	}

	// left rotate
	Node* rotateLeft(Node* node)
	{
		Node* middle = node->right;
		if (middle->left != nullptr)
			node->right = middle->left;
		else
			node->right = nullptr;
		middle->left = node;

		updateHeight(node);

		return middle;
	}

	// left right rotate
	Node* rotateLeftRight(Node* node)
	{
		node->left = rotateLeft(node->left);
		return rotateRight(node);
	}

	// right left rotate
	Node* rotateRightLeft(Node* node)
	{
		node->right = rotateRight(node->right);
		return rotateLeft(node);
	}

public:
	Node* head = nullptr;

	// insert function
	Node* insert(Node* root, string& name, int id)
	{
		//Code this function and ensure height of a node and all other nodes are adjusted as new elements are inserted
		if (root == nullptr)
			return new Node(name, id);

		if (id == root->id)
		{
			name = "unsuccessful";
			return root;
		}

		if (id < root->id)
			root->left = insert(root->left, name, id);

		if (id > root->id)
			root->right = insert(root->right, name, id);

		root = balanceTree(root);

		updateHeight(root);

		return root;
	}

	// potential insert helper function 
	Node* balanceTree(Node* root)
	{
		int avlFactor = getAVL(root);
		if (avlFactor > 1)
		{
			if (getAVL(root->left) > 0)
				root = rotateRight(root);
			else
				root = rotateLeftRight(root);
		}
		if (avlFactor < -1)
		{
			if (getAVL(root->right) < 0)
				root = rotateLeft(root);
			else
				root = rotateRightLeft(root);
		}

		return root;
	}

	void updateHeight(Node* root)
	{
		int leftHeight = -1;
		int rightHeight = -1;
		if (root->left != nullptr)
			leftHeight = root->left->height;
		if (root->right != nullptr)
			rightHeight = root->right->height;
		root->height = 1 + max(leftHeight, rightHeight);
	}

	int getAVL(Node* root)
	{
		int leftHeight = (root->left == nullptr) ? -1 : root->left->height;
		int rightHeight = (root->right == nullptr) ? -1 : root->right->height;
		return leftHeight - rightHeight;
	}

	// print functions
	string printInorder(Node* root)
	{
		string str;
		if (root->left != nullptr)
			str += printInorder(root->left);
		str += root->name + ", ";
		if (root->right != nullptr)
			str += printInorder(root->right);
		return str;
	}

	string printPreorder(Node* root)
	{
		string str;
		str += root->name + ", ";
		if (root->left != nullptr)
			str += printPreorder(root->left);
		if (root->right != nullptr)
			str += printPreorder(root->right);
		return str;
	}

	string printPostorder(Node* root)
	{
		string str;
		if (root->left != nullptr)
			str += printPostorder(root->left);
		if (root->right != nullptr)
			str += printPostorder(root->right);
		str += root->name + ", ";
		return str;
	}


	// remove function
	Node* remove(Node* root, int id, bool& complete)
	{
		if (root == nullptr)
			return root;
		else if (root->id > id)
			root->left = remove(root->left, id, complete);
		else if (root->id < id)
			root->right = remove(root->right, id, complete);
		else
		{
			complete = true;
			if (root->left == nullptr && root->right == nullptr)
			{
				delete root;
				return nullptr;
			}
			else if (root->left == nullptr)
			{
				Node* temp = root->right;
				delete root;
				return temp;
			}
			else if (root->right == nullptr)
			{
				Node* temp = root->left;
				delete root;
				return temp;
			}
			else
			{
				Node* temp = leftmostNode(root->right);
				root->name = temp->name;
				root->id = temp->id;
				root->right = remove(root->right, temp->id, complete);
			}
		}
		return root;
	}

	// removeInorder
	Node* removeInorder(Node* root, int n, bool& complete)
	{
		int count = 0;
		Node* desiredNode = findNthNode(root, n, count);
		if(count == n)
			root = remove(root, desiredNode->id, complete);
		return root;
	}

	Node* findNthNode(Node* root, int n, int& count)
	{
		if (root->left != nullptr)
			findNthNode(root->left, n, count);
		if (n == count)
			return root;
		else
			count++;
		if (root->right != nullptr)
			findNthNode(root->right, n, count);
	}

	Node* leftmostNode(Node* root)
	{
		if (root->left == nullptr)
			return root;
		return leftmostNode(root->left);
	}

	// search function
	string search(Node* root, int id)
	{
		string str = "";
		if (root->id == id)
			str += root->name;
		if(root->left != nullptr)
			str += search(root->left, id);
		if(root->right != nullptr)
			str += search(root->right, id);
		return str;
	}

	void search(Node* root, string name, vector<int>& ids)
	{
		if (root->name.compare(name) == 0)
			ids.push_back(root->id);
		if (root->left != nullptr)
			search(root->left, name, ids);
		if (root->right != nullptr)
			search(root->right, name, ids);
	}

	// all other helper functions to make these functions above easier to write
	Node* traverseID(Node* root, int id)
	{
		if (root == nullptr)
			return nullptr;
		if (root->id == id)
			return root;
		if (root->id > id)
			return traverseID(root->left, id);
		if (root->id < id)
			return traverseID(root->right, id);
	}
	
	vector<Node*> traverseName(Node* root, string name)
	{
		vector<Node*> vec;
		if (root->name == name)
			vec.push_back(root);
		if (root->name > name)
			return traverseName(root->left, name);
		if (root->name < name)
			return traverseName(root->right, name);
		return vec;
	}
};


// main method
// instantiate AVLTree

bool checkName(string name)
{
	const regex pattern("[a-zA-Z ]+");
	if (regex_match(name, pattern))
		return true;
	return false;
}

bool checkID(string ID)
{
	const regex pattern("[0-9]+");
	if (regex_match(ID, pattern) && ID.size() == 8)
		return true;
	return false;
}


int main()
{
	//get number of commands as string then convert to int
	string numCommands_s;
	getline(cin, numCommands_s);
	int numCommands = stoi(numCommands_s);

	//repeadly used variables
	string line, currCommand, name, id_s;
	int id;

	AVLTree tree;

	for (int i = 0; i < numCommands; i++)
	{
		getline(cin, line);
		istringstream in(line);

		in >> currCommand;

		if (currCommand.compare("insert") == 0)
		{
			//check if name or id and if valid
			//if it is insert and print successful
			// else print unsuccessful
			string empty;
			getline(in, empty, '"');
			getline(in, name, '"');
			in >> id_s;
			id = stoi(id_s);
			bool conditions = false;
			if (checkName(name) && checkID(id_s))
			{
				tree.head = tree.insert(tree.head, name, id);
				if (name.compare("unsuccessful") == 0)
					cout << "unsuccessful\n";
				else
					cout << "successful\n";
			}
			else
			{
				cout << "unsuccessful\n";
			}
		}

		else if (currCommand.compare("remove") == 0)
		{
			in >> id_s;
			if (isdigit(id_s.at(0)) && checkID(id_s))
			{
				id = stoi(id_s);
				bool complete = false;
				tree.head = tree.remove(tree.head, id, complete);
				if (complete)
					cout << "successful\n";
				else
					cout << "unsuccessful\n";
			}
			else
				cout << "unsuccessful\n";
		}

		else if (currCommand.compare("search") == 0)
		{
			if (tree.head == nullptr)
				cout << "unsuccessful\n";
			else
			{
				string nameid;
				in >> nameid;
				if (isdigit(nameid.at(0)) && checkID(nameid))
				{
					id = stoi(nameid);
					name = tree.search(tree.head, id);
					if (name.compare("") == 0)
						cout << "unsuccessful\n";
					else
						cout << name << endl;
				}
				else if (nameid.at(0) == '"')
				{
					name = nameid.substr(1, nameid.size() - 2);
					if (checkName(name))
					{
						vector<int> ids;
						tree.search(tree.head, name, ids);
						if (ids.size() == 0)
							cout << "unsuccessful\n";
						else
						{
							for (int i = 0; i < ids.size(); i++)
							{
								string temp = to_string(ids[i]);
								while (temp.length() != 8)
								{
									temp = '0' + temp;
								}
								cout << temp << endl;
							}
						}
					}
					else
						cout << "unsuccessful\n";
				}
				else
					cout << "unsuccessful\n";
			}
		}

		else if (currCommand.compare("printInorder") == 0)
		{
			string allNames = tree.printInorder(tree.head);
			allNames = allNames.substr(0, allNames.length() - 2);
			cout << allNames << endl;
		}

		else if (currCommand.compare("printPreorder") == 0)
		{
			string allNames = tree.printPreorder(tree.head);
			allNames = allNames.substr(0, allNames.length() - 2);
			cout << allNames << endl;
		}

		else if (currCommand.compare("printPostorder") == 0)
		{
			string allNames = tree.printPostorder(tree.head);
			allNames = allNames.substr(0, allNames.length() - 2);
			cout << allNames << endl;
		}

		else if (currCommand.compare("printLevelCount") == 0)
		{
			if (tree.head == nullptr)
				cout << "0\n";
			else
			{ 
				queue<Node*> q;
				q.push(tree.head);
				int lvlCount = 1;
				while (!q.empty())
				{
					int lvlSize = q.size();
					for (int i = 0; i < lvlSize; i++)
					{
						Node* temp = q.front();
						if (temp->left != nullptr)
							q.push(temp->left);
						if (temp->right != nullptr)
							q.push(temp->right);
						q.pop();
					}
					if (q.empty())
						cout << lvlCount << endl;
					lvlCount++;
				}
			}
		}

		else if (currCommand.compare("removeInorder") == 0)
		{
		in >> id_s;
		bool validNum = true;
		for (int i = 0; i < id_s.length(); i++)
		{
			if (!isdigit(id_s.at(i)))
				validNum = false;
		}
		if (validNum)
		{
			int index = stoi(id_s);
			bool complete = false;
			tree.head = tree.removeInorder(tree.head, index, complete);
			if (complete)
				cout << "successful\n";
			else
				cout << "unsuccessful\n";
		}
		else
			cout << "unsuccessful\n";
		}
	}
}