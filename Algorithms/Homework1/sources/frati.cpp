// Copyright 2 Alina Gorgovan
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

struct Game {
	int p1;
	int p2;
    bool visited;
    int sum;

    Game(int _p1, int _p2, bool _visited, int _sum) :
	p1(_p1), p2(_p2), visited(_visited),  sum(_sum) {}
};

class Frati {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	vector<Game> games;
    int n;

	void read_input() {
		ifstream fin("frati.in");
		fin >> n;
        int p1, p2;
        for (int i = 0; i < n; i++) {
            fin >> p1;
            fin >> p2;
            games.push_back(Game(p1, p2, false, p1 + p2));
        }
		fin.close();
	}

    static bool comparator(Game g1, Game g2) {
        if (g1.sum == g2.sum) {
            return g1.p1 > g2.p1;

        } else {
            return g1.sum > g2.sum;
        }
    }
	pair <int, int> get_result() {
        sort(games.begin(), games.end(), comparator);

        pair<int, int> js;
        js.first = 0;
        js.second = 0;
        int jonindex = 0;
        int samindex = 0;
        int samchoices = 0, jonchoices = 0;
        int oldsamindex;

        while (1) {
			// verifica daca s-au ales toata concursurile
            if (samchoices + jonchoices == n) {
                break;
            }

			// daca ajunge la un concurs deja luat sa treaca peste
            while (jonindex < n && games[jonindex].visited == true) {
                jonindex++;
            }
			// daca il poate lua sa il adune la suma
            if (games[jonindex].visited == false) {
                js.first += games[jonindex].p1;
                games[jonindex].visited = true;
                jonchoices++;
            }
            if (samchoices + jonchoices == n) {
                break;
            }

			// sam alege mereu dupa jon
            samindex = jonindex + 1;
			// daca ajunge la un concurs deja luat sa mearga mai departe
            while (samindex < n && games[samindex].visited == true) {
                samindex++;
            }

			// daca urmatorul concurs are suma egala cu cel curent avanseaza
			// cat timp ajunge la ultimul din secventa respectiva
            if (samindex + 1 < n) {
                while (games[samindex].sum == games[samindex + 1].sum &&
					games[samindex + 1].visited == false)
                    samindex++;
            }
			// daca poate lua concursul il va aduna la suma sa
            if (games[samindex].visited == false) {
                js.second += games[samindex].p2;
                samchoices++;
                games[samindex].visited = true;
            }
            if (samchoices + jonchoices == n) {
                break;
            }
            oldsamindex = samindex;
            int check = 0;

			// cat timp distanta dintre ei este cel putin 2 atunci se pot alege
			// consecutiv concursurile pentru cei doi frati pornind din ambele
			// capete ale distantei
            while (samindex - jonindex >= 2) {
                check = 1;
                jonindex++;
                if (games[jonindex].visited == true)
                    while (games[jonindex].visited == true) jonindex++;

                js.first += games[jonindex].p1;
                games[jonindex].visited = true;
                jonchoices++;
                samindex--;

                if (games[samindex].visited == false) {
                    js.second += games[samindex].p2;
                    games[samindex].visited = true;
                    samchoices++;
                } else {
					samindex = oldsamindex;
					samindex++;
                    while (games[samindex].visited == true)
                        samindex++;
                    while (games[samindex].sum == games[samindex + 1].sum &&
						games[samindex + 1].visited == false)
                        samindex++;

                    js.second += games[samindex].p2;
                    games[samindex].visited = true;
                    samchoices++;
				}
                if (samchoices + jonchoices == n) {
                    break;
                }
            }
            if (check == 1) {
                jonindex = oldsamindex;
                samindex = oldsamindex;
            }
        }
		return js;
	}

	void print_output(pair <int, int> result) {
		ofstream fout("frati.out");
        fout << result.first << " " << result.second;
		fout.close();
	}
};

int main() {
	Frati frati;
	frati.solve();
	return 0;
}
