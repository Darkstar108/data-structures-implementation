#include<iostream>
using namespace std;

const int n = 6;

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

int checkEdge (int G[n][n]) {
	for(int j = 0; j < n; ++j) {
    	for(int k = 0; k < n; ++k)
    		if(G[j][k] == 1)
    			return 1;
    }
    return 0;
}

//To perform Depth First Traversal
void depthFirst (int G[n][n], int u) {
	int v;
	cout<<u+1 <<" ";
	for(v = 0; v < n; ++v) {
		if(G[v][u] == 1) {
			G[v][u] = 0;
			G[u][v] = 0;
			depthFirst(G, v);
			break;
		}
	}
}

int main ()
{
	int G[n][n] = {{0,1,0,1,1,1},{1,0,1,1,1,0},{0,1,0,1,0,0},{1,1,1,0,1,0},{1,1,0,1,0,1},{1,0,0,0,1,0}};
    lqueue Q;
    Q.f = NULL;
    Q.r = NULL;
	depthFirst(G, 0);
    cout<<"\n";
    for(int j = 0; j < n; ++j) {
    	for(int k = 0; k < n; ++k)
    		cout<<G[j][k] <<" ";
    	cout<<"\n";
    }
	return 0;
}

