#include<iostream>
using namespace std;

//Find root of disjoint set
int find (int S[], int a) {
	if(S[a] == a)
		return a;
	return find(S, S[a]);
}

//Unify two disjoint sets
void unify (int S[], int a, int b) {
	a = find(S, a);
	b = find(S, b);
	if(a != b)
		S[b] = a;
} 

int main ()
{
	const int n = 9;
	int S[n], i;
	for(i = 0; i < n; ++i)
		S[i] = i;
	unify(S, 2, 4);
	unify(S, 3, 7);
	unify(S, 7, 8);
	unify(S, 7, 5);
	unify(S, 1, 2);
	unify(S, 0, 1);
	for(i = 0; i < n; ++i)
		cout<<find(S, i) <<" ";
    return 0;
}

