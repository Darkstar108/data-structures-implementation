#include<iostream>
using namespace std;

const int n = 6;

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

//Helper function to reset visited array
void resetVisited (int visited[n]) {
	int i, j;
	for(i = 0; i < n; ++i) 
		visited[i] = 0;
}

//Copy Matrix G to C
void copyMatrix(int G[n][n], int C[n][n]) {
	int i, j;
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j)
			C[i][j] = G[i][j];
	}
}

//Helper function for max flow to perform DFS and find paths
int maxFlowDFS (int F[n][n], int u, int sink, int visited[n], lqueue Q, int max = 62000) {
	int v;
	visited[u] = 1;
	enque(Q, u);
	if(u == sink) {
		u = deque(Q);
		v = deque(Q);
		while(v != -1) {
			F[v][u] = F[v][u]-max;
			F[u][v] = F[u][v]+max;
			u = v;
			v = deque(Q);
		}
		return -1;
	}
	else {
		for(v = 0; v < n; ++v) {
			if(F[v][u] > 0 && visited[v] == 0) {
				if(F[v][u] < max)
					max = F[v][u];
				int tmp = max;
				max = maxFlowDFS(F, v, sink, visited, Q, max);
				if(max == -1)
					break;
				max = tmp;
			}
		}
	}
	return max;
}

//Function to calculate max flow from source to sink
int maxFlow (int G[n][n], int source, int sink) {
	lqueue Q;
	Q.f = Q.r = NULL;
	int visited[n], F[n][n], i, j, max = -1;
	copyMatrix(G, F);
	while(max == -1) {
		resetVisited(visited);
		max = maxFlowDFS(F, source, sink, visited, Q);
	}
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j)
			cout<<F[i][j] <<" ";
		cout<<"\n";
	}
	max = 0;
	for(i = 0; i < n; ++i) 
		max = max+F[i][sink];
	return max;
}

int main ()
{
	int G[n][n] = {{0,0,0,0,0,0},{16,4,0,0,0,0},{13,0,0,9,0,0},{0,12,0,0,7,0},{0,0,14,0,0,0},{0,0,0,20,4,0}}; 
	//int G[n][n] = {{0,0,0,0,0,0},{10,0,0,0,0,0},{0,4,0,0,6,0},{10,2,0,0,0,0},{0,8,0,9,0,0},{0,0,10,0,10,0}}; 
	int C[n][n];
	copyMatrix(G, C);
	cout<<"\nMax Flow: " <<maxFlow(C, 0, 5);
	return 0;
}

