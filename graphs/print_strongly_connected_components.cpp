#include<iostream>
using namespace std;

const int n = 10;

struct lqueue {
	struct lnode *f;
	struct lnode *r;
};

struct lstack {
	struct lnode *top;
};

struct lnode {
	int data;
	struct lnode *next;
};
typedef struct lnode* LPTR;

//Push
void push (lstack &S, int x) {
	LPTR T;
	T = new(lnode);
	T->data = x;
	T->next = S.top;
	S.top = T;
}

//Pop
int pop (lstack &S) {
	LPTR T = S.top;
	int x;
	if(S.top == NULL)
		return -1;
	S.top = T->next;
	x = T->data;
	delete(T);
	return x;
}

//Top
int Top (lstack S) {
	if(S.top == NULL)
		return -1;
	return S.top->data;
}

//Enque
void enque (lqueue &Q, int T) {
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
int deque (lqueue &Q) {
	LPTR I = Q.f;
	int T;
	if(Q.f == NULL)
		return -1;
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

//Helper function to reset arrays
void resetArray(int visited[n], int i = 0) {
	if(i == n)
		return;
	visited[i] = 0;
	resetArray(visited, i+1);
}

//Checks if visited array is full and returns 1st non visited vertex
int checkVisited (int visited[n]) {
	int i, u = -1;
	for(i = 0; i < n; ++i) {
		if(visited[i] == 0) {
			u = i;
			break;
		}
	}
	return u;
}

//Finds vertex with max post order
int maxPostOrder (int postOrder[n]) {
	int i, max = -1, u = -1;
	for(i = 0; i < n; ++i) {
		if(postOrder[i] > max) {
			u = i;
			max = postOrder[i];
		}
	}
	return u;
}

//To perform Depth First Traversaln and push visited into stack for post order
void getPostOrderHelper (int G[n][n], int u, int visited[n], lstack &S) {
	int v;
	visited[u] = 1;
	push(S, u);
	for(v = 0; v < n; ++v) {
		if(G[v][u] == 1 && visited[v] != 1) {
			getPostOrderHelper(G, v, visited, S);
		}
	}
}

//Get post order of vertices using DFT
void getPostOrder(int G[n][n], int u, int visited[n], int postOrder[n]) {
	int i, post = 1;
	resetArray(visited);
	lstack S;
	S.top = NULL;
	while(u != -1) {
		getPostOrderHelper(G, u, visited, S);
		u = checkVisited(visited);
		i = pop(S);
		while(i != -1) {
			postOrder[i] = post++;
			i = pop(S);
		}
	}
}

//To perform Depth First Traversal
void printPostOrderHelper (int G[n][n], int u, int visited[n], int postOrder[n]) {
	int v;
	visited[u] = 1;
	postOrder[u] = -1;
	for(v = 0; v < n; ++v) {
		if(G[v][u] == 1 && visited[v] != 1) {
			printPostOrderHelper(G, v, visited, postOrder);
		}
	}
	cout<<u+1 <<" ";
}

//Print DFT using post order
void printPostOrder(int G[n][n], int visited[n], int postOrder[n]) {
	int u = maxPostOrder(postOrder);
	resetArray(visited);
	while(u != -1) {
		printPostOrderHelper(G, u, visited, postOrder);
		cout<<"\n";
		u = maxPostOrder(postOrder);
	}
}

//To perform Depth First Traversal
void printPostOrderHelper (int G[n][n], int u, int visited[n], int postOrder[n], lstack &S, int flag) {
	int v;
	visited[u] = 1;
	if(flag == 0)
		push(S, u);
	else
		postOrder[u] = -1;
	for(v = 0; v < n; ++v) {
		if(G[v][u] == 1 && visited[v] != 1) {
			printPostOrderHelper(G, v, visited, postOrder, S, flag);
		}
	}
	if(flag == 1)
		cout<<u+1 <<" ";
}

//Get post order(flag = 0) and print post order DFT(flag = 1)
void printPostOrder(int G[n][n], int u, int visited[n], int postOrder[n], int flag) {
	lstack S;
	S.top = NULL;
	resetArray(visited);
	int i, post = 1;
	//Get post order statements
	if(flag == 1)
		u = maxPostOrder(postOrder);
	while(u != -1) {
		printPostOrderHelper(G, u, visited, postOrder, S, flag);
		if(flag == 0) {
			u = checkVisited(visited);
			i = pop(S);
			while(i != -1) {
				postOrder[i] = post++;
				i = pop(S);
			}
		}
		else {
			cout<<"\n";
			u = maxPostOrder(postOrder);
		}
	}
}

//Reverses a graph
void reverseGraph(int G[n][n], int R[n][n]) {
	int i, j;
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j) 
			R[i][j] = G[j][i];
	}
}

//Prints strongly connected components of graph with 2 DFT function
void printStrongComponents (int G[n][n], int u, int visited[n]) {
	int postOrder[n], R[n][n];
	resetArray(postOrder);
	getPostOrder(G, u, visited, postOrder);
	reverseGraph(G, R);
	printPostOrder(R, visited, postOrder);
}

//Prints strongly connected components of graph with 1 DFT function
void printStrongComponents1DFT (int G[n][n], int u, int visited[n]) {
	int postOrder[n], R[n][n];
	resetArray(postOrder);
	printPostOrder(G, u, visited, postOrder, 0);
	reverseGraph(G, R);
	printPostOrder(R, u, visited, postOrder, 1);
}

int main ()
{
	int G[n][n] = {{0,0,1,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,0,0,0},
					{0,1,0,0,0,1,0,0,0,0},{1,0,1,0,0,0,0,0,0,0},
					{0,0,1,1,0,0,0,0,0,0},{0,1,0,0,0,0,1,0,0,0},
					{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,1,0,1,0},
					{0,0,0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,1,0,0}};
	int i, visited[n];
	printStrongComponents(G, 0, visited);
	cout<<"\n";
	printStrongComponents1DFT(G, 0, visited);
	return 0;
}

