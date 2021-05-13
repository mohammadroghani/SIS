#include<iostream>
#include<iomanip>
#include<vector>
#include<algorithm>
#include<cstring>
#include<random>
#include<future>

using namespace std;

static const int MAXN = 100 + 10;
static const int MAXM = 200000 + 10;
static const int INF = 1<<29;

struct Edge{
	int v, u;
	bool mark;
	Edge() { v = 0, u = 0, mark = false; }
	Edge(int _v, int _u, bool _mark) { v = _v, u = _u, mark = _mark; }
	int get_other(int x){
		if(x == v) return u;
		return v;
	}
	bool is_matching(){
		return v > u;
	}
};


struct Flow{
	int n, m;
	int to[2*MAXM], prv[2*MAXM], hed[MAXN], cap[2*MAXM];
	int dis[MAXN], pos[MAXN];

	void init(int _n) { n = _n, m = 0; memset (hed, -1, n * sizeof hed[0]); }

	inline void add_edge(int x, int y, int c) {
		to[m] = y, cap[m] = c, prv[m] = hed[x], hed[x] = m++; 
		to[m] = x, cap[m] = 0, prv[m] = hed[y], hed[y] = m++;
	}

	bool bfs(int source, int sink) {
		static int que[MAXN], head, tail;
		memset(dis, -1, n * sizeof dis[0]); head = tail = 0; dis[source] = 0; que[tail++] = source;
		while(head < tail) {
			int u = que[head++];
			for(int e = hed[u]; e != -1; e = prv[e])
				if(cap[e] > 0 && dis[to[e]] == -1)
					dis[to[e]] = dis[u]+1, que[tail++] = to[e];
			if(dis[sink] != -1) break;
		}
		return dis[sink] != -1;
	}

	int dfs(int u, int sink, int add = INF) {
		if(u == sink) return add;
		int res = 0;
		for(int &e = pos[u]; e != -1; e = prv[e])
			if(cap[e] > 0 && dis[to[e]] == dis[u]+1) {
				int cur = dfs(to[e], sink, min(add, cap[e]));
				cap[e] -= cur, cap[e^1] += cur, add -= cur, res += cur; if(add <= 0) break;
			} return res;
	}

	int getflow(int source, int sink) {
		int flow = 0;
		while(bfs(source, sink)) {
			memcpy(pos, hed, n * sizeof hed[0]); flow += dfs(source, sink);
		}
		return flow;
	}

};


struct SIS{
	Edge edge[MAXM], org[MAXM];
	bool matching[MAXM], visit[MAXN];
	vector<int> topol, adj[MAXN], C[MAXN];
	int comp[MAXN], cnum;
	double Q[MAXN][MAXN];


	void topological_sort(int v, bool incomp){
		visit[v] = true;
		for(int i = 0; i < adj[v].size(); i++) if(!edge[adj[v][i]].mark && (edge[adj[v][i]].v == v || edge[adj[v][i]].is_matching())){
			int u = edge[adj[v][i]].get_other(v);
			if(!incomp || comp[u] == comp[v])
				if(!visit[u]) topological_sort(u, incomp);
		}

		topol.push_back(v);
	}

	void SCC(int v, bool incomp, int tag){
		visit[v] = true;
		for(int i = 0; i < adj[v].size(); i++) if(!edge[adj[v][i]].mark && (edge[adj[v][i]].u == v || edge[adj[v][i]].is_matching())){
			int u = edge[adj[v][i]].get_other(v);
			if(!incomp || comp[u] == comp[v])
				if(!visit[u]) SCC(u, incomp, tag);
		}

		comp[v] = tag;
	}

	bool find_path(int v, int dest){
		if(v == dest) return true;
		visit[v] = true;
		for(int i = 0; i < adj[v].size(); i++) if(!edge[adj[v][i]].mark && (edge[adj[v][i]].v == v || edge[adj[v][i]].is_matching())) {
			int u = edge[adj[v][i]].get_other(v);
			if(comp[u] == comp[v] && !visit[u]){
				if(find_path(u, dest)){
					swap(edge[adj[v][i]].u, edge[adj[v][i]].v);
					return true;
				}
			}
		}
		return false;
	}

	double sample(int N, vector<pair<int, int> > SIS_edges, vector<vector<double> > Q, int iterations){	
		Flow* f = new Flow();
		f->init(2 * N + 2);

		for(int i = 1; i <= N; i++) f->add_edge(0, i, 1);
		for(int i = 1; i <= N; i++) f->add_edge(N + i, 2 * N + 1, 1);

		for(int i = 0; i < SIS_edges.size(); i++){
			int v = SIS_edges[i].first;
			int u = SIS_edges[i].second;
			f->add_edge(v, N + u, 1);
			edge[i] = Edge(v, N + u, false);
			org[i] = edge[i];
		}
		f->getflow(0, 2 * N + 1);
		for(int i = 4 * N; i < f->m; i += 2) matching[(i - 4 * N) / 2] = !f->cap[i];
		delete f;

		double B_sum = 0;
		double B_count = 0;

		for(int tt = 0; tt < iterations; tt++){	
			for(int i = 0; i < MAXN; i++) {
				adj[i].clear();
				C[i].clear();
			}
			memset(comp, 0, sizeof(comp));
			memset(visit, 0, sizeof(visit));
			topol.clear();
			cnum = 0;

			for(int i = 0; i < SIS_edges.size(); i++){
				edge[i] = org[i];
				if(matching[i]) swap(edge[i].u, edge[i].v);
				adj[edge[i].u].push_back(i);
				adj[edge[i].v].push_back(i);
			}

			for(int i = 1; i <= 2 * N; i++) if(!visit[i]) topological_sort(i, false);
			memset(visit, false, sizeof visit);
			for(int i = 2 * N - 1; i >= 0; i--) if(!visit[topol[i]]) SCC(topol[i], false, cnum++);
			for(int i = 1; i <= 2 * N; i++) C[comp[i]].push_back(i);
			topol.clear();
			memset(visit, 0, sizeof(visit));	
			vector<int> perm;
			for(int i = 1; i <= N; i++) perm.push_back(i);

			random_device rd1; 	
			auto rng = default_random_engine { rd1() };
			shuffle(perm.begin(), perm.end(), rng);

			double B = 1;

			random_device rd2;
			mt19937 gen(rd2());

			for(int i = 0; i < N; i++){
				vector<int> cur_edges;
				vector<double> prob;
				int u = perm[i];

				for(int j = 0; j < adj[u].size(); j++) if(!edge[adj[u][j]].mark){
					int v = edge[adj[u][j]].get_other(u);

					if(comp[v] == comp[u]){
						cur_edges.push_back(j);
						prob.push_back(Q[u - 1][v - N - 1]);
					}
				}
				if(cur_edges.size() == 0){
					B = 0;
					break;
				}

				discrete_distribution<> distrib(prob.begin(), prob.end());

				double fac = 0;
				for(int j = 0; j < cur_edges.size(); j++){
					int pos = cur_edges[j];
					int v = edge[adj[u][pos]].get_other(u);
					fac += Q[u - 1][v - N - 1];
				}
				int index = cur_edges[distrib(gen)];
				int v = edge[adj[u][index]].get_other(u);
				fac /= Q[u - 1][v - N - 1];
				B *= fac;

				
				if(edge[adj[u][index]].v == u){
					find_path(edge[adj[u][index]].u, u);
					for(int j = 0; j < C[comp[u]].size(); j++) visit[C[comp[u]][j]] = false; //new
				}
				edge[adj[u][index]].mark = true;
				topol.clear();

				for(int j = 0; j < C[comp[u]].size(); j++) if(!visit[C[comp[u]][j]]) topological_sort(C[comp[u]][j], true);
				for(int j = 0; j < C[comp[u]].size(); j++) visit[C[comp[u]][j]] = false; //new
				u = topol.back();

				for(int j = topol.size() - 1; j >= 0; j--) if(!visit[topol[j]]){
					if(j == topol.size() - 1) SCC(topol[j], true, comp[u]);
					else SCC(topol[j], true, cnum++);
				}

				for(int j = 0; j < C[comp[u]].size(); j++) visit[C[comp[u]][j]] = false; //new
				vector<int> temp;
				for(int j = 0; j < C[comp[u]].size(); j++) temp.push_back(C[comp[u]][j]);
				C[comp[u]].clear();
				for(int j = 0; j < temp.size(); j++) C[comp[temp[j]]].push_back(temp[j]);

			}
			B_sum += B;
			B_count++;
		}
		return B_sum / B_count;
	}
};


vector<vector<double> > sinkhorn(vector<vector<int> > A){
	int n = A.size();
	int m = A[0].size();
	vector<vector<double> > Q;
	for(int i = 0; i < A.size(); i++){
		vector<double> current_row;
		for(int j = 0; j < A[i].size(); j++) current_row.push_back((double)A[i][j]);
		Q.push_back(current_row);
	}
	for(int k = 0; k < 100; k++){
		for(int i = 0; i < n; i++){
			double sum = 0;
			for(int j = 0; j < m; j++) sum += Q[i][j];
			if(sum > 0) for(int j = 0; j < m; j++) Q[i][j] /= sum;
		}
		for(int j = 0; j < m; j++){
			double sum = 0;
			for(int i = 0; i < n; i++) sum += Q[i][j];
			if(sum > 0) for(int i = 0; i < n; i++) Q[i][j] /= sum;
		}
	}
	return Q;
}

void card_guessing(int n, int m, int iterations){
	int correct = 0;

	vector<int> deck;
	for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
			deck.push_back(i);

	random_device rd;
	auto rng = default_random_engine { rd() };
	shuffle(deck.begin(), deck.end(), rng);

	vector<vector<int>> A(n * m, vector<int> (n * m, 1));
	int index = 0;
	
	int options = 1;	

	for(int row = 0; row < n * m; row++){
		if(deck[row] != index){
			for(int column = index * m; column < (index + 1) * m; column++){
				A[row][column] = 0;
			}
			continue;
		}
		
		if(index + 1 == options && index + 1 < n) options++;

		correct += 1;
		if(row == n * m - 1) break;

		int last = 0;
		for(int i = index * m; i < (index + 1) * m; i++){
			if(A[row][i] == 1) last = i - index * m + 1;
			else break;
		}
		for(int i = 0; i < n * m; i++){
			A[row][i] = 0;
			A[i][index * m + last - 1] = 0;
		}
		A[row][index * m + last - 1] = 1;

		row++;
		vector<double> dist;
		for(int column = 0; column < options; column++){	
			int last = 0;
			for(int i = column * m; i < (column + 1) * m; i++){
				if(A[row][i] == 1) last = i - column * m + 1;
				else break;
			}
			if(last > 0){
				vector<vector<int> > AA = A;
				for(int i = 0; i < n * m; i++){
					AA[row][i] = 0;
					AA[i][column * m + last - 1] = 0;
				}
				AA[row][column * m + last - 1] = 1;
				vector<pair<int, int> > current_edges;
				for(int i = 0; i < n * m; i++)
					for(int j = 0; j < n * m; j++)
						if(AA[i][j] == 1)
							current_edges.push_back(make_pair(i + 1, j + 1));

				vector<vector<double> > Q = sinkhorn(AA);
				SIS* sis = new SIS();
				double matches = sis->sample(n * m, current_edges, Q, iterations);
				delete sis;
				dist.push_back(matches * (double) last);
			}
			else{
				dist.push_back(0);
			}
		}
		index = max_element(dist.begin(), dist.end()) - dist.begin();
		row--;

	}
	cout << correct << endl;
}

int main(){
	int n, m, samples, iterations;
	cin >> n >> m >> samples >> iterations;
	for(int i = 0; i < samples; i++)
		card_guessing(n, m, iterations);
	return 0;
}
