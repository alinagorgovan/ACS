#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
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
    /* Muchiile se retin intr-o coada de prioritati pentru a se pastra
    in ordine crescatoare */
	std::priority_queue<int, std::vector<int>, std::greater<int> > adj[kNmax];

	void read_input() {
		ifstream fin("minlexbfs.in");
		fin >> n >> m;
		for (int i = 1, x, y; i <= m; i++) {
			fin >> x >> y;
			adj[x].push(y);
			adj[y].push(x);
		}
		fin.close();
	}

    /*
    Functia implementeaza algoritmul BFS si returneaza parcurgerea
    minima lexicografic.
    */
	vector<int> get_result() {
		vector<int> visited(n + 1, 0);
        vector<int> result;
        queue<int> q;

        /* Seteaza ca sursa nodul 1 */
        int source = 1;
        visited[source] = 1;
        q.push(source);

        /* Cat timp mai sunt noduri de vizitat se vor extrage din coada */
        while (!q.empty()) {
            int s = q.front();
            result.push_back(s);
            q.pop();
            /* Parcurge nodurile vecine si daca sunt nevizitate le adauga in coada */
            while (!adj[s].empty()) {
                int x = adj[s].top();
                adj[s].pop();
                if (visited[x] == 0) {
                    visited[x] = 1;
                    q.push(x);
                }
            }
            visited[s] = 1;
        }
        return result;
	}

	void print_output(vector<int> result) {
		ofstream fout("minlexbfs.out");
		for (int i = 0; i < n; i++) {
			fout << result[i] << (i == n - 1 ? '\n' : ' ');
		}
		fout.close();
	}
};

int main() {
	Task *task = new Task();
	task->solve();
	delete task;
	return 0;
}
