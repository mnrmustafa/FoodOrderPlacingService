#include<bits/stdc++.h>
#include<unordered_map>
#include<string>
#include<mysql.h>
#include<sstream>

using namespace std;

//Global VAriable
int qstate;
MYSQL* conn;

//Connecting to database
class db_response
{
public:
	static void ConnectionFunction()
	{
		conn = mysql_init(0);
		if (conn)
		{
			cout << "Database connected" << endl;
			system("cls");
		}
		else
			cout << "Failed to connect" << mysql_errno(conn) << endl;
		conn = mysql_real_connect(conn, "localhost", "root", "", "fops", 3306, NULL, NULL);
		if (conn)
		{
			cout << "Database connected to MySQL " << conn << endl;
			system("pause");
			system("cls");
		}
		else
			cout << "Failed to connect" << mysql_errno(conn) << endl;
	}
};

//Inserting Papa Johns order in database
void insertPPJOrder(string tno, string fname, string price)
{

	string insert_query = "INSERT INTO ppj (TABLE_NO, FOOD_NAME, PRICE) VALUES ('" + tno + "','" + fname + "','" + price + "')";
	const char* q = insert_query.c_str();
	mysql_query(conn, q);
}

//Inserting Papa Johns customers info in database
void insertPPJName(string tno, string name)
{

	string insert_query = "INSERT INTO ppjt (TABLE_NO, NAME) VALUES ('" + tno + "','" + name + "')";
	const char* q = insert_query.c_str();
	mysql_query(conn, q);
}

//Deleting Papa Johns order from database
void deletePPJOrder(string tno)
{
	string delete_query = "DELETE FROM ppj WHERE TABLE_NO='" + tno + "'";
	const char* q = delete_query.c_str();
	mysql_query(conn, q);
}

//Deleting Papa Johns customer info from database
void deletePPJName(string tno)
{
	string delete_query = "DELETE FROM ppjt WHERE TABLE_NO='" + tno + "'";
	const char* q = delete_query.c_str();
	mysql_query(conn, q);
}

//Inserting McDonlad's order in database
void insertMcdOrder(string tno, string fname, string price)
{

	string insert_query = "INSERT INTO mcd (TABLE_NO, FOOD_NAME, PRICE) VALUES ('" + tno + "','" + fname + "','" + price + "')";
	const char* q = insert_query.c_str();
	mysql_query(conn, q);
}

//Inserting McDonald's customers info in database
void insertMcdName(string tno, string name)
{

	string insert_query = "INSERT INTO mcdt (TABLE_NO, NAME) VALUES ('" + tno + "','" + name + "')";
	const char* q = insert_query.c_str();
	mysql_query(conn, q);
}

//Deleting McDonald's order from database
void deleteMcdOrder(string tno)
{
	string delete_query = "DELETE FROM mcd WHERE TABLE_NO='" + tno + "'";
	const char* q = delete_query.c_str();
	mysql_query(conn, q);
}

//Deleting McDonald's customer info from database
void deleteMcdName(string tno)
{
	string delete_query = "DELETE FROM mcdt WHERE TABLE_NO='" + tno + "'";
	const char* q = delete_query.c_str();
	mysql_query(conn, q);
}




// A BTree node
class BTreeNode
{
	vector<pair<int, string>> keys; // An array of pair of keys
	int t;	 // Minimum degree (defines the range for number of keys)
	BTreeNode** C; // An array of child pointers
	int n;	 // Current number of keys
	bool leaf; // Is true when node is leaf. Otherwise false

public:

	BTreeNode(int _t, bool _leaf); // Constructor

	// A function to traverse all nodes in a subtree rooted with this node
	void traverse();

	// A function to search a key in subtree rooted with this node.
	BTreeNode* search(int k); // returns NULL if k is not present.

	// A function that returns the index of the first key that is greater
	// or equal to k
	int findKey(int k);

	// A utility function to insert a new key in the subtree rooted with
	// this node. 
	void insertNonFull(int k, string name);

	// A utility function to split the child y of this node. i is index
	// of y in child array C[].
	void splitChild(int i, BTreeNode* y);

	// A wrapper function to remove the key k in subtree rooted with
	// this node.
	void remove(int k);

	// A function to remove the key present in idx-th position in
	// this node which is a leaf
	void removeFromLeaf(int idx);

	// A function to remove the key present in idx-th position in
	// this node which is a non-leaf node
	void removeFromNonLeaf(int idx);

	// A function to get the predecessor of the key- where the key
	// is present in the idx-th position in the node
	int getPredInt(int idx);
	string getPredStr(int idx);

	// A function to get the successor of the key- where the key
	// is present in the idx-th position in the node
	int getSuccInt(int idx);
	string getSuccStr(int idx);

	// A function to fill up the child node present in the idx-th
	// position in the C[] array if that child has less than t-1 keys
	void fill(int idx);

	// A function to borrow a key from the C[idx-1]-th node and place
	// it in C[idx]th node
	void borrowFromPrev(int idx);

	// A function to borrow a key from the C[idx+1]-th node and place it
	// in C[idx]th node
	void borrowFromNext(int idx);

	// A function to merge idx-th child of the node with (idx+1)th child of
	// the node
	void merge(int idx);

	// Make BTree friend of this so that we can access private members of
	// this class in BTree functions
	friend class BTree;
};

class BTree
{
	BTreeNode* root; // Pointer to root node
	int t; // Minimum degree
public:

	// Constructor (Initializes tree as empty)
	BTree(int _t)
	{
		root = NULL;
		t = _t;
	}

	void traverse()
	{
		if (root != NULL) root->traverse();
	}

	// function to search a key in this tree
	BTreeNode* search(int k)
	{
		return (root == NULL) ? NULL : root->search(k);
	}

	// The main function that inserts a new key in this B-Tree
	void insertTree(int k, string name);

	// The main function that removes a new key in thie B-Tree
	void remove(int k);

};

BTreeNode::BTreeNode(int t1, bool leaf1)
{

	t = t1;
	leaf = leaf1;


	keys.resize(2 * t - 1);
	//keys = new int[2 * t - 1];
	C = new BTreeNode * [2 * t];

	// Initialize the number of keys as 0
	n = 0;
}


int BTreeNode::findKey(int k)
{
	int idx = 0;
	while (idx < n && keys[idx].first < k)
		++idx;
	return idx;
}


void BTreeNode::remove(int k)
{
	int idx = findKey(k);


	if (idx < n && keys[idx].first == k)
	{


		if (leaf)
			removeFromLeaf(idx);
		else
			removeFromNonLeaf(idx);
	}
	else
	{


		if (leaf)
		{
			cout << "The key " << k << " does not exist in the tree\n";
			return;
		}


		bool flag = ((idx == n) ? true : false);


		if (C[idx]->n < t)
			fill(idx);


		if (flag && idx > n)
			C[idx - 1]->remove(k);
		else
			C[idx]->remove(k);
	}
	return;
}

void BTreeNode::removeFromLeaf(int idx)
{


	for (int i = idx + 1; i < n; ++i)
		keys[i - 1] = keys[i];

	n--;

	return;
}


void BTreeNode::removeFromNonLeaf(int idx)
{

	int k = keys[idx].first;


	if (C[idx]->n >= t)
	{
		int predInt = getPredInt(idx);
		string predStr = getPredStr(idx);
		keys[idx].first = predInt;
		keys[idx].second = predStr;
		C[idx]->remove(predInt);
	}


	else if (C[idx + 1]->n >= t)
	{
		int succInt = getSuccInt(idx);
		string succStr = getSuccStr(idx);
		keys[idx].first = succInt;
		keys[idx].second = succStr;
		C[idx + 1]->remove(succInt);
	}


	else
	{
		merge(idx);
		C[idx]->remove(k);
	}
	return;
}


int BTreeNode::getPredInt(int idx)
{

	BTreeNode* cur = C[idx];
	while (!cur->leaf)
		cur = cur->C[cur->n];


	return cur->keys[cur->n - 1].first;
}

string BTreeNode::getPredStr(int idx)
{

	BTreeNode* cur = C[idx];
	while (!cur->leaf)
		cur = cur->C[cur->n];


	return cur->keys[cur->n - 1].second;
}

int BTreeNode::getSuccInt(int idx)
{


	BTreeNode* cur = C[idx + 1];
	while (!cur->leaf)
		cur = cur->C[0];


	return cur->keys[0].first;
}

string BTreeNode::getSuccStr(int idx)
{


	BTreeNode* cur = C[idx + 1];
	while (!cur->leaf)
		cur = cur->C[0];

	return cur->keys[0].second;
}


void BTreeNode::fill(int idx)
{


	if (idx != 0 && C[idx - 1]->n >= t)
		borrowFromPrev(idx);


	else if (idx != n && C[idx + 1]->n >= t)
		borrowFromNext(idx);


	else
	{
		if (idx != n)
			merge(idx);
		else
			merge(idx - 1);
	}
	return;
}


void BTreeNode::borrowFromPrev(int idx)
{

	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx - 1];


	for (int i = child->n - 1; i >= 0; --i)
		child->keys[i + 1] = child->keys[i];


	if (!child->leaf)
	{
		for (int i = child->n; i >= 0; --i)
			child->C[i + 1] = child->C[i];
	}


	child->keys[0] = keys[idx - 1];


	if (!child->leaf)
		child->C[0] = sibling->C[sibling->n];


	keys[idx - 1] = sibling->keys[sibling->n - 1];

	child->n += 1;
	sibling->n -= 1;

	return;
}


void BTreeNode::borrowFromNext(int idx)
{

	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx + 1];


	child->keys[(child->n)] = keys[idx];


	if (!(child->leaf))
		child->C[(child->n) + 1] = sibling->C[0];


	keys[idx] = sibling->keys[0];


	for (int i = 1; i < sibling->n; ++i)
		sibling->keys[i - 1] = sibling->keys[i];


	if (!sibling->leaf)
	{
		for (int i = 1; i <= sibling->n; ++i)
			sibling->C[i - 1] = sibling->C[i];
	}


	child->n += 1;
	sibling->n -= 1;

	return;
}


void BTreeNode::merge(int idx)
{
	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx + 1];


	child->keys[t - 1] = keys[idx];


	for (int i = 0; i < sibling->n; ++i)
		child->keys[i + t] = sibling->keys[i];


	if (!child->leaf)
	{
		for (int i = 0; i <= sibling->n; ++i)
			child->C[i + t] = sibling->C[i];
	}


	for (int i = idx + 1; i < n; ++i)
		keys[i - 1] = keys[i];


	for (int i = idx + 2; i <= n; ++i)
		C[i - 1] = C[i];


	child->n += sibling->n + 1;
	n--;


	delete(sibling);
	return;
}


void BTree::insertTree(int k, string name)
{

	if (root == NULL)
	{

		root = new BTreeNode(t, true);
		root->keys[0].first = k; // Insert key
		root->keys[0].second = name; // Insert name
		root->n = 1; // Update number of keys in root
	}
	else // If tree is not empty
	{
		// If root is full, then tree grows in height
		if (root->n == 2 * t - 1)
		{
			// Allocate memory for new root
			BTreeNode* s = new BTreeNode(t, false);

			// Make old root as child of new root
			s->C[0] = root;

			// Split the old root and move 1 key to the new root
			s->splitChild(0, root);


			int i = 0;
			if (s->keys[0].first < k)
				i++;
			s->C[i]->insertNonFull(k, name);

			// Change root
			root = s;
		}
		else // If root is not full, call insertNonFull for root
			root->insertNonFull(k, name);
	}
}


void BTreeNode::insertNonFull(int k, string name)
{
	// Initialize index as index of rightmost element
	int i = n - 1;

	// If this is a leaf node
	if (leaf == true)
	{

		while (i >= 0 && keys[i].first > k)
		{
			keys[i + 1] = keys[i];
			i--;
		}

		// Insert the new key at found location
		keys[i + 1].first = k;
		keys[i + 1].second = name;
		n = n + 1;
	}
	else // If this node is not leaf
	{
		// Find the child which is going to have the new key
		while (i >= 0 && keys[i].first > k)
			i--;

		// See if the found child is full
		if (C[i + 1]->n == 2 * t - 1)
		{
			// If the child is full, then split it
			splitChild(i + 1, C[i + 1]);


			if (keys[i + 1].first < k)
				i++;
		}
		C[i + 1]->insertNonFull(k, name);
	}
}


void BTreeNode::splitChild(int i, BTreeNode* y)
{

	BTreeNode* z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;

	// Copy the last (t-1) keys of y to z
	for (int j = 0; j < t - 1; j++)
		z->keys[j] = y->keys[j + t];

	// Copy the last t children of y to z
	if (y->leaf == false)
	{
		for (int j = 0; j < t; j++)
			z->C[j] = y->C[j + t];
	}

	// Reduce the number of keys in y
	y->n = t - 1;


	for (int j = n; j >= i + 1; j--)
		C[j + 1] = C[j];

	// Link the new child to this node
	C[i + 1] = z;


	for (int j = n - 1; j >= i; j--)
		keys[j + 1] = keys[j];

	// Copy the middle key of y to this node
	keys[i] = y->keys[t - 1];

	// Increment count of keys in this node
	n = n + 1;
}

// Function to traverse all nodes in a subtree rooted with this node
void BTreeNode::traverse()
{

	int i;
	for (i = 0; i < n; i++)
	{

		if (leaf == false)
			C[i]->traverse();
		cout << " " << keys[i].first << " " << keys[i].second;
	}

	// Print the subtree rooted with last child
	if (leaf == false)
		C[i]->traverse();
}

// Function to search key k in subtree rooted with this node
BTreeNode* BTreeNode::search(int k)
{
	// Find the first key greater than or equal to k
	int i = 0;
	while (i < n && k > keys[i].first)
		i++;

	// If the found key is equal to k, return this node
	if (keys[i].first == k)
		return this;

	// If key is not found here and this is a leaf node
	if (leaf == true)
		return NULL;

	// Go to the appropriate child
	return C[i]->search(k);
}

void BTree::remove(int k)
{
	if (!root)
	{
		//cout << "The tree is empty\n";
		return;
	}

	// Call the remove function for root
	root->remove(k);


	if (root->n == 0)
	{
		BTreeNode* tmp = root;
		if (root->leaf)
			root = NULL;
		else
			root = root->C[0];

		// Free the old root
		delete tmp;
	}
	return;
}



//Global Variable 
//int serial = 1;

//Doubly Linked List to store the orders and their price
struct DLL
{
	struct DLL* prev;
	string OrderName;
	double price;
	int index;
	//struct DLL* first = NULL;
	//struct DLL* second = NULL;
	struct DLL* next;
}*second=NULL;

void insertDLL(DLL *&first,int &serial,string OrderName, double price)
{
	if (first == NULL)
	{
		first = new DLL;
		first->prev = NULL;
		first->next = NULL;
		first->price = price;
		first->OrderName = OrderName;
		first->index = serial;
		serial++;
	}
	else
	{
		struct DLL* p, * t;
		p = first;
		while (p->next != NULL)
			p = p->next;
		t = new DLL;
		t->OrderName = OrderName;
		t->price = price;
		t->next = NULL;
		p->next = t;
		t->prev = p;
		t->index = serial;
		serial++;
	}
}

//Displaying Doubly Linked List
void Display(struct DLL*& p)
{
	while (p != NULL)
	{
		cout << "     " << p->index << " - " << p->OrderName << " -> " << p->price << " SAR" << endl;
		p = p->next;
	}
	cout << endl;
}

// Btree global variable for PPJ and McD
BTree ppj(2);
BTree mcd(2);

void deletePPJ()
{
	cout << "     How would you like to pay for your order:" << endl;
	cout << "     1)Cash" << endl;
	cout << "     2)Card" << endl;
	int payment;
	cin >> payment;
	if (payment == 1)
	{
		cout << "     Please enter table number and a person from the restaurant will come to take your cash " << endl;
		int tnum;
		cin >> tnum;
		//A[tnum + 1] = tnum;
		ppj.remove(tnum);
		//Delete(root, tnum);
		//Inorder(root);
		/*Insert code to delete info from database*/
		string tno = to_string(tnum);
		deletePPJName(tno);
		deletePPJOrder(tno);
	}
	else if (payment == 2)
	{
		cout << "     Please enter your 16-digit card number" << endl;
		long long cardnum;
		cin >> cardnum;
		cout << "     Please enter your password" << endl;
		string password;
		cin >> password;
		cout << "     Please enter your table number" << endl;
		int tnum;
		cin >> tnum;
		//A[tnum + 1] = tnum;
		ppj.remove(tnum);
		//Delete(root, tnum);
		//Inorder(root);
		/*Insert code to delete info from database*/
		string tno = to_string(tnum);
		deletePPJName(tno);
		deletePPJOrder(tno);
	}
}

void deleteMcd()
{
	cout << "     How would you like to pay for your order:" << endl;
	cout << "     1)Cash" << endl;
	cout << "     2)Card" << endl;
	int payment;
	cin >> payment;
	if (payment == 1)
	{
		cout << "     Please enter table number and a person from the restaurant will come to take your cash " << endl;
		int tnum;
		cin >> tnum;
		//A[tnum + 1] = tnum;
		mcd.remove(tnum);
		//Delete(root, tnum);
		//Inorder(root);
		/*Insert code to delete info from database*/
		string tno = to_string(tnum);
		deleteMcdName(tno);
		deleteMcdOrder(tno);
	}
	else if (payment == 2)
	{
		cout << "     Please enter your 16-digit card number" << endl;
		long long cardnum;
		cin >> cardnum;
		cout << "     Please enter your password" << endl;
		string password;
		cin >> password;
		cout << "     Please enter your table number" << endl;
		int tnum;
		cin >> tnum;
		//A[tnum + 1] = tnum;
		mcd.remove(tnum);
		//Delete(root, tnum);
		//Inorder(root);
		/*Insert code to delete info from database*/
		string tno = to_string(tnum);
		deleteMcdName(tno);
		deleteMcdOrder(tno);
	}
}

static int firstint = 0;

class varDLL {
public:
	DLL* first;
	int serial;
};


int main()
{

	system("title Food Order Placing Service");

	//Connectiing to DataBase
	db_response::ConnectionFunction();

	/*insert("Pizza", 100);
	insert("Burger", 300);
	insert("Kabab", 200);
	Display(first);*/
	
while (true) {
		
	int start = 0;
	int a = 0;
	//insertTree(root, 20, "fdsfsa");
	//insertTree(root, 15, "fdsfsa");
	//insertTree(root, 30, "fdsfsa");
	//insertTree(root, 10, "fdsfsa");
	cout << "    Welcome to Food Order Placing Service. " << endl << endl;
	string name;
	int tablenum;
	cout << "    Please enter your name: ";
	//getline(cin, name);
	cin >> name;
	cout << endl << endl;
	cout << "    Thank you, " << name << ". Please enter '1' for the restaurant list  " << endl << endl;
	cin >> start;
	varDLL ppjArray[13];
	varDLL mcdArray[11];

	if (start == 1)
	{
		cout << "    1) Papa Johns" << endl;
		cout << "    2) McDonald" << endl;
		int r;
		cout << "    Please enter the serial number of the restuarant you want to choose: " << endl;
		cin >> r;
		cout << endl;
		string firstppj, firstmcd;

		

		switch (r)
		{
			//Papa Johns

			{
		case 1:
			cout << "    1. Old customer" << endl;
			cout << "    2. New Customer" << endl;
			int cust;
			cin >> cust;
			unordered_map<int, DLL*> tableOrders;
			


			if (cust == 1)
			{
				deletePPJ();
				break;
			}

			//Menu

			int tablenum;
			int* A = new int[13];
			for (int i = 0; i < 13; i++)
			{
				A[i] = i + 1;
			}
			cout << "    Table number's available are: ";
			for (int i = 0; i < 13; i++)
			{
				BTreeNode* tempnode = ppj.search(i + 1);
				if (tempnode == NULL)
					cout << i + 1 << " ";
			}
			cout << endl << endl;

			cout << "    Please enter your table number: ";
			cin >> tablenum;
			ppjArray[tablenum].first = NULL;
			ppjArray[tablenum].serial = 1;
			//cout << ppjArray[tablenum].first << endl;
			
			string tno = to_string(tablenum);
			A[tablenum - 1] = 0;
			ppj.insertTree(tablenum, name);
			insertPPJName(tno, name);

			for (int i = 0; i < 100; i++)
			{
				cout << "         MENU" << endl << endl;
				cout << "         1) Starters" << endl;
				cout << "         2) Salads" << endl;
				cout << "         3) Classic Pizza" << endl;
				cout << "         4) Pizzas of all Around the World" << endl;
				cout << "         5) Speciality Pizzas" << endl;
				cout << "         6) Beverages" << endl;
				int M;
				cin >> M;

				cout << "                    ITEM NAME                    " << endl << endl;
				switch (M)
				{
					//Starters
				case 1:
					cout << "            1-Cheese Sticks   " << endl << endl;
					cout << "            2-Garlic Parmesan Bread Sticks" << endl << endl;
					cout << "            3-Potato Wedges" << endl << endl;
					cout << "            4-Chicken Wings" << endl << endl;
					cout << "            5-Pepperoni Rolls" << endl << endl;
					int ST;
					cin >> ST;
					char C;

					cout << "          ITEM TYPE                        PRICE(SAR)" << endl << endl;
					switch (ST)
					{
						//Starters Options
						int st;
					case 1:
						cout << "          1-Normal                           29" << endl;
						cin >> st;
						if (st == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Cheese Sticks", 29);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else {
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}

						}
						break;

					case 2:
						cout << "          1-Garlic Sauce                     14" << endl << endl;
						cout << "          2-BBQ Sauce                        14" << endl << endl;
						cout << "          3-Ranch Sauce                      14" << endl << endl;
						cin >> st;
						if (st == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Garlic Parmesan Bread Sticks with Garlic Sauce", 14);
						else if (st == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Garlic Parmesan Bread Sticks with BBQ Sauce", 14);
						else if (st == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Garlic Parmesan Bread Sticks with Ranch Sauce", 14);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 3:
						cout << "          1-Normal                           15" << endl << endl;
						cin >> st;
						if (st == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Potato Wedges", 15);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 4:
						cout << "          1-Garlic Sauce                     31" << endl << endl;
						cout << "          2-BBQ Sauce                        31" << endl << endl;
						cout << "          3-Ranch Sauce                      31" << endl << endl;
						cin >> st;
						if (st == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Chicken Wings with Garlic Sauce", 31);
						else if (st == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Chicken Wings with BBQ Sauce", 31);
						else if (st == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Chicken Wings with Ranch Sauce", 31);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 5:
						cout << "          1-Normal                           24" << endl << endl;
						cin >> st;
						if (st == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Pepperoni Rolls", 24);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					}//End of Starter Options
					break;
					//End of Starters

				case 2:
					//Salads
					cout << "            1-Garden Salad" << endl << endl;
					cout << "            2-Chicken Tender Salad" << endl << endl;
					cout << "            3-Papa's Club Salad" << endl << endl;
					int SA;
					cin >> SA;
					cout << "          ITEM TYPE                        PRICE(SAR)" << endl << endl;
					switch (SA)
					{
						//Salad Options
						int sa;
					case 1:
						cout << "          1-Small                            21" << endl << endl;
						cout << "          2-Medium                           24" << endl << endl;
						cin >> sa;
						if (sa == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Garden Salad-Small", 21);
						else if (sa == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Garden Salad-Medium", 24);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 2:
						cout << "          1-Small                            21" << endl << endl;
						cout << "          2-Medium                           24" << endl << endl;
						cin >> sa;
						if (sa == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Chicken Tender Salad-Small", 21);
						else if (sa == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Chicken Tender Salad-Medium", 24);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 3:
						cout << "          1-Small                            21" << endl << endl;
						cout << "          2-Medium                           24" << endl << endl;
						cin >> sa;
						if (sa == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Papa's Club Salad-Small", 21);
						else if (sa == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Papa's Club Salad-Medium", 24);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					}//End of Salad Options
					break;
					//End of Salad

				case 3:
					//Classic Pizza
					cout << "            1-Super Papa's Pizza" << endl << endl;
					cout << "            2-Pepperoni Pizza" << endl << endl;
					cout << "            3-All The Meats Pizza" << endl << endl;
					int CL;
					cin >> CL;

					cout << "          ITEM TYPE                        PRICE(SAR)" << endl << endl;
					switch (CL)
					{
						//Classic Pizza Options
						int cl;
					case 1:
						cout << "          1-Small                            37.00" << endl << endl;
						cout << "          2-Medium                           59.00" << endl << endl;
						cout << "          3-Large                            75.00" << endl << endl;
						cin >> cl;
						if (cl == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Super Papa's Pizza-Small", 37);
						else if (cl == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Super Papa's Pizza-Medium", 59);
						else if (cl == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Super Papa's Pizza-Large", 75);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 2:
						cout << "          1-Small                            37.00" << endl << endl;
						cout << "          2-Medium                           59.00" << endl << endl;
						cout << "          3-Large                            75.00" << endl << endl;
						cin >> cl;
						if (cl == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Pepperoni Pizza-Small", 37);
						else if (cl == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Pepperoni Pizza-Medium", 59);
						else if (cl == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Pepperoni Pizza-Large", 75);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 3:
						cout << "          1-Small                            37.00" << endl << endl;
						cout << "          2-Medium                           59.00" << endl << endl;
						cout << "          3-Large                            75.00" << endl << endl;
						cin >> cl;
						if (cl == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "All The Meat's Pizza-Small", 37);
						else if (cl == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "All The Meat's Pizza-Medium", 59);
						else if (cl == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "All The Meat's Pizza-Large", 75);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					}//End of Classic Pizza Options
					break;
					//End of Classic Pizza

				case 4:
					//Pizzas of all Around the World
					cout << "            1-Mexican Ole Pizza" << endl << endl;
					cout << "            2-Little Italy Pizza" << endl << endl;
					cout << "            3-Hawaiian Pizza" << endl << endl;
					cout << "            4-Butter Chicken Pizza" << endl << endl;
					int WO;
					cin >> WO;

					cout << "          ITEM TYPE                        PRICE(SAR)" << endl << endl;
					switch (WO)
					{
						//Pizzas of all Around World the Options
						int wo;
					case 1:
						cout << "          1-Small                            37.00" << endl << endl;
						cout << "          2-Medium                           59.00" << endl << endl;
						cout << "          3-Large                            75.00" << endl << endl;
						cin >> wo;
						if (wo == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Mexican Ole Pizza-Small", 37);
						else if (wo == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Mexican Ole Pizza-Medium", 59);
						else if (wo == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Mexican Ole Pizza-Large", 75);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 2:
						cout << "          1-Small                            37.00" << endl << endl;
						cout << "          2-Medium                           59.00" << endl << endl;
						cout << "          3-Large                            75.00" << endl << endl;
						cin >> wo;
						if (wo == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Little Italy Pizza-Small", 37);
						else if (wo == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Little Italy Pizza-Medium", 59);
						else if (wo == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Little Italy Pizza-Large", 75);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 3:
						cout << "          1-Small                            37.00" << endl << endl;
						cout << "          2-Medium                           59.00" << endl << endl;
						cout << "          3-Large                            75.00" << endl << endl;
						cin >> wo;
						if (wo == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Hawaiian Pizza-Small", 37);
						else if (wo == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Hawaiian Pizza-Medium", 59);
						else if (wo == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Hawaiian Pizza-Large", 75);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 4:
						cout << "          1-Small                            37.00" << endl << endl;
						cout << "          2-Medium                           59.00" << endl << endl;
						cout << "          3-Large                            75.00" << endl << endl;
						cin >> wo;
						if (wo == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Butter Chicken Pizza-Small", 37);
						else if (wo == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Butter Chicken Pizza-Medium", 59);
						else if (wo == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Butter Chicken Pizza-Large", 75);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;
					}//End of Pizzas all Around the World Options
					break;
					//End of Pizzas all Around the World

				case 5:
					//Speciality Pizzas
					cout << "            1-Spicy Chicken Ranch Pizza" << endl << endl;
					cout << "            2-Garlic Parmesan Chicken Pizza" << endl << endl;
					cout << "            3-Chicken Super Papa's Pizza" << endl << endl;
					cout << "            4-Chicken BBQ Pizza" << endl << endl;
					cout << "            5-Hot and Spicy Pizza" << endl << endl;
					cout << "            6-Green Garden Delight Pizza" << endl << endl;
					int SP;
					cin >> SP;

					cout << "          ITEM TYPE                        PRICE(SAR)" << endl << endl;
					switch (SP)
					{
						//Speciality Pizzas Options
						int sp;
					case 1:
						cout << "          1-Small                            37.00" << endl << endl;
						cout << "          2-Medium                           59.00" << endl << endl;
						cout << "          3-Large                            75.00" << endl << endl;
						cin >> sp;
						if (sp == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Spicy Chicken Ranch Pizza-Small", 37);
						else if (sp == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Spicy Chicken Ranch Pizza-Medium", 59);
						else if (sp == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Spicy Chicken Ranch Pizza-Large", 75);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 2:
						cout << "          1-Small                            37.00" << endl << endl;
						cout << "          2-Medium                           59.00" << endl << endl;
						cout << "          3-Large                            75.00" << endl << endl;
						cin >> sp;
						if (sp == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Garlic Parmesan Chicken Pizza-Small", 37);
						else if (sp == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Garlic Parmesan Pizza-Medium", 59);
						else if (sp == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Garlic Parmesan Chicken Pizza-Large", 75);

						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 3:
						cout << "          1-Small                            37.00" << endl << endl;
						cout << "          2-Medium                           59.00" << endl << endl;
						cout << "          3-Large                            75.00" << endl << endl;
						cin >> sp;
						if (sp == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Chicken Super Papa's Pizza-Small", 37);
						else if (sp == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Chicken Super Papa's Pizza-Medium", 59);
						else if (sp == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Chicken Super Papa's Pizza-Large", 75);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 4:
						cout << "          1-Small                            37.00" << endl << endl;
						cout << "          2-Medium                           59.00" << endl << endl;
						cout << "          3-Large                            75.00" << endl << endl;
						cin >> sp;
						if (sp == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Chicken BBQ Pizza-Small", 37);
						else if (sp == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Chicken BBQ Pizza-Medium", 59);
						else if (sp == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Chicken BBQ Pizza-Large", 75);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 5:
						cout << "          1-Small                            37.00" << endl << endl;
						cout << "          2-Medium                           59.00" << endl << endl;
						cout << "          3-Large                            75.00" << endl << endl;
						cin >> sp;
						if (sp == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Hot and Spicy Pizza-Small", 37);
						else if (sp == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Hot and Spicy Pizza-Medium", 59);
						else if (sp == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Hot and Spicy Pizza-Large", 75);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 6:
						cout << "          1-Small                            37.00" << endl << endl;
						cout << "          2-Medium                           59.00" << endl << endl;
						cout << "          3-Large                            75.00" << endl << endl;
						cin >> sp;
						if (sp == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Green Garden Delight Pizza-Small", 37);
						else if (sp == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Green Garden Delight Pizza-Medium", 59);
						else if (sp == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Green Garden Delight Pizza-Large", 75);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;
					}//End of Speciality Pizzas Options
					break;
					//End of Speciality Pizzas

				case 6:
					//Beverages
					cout << "            1-Pepsi" << endl << endl;
					cout << "            2-Pepsi Diet" << endl << endl;
					cout << "            3-7Up" << endl << endl;
					cout << "            4-Coca Cola" << endl << endl;
					cout << "            5-Mirinda" << endl << endl;
					cout << "            6-Water" << endl << endl;
					int BE;
					cin >> BE;

					cout << "          ITEM TYPE                        PRICE(SAR)" << endl << endl;
					switch (BE)
					{
						//Beverages Options
						int be;
					case 1:
						cout << "          1-Can                              4.00" << endl << endl;
						cout << "          2-Small                            7.00" << endl << endl;
						cout << "          3-Medium                           9.00" << endl << endl;
						cout << "          4-Large                            10.00" << endl << endl;
						cin >> be;
						if (be == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Pepsi-Can", 4);
						else if (be == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Pepsi-Small", 7);
						else if (be == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Pepsi-Medium", 9);
						else if (be == 4)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Pepsi-Large", 10);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 2:
						cout << "          1-Can                              4.00" << endl << endl;
						cout << "          2-Small                            7.00" << endl << endl;
						cout << "          3-Medium                           9.00" << endl << endl;
						cout << "          4-Large                            10.00" << endl << endl;
						cin >> be;
						if (be == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Pepsi Diet-Can", 4);
						else if (be == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Pepsi Diet-Small", 7);
						else if (be == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Pepsi Diet-Medium", 8);
						else if (be == 4)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Pepsi Diet-Large", 10);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 3:
						cout << "          1-Can                              4.00" << endl << endl;
						cout << "          2-Small                            7.00" << endl << endl;
						cout << "          3-Medium                           9.00" << endl << endl;
						cout << "          4-Large                            10.00" << endl << endl;
						cin >> be;
						if (be == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "7Up-Can", 4);
						else if (be == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "7Up-Small", 7);
						else if (be == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "7Up-Medium", 9);
						else if (be == 4)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "7Up-Large", 10);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 4:
						cout << "          1-Can                              4.00" << endl << endl;
						cout << "          2-Small                            7.00" << endl << endl;
						cout << "          3-Medium                           9.00" << endl << endl;
						cout << "          4-Large                            10.00" << endl << endl;
						cin >> be;
						if (be == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Coca Cola-Can", 4);
						else if (be == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Coca Cola-Small", 7);
						else if (be == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Coca Cola-Medium", 9);
						else if (be == 4)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Coca Cola-Large", 10);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 5:
						cout << "          1-Can                              4.00" << endl << endl;
						cout << "          2-Small                            7.00" << endl << endl;
						cout << "          3-Medium                           9.00" << endl << endl;
						cout << "          4-Large                            10.00" << endl << endl;
						cin >> be;
						if (be == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Mirinda-Can", 4);
						else if (be == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Mirinda-Small", 7);
						else if (be == 3)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Mirinda-Medium", 9);
						else if (be == 4)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Mirinda-Large", 10);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 6:
						cout << "          1-Small                            3.00" << endl << endl;
						cout << "          2-Medium                           5.00" << endl << endl;
						cin >> be;
						if (be == 1)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Water-Small", 3);
						else if (be == 2)
							insertDLL(ppjArray[tablenum].first, ppjArray[tablenum].serial, "Water-Medium", 5);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;
					}//End of Beverages Options
					break;
					//End of Beverages

				}//End of Menu

				break;//Breaking out of for loop
			}//End of for loop

			/*Insert code to insert info from database*/
			DLL* p = ppjArray[tablenum].first;
			string price;
			while (p != NULL)
			{
				price = to_string(p->price);
				insertPPJOrder(tno, p->OrderName, price);
				p = p->next;
			}
			
			if (a == 1)
			{
				// Inserting the table number and the orders in the map
				
				tableOrders.insert({ tablenum,ppjArray[tablenum].first, });
				
				cout << "     Here is your complete order:" << endl << endl;
				cout << "         ITEM TYPE -> PRICE" << endl << endl;
				DLL* p = ppjArray[tablenum].first;
				Display(ppjArray[tablenum].first);
				double total = 0;
				
				while (p != NULL)
				{
					total += p->price;
					//cout << p->price << endl;
					p = p->next;
				}
				cout << "     Total = " << total <<"SAR"<< endl << endl;
				//Inorder(root);
				
				system("pause");
				cout << "    Would you like to pay now? (Y/N) " << endl << endl;
				string tempcheck;
				cin >> tempcheck;
				if (tempcheck == "N" or tempcheck=="n")
					continue;
				cout << endl;
				cout << "     How would you like to pay for your order:" << endl;
				cout << "     1)Cash" << endl;
				cout << "     2)Card" << endl;
				int payment;
				cin >> payment;
				if (payment == 1)
				{
					cout << "     Please enter table number and a person from the restaurant will come to take your cash " << endl;
					int tnum;
					cin >> tnum;
					A[tnum + 1] = tnum;
					ppj.remove(tnum);
					//Delete(root, tnum);
					//Inorder(root);

					/*Insert code to delete info from database*/
					string tno = to_string(tnum);
					deletePPJName(tno);
					deletePPJOrder(tno);
				}
				else if (payment == 2)
				{
					cout << "     Please enter your 16-digit card number" << endl;
					long long cardnum;
					cin >> cardnum;
					cout << "     Please enter your password" << endl;
					string password;
					cin >> password;
					cout << "     Please enter your table number" << endl;
					int tnum;
					cin >> tnum;
					A[tnum + 1] = tnum;
					ppj.remove(tnum);
					//Delete(root, tnum);
					//Inorder(root);

					/*Insert code to delete info from database*/
					string tno = to_string(tnum);
					deletePPJName(tno);
					deletePPJOrder(tno);
				}
			}
			}
			break;//Breaking out of first restaurant


		//McDonalds
		case 2:

			cout << "    1. Old customer" << endl;
			cout << "    2. New Customer" << endl;
			int cust;
			cin >> cust;

			unordered_map<int, DLL*> tableOrders;
			if (cust == 1)
			{
				deleteMcd();
				break;
			}

			//Menu
		{
			int tablenum;
			int* B = new int[11];
			for (int i = 0; i < 11; i++)
			{
				B[i] = i + 1;
			}
			cout << "    Table number's are: ";
			for (int i = 0; i < 11; i++)
			{
				BTreeNode* tempnode = mcd.search(i + 1);
				if (tempnode == NULL)
					cout << i + 1 << " ";
			}
			cout << endl << endl;

			cout << "    Please enter your table number: ";
			cin >> tablenum;
			mcdArray[tablenum].first = NULL;
			mcdArray[tablenum].serial = 1;
			
			mcd.insertTree(tablenum, name);
			B[tablenum - 1] = 0;
			string tno = to_string(tablenum);
			B[tablenum - 1] = 0;
			//mcd.remove(tablenum);
			//insertTree(root, tablenum, name);
			insertMcdName(tno, name);
			
			for (int i = 0; i < 100; i++)
			{
				cout << "     1)Sandwich and Meals" << endl;
				cout << "     2)Deserts" << endl;
				cout << "     3)Beverages" << endl;
				int M;
				cin >> M;

				cout << "                    ITEM NAME                    " << endl << endl;
				switch (M)
				{
					//Sandwich and Meals
				case 1:

					cout << "            1-Grand Chicken Deluxe" << endl << endl;
					cout << "            2-Grand Chicken Special" << endl << endl;
					cout << "            3-Grand Chicken Spicy" << endl << endl;
					cout << "            4-Big Tasty" << endl << endl;
					cout << "            5-Big Mac" << endl << endl;
					cout << "            6-McChicken Spicy" << endl << endl;
					cout << "            7-McArabia Chicken" << endl << endl;
					cout << "            8-Beef Burger" << endl << endl;
					cout << "            9-Cheese Burger" << endl << endl;
					cout << "            10-Chicken Burger" << endl << endl;
					int S;
					cin >> S;

					cout << "          ITEM TYPE                        PRICE(SAR)" << endl << endl;
					switch (S)
					{
						//Meals Options
						int O;
						char C;
					case 1:
						cout << "          1-Meal                             28.00" << endl << endl;
						cout << "          2-Sandwich                         21.00" << endl << endl;
						cin >> O;
						if (O == 1)
						{
							int d;
							cout << "          Choose Your Drink" << endl << endl;
							cout << "          1-Pepsi" << endl << endl;
							cout << "          2-Coca Cola" << endl << endl;
							cout << "          3-7Up" << endl << endl;
							cin >> d;
							if (d == 1)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Grand Chicken Deluxe-Meal with Pepsi", 28);
							else if (d == 2)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Grand Chicken Deluxe-Meal with Coca Cola", 28);
							else if (d == 3)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Grand Chicken Deluxe-Meal 7Up", 28);
						}

						else if (O == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Grand Chicken Deluxe-Sandwich", 21);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 2:
						cout << "          1-Meal                             28.00" << endl << endl;
						cout << "          2-Sandwich                         21.00" << endl << endl;
						cin >> O;
						if (O == 1) {
							int d;
							cout << "          Choose Your Drink" << endl << endl;
							cout << "          1-Pepsi" << endl << endl;
							cout << "          2-Coca Cola" << endl << endl;
							cout << "          3-7Up" << endl << endl;
							cin >> d;
							if (d == 1)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Grand Chicken Special-Meal with Pepsi", 28);
							else if (d == 2)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Grand Chicken Special-Meal with Coca Cola", 28);
							else if (d == 3)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Grand Chicken Special-Meal with 7Up", 28);
						}

						else if (O == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Grand Chicken Special-Sandwich", 21);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 3:
						cout << "          1-Meal                             28.00" << endl << endl;
						cout << "          2-Sandwich                         21.00" << endl << endl;
						cin >> O;
						if (O == 1)
						{
							int d;
							cout << "          Choose Your Drink" << endl << endl;
							cout << "          1-Pepsi" << endl << endl;
							cout << "          2-Coca Cola" << endl << endl;
							cout << "          3-7Up" << endl << endl;
							cin >> d;
							if (d == 1)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Grand Chicken Spicy-Meal with Pepsi", 28);
							else if (d == 2)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Grand Chicken Spicy-Meal with Coca Cola", 28);
							else if (d == 3)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Grand Chicken Spicy-Meal wiith 7Up", 28);
						}

						else if (O == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Grand Chicken Spicy-Meal", 21);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 4:
						cout << "          1-Meal                             28.00" << endl << endl;
						cout << "          2-Sandwich                         21.00" << endl << endl;
						cin >> O;
						if (O == 1)
						{
							int d;
							cout << "          Choose Your Drink" << endl << endl;
							cout << "          1-Pepsi" << endl << endl;
							cout << "          2-Coca Cola" << endl << endl;
							cout << "          3-7Up" << endl << endl;
							cin >> d;
							if (d == 1)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Big Tasty-Meal with Pepsi", 28);
							else if (d == 2)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Big Tasty-Meal with Coca Cola", 28);
							else if (d == 3)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Big Tasty-Meal with 7Up", 28);
						}

						else if (O == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Big Tasty-Sandwich", 21);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 5:
						cout << "          1-Meal                             22.00" << endl << endl;
						cout << "          2-Sandwich                         15.00" << endl << endl;
						cin >> O;
						if (O == 1)
						{
							int d;
							cout << "          Choose Your Drink" << endl << endl;
							cout << "          1-Pepsi" << endl << endl;
							cout << "          2-Coca Cola" << endl << endl;
							cout << "          3-7Up" << endl << endl;
							cin >> d;
							if (d == 1)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Big Mac-Meal with Pepsi", 22);
							else if (d == 2)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Big Mac-Meal with Coca Cola", 22);
							else if (d == 3)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Big Mac-Meal with 7Up", 22);
						}

						else if (O == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Big Mac-Sandwich", 15);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 6:
						cout << "          1-Meal                             24.00" << endl << endl;
						cout << "          2-Sandwich                         16.00" << endl << endl;
						cin >> O;
						if (O == 1)
						{
							int d;
							cout << "          Choose Your Drink" << endl << endl;
							cout << "          1-Pepsi" << endl << endl;
							cout << "          2-Coca Cola" << endl << endl;
							cout << "          3-7Up" << endl << endl;
							cin >> d;
							if (d == 1)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "McChicken Spicy-Meal with Pepsi", 24);
							else if (d == 2)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "McChicken Spicy-Meal with Coca Cola", 24);
							else if (d == 3)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "McChicken Spicy-Meal with 7Up", 24);
						}

						else if (O == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "McChicken Spicy-Sandwich", 16);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 7:
						cout << "          1-Meal                             25.00" << endl << endl;
						cout << "          2-Sandwich                         17.00" << endl << endl;
						cin >> O;
						if (O == 1)
						{
							int d;
							cout << "          Choose Your Drink" << endl << endl;
							cout << "          1-Pepsi" << endl << endl;
							cout << "          2-Coca Cola" << endl << endl;
							cout << "          3-7Up" << endl << endl;
							cin >> d;
							if (d == 1)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "McArabia Chicken-Meal with Pepsi", 25);
							else if (d == 2)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "McArabia Chicken-Meal with Coca Cola", 25);
							else if (d == 3)
								insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "McArabia Chicken-Meal with 7Up", 25);
						}

						else if (O == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "McArabia Chicken-Sandwich", 17);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 8:
						cout << "          1-Sandwich                         5.50" << endl << endl;
						cin >> O;
						if (O == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Beef Burger", 5.5);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 9:
						cout << "          1-Sandwich                         6.50" << endl << endl;
						cin >> O;
						if (O == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Cheese Burger", 6.5);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 10:
						cout << "          1-Sandwich                         6.50" << endl << endl;
						cin >> O;
						if (O == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Chicken Burger", 6.5);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					}//End of Meal Options
					break;
					//End of Sandwhiche and Meals

					//Deserts
				case 2:

					cout << "            1-McFlurry Tiramisu" << endl << endl;
					cout << "            2-McFlurry Lotus" << endl << endl;
					cout << "            3-McFlurry Oreo" << endl << endl;
					cout << "            4-Fudge Sundae" << endl << endl;
					cout << "            5-Caramel Sundae" << endl << endl;
					cout << "            6-Soft Swirl" << endl << endl;
					int D;
					cin >> D;

					char C;
					cout << "           ITEM TYPE                        PRICE(SAR)" << endl << endl;
					switch (D)
					{
						//Desert Options
						int I;
					case 1:
						cout << "           1-Normal                           10.00 " << endl << endl;
						cout << "           2-Extra Coffee Sauce               12.50" << endl << endl;
						cin >> I;
						if (I == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "McFlurry Tiramisu", 10);
						else if (I == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "McFlurry Tiramisu with Extra Coffee Sauce", 12.50);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 2:
						cout << "           1-Normal                           10.00 " << endl << endl;
						cout << "           2-Biscoff Topping                  12.50" << endl << endl;
						cin >> I;
						if (I == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "McFlurry Lotus", 10);
						else if (I == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "McFlurry Lotus with Biscoff Topping", 12.5);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else {
							a = 1;
							break;
						}
						break;

					case 3:
						cout << "           1-Normal                           10.00 " << endl << endl;
						cout << "           2-Extra Oreo                       12.50" << endl << endl;
						cin >> I;
						if (I == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "McFlurry Oreo", 10);
						else if (I == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "McFlurry Oreo with Extra oreo", 12.5);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 4:
						cout << "           1-Normal                           7.00 " << endl << endl;
						cout << "           2-Extra Fudge                      9.50" << endl << endl;
						cin >> I;
						if (I == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Fudge Sundae", 7);
						else if (I == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Fudge Sundae with Extra Fudge", 9.5);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 5:
						cout << "           1-Normal                           7.00 " << endl << endl;
						cout << "           2-Extra Caramel Topping            9.50" << endl << endl;
						cin >> I;
						if (I == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Caramel Sundae", 7);
						else if (I == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Caramel Sundae with Extra Caramel Topping", 9.5);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 6:
						cout << "           1-Chocolate                         1.00" << endl << endl;
						cout << "           2-Vanilla                           1.00" << endl << endl;
						cin >> I;
						if (I == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Chocolate Icecream", 1);
						else if (I == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Vanilla Icecream", 1);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					}//End of Desert Options					
					break;
					//End of Deserts

					//Beverages
				case 3:

					cout << "     1-Orange Juice" << endl << endl;
					cout << "     2-Apple Juice" << endl << endl;
					cout << "     3-Coca Cola" << endl << endl;
					cout << "     4-7Up" << endl << endl;
					cout << "     5-Pepsi" << endl << endl;
					cout << "     6-Mineral Water" << endl << endl;
					int B;
					cin >> B;

					cout << "           ITEM TYPE                        PRICE(SAR)" << endl << endl;
					switch (B)
					{
						//Beverages Option
						int B;
					case 1:
						cout << "           1-Large                            11.00" << endl << endl;
						cout << "           2-Medium                           9.00" << endl << endl;
						cout << "           3-Small                            7.00" << endl << endl;
						cin >> B;
						if (B == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Orange Juice-Large", 11);
						else if (B == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Orange Juice-Medium", 9);
						else if (B == 3)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Orange Juice-Small", 7);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 2:
						cout << "           1-Large                            11.00" << endl << endl;
						cout << "           2-Medium                           9.00" << endl << endl;
						cout << "           3-Small                            7.00" << endl << endl;
						cin >> B;
						if (B == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Apple Juice-Large", 11);
						else  if (B == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Apple Juice-Medium", 9);
						else if (B == 3)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Apple Juice-Small", 7);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 3:
						cout << "           1-Large                            9.00" << endl << endl;
						cout << "           2-Medium                           8.00" << endl << endl;
						cout << "           3-Small                            7.00" << endl << endl;
						cin >> B;
						if (B == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Coca Cola-Large", 9);
						else if (B == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Coca Cola-Medium", 8);
						else if (B == 3)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Coca Cola-Small", 7);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 4:
						cout << "           1-Large                            9.00" << endl << endl;
						cout << "           2-Medium                           8.00" << endl << endl;
						cout << "           3-Small                            7.00" << endl << endl;
						cin >> B;
						if (B == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "7Up-Large", 9);
						else if (B == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "7Up-Medium", 8);
						else if (B == 3)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "7Up-Small", 7);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 5:
						cout << "           1-Large                            9.00" << endl << endl;
						cout << "           2-Medium                           8.00" << endl << endl;
						cout << "           3-Small                            7.00" << endl << endl;
						cin >> B;
						if (B == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Pepsi-Large", 9);
						else if (B == 2)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Pepsi-Medium", 8);
						else if (B == 3)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Pepsi-Small", 7);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					case 6:
						cout << "           1-Normal                            3.00" << endl << endl;
						cin >> B;
						if (B == 1)
							insertDLL(mcdArray[tablenum].first, mcdArray[tablenum].serial, "Mineral Water", 3);
						cout << "Would you like to order more?(Y/N)" << endl;
						cin >> C;
						if (C == 'Y' || C == 'y')
							continue;
						else
						{
							cout << "Confirm order?(Y/N)" << endl;
							char con;
							cin >> con;
							if (con == 'n' || con == 'N')
								continue;
							else
							{
								a = 1;
								break;
							}
						}
						break;

					}//End of Beverages Options
					break;
					//End of Beverages
				}//End of menu
				break;

			}//End of for loop for Mcdonald

			/*Insert code to send info to database*/
			DLL* p = mcdArray[tablenum].first ;
			string price;
			while (p != NULL)
			{
				price = to_string(p->price);
				insertMcdOrder(tno, p->OrderName, price);
				p = p->next;
			}

			if (a == 1)
			{
				tableOrders.insert({ tablenum,mcdArray[tablenum].first });
				cout << "     Here is your complete order:" << endl << endl;
				cout << "         ITEM TYPE -> PRICE" << endl << endl;
				DLL* p = mcdArray[tablenum].first;
				Display(mcdArray[tablenum].first);
				double total = 0;
				while (p != NULL)
				{
					total += p->price;
					p = p->next;
				}
				cout << "     Total = " << total << "SAR"<<endl << endl;
				//Inorder(root);
				system("pause");
				cout << "    Would you like to pay now? (Y/N) " << endl << endl;
				string tempcheck;
				cin >> tempcheck;
				if (tempcheck == "N" or tempcheck == "n")
					continue;
				cout << "     How would you like to pay for your order:" << endl;
				cout << "     1)Cash" << endl;
				cout << "     2)Card" << endl;
				int payment;
				cin >> payment;
				if (payment == 1)
				{
					cout << "     Please enter table number and a person from the restaurant will come to take your cash " << endl;
					int tnum;
					cin >> tnum;
					B[tnum + 1] = tnum;
					mcd.remove(tnum);
					//Delete(root, tnum);
					//Inorder(root);
					/*Insert code to delete info from database*/
					string tno = to_string(tnum);
					deleteMcdName(tno);
					deleteMcdOrder(tno);
				}
				else if (payment == 2)
				{
					cout << "     Please enter your 16-digit card number" << endl;
					long long cardnum;
					cin >> cardnum;
					cout << "     Please enter your password" << endl;
					string password;
					cin >> password;
					cout << "     Please enter your table number" << endl;
					int tnum;
					cin >> tnum;
					B[tnum + 1] = tnum;
					mcd.remove(tnum);
					//Delete(root, tnum);
					//Inorder(root);
					/*Insert code to delete info from database*/
					string tno = to_string(tnum);
					deleteMcdName(tno);
					deleteMcdOrder(tno);
				}
			}
		}
		break;//breaking out of second restaurant 
		}//End of main switch statement
	}
	else
		cout << "Please Enter 1";
	cout << "    End program? (Y/N) " << endl << endl;
	string end;
	cin >> end;
	if (end == "y" or end == "Y")
		break;
	}// end of while program
	/*Tree* temp = SearchTable(1);
	if (temp != NULL)
		cout << temp->tableNumber << " " << temp->Name << endl;
	else
		cout << "Not found" << endl;*/
}//End of main


