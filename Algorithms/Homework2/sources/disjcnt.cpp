#include <bits/stdc++.h>
using namespace std;

const int kNmax = 100005;

class Task {
 public:
    void solve() {
        read_input();
        print_output(get_result());
    }

 private:
    int n;
    int m;
    vector<int> adj[kNmax];
    vector<int> idx;
    vector<int> low;
    vector<int> parent;
    set<pair<int, int> > double_edges;
    int time = 0;

    void read_input() {
        ifstream fin("disjcnt.in");
        fin >> n >> m;
        for (int i = 1, x, y; i <= m; i++) {
            fin >> x >> y;
            /*
            Daca o muchie se repeta atunci o introduc in set-ul double_edges.
            */
            for (unsigned int j = 0; j < adj[x]. size(); j++) {
                if (adj[x][j] == y) {
                    double_edges.insert(make_pair(x, y));
                }
            }
            adj[x].push_back(y);
            adj[y].push_back(x);
        }
        fin.close();
    }
    /*
    Functia extrage componenta biconexa de pe stiva stiind ca v e punct
    de articulatie.
    */
    void get_comp(int v, stack<int> &s, vector<int> &comp) {
        int u;
        /* Extrage nodurile de pe stiva cat timp se ajunge la nodul v */
        do {
            u = s.top();
            comp.push_back(u);
            s.pop();
        } while (u != v);
    }

    /*
    Functia verifica daca intre nodurile u si v exista muchie duplicata.
    */
    bool check_double_edges(int u, int v) {
        pair<int, int> p1 = make_pair(u, v);
        pair<int, int> p2 = make_pair(v, u);
        for (pair<int, int> i : double_edges) {
            if (p1 == i || p2 == i)
                return true;
        }
        return false;
    }
    /*
    Implementare pentru algoritmul lui Tarjan care intoarce componentele
    biconexe din graf.
    */
    void tarjan(int v, stack<int> &s, vector<vector<int> > &components) {
        /*
        Actualizeaza timpul de descoperire si nivelul pentru nodul curent v.
        */
        time++;
        idx[v] = time;
        low[v] = time;
        s.push(v);
        /*
        Pentru vecinii nodului v apeleaza recursiv  functia daca nu au mai fost
        vizitati.
        */
        for (unsigned int i = 0; i < adj[v].size(); i++) {
            int u = adj[v][i];
            /* Verifica daca nodul face parte dintr-o muchie critica */
            if (u != -1) {
                /* Verifica daca nodul a fost vizitat */
                if (idx[u] == -1) {
                    /* Seteaza parintele si apeleaza recursiv fnctia tarjan */
                    parent[u] = v;
                    tarjan(u, s, components);
                    /* Seteaza timpul de descoperire penttu v ca timpul minim
                    de descoperire dintre el si vecinii sai. */
                    low[v] = min(low[v], low[u]);
                    /* Daca v e punct de articulatie, extrage componenta
                    biconexa de pe stiva. */
                    if (low[u] >= idx[v]) {
                        vector<int> comp;
                        comp.push_back(v);
                        get_comp(u, s, comp);
                        components.push_back(comp);
                    }
                }
                else if (u != parent[v])
                    low[v] = min(low[v], idx[u]);
            }
        }
    }
    /*
    Functia calculeaza muchiile critice din graf folosind o parcurgere DFS.
    Algoritmul verifica daca o muchie se afla intr-un ciclu comparand
    timpul de descoperire al copilului nodului u cu nivelul nodului v. Daca
    o muchie nu se afla intr-un ciclu atunci ea este muchie critica.
    */
    void critical_edges(int v, int parent, vector<int> &idx, vector<int> &low, vector<pair<int, int> > &sol) {
        /*
        Actualizeaza timpul de descoperire si nivelul pentru nodul curent v.
        */
		idx[v] = time;
		low[v] = time;
        time = time + 1;
        /* Se verifica vecinii nodului curent v */
		for (unsigned int i = 0; i < adj[v].size(); i++) {
            int u = adj[v][i];
            /* Verifica daca nu a ajuns la parinte, adica nu este intr-un ciclu */
			if (u != parent) {
                /* daca copilul nu a fost descoperit atunci se incearca acest lucru */
				if (idx[u] == -1) {
					critical_edges(u, v, idx, low, sol);
                    if (low[u] < low[v])
					    low[v] = low[u];
					if (low[u] > idx[v]) {
						pair<int, int> e = make_pair(v, u);
						sol.push_back(e);
					}
				} else {
                    if (idx[u] < low[v])
					    low[v] = idx[u];
				}
			}
		}
	}
    /*
    Se calculeaza muchiile critice din graf urmand sa se marcheze in graf ca
    muchii critice pentru a putea fi ignorate la calculul componentelor biconexe.
    */
    void remove_critical_edges() {
        vector<pair<int, int> > sol;
        idx.resize(n + 1, -1);
        low.resize(n + 1, -1);

        /*
        Se calculeaza muchiile critice pornind de la nodul 1.
        */
        for (int i = 1; i <= n; i++) {
            if (idx[i] == -1) {
                critical_edges(i, i, idx, low, sol);
            }
        }
        /*
        Pentru toate muchiile critice obtinute se verifica daca sunt duplicate.
        Daca nu sunt, atunci se marcheaza in graf.
        */
        for (pair<int, int> e : sol) {
            if (!(check_double_edges(e.first, e.second))) {
                for (unsigned int i = 0; i < adj[e.first].size(); i++) {
                    if (adj[e.first][i] == e.second) {
                        adj[e.first][i] = -1;
                    }
                }
                for (unsigned int i = 0; i < adj[e.second].size(); i++) {
                    if (adj[e.second][i] == e.first) {
                        adj[e.second][i] = -1;
                    }
                }
            }
        }

    }
    /*
    Functia returneaza numarul de elemente biconexe din graf.
    Acest lucru se realizeaza prin: gasirea muchiilor critice, eliminarea lor
    in cazul in care nu sunt duplicate si gasirea componentelor biconexe cu
    ajutorul algoritmului lui Tarjan.
    */
    long long get_result() {
        /* Elimina muchiile critice din graf. */
        remove_critical_edges();

        vector<vector<int>> sol;
        stack<int> s;
        idx.clear();
        idx.resize(n + 1, -1);
        low.clear();
        low.resize(n + 1);
        parent.resize(n + 1, 0);
        set<int> nodes;
        time = 0;

        /* Aplica algoritmul lui Tarjan incepand din nodul 1. */
        for (int i = 1; i <= n; i++) {
            if (idx[i] == -1)
                tarjan(i, s, sol);
        }

        long long biconn_comp = 0;
        /* Pentru toate componentele biconnexe, parcurge nodurile si le
        introduce intr-un set pentru a nu avea duplicate. */
        for (unsigned int i = 0; i < sol.size(); i++) {
            for (unsigned int j = 0; j < sol[i].size(); j++) {
                nodes.insert(sol[i][j]);
            }
        }
        /*
        Calculeaza numarul de posibilitati de combinare a nodurilor ce
        formeaza componentele biconexe.
        Combinari de nn luate cate 2, unde nn e numarul de noduri din set
        */
        biconn_comp = 1LL* (1LL * nodes.size() * (nodes.size() - 1)) / 2;
        return biconn_comp;
    }

    void print_output(long long result) {
        ofstream fout("disjcnt.out");
        fout << result;
        fout.close();
    }
};

int main() {
    Task *task = new Task();
    task->solve();
    delete task;
    return 0;
}
