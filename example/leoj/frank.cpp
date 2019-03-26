#include <bits/stdc++.h>
using namespace std;
/*
const int MAX_N = 1e5+7;
const int MAX_TOT = 1e5+7;

struct ACho{
	struct state {
		int next[26];
		int fail, cnt;
	}stateTable[MAX_TOT];

	int size;

	std::queue<int> que;

	void init(){
		while(que.size()) que.pop();

		for(int i = 0; i < MAX_TOT; i++) {
			memset(stateTable[i].next, 0, sizeof(stateTable[i].next));
			stateTable[i].fail = stateTable[i].cnt = 0;
		}
		size = 1;
	}

	void insert(char *S) {
		int n = strlen(S);
		int now = 0;
		for (int i = 0; i < n; ++i) {
			char c = S[i];
			if(~stateTable[now].next[c - 'a']){
				stateTable[now].next[c - 'a'] = size++;
			}
			now = stateTable[now].next[c - 'a'];
		}
		stateTable[now].cnt++;
	}

	void build() {
		stateTable[0].fail = -1;
		que.push(0);
		while(que.size()){
			int u = que.front();
			que.pop();

			for (int i = 0; i < 26; ++i){
				if(stateTable[u].next[i]) {
					if(u == 0) stateTable[stateTable[u].next[i]].fail = 0;
					else {
						int v = stateTable[u].fail;
						while(v != -1) {
							if(stateTable[v].next[i]) {
								stateTable[stateTable[u].next[i]].fail = stateTable[v].next[i];
							}
							v = stateTable[v].fail;
						}
						if(v == -1) stateTable[stateTable[u].next[i]].fail = 0;
					}
					que.push(stateTable[u].next[i]);
				}
			}
		}
	}

	int Get(int u) {
		int res = 0;
		while(u) {
			res = res + stateTable[u].cnt;
			stateTable[u].cnt = 0;
			u = stateTable[u].fail;
		}
	}

	int match(char *S){
		int n = strlen(S);
		int res = 0, now = 0;
		for (int i = 0; i < n; ++i){
			char c = S[i];
			if(stateTable[now].next[c - 'a'])
				now = stateTable[now].next[c - 'a'];
			else {
				int p = stateTable[now].fail;
				while(p != -1 && stateTable[p].next[c - 'a'] == 0) p  = stateTable[p].fail;

				if(p == -1) now = 0;
				else now = stateTable[p].next[c - 'a'];
			}
			if (stateTable[now].cnt)
				res += Get(now);
		}
		return res;
	}


}acho;
*/
/*
int N;
char S[MAX_N];*/
int main(int argc, char const *argv[])
{
/*
	int t;
	scanf("%d",&t);
	while(t--) {
		acho.init();
		scanf("%d", &N);
		for (int i = 0; i < N; ++i)
		{
			scanf("%s",S);
			acho.insert(S);
		}
		acho.build();
		scanf("%s",S);
		printf("%d\n", acho.match(S));
	}
*/
int a;
scanf("%d",&a);
printf("%d",a+1);
	return 0;
}