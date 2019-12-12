#include<iostream>
using namespace std;

const int d = 2;

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
void addNode (BDPTR &T, int k, BDPTR P);

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
int checkSibling (BDPTR P, BDPTR T, int s = -1, int i = 0) {
	if(i == P->n+1)
		return s;
	if(P->ptr[i] == T) {
		return checkSibling(P, T, s, i+1);
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
	return checkSibling(P, T, s, i+1);
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
		addNode(P, i, NULL);
		medianPtr(P, i, oldNode, newNode);
		oldNode = P;
	}
	else {
		insert(P, i);
		medianPtr(P, i, oldNode, newNode);
	}
}

//Helper function for Key Redistribution 1, returns pointer no. of child T in Parent P
int findChildPtr (BDPTR T, BDPTR P) {
	int i;
	for(i = 0; i <= P->n; ++i) {
		if(P->ptr[i] == T)
			return i;
	}
	return 0;
}

//Helper function for Key Redistribution 1, used to shift keys to the left
void shiftKeysLeft (BDPTR &T, BDPTR &P, int s) {
	insert(P->ptr[s], P->key[s]);
	P->key[s] = T->key[0];
	int i;
	for(i = 0; i < 2*d; ++i) {
		T->key[i] = T->key[i+1];
		T->ptr[i] = T->ptr[i+1];
	}
	--T->n;
	T->key[T->n] = -1;
}

//Helper function for Key Redistribution 1, used to shift keys to the right
void shiftKeysRight (BDPTR &T, BDPTR &P, int s) {
	insert(P->ptr[s], P->key[s-1]);
	P->key[s-1] = T->key[T->n-1];
	--T->n;
	T->key[T->n] = -1;
}

//Key Redistribution 2 (With any siblings by shifting keys)
void keyRedist2 (BDPTR &T, BDPTR &P, int s) {
	int t = findChildPtr(T, P);
	//Shift Leftwards
	if(s < t) {
		while(t > s) {
			shiftKeysLeft(P->ptr[t], P, t-1);
			--t;
		}
	}
	//Shift Rightwards
	else {
		while(t < s) {
			shiftKeysRight(P->ptr[t], P, t+1);
			++t;
		}
	}
}

//Add key to B-Tree
//P = Parent
void addNode (BDPTR &T, int k, BDPTR P) {
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
				if(k < T->key[i]) {
					addNode(T->ptr[i], k, T);
					break;
				}
				if(T->key[i] == -1) {
					addNode(T->ptr[i], k, T);
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
				int s = checkSibling(P, T);
				//If no sibling with space
				if(s == -1)
					splitNode(T, P);
				else {
					keyRedist2(T, P, s);
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
	int a[] = {4,7,22,21,35,12,23,39,16,32,29,46,28,43,64,9,49,53,40,42,24,60,5,20,70}, n = 25, i;
	//int a[] = {7,2,5,9,8,6,4,1,3}, n = 9, i;
	for(i = 0; i < n; ++i) {
		addNode(T, a[i], NULL); 
		printLevelByLine(T);
	}
    return 0;
}

