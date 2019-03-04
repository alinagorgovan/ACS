Gorgovan Alina-Valentina
334CB

            Algoritmi Paraleli si Distribuiti - Tema 2


Primul pas in rezolvarea temei a fost completarea functiilor din clasa
CommunicationChannel. Am separat modul de comunicatie in 2 "canale" separate,
unul unde minerii scriu si vrajitorii citesc iar cel de-al doile unde vrajitorii
scriu si minerii citesc.

Astfel problema s-a spart in doua subprobleme similare cu problema Multiple
Producer Multiple Consumer facuta la laborator.

Primul canal de comunicatie, miners, este de tipul LinkedBlockingQueue si l-am
folosit pentru a pune mesajele de la mineri. Atunci cand un miner termina
executia unei camere, se va apela metoda put() care blocheaza accesul pe timpul
adaugarii, iar pentru extragere metoda take() care deasemenea blocheaza accesul
pe timpul extragerii.

Al doilea canal de comunicatie, wizards, este de tipul ConcurrentHashMap, unde
cheia este de tipul Integer si reprezinta ID-ul unui thread Wizard iar valoarea
este de tipul ArrayList<Message> si reprezinta lista de mesaje unde vrajitorul
cu ID-ul respectiv va adauga mesajele sale, separand astfel mesajele vrajitorilor.
Datorita acestei implementari, mesajele de tipul "END" au devenit redundante si
am considerat ca este mai eficient sa nu le adaug in lista de mesaje.
La adaugarea mesajului unui vrajitor, se ia ID-ul threadului curent si se cauta
in cheile existente in hashmap Id-ul respectiv. Daca nu a fost gasit, adica
vrajitorul adauga pentru prima data mesaje, se creaza o noua intrare in map
cu cheia noua si valoarea fiind o lista care contine un singur element, mesajul
ce trebuie adaugat. Daca ID-ul este deja cheie in map, atunci inseamna ca
lista este deja creata si se va adauga direct mesajul daca acesta nu e de tipul
END. Daca e mesaj EXIT atunci il va pune de 2 ori deoarece minerii pot
lua din array doar daca sunt cel putin 2 mesaje.
La extragerea unui mesaj din map, minerul va parcurge toate listele cu mesaje
si va extrage din prima lista unde gaseste cel putin 2 mesaje.
Astfel, va sti mereu ca primul mesaj extras este camera parinte si al doilea
este camera adiacenta. Setez camera parinte in camera ce urmeaza sa fie
explorata si o returnez. Daca in urma parcurgerii tuturor listelor din map
nu s-au gasit mesaje atunci returnez null.

In clasa Miner, am adaugat cele 3 atribute necesare, solved, hashCount si
canalul channel si am modificat constructorul corespunzator.
Metoda run() contine o bucla infinita care se opreste doar cand s-a primit un
mesaj de tipul EXIT. Primul pas pe care il face un miner e sa apeleze metoda
getMessageWizardsChannel si sa extraga un mesaj de pe canalul vrajitorilor.
Daca a primit null, adica nu s-a gasit nimic pe canal, sau daca camera primita
a fost deja rezolvata, atunci reia procesul. Daca se trece de verificarile
precedente atunci se cripteaza mesajul folosind metodele din scheletul de cod,
se actualizeaza mesajul din obiectul deja existent, se adauga ca rezolvat in
setul solved si se pune pe canalul minerilor.
