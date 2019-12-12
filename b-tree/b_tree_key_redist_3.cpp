#include<iostream>
using namespace std;

const int d = 1;

struct bdnode {
	int n;
	int key[2*d+1];
	struct bdnode *ptr[2*d+2];
};
typedef struct bdnode *BDPTR;

struct lqueue {
	struct lnode *f;
	struct lnode *r;
};

struct lstack {
	struct lnode *top;
};

struct lnode {
	BDPTR data;
	struct lnode *next;
};
typedef struct lnode* LPTR;

//Function Prototypes of Functions related to Create
int isLeaf (BDPTR T, int i);
void sortKeys (int key[], BDPTR ptr[], int n);
void insert (BDPTR &T, int k);
void copy (BDPTR Node, BDPTR O, int beg, int end, int dif);
int checkSibling (BDPTR P, BDPTR T, int i);
void medianPtr(BDPTR &T, int med, BDPTR oldNode, BDPTR newNode);
void splitNode (BDPTR &oldNode, BDPTR &P);
void keyRedist (BDPTR &T, BDPTR &P, int s);
void addNode (BDPTR &T, int k, BDPTR P, BDPTR GP);

//Push
void push (lstack &S, BDPTR x) {
	LPTR T;
	T = new(lnode);
	T->data = x;
	T->next = S.top;
	S.top = T;
}

//Pop
BDPTR pop (lstack &S) {
	LPTR T = S.top;
	BDPTR x;
	if(S.top == NULL)
		return NULL;
	S.top = T->next;
	x = T->data;
	delete(T);
	return x;
}

//Top
BDPTR Top (lstack S) {
	if(S.top == NULL)
		return NULL;
	return S.top->data;
}

//Enque
void enque (lqueue &Q, BDPTR T) {
	LPTR I = new(lnode);
	I->data = T;
	if(Q.r == NULL) {
		Q.r = I;
		Q.f = I;
	}
	else
		Q.r->next = I;
	Q.r = I;	
	I->next = NULL;	
}

//Deque
BDPTR deque (lqueue &Q) {
	LPTR I = Q.f;
	BDPTR T;
	if(Q.f == NULL)
		return NULL;
	if(Q.f == Q.r) {
		Q.f = NULL;
		Q.r = NULL;
	}
	else
		Q.f = I->next;
	T = I->data;
	delete(I);
	return T;
}

//Check if node is leaf
int isLeaf (BDPTR T, int i = 0) {
	if(i == 2*d)
		return 1;
	if(T->ptr[i])
		return 0;
	else
		isLeaf(T, i+1);
}

//Sort Keys
void sortKeys (int key[], BDPTR ptr[], int n) {
	int i;
	for(i = n; i > 0; --i) {
		if(key[i] < key[i-1]) {
			//Swap Keys
			int tmp = key[i-1];
			key[i-1] = key[i];
			key[i] = tmp;
			//Swap Pointers
			BDPTR I = ptr[i+1];
			ptr[i+1] = ptr[i];
			ptr[i] = I;
		}
		else
			break;
	}
}

//Add key to the correct child
void addKeyChild (BDPTR T, int k, int i = 0) {
	if(i == T->n) {
		insert(T->ptr[i], k);
		return;
	}
	if(k < T->key[i]) {
		insert(T->ptr[i], k);
		return;
	}
	else
		addKeyChild(T, k, i+1);
}

//Insert key to Node and Sort keys and pointers
void insert (BDPTR &T, int k) {
	T->key[T->n] = k;
	++T->n;
	int i;
	sortKeys(T->key, T->ptr, T->n-1);
}

//Helper Function for Node Splitting to copy keys & pointers
//dif used for entering to new node, beg & end to control loop
void copy (BDPTR Node, BDPTR O, int beg, int end, int dif = 0) {
	int i;
	for(i = beg; i < end; ++i) {
		if(O) {
			Node->key[i] = O->key[i+dif];
			Node->ptr[i+1] = O->ptr[i+1+dif];
		}
		else {
			Node->key[i] = -1;
			Node->ptr[i+1] = NULL;
		}
	}
}

//Check if there exists sibling with space
int checkSibling (BDPTR T, BDPTR P, int s = -1, int i = 0) {
	if(i == P->n+1)
		return s;
	if(P->ptr[i] == T) {
		return checkSibling(T, P, s, i+1);
	}
	//Sibling with space exists
	if(P->ptr[i] && P->ptr[i]->n != 2*d) {
		//Sibling next to overflow node
		if(P->ptr[i+1] == T || P->ptr[i-1] == T)
			return i;
		else {
			s = i;
		}	
	}
	return checkSibling(T, P, s, i+1);
}

//Check if cousin exists with space 
int checkCousin(BDPTR T, BDPTR P, BDPTR GP, int p = 0, int c = -1) {
	//GrandParent dosent exist
	if(GP == NULL)
		return -1;
	if(p > GP->n)
		return c;
	if(GP->ptr[p] == P)
		return checkCousin(T, P, GP, p+1, c);
	int t = 0;
	//Check sibling of parent for cousins with space
	while(t <= GP->ptr[p]->n) {
		if(GP->ptr[p]->ptr[t]->n == 2*d)
			++t;
		else {
			//If adjecent sibling of Parent
			if(GP->ptr[p+1] == P || GP->ptr[p-1] == P)
				return p;
			c = p;
			return checkCousin(T, P, GP, p+1, c);
		}
	}
	return checkCousin(T, P, GP, p+1, c);
}

//Insert old and new node after split
void medianPtr(BDPTR &T, int med, BDPTR oldNode, BDPTR newNode) {
	int i;
	for(i = 0; i <= 2*d; ++i) {
		if(T->key[i] == med)
			break;
	}
	T->ptr[i] = oldNode;
	T->ptr[i+1] = newNode;
}

//Perform Node Splitting
void splitNode (BDPTR &oldNode, BDPTR &P) {
	BDPTR newNode = new(bdnode);
	int i;
	//Copying right of median to new node
	copy(newNode, oldNode, 0, d, d+1);
	newNode->ptr[0] = oldNode->ptr[d+1];
	newNode->n = d;
	//Making rest of new node empty
	copy(newNode, NULL, d, 2*d+1);
	//Store median
	i = oldNode->key[d];
	//Making rest of old node empty
	copy(oldNode, NULL, d, 2*d+1);
	oldNode->n = d;
	
	if(P == NULL) {
		addNode(P, i, NULL, NULL);
		medianPtr(P, i, oldNode, newNode);
		oldNode = P;
	}
	else {
		insert(P, i);
		medianPtr(P, i, oldNode, newNode);
	}
}

//Helper function for Key Redistribution, stores keys in an array
int storeKeysArr (int arr[], BDPTR P) {
	int i = 0, j = 0, n = 0;
	for(i = 0; i <= P->n; ++i) {
		if(P->ptr[i]) {
			for(j = 0; j < P->ptr[i]->n; ++j)
				arr[n++] = P->ptr[i]->key[j];
		}
		if(P->key[i] != -1)
			arr[n++] = P->key[i];
	}
	return n;
}

//Helper Function for Key Redistribution, inserts keys from array
void insertFromArr (int arr[], BDPTR &P, int n) {
	int i = 0, j = 0, k = 0;
	while(i != n) {
		if(P->ptr[j]->n == 2*d) {
			P->key[j] = arr[i++];
			++j; k = 0;
		}
		else {
			P->ptr[j]->key[k++] = arr[i++];
			++P->ptr[j]->n;
		}
	}
}

//Helper Function for Key Redistribution, empties all children for P
void emptyChildren (BDPTR &P) {
	int i;
	for(i = 0; i <= P->n; ++i) {
		copy(P->ptr[i], NULL, 0, 2*d+1);
		P->ptr[i]->ptr[0] = NULL;
		P->ptr[i]->n = 0;
	}
}

//Helper Function for Key Redistribution 3, shifts array elements 1 step to left and returns 1st or last element
int shiftArr (int arr[], int &n, int p) {
	int i, key;
	if(p == -1) {
		key = arr[0];
		for(i = 0; i < n-1; ++i)
			arr[i] = arr[i+1];
		--n;
	}
	else {
		key = arr[--n];
	}
	return key;
}

//Key Redistribution 2 (With any siblings)
void keyRedist2 (BDPTR &P) {
	int arr[8*d+1];
	int a;
	//Store all keys in an array
	a = storeKeysArr(arr, P);
	//Emptying all siblings
	emptyChildren(P);
	//Inserting keys
	insertFromArr(arr, P, a);
}

//Key Redistribution 3 (With Cousins)
void keyRedist3 (BDPTR &P, BDPTR &GP, int c) {
	int i, p, key, tmp, arr[8*d+1], a;
	//Find position of parent
	for(p = 0; i <= GP->n; ++p) {
		if(GP->ptr[p] == P)
			break;
	}
	while(p != i) {
		a = storeKeysArr(arr, GP->ptr[p]);
		if(p > c)
			key = shiftArr(arr, a, -1);
		else
			key = shiftArr(arr, a, 1);
		emptyChildren(GP->ptr[p]);
		insertFromArr(arr, GP->ptr[p], a);
		if(p > c) {
			addKeyChild(GP->ptr[p-1], GP->key[p-1]);
			GP->key[p-1] = key;
			--p;
		}
		else {
			addKeyChild(GP->ptr[p+1], GP->key[p]);
			GP->key[p] = key;
			++p;
		}	
	}
	keyRedist2(GP->ptr[p]);
}

//Add key to B-Tree
//P = Parent, GP = GrandParent
void addNode (BDPTR &T, int k, BDPTR P, BDPTR GP) {
	if(T) {
		int i;
		//If leaf node
		if(isLeaf(T) == 1) {
			insert(T, k);
		}
		//If non Leaf
		else {
			for(i = 0; i <= 2*d; ++i) {
				if(k == T->key[i])
					return;
				if(i == 2*d) {
					addNode(T->ptr[i], k, T, P);
					break;	
				}
				if(k < T->key[i]) {
					if(T->ptr[i])
						addNode(T->ptr[i], k, T, P);
					else
						insert(T, k);
					break;
				}
				if(T->key[i] == -1) {
					addNode(T->ptr[i], k, T, P);
					break;
				}
			}
		}
		//Check overflow
		if(T->n == 2*d+1) {
			//If root node
			if(P == NULL)
				splitNode(T, P);
			else {
				int s = checkSibling(T, P);
				//If no sibling with space
				if(s == -1) {
					s = checkCousin(T, P, GP);
					//No cousin with space exists
					if(s == -1)
						splitNode(T, P);
					//Cousin with space exists
					else {
						keyRedist3(P, GP, s);
					}
				}
				else {
					keyRedist2(P);
				}
			}
		}
	}
	//Tree doesnt exist
	else {
		T = new(bdnode);
		T->n = 1;
		T->key[0] = k;
		int i;
		for(i = 1; i < 2*d+1; ++i)
			T->key[i] = -1;
		for(i = 0; i < 2*d+2; ++i)
			T->ptr[i] = NULL;
		return;
	}
}

//To find a key in M-way Search Tree
int find (BDPTR T, int k, int c = 0) {
	if(T) {
		if(c < 2*d) {
			if(k < T->key[c])
				find(T->ptr[c], k);
			else if(k > T->key[c])
				find(T, k, c+1);
			else
				return 1;
		}
		else
			find(T->ptr[2*d], k);
	}
	else
		return 0;
}

void printLevelByLine (BDPTR T) {
	lqueue Q;
	Q.f = NULL;
	Q.r = NULL;
	int i;
	enque(Q, T);
	BDPTR Tmp = new(bdnode);
	Tmp->n = -1;
	enque(Q, Tmp);
	T = deque(Q);
	cout<<"____________________________________________________________\n";	
	while(T != NULL) {
		if(T->n == -1) {
			cout<<"\n";
			T = deque(Q);
			enque(Q, Tmp);
			continue;
		}
		else {
			for(i = 0; i <= T->n; ++i) {
				if(T->ptr[i])
					cout<<"(" <<T->ptr[i]->key[0] <<")";
				else
					cout<<"(.)";
				cout<<T->key[i] <<" ";
			}
			cout<<"\t";
		}
		for(i = 0; i <= T->n; ++i) {
			if(T->ptr[i])
				enque(Q, T->ptr[i]);
		}
		T = deque(Q);
	}
}

int main ()
{
	BDPTR T = NULL;
	int a[] = {9,2,15,30,5,40,25,7,3,20,10,22}, n = 12, i;
	//int a[] = {7,2,5,9,8,6,4,1,3}, n = 9, i;
	for(i = 0; i < n; ++i) {
		addNode(T, a[i], NULL, NULL); 
		printLevelByLine(T);
	}
    return 0;
}

