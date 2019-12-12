#include<iostream>
using namespace std;

const int n = 7;

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
void resetArray (int visited[n], int dist[n], int prev[n], int i = 0) {
	if(i == n)
		return;
	visited[i] = 0;
	dist[i] = 62000;
	prev[i] = -1;
	resetArray(visited, dist, prev, i+1);
}

//Count degree of vertex in Undirected Graph
int degree (int G[n][n], int v) {
	int i, degree = 0;
	for(i = 0; i < n; ++i) {
		if(G[v][i] == 1)
			++degree;
	}
	return degree;
}

//Count indegree of vertex
int indegree (int G[n][n], int v) {
	int i, indegree = 0;
	for(i = 0; i < n; ++i) {
		if(G[v][i] == 1)
			++indegree;
	}
	return indegree;
}

//Count outdegree of vertex
int outdegree (int G[n][n], int v) {
	int i, outdegree = 0;
	for(i = 0; i < n; ++i) {
		if(G[i][v] == 1)
			++outdegree;
	}
	return outdegree;
}

//Copy Matrix G to C
void copyMatrix(int G[n][n], int C[n][n]) {
	int i, j;
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j)
			C[i][j] = G[i][j];
	}
}

//Counts Edges of Graph
int countEdges (int G[n][n]) {
	int i, j, edges = 0;
	for(i = 0; i < n; ++i) {
		for(j = i+1; j < n; ++j) {
			if(G[j][i] == 1)
				++edges;
		}
	}
	return edges;
}

//Insert k to Heap
void insert (int H[], int n, int k, int j) {
	if(j == n)
		H[j] = k;
	if(j > 0 && H[j] < H[(j-1)/2]) {
		int tmp = H[(j-1)/2];
		H[(j-1)/2]  = H[j];
		H[j] = tmp;
		insert(H, n, k, (j-1)/2);
	}
}

//Delete key from Heap
void heapDel (int H[], int n) {
	int i = 0, min;
	cout<<H[i] <<" ";
	H[i] = H[--n];
	while(i < n) {
		min = (H[2*i+1] < H[2*i+2]) ? 2*i+1 : 2*i+2;
		if(min >= n)
			break;
		if(H[i] > H[min]) {
			int tmp = H[min];
			H[min] = H[i];
			H[i] = tmp;
			i = min;
		}
		else
			break;
	}
}

//Inserts edges to heap
void insertEdgeHeap (int H[], int G[n][n], int size) {
	int i, j;
	for(i = 0; i < n; ++i) {
		for(j = i+1; j < n; ++j) {
			if(G[i][j] > 0)
				insert(H, size, G[i][j], size);
		}
	}
}

//Print Dijkstra's shortest path
void dijkstra (int G[n][n], int visited[n], int dist[n], int prev[n], int s, int e) {
	//When we reach destination
	if(s == e) {
		lstack S;
		G[prev[s]][s] = 0;
		S.top = NULL;
		push(S, s);
		int i = prev[s];
		cout<<"\n";
		while(i != -1) {
			push(S, i);
			i = prev[i];
		}
		i = pop(S);
		while(i != -1) {
			cout<<i+1 <<" ";
			i = pop(S);
		}
		return;
	}
	if(dist[s] == 62000)
		dist[s] = 0;
	int v;
	for(v = 0; v < n; ++v) {
		if(G[v][s] > 0 && visited[v] == 0 && (dist[v] > dist[s] + G[v][s])) {
			dist[v] = dist[s]+G[v][s];
			prev[v] = s;
		}
	}
	visited[s] = 1;
	dijkstra(G, visited, dist, prev, s, e);
}

int main ()
{
	int G[n][n] = {{0,0,4,0,0,0,0},{2,0,0,0,0,0,0},{0,0,0,2,0,0,0},{1,3,0,0,0,0,0},
					{0,10,0,2,0,0,0},{0,0,5,8,0,0,1},{0,0,0,4,6,0,0}}; 
	int i, visited[n], dist[n], prev[n];
	int C[n][n];
	copyMatrix(G, C);
	const int edges = countEdges(C);
	int H[edges];
	insertEdgeHeap(H, C, edges);
	for(i = 0; i < n; ++i) 
		heapDel(H, n-i);
	resetArray(visited, dist, prev);
	dijkstra(C, visited, dist, prev, 0, 5); 
	return 0;
}

