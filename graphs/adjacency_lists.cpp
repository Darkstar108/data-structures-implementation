#include<iostream>
using namespace std;

const int n = 5;

struct inner {
	inner* rptr;
	int data;
};

struct outer {
	outer *dptr;
	inner *rptr;
	int data;
};

struct lqueue {
	struct lnode *f;
	struct lnode *r;
};

struct lnode {
	int data;
	struct lnode *next;
};
typedef struct lnode* LPTR;

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

//Helper function of Create Outer that creates inner nodes
inner *createInner (inner *I, int list[n][n], int i, int j = 1) {
	if(list[i][j] == '\0')
		return NULL;
	I = new(inner);
	I->data = list[i][j];
	I->rptr = createInner(I->rptr, list, i, j+1);
	return I;
}

//Create adjacency list
outer *createOuter (outer *G, int list[n][n], int i = 0) {
	if(i == n)
		return NULL;
	int j = 0;
	G = new(outer);
	G->data = list[i][j++];
	G->rptr = createInner(G->rptr, list, i);
	G->dptr = createOuter(G->dptr, list, i+1);
	return G;
}

//Print adjacency list
void printList (outer *G) {
	if(G == NULL)
		return;
	inner *I = G->rptr;
	cout<<G->data;
	while(I != NULL) {
		cout<<"->" <<I->data;
		I = I->rptr;
	}
	cout<<"\n";
	printList(G->dptr);
}

//Helper function to reset visisted array
void resetVisited (int visited[n], int i = 0) {
	if(i == n)
		return;
	visited[i] = 0;
	resetVisited(visited, i+1);
}

//To perform Breadth First Traversal
void breadthFirst (outer *G, int visited[n]) {
	outer *O = G;
	lqueue Q;
	Q.f = NULL;
	Q.r = NULL;
	int v, u = O->data;
	enque(Q, u);
	visited[u-1] = 1;
	u = deque(Q);
	while(u != -1) {
		inner *I = O->rptr;
		while(I != NULL) {
			v = I->data;
			if(visited[v-1] == 0) {
				enque(Q, v);
				visited[v-1] = 1;
				cout<<"\n" <<u <<"->" <<v;
			}
			I = I->rptr;
		}
		u = deque(Q);
		O = G;
		for(v = 0; v < u-1; ++v) 
			O = O->dptr;
	}
}

//To perform Depth First Traversal
void depthFirst (outer *G, int u, int visited[n]) {
	int v;
	visited[u-1] = 1;
	outer *O = G;
	for(v = 0; v < u-1; ++v)
		O = O->dptr;
	inner *I = O->rptr;
	while(I != NULL) {
		v = I->data;
		if(visited[v-1] == 0) {
			cout<<"\n" <<u <<"->" <<v;
			depthFirst(G, v, visited);
		}
		I = I->rptr;
	}
}

//To print all paths
int printPaths (outer *G, int s, int e, int visited[n], lqueue Q, int paths = 0) {
	int v;
	enque(Q, s);
	visited[s-1] = 1;
	outer *O = G;
	for(v = 0; v < s-1; ++v)
		O = O->dptr;
	inner *I = O->rptr;
	while(I != NULL) {
		v = I->data;
		if(v == e) {
			v = deque(Q);
			cout<<"\n" <<v;
			v = deque(Q);
			while(v != -1) {
				cout<<"->" <<v;
				v = deque(Q);
			}
			cout<<"->" <<e;
			++paths;
			break;
		}
		if(visited[v-1] == 0) {
			paths = printPaths(G, v, e, visited, Q, paths);
		}
		I = I->rptr;
	}
	visited[s-1] = 0;
	return paths;
}

//To print all possible DFT's
void printDFTs (outer *G, int u, int visited[n], lqueue Q, int count = 0) {
	int v;
	enque(Q, u);
	visited[u-1] = 1;
	++count;
	outer *O = G;
	for(v = 0; v < u-1; ++v)
		O = O->dptr;
	inner *I = O->rptr;
	while(I != NULL) {
		v = I->data;
		if(visited[v-1] == 0) {
			printDFTs(G, v, visited, Q, count);
		}
		I = I->rptr;
	}
	if(count == n) {
		v = deque(Q);
			cout<<"\n" <<v;
			v = deque(Q);
			while(v != -1) {
				cout<<"->" <<v;
				v = deque(Q);
			}
	}
	visited[u-1] = 0;
}

//To check if there is a cycle in the graph
int checkCycle (outer *G, int u, int visited[n], int prev = -1) {
	int v;
	visited[u-1] = 1;
	outer *O = G;
	for(v = 0; v < u-1; ++v)
		O = O->dptr;
	inner *I = O->rptr;
	while(I != NULL) {
		v = I->data;
		if(visited[v-1] == 1 && v != prev && prev != -1) 
			return 1;
		else if(visited[v-1] == 0) 
			return checkCycle(G, v, visited, u);
		I = I->rptr;
	}
	return 0;
}

int main ()
{
	int list[n][n] = {{1,2,3,4},{2,1,3,5},{3,1,2,4},{4,1,3,5},{5,2,4}}; //Undirected Cyclic
	//int list[n][n] = {{1,2,3},{2,3,5},{3},{4,1,3},{5,4}}; //Directed Acyclic
	int i, visited[n];
	lqueue Q;
	Q.f = NULL;
	Q.r = NULL;
	resetVisited(visited);
	outer *G = NULL;
	G = createOuter(G, list);
	printList(G);
	cout<<"\nCycle?: " <<checkCycle(G, 1, visited);
    resetVisited(visited);		
	breadthFirst(G, visited);
	cout<<"\n-----------------------------";
	resetVisited(visited);
	depthFirst(G, G->data, visited);
	resetVisited(visited);
	cout<<"\n-----------------------------";
	cout<<"\nPaths: " <<printPaths(G, 1, 5, visited, Q);
	resetVisited(visited);
	cout<<"\n-----------------------------";
	printDFTs(G, G->data, visited, Q);	
    return 0;
}

