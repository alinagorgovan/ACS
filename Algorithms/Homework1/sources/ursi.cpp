// Copyright 2018 Alina Gorgovan
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;
const int MOD  = 1e9 + 7;

class Ursi {
 public:
	void solve() {
		read_input();
		print_output();
	}

 private:
	string sir;


	void read_input() {
		ifstream fin("ursi.in");
		getline(fin, sir);
		fin.close();
	}

	int get_result() {
        int nr = 0;
        for (unsigned int i = 0; i < sir.size(); i++) {
            if (sir[i] == '^') nr++;
        }
		vector <vector <int> > dp(sir.size(), vector<int> (nr + 1));

        // conditii pentru verificare validitatii string-ului
		if(nr % 2 == 1 ||sir[0] == '_' || sir[sir.size() - 1] == '_') return 0;

		// dp[i][j] -> nr de moduri in care se pot asigna caracterele citite
        // pana la i cu j '^' deschise adica j fete in completare

        // caz de baza
        dp[0][1] = 1;
        for (unsigned int i = 1; i < sir.size(); i++) {
            // incepe de la a doua pozitie din sir
			if (sir[i] == '_') {
				for (int j = 0; j <= nr; j++) {
                    // un caracter de tip '_' se poate combina cu toate
                    // caracterele de tip '^' in j moduri
					dp[i][j] = 1LL * j * dp[i - 1][j] % MOD;
				}
			} else {
				for (int j = 0; j <= nr; j++) {
					if (j == 0) {
						dp[i][j] = dp[i - 1][j + 1];
                    } else if (j == nr) {
                        dp[i][j] = 1LL * dp[i - 1][j - 1] % MOD;
					} else {
                        // un nou caracter de tip '^' se poate combina cu toate
                        // celelalte fete reaizate inaintea lui in (j+1) moduri
                        // astfel cu j caractere '^' vor fi cate posibilitati
                        // existau si la j-1 + cele j+1 moduri in care se pot
                        // crea fete cu un noul caractr citit
						dp[i][j] = (dp[i - 1][j - 1] + 1LL * (j + 1) *
                        dp[i - 1][j + 1] % MOD) % MOD;
					}
				}
			}
        }
        // s-a trecut prin tot sirul si au ramas 0 caractere neasignate unei
        // fete zambitoare :)
        return dp[sir.size()-1][0];
	}
	void print_output() {
		ofstream fout("ursi.out");
		fout << get_result();
		fout.close();
	}
};

int main() {
	Ursi ursi;
	ursi.solve();
	return 0;
}
