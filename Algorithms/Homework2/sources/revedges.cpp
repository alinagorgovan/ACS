#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
using namespace std;

const int kNmax = 501;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	int n;
    int m;
    int q;
	int a[kNmax][kNmax];
    std::vector<pair<int, int> > queries;

	void read_input() {
		ifstream fin("revedges.in");
		fin >> n >> m >> q;
        for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				a[i][j] = INT_MAX / 2 - 1;
			}
		}
        int x, y;
        for (int i = 1; i <= m; i++) {
            fin >> x >> y;
            a[x][y] = 0;
        }
        for (int i = 1; i <= q; i++) {
            fin >> x >> y;
            queries.push_back(make_pair(x, y));
        }
		fin.close();
	}


    /*
    Functia implementeaza algoritmul Floyd-Warshall pentru a calcula
    drumul minim intre oricare 2 noduri din graf. Am modificat graful astfel:
    Unde era muchie am considerat costul 0, si am adaugat cost pentru muchia
    inversa 1.
    */
	vector<int> get_result() {
        /* Modifica muchiile din graf */
        for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
                /* Daca exista muchie de la i la j si de la j la i nu, atunci
                adauga muchie cu cost 1 de la j la i */
                if (a[i][j] == 0 && a[j][i] == INT_MAX / 2 - 1) {
                    a[j][i] = 1;
                }
                if (i == j) {
					a[i][j] = 0;
				}
            }
        }
        /* Aplica algoritmul Floyd-Warshall */
		for (int k = 1; k <= n; k++) {
			for (int i = 1; i <= n; i++) {
				for (int j = 1; j <= n; j++) {
					if (a[i][j] > a[i][k] + a[k][j]) {
						a[i][j] = a[i][k] + a[k][j];
					}
				}
			}
		}
        std::vector<int> result;
        /* Extrage din matrice rezultatele pentru queriuri */
		for (int i = 0; i < q; i++) {
            int x = queries[i].first;
            int y = queries[i].second;
            result.push_back(a[x][y]);
		}
        return result;
	}

	void print_output(std::vector<int> result) {
		ofstream fout("revedges.out");
		for (unsigned int i = 0; i < result.size(); i++) {
            fout << result[i] << endl;
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
