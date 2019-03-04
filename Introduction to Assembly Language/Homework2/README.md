TEMA2 IOCLA

TASK1:
In realizarea orimului task am luat parametrii de pe stiva si dupa am parcurs
byte cu byte ambele siruri facand xor intre byte-ul sirului cu byte-ul din cheie
Asta se repeta pana cand se intalneste null terminator in primul sir.
Pentru a suprascrie mesajul criptat imediat dupa ce se face xor, byte-ul decriptat se
muta in sir pe pozitia curenta.

TASK2:

Copiez stringul pe stiva pentru a avea acces deplin la sirul original.
Stiva o parcurg invers cu esi. Pentru a decripta rolling xor, primul caracter
ramane asa cum este, pentru urmatorul fac xor cu caracterul precedent criptat.
De aceea am vazut util sa folosesc stiva pentru a putea accesa direct caracterele
criptate. Repet procesul pana gasesc null. Eliberez stiva si rezultatul ramane
in stringul original.

TASK3:
Pentru inceput am transformat ambele siruri caracter cu caracter din ascii in hexa.
Spre exemplu caracterul 'a' a devenit 0xa prin scaderea numarului 87 pentru litere si 48 pentru
cifre. Am facut acest lucru atat pentru cheie cat si pentru string.
Pentru a crea un byte din doi alaturati am ales sa shiftez la stanga primul astfel incat
pe primele 4 pozitii sa se afle bitii din primul byte si dupa suma cu cel se-al doilea pe ultimele
4 pozitii sa se afle bitii acestuia. Astfel din doua caractere de forma '0xa' si '0x5' s-a format
un octet de forma '0xa5'. Dupa realizarea acestui proces atat pentru sir cat si pentru cheie
am realizat un xor si am pus octetul decriptat in sirul initial pe pozitia citirri primului byte.
Pentru a pune caractrele decriptate in ordine a trebuit sa mut toate caracterele de pe pozitia i pe
pozitia i/2. Acest lucru l-am realizat cu ajutorul a doi contori, unul reprezentand i/2 si celallt i.
Urmatorul pas a fost sa pun zerouri pe jumatatea de sir pe care existau caracter de care nu mai aveam
nevoie. Si am mai realizat o bucla care adauga 1 la toate elementele din cheie pentru a nu mai
exista caractere nenule si pentru a ma putea muta usor la urmatorul string in main.
TASK4:

Prima data trebuie sa fac conversia folosind tabelul de substitutie din enunt.
Pentru asta am dedus doua offset-uri care sa ma duca din cod ASCII in codul de la base32.
Pentru litere 65, iar pentru cifre 24. Testez pentru o valoare netura (intre litere si
cifre) 60 => sub 60 cifra peste litere CAPS. Substituria se realizeaza pe stiva.
Dupa ce am terminat de convertit, incep sa umplu octetii cu cate 5 biti din fiecare
valoare de pe stiva. (mi se garanteaza ca nu se vor depasii cei 5 biti pentru ca valorile sunt
pana la 32=2^(5+1) => base32). Realizez acest lucru printr-o serie de 8 shiftari. Este simplu
de explicat, le-am gandit pe foaie si au rezultat numarul de biti de shiftare. La sfarsit pun
null terminator.
