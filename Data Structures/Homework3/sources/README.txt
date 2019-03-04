Gorgovan Alina-Valentina
314CB

fisiere: main.c struct2.h

Obs: Pe checker-ul local primeam 64 de puncte iar pe vmchecker.

Pentru implementarea unui trie am creat o structura cu 5 campuri. Prima functie implementata aloca o astfel de structura si initializeaza campurile.

In functia _add_ exista 3 cazuri principale abordate: daca nodul nu are alti copii, daca are mai multi copii sau daca unul din copii are aceeasi valoare cu litera care trebuie inserata. In primul caz se aloca structura pentru copil si se stabileste legatura parinte-copil. In al doilea caz  se trece pe pozitia respectiva iar in ultimul caz se realoca spatiu pentru vectorul de copii si se face sortarea inserata.

Functia _find_ parcurge in trie litera cu litera cuvantul primit ca parametru. Daca numarul de parcurgeri nu va fi egal cu lungimea cuvantului insea,ma ca acesta nu exista in trie si se returneaza 0. Altfel se returneaza valoarea campului "ok" care arata daca nodul este sfarsitul unui cuvant.

Pentru functia _remove_ am implementata mai inati o functie auxiliara Free_Node care elibereaza un nod din trie avand grija sa nu modifice ordinea elementelor si sa se actualizeze numarul de copii. In functia principala removee, mai intai verific daca cuvantul exista in trie, daca nu exista ies din functie. Altfel ma mut pe ultimul nod corespunzator cuvantului. Daca nodul are copii atunci doar marcam 0 variabila 'ok', dar daca nu are copii atunci eliberam nod cu nod pana cand ajungem intr-un nod care mai are copii sau in radacina.

Pentru functia _find_longest_prefix_ am implementata o functie auxiliara find_aux care returneaza 1 daca exista secventa de caractere in trie si prin efect lateral returneaza numarul de copii al ultimului nod. In functia principala apelez find_aux pana cand se indeplineste conditia de avea 0 sau mai multi copii si de a exista in trie.

O alta functie auxiliara este printTrie care returneaza un string cu toate cuvintele ce pornesc dintr-un anumit nod din trie.
Pentru functia _find_all_wit_prefix_   m-am deplasat pana in nodul terminal din prefix de unde am apelat functia printTrie daca prefixulexista in trie sau returnam "None" daca nu exista.

Functia mean_length ptimeste ca parametru un prefix si un vector de cuvinte si cu ajutorul functiilor strtok si strcat returneaza media lungimii cuvintelor.

In functia main am implementat o bucla in care citesc numele operatiei si in functie de acesta citesc urmatorii parametri precum w-cuvant de adaugat/scos din trie sau pref-prefixul utilizat.
Sirul str este alocat pentru a salva in el cuvintele returnate de find_all_with_prefix .
Dupa fiecare apel al functiei find_all_with_prefix  sau mean_length golesc sirurile pref si str.