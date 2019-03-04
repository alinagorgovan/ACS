// Copyright 2018 Alina Gorgovan
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

int lines = 0;

class Numaratoare {
 public:
	void solve() {
		read_input();
		get_result();
	}

 private:
    int s, n, i;


    void read_input() {
		ifstream fin("numaratoare.in");
		fin >> s >> n >> i;
        fin.close();
	}

    // functie de backtracking care calculeaza recursiv toate variantele de
    // sume cu n_div elemente
    int backtracking(int v[], int sum, int n_div) {
        if (n_div == 0) {
            // daca numarul de variante calculate a ajuns la indexul cerut
            // atunci executia se va opri cu codul 1, adica succes
            if (lines == i) return 1;
            // daca s-a gasit o suma se incrementeaza numarul de linii
            lines++;
            return 0;
        }

        if (sum < n_div)
            return 0;

        // numarul maxim care poate fi pus in suma
        int first = sum - n_div + 1;
        if (n_div != n && v[n - n_div - 1] < first)
            first = v[n - n_div - 1];

        for (int j = first; j > 0; j--) {
            if (j * n_div < sum)
                return 0;
            // se completeaza vectorul cu valoarea j gasita
            v[n - n_div] = j;
            if (backtracking(v, sum - j, n_div - 1))
                return 1;
        }
        return 0;
    }
	void get_result() {
        int v[n] = { 0 };
        bool check = false;
        if (backtracking(v, s, n))
            check = true;
        print_output(v, check);
	}
	void print_output(int v[], bool ok) {
		ofstream fout("numaratoare.out");
        if (ok) {
    		fout << s << "=";
            for (int i = 0; i < n - 1; i++) {
                fout << v[i] << "+";
            }
            fout << v[n - 1];
    		fout.close();
        } else {
            fout << "-";
        }
	}
};

int main() {
	Numaratoare numaratoare;
	numaratoare.solve();
	return 0;
}
