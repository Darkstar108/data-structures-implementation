#include<iostream>
#include<bits/stdc++.h>
using namespace std;

const int n = 10;

//Function to reset visisted array
void resetVisited (int visited[n], int i = 0) {
	if(i == n)
		return;
	visited[i] = 0;
	resetVisited(visited, i+1);
}

//Print adjacency matrix
void printMatrix(int G[n][n]) {
	int i, j;
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j)
			cout<<G[i][j] <<" ";
		cout<<"\n";
	}
}

//Copy Matrix G to C
void copyMatrix(int G[n][n], int C[n][n]) {
	int i, j;
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j)
			C[i][j] = G[i][j];
	}
}

//Find articulation points and bi-connected components
int articulation (int G[n][n], int visited[n], int num[n], int low[n], stack<int> &S, int u, int parent = -1, int i = 0) {
	visited[u] = 1;
	num[u] = i;
	low[u] = i;
	int v;
	for(v = 0; v < n; ++v) {
		if(G[v][u] > 0) {
			//If not visited adjacent vertex (Tree edge)
			if(visited[v] == 0) {
				S.push(u);
				S.push(v);
				i = articulation(G, visited, num, low, S, v, u, i+1);
				low[u] = (low[u] < low[v]) ? low[u] : low[v];
			}
			//If adjacent vertex is visited but not parent (Back edge)
			else if(visited[v] == 1 && v != parent && num[u] > num[v]) {
				low[u] = (low[u] < num[v]) ? low[u] : num[v];
				S.push(u);
				S.push(v);
			}
		}
	}
	if(low[u] >= num[parent] && parent != -1) {
		cout<<"\nArticulation Point: " <<parent+1;
		cout<<"\nBiconnected-Component:\n";
		while(low[S.top()] >= parent || S.top() == parent) {
			cout<<S.top()+1 <<"-";
			S.pop();
			cout<<S.top()+1 <<"\t";
			S.pop();
			if(S.empty())
				break;
		}
	}
	return i;
}

int main ()
{
	int G[n][n] = {{0,1,1,1,0,0,0,0,0,0},{1,0,1,0,0,0,0,0,0,0},
					{1,1,0,1,1,0,1,0,0,0},{1,0,1,0,0,0,0,0,0,0},
					{0,0,1,0,0,1,1,0,0,0},{0,0,0,0,1,0,1,1,1,0},
					{0,0,1,0,1,1,0,0,0,0},{0,0,0,0,0,1,0,0,1,0},
					{0,0,0,0,0,1,0,1,0,1},{0,0,0,0,0,0,0,0,1,0}};
	int i, visited[n], num[n], low[n], C[n][n];
	resetVisited(visited);
	copyMatrix(G, C);
	stack<int> S;
	articulation(C, visited, num, low, S, 0);
	cout<<"\n";
	for(i = 0; i < n; ++i)
		cout<<num[i] <<" ";
	cout<<"\n";
	for(i = 0; i < n; ++i)
		cout<<low[i] <<" ";
	return 0;
}

