Gorgovan Alina-Valentina
334CB

Algoritmi Paraleli si Distribuiti - Tema 1

Cerinta 1 - Super Sampling Anti Aliasing

    Pentru rezolvarea acestei cerinte am creat structurile image si input_output.
In structura image am salvat pixeli sub forma unei matrice de tipul void** care
la alocare va contine elemente de tipul rgb_pixel sau gray_pixel.
Structura input_output contine un pointer catre imaginea de input si cea de
output, dar si id-ul threadului curent.
    In functie de resize_factor, threadurile vor porni executia pe o anumita
functie: resize_even sau resize_three. Metoda de paralelizare este aceeasi
pentru ambele functii, diferenta dintre acestea fiind doar algoritmul de resize
aplicat.
    Calculul l-am facut pornind de la dimensiunile imaginii finale. Pentru
fiecare element [i,j] din aceasta am calculat coordonatele submatricei corespondente
din matricea initiala. "Patratul" contine liniile de la i*resize_factor la
(i+1)*resize_factor si coloanele de la j*resize_factor la (j+1)*resize_factor
rezultand asa o submatrice sub forma unui patrat de latura resize_factor.
    In cazul resize_factor par se calculeaza suma elementelor din submatrice,
se imparte la resize_factor^2 si se pune rezultatul in matricea finala la
pozitia [i,j]. Daca poza e de tipul RGB, calculele se realizeaza separat pentru
fiecare culoare.
    In cazul resize_factor egal cu 3 atunci se foloseste Kernelul Gaussian
din enunt si se inmulteste fiecare element cu cel corespunzator din Kernel
facandu-se suma tutuor inmultirilor. Daca poza e de tipul RGB, calculele se
realizeaza separat pentru fiecare culoare.
    In ambele cazuri paralelizare am facut-o impartind fiecarui thread un
numar corespunzator de linii din matricea finala. Pentru fiecare linie din
matricea finala fiecare thread va calcula width/resize_factor submatrice.


Cerinta 2 - Micro Renderer

    La aceasta cerinta a fost necesar ca structura image sa contina campurile:
width, thread_id si matricea de pixeli de tipul unsigned char**.
    Functia threadFunction va primi ca argument o structura de tip image
si in functie de pozitia [x,y] a fiecarui pixel din imagine va determina
daca acesta apartine dreptei -1 * x + 2 * y + 0 = 0 de dimensiune 3 cm.
Matricea este patratica asa ca x si y apartin [0, img->width]. Pentru a putea
aplica verificarea asupra pozitia pixelului [x,y] trebuie scalata in centimetri.
Stiind ca imaginea finala are dimensiunile 100cm x 100cm.
    Dupa convertirea in centimetri se va aplica formula distantei de la un punct
la o dreapta : d = |a * x + b * y + c| / sqrt(a^2 + b^2). Conform cerintei,
comparam distanta obtinuta cu 3 si se stabileste astfel culoarea pixelului.
    Paralelizarea am realizat-o impartind liniile din matricea de pixeli la
numarul de threaduri din program. Astfel ca fiecare thread lucreaza pe o parte
separata de matrice.
