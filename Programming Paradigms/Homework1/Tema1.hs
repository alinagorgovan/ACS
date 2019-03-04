module Tema1 (
        solveSimple,
        solveCosts
        ) where

import Data.Array

infinit = 999999

solveSimple :: (Int, [(Int, Int, Int)]) -> Maybe ([Int], Int)

--Functia calculeaza minimul dintre doua perechi (Nod, Cost)
--si returneaza nodul cu costul cel mai mic.
minim (a, c1) (b, c2)
    | c1 > c2  = (b, c2)
    | otherwise  = (a, c1)

--Functia primeste ca parametrii un n, nodul destinatie, d este
--matricea de costuri si valoarea destinatie si va reconstrui drumul
--uitandu-se la nodul din tuplu pana ajunge la sfarsit adica la nodul n
getPath n d (x1, x2) r =
    if x1 == n then r ++ (x1:[])
    else (x1:[]) ++ (getPath n d (d ! (x1 , n-1)) r)

solveSimple (n, m) =
        let
	    bounds = ((1, 0), (n, n - 1))
        --d[v][i] reprezinta un tuplu format din costul minim calculat
        --de la nodul v la nodul n in cel mult i muchii
        --elementele matricii sunt tupluri de forma (Nod, Cost) unde Nod
        --reprezinta nodul adiacent nodului curent v pentru care s-a gasit
        --drumul de cost minim cu lungimea Cost
	    d = listArray bounds [cost v i | (v, i) <- range bounds]
	    cost v i
            --cazuri de baza
	    	| (i == 0) && (v /= n) = (v, infinit)
	    	| v == n = (v, 0)
	    	| otherwise =
            --d[v][i] = minim (len(v, x) + d[x][i-1]) unde x e nod adiacent pentru v
                (foldr minim (0, infinit) (map (\(n1, c) -> let c1 = c + (snd (d ! (n1, i-1))) in (n1, c1))
                (foldr (\(x1, x2, x3) r -> if x1 == v then (x2, x3):r else if x2 == v then (x1, x3):r else r) [] m)))
	     in
            --daca nu exista muchii
            if (length m) == 0 then Nothing else
            --daca graful nu e conex si nu se poate ajunge de la 1 la n
            if (snd (d ! (1,n-1))) >= infinit then Nothing else
            Just (1:(getPath n d (d ! (1, n-1)) []), (snd (d ! (1,n-1))))

solveCosts :: (Int, Int, [Int], [(Int, Int, Int)]) -> Maybe ([(Int, Int)], Int)
solveCosts (n, sum, lc, m) =
    let
    bounds = ((1, 0), (n, n - 1))
    d = listArray bounds [cost v i | (v, i) <- range bounds]
    aux = listArray (1,n) lc
    --pentru o lista de noduri, functia returneaza o lista cu taxele
    --fiecarui nod din lista respectiva accesand vectorul de taxe
    gettaxes [] = []
    gettaxes (x:xs) = (aux ! x) : (gettaxes xs)
    --calculeaza minimul intre doua perechi (Nod, Cost) tinand cont si de al
    --doilea criteriu, taxele de intrare
    minim2 (a, c1) (b, c2) =
        if c1 > c2  then (b, c2) else
        if c2 > c1  then (a, c1) else
        if (aux ! a < aux ! b) then (a, c1) else (b, c2)
    --matricea se completeaza asemeni functiei anterioare
    cost v i
        | (i == 0) && (v /= n) = (v, infinit)
        | v == n = (v, 0)
        | otherwise =
            (foldr  minim2 (0, infinit) (map (\(n1, c) -> let c1 = c + (snd (d ! (n1, i-1))) in (n1, c1))
            (foldr (\(x1, x2, x3) r -> if x1 == v then (x2, x3):r else if x2 == v then (x1, x3):r else r) [] m)))

     in
        --daca nu exista muchii
        if (length m) == 0 then Nothing else
        --daca graful nu e conex si nu se poate ajunge de la 1 la n
        if (snd (d ! (1,n-1))) >= infinit then Nothing else
        --daca suma de bani initiala e 0
        if (sum == 0) then Nothing else
         let
            --se reconstruieste ruta parcursa pentru obtinerea drumului minim
            road = (1: (getPath n d (d ! (1, n-1)) []))
            --se obtine lista cu taxele corespunzatoare costurilor
            taxes = 0:(tail (gettaxes road))
            --suma totala cheltuita
            roadSum = foldr (+) 0 taxes
            n2 = length taxes
            --finalcosts va contine suma de bani pe parcursul drumului, suma
            --ce scade dupa fiecare nod prin care se trece
            finalcosts = listArray (0, n2) [getcost i  | i <- range (0, n2)]
            getcost i =
                if  i == 0 then sum
                else ((finalcosts ! (i - 1)) - (taxes !! i))
         in
            --daca s-a cheltuit mai mult pe drum decat suma initiala
            if roadSum > sum then Nothing else
            --altfel compune lista de tupluri (Nod, Suma Ramasa)
            Just (zipWith (\x y -> (x,y)) road (elems finalcosts), (snd (d ! (1,n-1))))
