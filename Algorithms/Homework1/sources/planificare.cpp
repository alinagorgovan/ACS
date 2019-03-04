// Copyright 2018 Alina Gorgovan
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

class Planificare {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	int p, d, b;
    std::vector<int> durate;

	void read_input() {
		ifstream fin("planificare.in");
		fin >> p >> d >> b;
        int temp;
        for (int i = 0; i < p; i++) {
            fin >> temp;
            durate.push_back(temp);
        }
		fin.close();
	}

    long long getCost(int i, int k) {
        long long suma = 0;

        for (int j = i; j <= p - k - 1; j++) {
            suma += durate[j] + b;
        }

        suma = suma - b;
        long long cost = (d - suma) * (d - suma) * (d - suma);

        return cost;
    }
    // functii returneaza o pereche formata din costul total al probelor si
    // numarul optim e concursuri
	pair <long long, int> get_result() {
        pair <long long, int> result;

        long long dp[p + 1];
        long long concursuri[p + 1] = { 0 };

        int index = 0;

        // caz de baza
        dp[p] = 0;
        concursuri[p] = 1;

        // se parcurg probele de la sfarsit la inceput
        for (int i = p - 1; i >= 0; i--) {
            // se parcurge numarul de concursuri existente
            for (int k = index; k <= p - i -1 ; k++) {
                // se calculeaza costul pentru probele de la i la p
                long long cost = getCost(i, k);
                long long totalcost = dp[p - k] + cost;

                // daca se obtine cost 0 asta inseamna ca in concursul curent nu
                // mai incap probe si se forteaza trecerea la urmatorul
                if (cost < 0) {
                    index++;
                    continue;
                }
                // daca se incearca introducerea in primul concurs
                if (k == index) {
                    // daca este ultimul concurs atunci costul pentru acesta
                    // va fi 0 si numarul de concursuri ramane 1
                    if (k == 0) {
                        dp[i] = 0;
                        concursuri[i] = 1;
                        continue;
                    // altfel va fi pus in vector prima valoarea pentru ca nu
                    // are cu ce sacompare
                    } else {
                        dp[i] = totalcost;
                        concursuri[i] = 1 + concursuri[p - k];
                        continue;
                    }
                }

                // daca costul total calculta la iteratia curenta este mai mic
                // decat costul existent in vector atunci actualizeaza valoarea
                // in vectorul de costuri iar pentru numarul de concursuri
                // se incrementeaza valoarea de pe pozitia unde s-a gasit minim
                if (totalcost < dp[i]) {
                    dp[i] = totalcost;
                    concursuri[i] = 1 + concursuri[p - k];
                }
            }
        }

        result.first = dp[0];
        result.second = concursuri[0];

        return result;
	}

	void print_output(pair <long long, int> result) {
		ofstream fout("planificare.out");
        fout << result.first << " " << result.second;
		fout.close();
	}
};

int main() {
	Planificare planificare;
	planificare.solve();
	return 0;
}
