
% Verifica daca exista muchie intre doua noduri X si Y
edge(X,Y,[_,E]) :- member([X,Y],E).

% Verifica daca exista muchie intre doua noduri X si Y si Y are culoarea Color
edgeColor(Color, X,Y, [V,E]):- member([X,Y],E), member([Y, Color], V).

contains(E, [E|_]) :- !.
contains(E, [H|T]) :- E \= H, contains(E, T).

% Returneaza o lista cu toate culorile nodurilor din graf
getColors([[], _], []).
getColors([[[_, K]|T], E], R):- getColors([T, E], Aux), R = [K | Aux], !.

% Daca formula este valid calculeza cel mai scurt drum fara a tine cont de alte constrangeri
getPath(From,To,Graph,Formula,Path):- Formula = valid, bfs(From,To,Graph,Path), !.

% Daca Formula unifica cu not atunci se verifica ca nodul de plecare sa nu aiba
% culoarea respectiva si se calculeaza drumul minim in caz de succes sau se
% unifica cu false in caz de fail.
getPath(From,To,Graph,Formula,Path):- Formula = not(Color), Graph = [V, _],
                                                (not(member([From, Color], V))->bfs(From,To,Graph,Path); Path = false), !.

% Daca Formula unifica cu global atunci se va apela predicatul bfsColor care
% returneaza cea mai scurta cale unde toate nodurile au culoarea Color
getPath(From,To,Graph,Formula,Path):- Formula = global(Color), bfsColor(From,To,Graph,Path,Color), !.

% Daca Formula unifica cu next atunci se verifica daca culoarea respectiva
% exista in graf iar daca exista se va verifica ca urmatorul nod sa fie de culoarea Color
getPath(From,To,Graph,Formula,Path):- Formula = next(Color), getColors(Graph, Aux),
                                                (member(Color, Aux),!->bfsNext(From,To,Graph,Path,Color); Path = false), !.

% Daca Formula unifica cu future atunci se va apela predicatul bfsFuture care
% verifica sa existe undeva in cale un nod de culoarea Color
getPath(From,To,Graph,Formula,Path):- Formula = future(Color), bfsFuture(From,To,Graph,Path,Color), !.

% Daca Formula unifica cu until atunci se va apela calcula cea mai scurta cale
% incepand cu nodul de culoarea Color1 pana se intalneste Color2
getPath(From,To,Graph,Formula,Path):- Formula = until(Color1, Color2), bfsFirstColor(From,To,Graph,Path, Color1, Color2), !.

% Daca Formula unifica cu and atunci se va apela getPath pentru ambele formule
% iar daca drumurile returnate unififca atunci calea respeta constrangerile
getPath(From,To,Graph,Formula,Path):- Formula = and(F1, F2), getPath(From,To,Graph,F1,Path1),!,
                                                getPath(From,To,Graph,F2,Path2), Path1 = Path2, !, Path = Path1, !.

% Daca Formula unifica cu or atunci se va apela getPath pentru ambele formule
% si se va returna unul din drumurile obtinute
getPath(From,To,Graph,Formula,Path):- Formula = or(F1, F2), getPath(From,To,Graph,F1,Path1),
                                                getPath(From,To,Graph,F2,Path2), Path = Path1, !.

% Daca Formula nu a unificat inca  pentru ambele formule
% si se va returna unul din drumurile obtinute care este valid
getPath(From,To,Graph,Formula,Path):- getColors(Graph, Aux), member(Formula, Aux),!, Graph = [V,E],
                                      member([From,Formula],V), !,bfs(From,To,Graph,Path), !.

getPath(From,To,Graph,Formula,Path):- bfs(From,To,Graph,Path), !.


% Predicate auxiliare pentru a realiza parcurgerea grafului
bfs(From,To,[V,E],Path) :- bfs_aux(From, [(From,0)], To, [V,E], [],Path).
bfsFirstColor(From,To,[V,E],Path, Color1, Color2) :- member([From, Color1], V), bfs_aux(From, [(From,0)], To, [V,E], [],Path).
bfsColor(From,To,[V,E],Path, Color) :- bfs_auxColor(From, [(From,0)], To, [V,E], [],Path, Color).
bfsNext(From,To,[V,E],Path, Color) :- bfs_auxNext(From, [(From,0)], To, [V,E], [],Path, Color).
bfsFuture(From,To,[V,E],Path, Color) :- bfs_auxFuture(From, [(From,0)], To, [V,E], [],Path, Color).

% Bfs Simplu
% Daca primul nod din coada este cel destinatie, reconstruieste drumul si se opreste
bfs_aux(From, [(Node,Parent) | Q],To,[V,E], Closed, Path) :- Node = To, reconstruct_path(From, (Node,Parent), Closed, PathNew), reverse(PathNew, Path2),
                                                             append(Path2, [Parent], Path3), append(Path3, [To], Path), !.
% Daca primul nod din coada a mai fost vizitat, atunci se trece la urmatorul
bfs_aux(From, [(Node,Parent) | Q],To,[V,E], Closed, Path) :- member((Node, _), Closed), !, bfs_aux(From, Q, To, [V, E], Closed, Path) .
% Pentru nodul ce urmeaza sa fie vizitat se gasesc vecinii, pentru fiecare vecin
% se adauga ca parinte nodul din care a fost vizitat, lista cu vecini se adauga la
% sfarsitul cozii si se reapeleaza functia pentru cu coada si nodul vizitat
% estea adugat in lista Closed.
bfs_aux(From, [(Node,Parent) | Q],To,[V,E], Closed, Path) :- findall((NextNode, Node), edge(Node, NextNode, [V,E]), Neighbours), append(Q, Neighbours, NewQ),
                                                             bfs_aux(From, NewQ, To, [V,E],[(Node, Parent) | Closed], Path).

% Path-ul este reconstruit parcurgandu-se lista de noduri din parinte in
% parinte pana se ajunge la nodul de inceput
reconstruct_path(From, (Node, Parent), Q, []) :- Parent = From .
reconstruct_path(From, (Node, Parent), Q, Sol) :- contains((Parent, NextParent), Q), reconstruct_path(From, (Parent, NextParent), Q, Aux),
                                                  append([NextParent], Aux, Sol) .

% Bfs Global
% Algoritmul e la fel ca la bfs simplu doar ca acum se extrag doar vecinii
% care au culoarea necesara.
bfs_auxColor(From,C, _, _, _, Path, Color):- C = [], Path = false.
bfs_auxColor(From, [(Node,Parent) | Q],To,[V,E], Closed, Path, Color) :- Node = To, reconstruct_path(From, (Node,Parent), Closed, PathNew),
                                                                         reverse(PathNew, Path2), append(Path2, [Parent], Path3), append(Path3, [To], Path), !.
bfs_auxColor(From, [(Node,Parent) | Q],To,[V,E], Closed, Path, Color) :- member((Node, _), Closed), !, bfs_auxColor(From, Q, To, [V, E], Closed, Path, Color) .
bfs_auxColor(From, [(Node,Parent) | Q],To,[V,E], Closed, Path, Color) :- findall((NextNode, Node), edgeColor(Color, Node, NextNode, [V,E]), Neighbours),
                                                                         append(Q, Neighbours, NewQ),
                                                                         bfs_auxColor(From, NewQ, To, [V,E],[(Node, Parent) | Closed], Path, Color).

% Bfs Next
% Se extrage lista cu vecinii care au culoarea necesara, iar daca lista nu e
% goala se continua cautarea cu bfs simplu, altfel se intoarce false
bfs_auxNext(From, [(Node,Parent) | Q],To,[V,E], Closed, Path, Color) :- findall((NextNode, Node), edgeColor(Color, Node, NextNode, [V,E]), Neighbours),
                                                                           (Neighbours \= [] -> append(Q, Neighbours, NewQ),
                                                                           bfs_aux(From, NewQ, To, [V,E],[(Node, Parent) | Closed], Path);Path = false),!.

% Bfs Future
bfs_auxFuture(From,C, _, _, _, Path, Color):- C = [], Path = false.
bfs_auxFuture(From, [(Node,Parent) | Q],To,[V,E], Closed, Path, Color) :- Node = To, reconstruct_path(From, (Node,Parent), Closed, PathNew), reverse(PathNew, Path2),
                                                                          append(Path2, [Parent], Path3), append(Path3, [To], Path), !.
bfs_auxFuture(From, [(Node,Parent) | Q],To,[V,E], Closed, Path, Color) :- member((Node, _), Closed), !, bfs_auxFuture(From, Q, To, [V, E], Closed, Path, Color) .
% Se extrage lista cu vecinii care au culoarea necesara, iar daca lista e
% goala se extrag toti vecinii indiferent de culoare si se continua cautareacu
% bfs future.
bfs_auxFuture(From, [(Node,Parent) | Q],To,[V,E], Closed, Path, Color) :- findall((NextNode, Node), edgeColor(Color, Node, NextNode, [V,E]), Neighbours), Neighbours = [],
                                                                          findall((NextNode, Node), edge(Node, NextNode, [V,E]), Neighbours2), append(Q, Neighbours2, NewQ),
                                                                          bfs_auxFuture(From, NewQ, To, [V,E],[(Node, Parent) | Closed], Path, Color).
% Se extrage lista cu vecinii care au culoarea necesara, iar daca lista nu e
% goala se continua cautarea cu bfs simplu, considerandu-se ca a fost gasit
% nodul de culoarea ceruta
bfs_auxFuture(From, [(Node,Parent) | Q],To,[V,E], Closed, Path, Color) :- findall((NextNode, Node), edgeColor(Color, Node, NextNode, [V,E]), Neighbours), Neighbours \= [],
                                                                          append(Q, Neighbours, NewQ), bfs_aux(From, NewQ, To, [V,E],[(Node, Parent) | Closed], Path).
