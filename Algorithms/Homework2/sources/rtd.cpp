#include <bits/stdc++.h>
using namespace std;

const int kNmax = 501;
/* Configuratia maxima : 6 * 64 + 5 * 8 + 4 = 428 */
const int diceMax = 429;

typedef tuple<int, int, int, int> tuple2;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	int n;
	int m;
    pair<int, int> s;
    pair<int, int> f;
    int k;
	int dice_config[6];
    int cost[kNmax][kNmax][diceMax];
    int visited[kNmax][kNmax][diceMax];
    int map[kNmax][kNmax];

	void read_input() {
		ifstream fin("rtd.in");
		fin >> n >> m;
        fin >> s.first >> s.second;
        fin >> f.first >> f.second;
        fin >> k;
		for (int i = 1; i < 7; i++) {
			fin >> dice_config[i];
		}
        int x, y;
        for (int i = 0; i < k; i++) {
            fin >> x >> y;
            map[x][y] = -1;
		}
		fin.close();
	}

    /*
    Verifica daca o valoare din matrice cu indicii primiti ca parametru este
    o valida, adica pozitia respectiva nu a mai fost vizitata si nu este blocata
    */
    bool is_valid(int row, int col, int config) {
        return ((row >= 1) && (row <= n) &&
                (col >= 1) && (col <= m) &&
                (map[row][col] != -1) && (visited[row][col][config] == 0));
    }
    /*
    Functia returneaza codificarea configuratiei curente a pozitiei zarului.
    Codificarea aleasa este urmatoarea :
        bottom * 64 + front * 8 + right
    */
    int get_config(int current_config[]) {
        return 64 * current_config[1] + 8 * current_config[2] + current_config[3];
    }
    /*
    Permuta configuratia curenta astfel incat sa obtinem configuratia
    corespunzatoare rotirii la dreapta a zarului din pozitia curenta.
    */
    int rotate_right(int current_config[]) {
        int *aux = (int*)calloc(4, sizeof(int));
        aux[1] = current_config[3];
        aux[2] = current_config[2];
        aux[3] = current_config[6];

        return get_config(aux);
    }
    /*
    Permuta configuratia curenta astfel incat sa obtinem configuratia
    corespunzatoare rotirii la stanga a zarului din pozitia curenta.
    */
    int rotate_left(int current_config[]) {
        int *aux = (int*)calloc(4, sizeof(int));
        aux[1] = current_config[4];
        aux[2] = current_config[2];
        aux[3] = current_config[1];

        return get_config(aux);
    }
    /*
    Permuta configuratia curenta astfel incat sa obtinem configuratia
    corespunzatoare rotirii spre partea de sus a gridului a zarului din
    pozitia curenta.
    */
    int rotate_back(int current_config[]) {
        int *aux = (int*)calloc(4, sizeof(int));
        aux[1] = current_config[5];
        aux[2] = current_config[1];
        aux[3] = current_config[3];

        return get_config(aux);
    }
    /*
    Permuta configuratia curenta astfel incat sa obtinem configuratia
    corespunzatoare rotirii spre partea de jos a gridului a zarului din
    pozitia curenta.
    */
    int rotate_front(int current_config[]) {
        int *aux = (int*)calloc(4, sizeof(int));
        aux[1] = current_config[2];
        aux[2] = current_config[6];
        aux[3] = current_config[3];

        return get_config(aux);
    }
    /*
    In functie de vecinul pe care vrem sa il vizitam, extragem toate pozitiile
    fetelor zarului din configuratia respectiva si obtinem noua configuratie
    in fucntie de directia de deplasare pe care vrem sa mergem:
    i = 0 => dreapta
    i = 1 => stanga
    i = 2 => inainte
    i = 3 => inapoi
    */
    int get_newconfig(int i, int config) {
        int *aux = (int*)calloc(7, sizeof(int));
        /* se decodifica configuratia aplicand operatiile inverse
        si se foloseste faptul ca suma a doua fete opuse este 7
        pentru a obtine pozitia tuturor fetelor zarului */
        aux[1]  = config / 64;
        aux[2] = (config - aux[1] * 64) / 8;
        aux[3] = config - aux[1] * 64 - aux[2] * 8;
        aux[4] = 7 - aux[3];
        aux[5] = 7 - aux[2];
        aux[6] = 7 - aux[1];

        switch (i) {
            case 0 : return rotate_right(aux);
            case 1 : return rotate_left(aux);
            case 2 : return rotate_front(aux);
            case 3 : return rotate_back(aux);
        }
        return 0;
    }
    /*
    Functia returneaza celule vecine pozitiei curente din grid in care zarul
    se poate deplasa la urmatoarea mutare. Vecinii se verifica in ordinea
    urmatoare:
        (x, y + 1) -> dreapta
        (x, y - 1) -> stanga
        (x - 1, y) -> inainte
        (x + 1, y) -> inapoi
    Pentru verificare daca un vecin este valid se foloseste functia is_valid
    iar pentru obtinerea noii configuratii, functia get_newconfig.
    */
    vector<tuple<int, int, int> > get_neighbors(int x, int y, int config) {
        int rows[4] = {0, 0, -1, 1};
        int cols[4] = {1, -1, 0, 0};
        vector<tuple<int, int, int> > neighbors;
        for (int i = 0; i < 4; i++) {
            int nextX = x + rows[i];
            int nextY = y + cols[i];
            int new_config = get_newconfig(i, config);
            if (is_valid(nextX, nextY, new_config)) {
                neighbors.push_back(make_tuple(nextX, nextY, new_config));
            }
        }
        return neighbors;
    }
    /*
    Functia implementeaza algoritmul lui Dijkstra pentru a gasi drumul cu costul
    cel mai mic de la (Sx, Sy) la (Fx, Fy).
    Rezultatele partiale sunt retinute in matricea tridimensionala cost[][][].
    cost[x][y][config] = Costul de a ajunge de la sursa in pozitia (x, y) cu
    configuratia zarului config.
    */
    int get_result() {
        int minimum_cost = INT_MAX;
        /* Coada de prioritati sorteaza nodurile ce urmeaza sa fie vizitate
        dupa costul acestora. */
        priority_queue<tuple2, vector <tuple2> , greater<tuple2> > pq;
        /* Initializarea cu "infinit" */
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                for (int k = 83; k < diceMax; k++)
                    cost[i][j][k] = INT_MAX;
            }
        }
        int currX, currY, currCost;
        currX = s.first;
        currY = s.second;
        int indexes[7] = {0, 1, 2, 3, 4, 5, 6};
        int currConfig = get_config(indexes);
        /* Se seteaza costurile initiale si se adauga sursa in coada. */
        cost[currX][currY][currConfig] = dice_config[1];
        pq.push(make_tuple(cost[currX][currY][currConfig], currX, currY, currConfig));

        while(!pq.empty()) {
            /* Extrage primul element din coada */
            tuple2 current = pq.top();
            pq.pop();

            currCost = get<0>(current);
            currX = get<1>(current);
            currY = get<2>(current);
            currConfig = get<3>(current);

            /* Daca pozitia respectiva a mai fost vizitata nu se mai proceseaza */
            if (visited[currX][currY][currConfig] == 1)
                continue;
            visited[currX][currY][currConfig] = 1;

            /* Gaseste vecinii valizi pentru urmatoarea mutare */
            vector<tuple<int, int, int> > neighbors = get_neighbors(currX, currY, currConfig);
            /* Pentru toti vecinii se incearca relaxarea muchiei si se adauga
            in coada. */
            for (unsigned int i = 0; i < neighbors.size(); i++) {
                tuple<int, int, int> p = neighbors[i];

                int nextX = get<0>(p);
                int nextY = get<1>(p);
                int nextConfig = get<2>(p);
                int nextCost = dice_config[nextConfig / 64];

                if (cost[nextX][nextY][nextConfig] > currCost + nextCost) {
                    cost[nextX][nextY][nextConfig] = currCost + nextCost;
                    pq.push(make_tuple(cost[nextX][nextY][nextConfig], nextX, nextY, nextConfig));
                }
            }
        }
        /* Se alege din matrice costul minim cu care s-a ajuns
        la destinatie */
        for (int i = 83; i < diceMax; i++) {
            if (cost[f.first][f.second][i] < minimum_cost)
                minimum_cost = cost[f.first][f.second][i];
        }
        return minimum_cost;
    }
	void print_output(int result) {
		ofstream fout("rtd.out");
		fout << result << '\n';
		fout.close();
	}
};

int main() {
	Task *task = new Task();
	task->solve();
	delete task;
	return 0;
}
