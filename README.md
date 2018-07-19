# Sarpe
Simple demo for a snake game with multiple players. Self playing. So, place your bets or move on.

## Entitati:

### I. Jucator:
*Structura ce tine detalii despre un jucator.*

#### Membrii:
 * id: identificator primit de la libraria de com: (0...nrJucatori]  -> int
 * nume: nume trimis de jucator, sau: "Jucator_" + std::to_string(id) -> std::string
 * scor: lungime sarpe -> int

### II. Sarpe
*Structura ce tine coordonatele unui sarpe.*

#### Membrii:
 * directie: enum SUS/JOS/STANGA/DREAPTA 
 * pozitie cap: int x, y
 * corp (lista de directii/enum: SUS/JOS/STANGA/DREAPTA) -> std::vector<directie> / std::list<directie>

exemplu:
>       _0______
>      0|A      |
>      1|aaa    |
>      2|  a    |
>      3|_______|
>
>          cap (0,0)
>          corp(jos,dreapta,dreapta,jos)
>             => lungime-sarpe = corp.size + 1 = 5;
 Codificare sarpe pe harta:
>   Cap  = ‘A’ + Jucator.id
>   Corp = ‘a’ + Jucator.id

#### Metode:

##### 1. Initializare sarpe
  1. Primeste ca si parametru pozitia capului si lungimea.
  1. Genereaza aleator o directie
  1. Initializeaza toate elementele corpului pe directia aleasa

exemplu:
>         		  Sarpe(x=1,y=5,lungime=4) =>
>                      Pozitie cap = (1,5)
>                      directie = aleasa aleator
>                      corp = (directie, directie, directie)

### III. Tabla
*Clasa ce definste o tabla de joc in functie de o lista de jucatori.*

#### Membrii:
 * latime harta:    int width
 * inaltime harta:  int height
 * harta:           char *
 * serpi:           lista de serpi - util ar fi std::map<id_jucator, Sarpe> pt a putea actualiza usor o directie anume

#### Metode:

##### 1. Generare harta - privat
Genereaza o harta in functie de numarul de jucatori

exemplu
>          width  = 3 * nrJucatori * lungime_sarpe
>          height = 3 * nrJucatori * lungime_sarpe
>          harta  = new char[width * height]
Accesare:
>          harta[x][y]  = harta[y * width + x];

##### 2. Generare serpi - privat
 Impartime ecranul in n cadrane egale, unde n = nrJucatori. 
 Pt fiecare jucator plaseaza cate un sarpe in centrul fiecarui cadran, dupa care ii deseneaza.
 
exemplu:
>     Pt trie serpi pozitiile de start ar putea fi:
>       _0123456_
>      0|       |
>      1| a |   |
>      2| A | Bb|
>      3|___|___|
>      4|   |   |
>      5|cC |   |
>      6|   |   |
>      7|___|___|

Implementare:
>            dimCadranY = m_height / sqrt(nrJucatori);
>            dimCadranX = m_width  / sqrt(nrJucatori);
>
>            offsetX = dimCadranX / 2;
>            offsetY = dimCadranY / 2;
>
>          Pt fiecare jucator
>            generam un sarpe la (offsetX, offsetY)
>            offsetX += dimCadranX;
>            Daca (offsetx >= width) resetam offsetX la (dimCadranX / 2) si crestem


##### 3. Genereaza mancare - privat
 Primeste ca parametru numarul maxim de bobite pe care sa le genereze.
 Genereaza aleator un x si y. daca la pozitia indicata nu se afla nimic => pozitia este indicata ca si ocupata cu mancare. daca nu, se ignora bobita. Se evita generarea unui numar fix de bobite pt a nu risca sa ramanem blocati.

##### 4. Initializare tabla - public / constructor
 Primeste ca si parametru o lista de jucatori. Mai departe, in functie de numarul de jucatori, se ocupa de:
   * generare harta
   * generare serpi
   * generare mancare

##### 5. Actualizeaza directie - public
 Primeste ca parametru un id de jucator si o directie.
 * Verifica daca directia este valida
     * daca da => actualizeaza directia sarpelui.

##### 6. Stergere serpi - privat
  * Pentru fiecare sarpe:
     * marcheaza pozitiile ocupate de el pe harta ca fiind goale.

##### 7. Deseneaza serpi - privat
  * Pentru fiecare sarpe din lista
    * marcheaza pozitiile ocupate de el pe harta ca fiind ocupate. cap = 'A' + Jucator.id, corp = 'a' + Jucator.id

##### 8. Avanseaza joc - public
 * Sterge serpi
 * Creaza o harta intermediara

 * Pt fiecare sarpe
    * Calculeaza urmatoarea pozitie in functie de directie si pozitie cap.
       * Ceva de genu: {poz[0] = cap; for(i=0; i<corp.size();i++) poz[i+1]=poz[i]+corp[i];}
    * Verifica daca coordonata/pozitia este in afara hartii
       * Daca DA => sarpe.mort => scoate-l din lista la iesirea din loop
    * Verifica daca coordonata/pozitia contine mancare
       * Daca DA => actualizare corp:
          * pozitie cap se modifica
          * adauga element la inceputul listei: corp.push_front(directie_inversata(sarpe.directie()))
        * Daca NU => actualizare corp:
          * stergem ultima pozitie: corp.pop_back()
    * Pt fiecare element din corpul sarpelui
           * Marcheaza in tabela intermediara pozitiile ocupte de corp, exceptand capul

 * Pt fiecare sarpe
   * Verifica daca pozitia capului coincide cu un alt sarpe
     * Daca DA, marcheaza-i pe amandoi ca fiind morti => scoate-i din lista
   * Verifica daca pozitia capului coincide cu o pozitie ocupata din harta intermediara
     * Daca DA, marcheaza-l ca fiind mort => scoate-l din lista

 * Deseneaza serpii ramasi
 * Genereaza mancare


### IV. Manager joc
*Realizeaza legatura dintre libraria de comunicatii si Tabla de joc. Poate fi o clasa sau metoda principala.*

#### Membrii:
 * status joc: enum ASTEPTARE/IN_CURS/OPRIT
 * jucatori: vector de obiecte de tip Jucator - std::vector<Jucator> 

#### Metode:
##### 1. ia nr jucatori
 * Verifica si returneaza numarul de clienti conectati

##### 2. start_joc
 * Creaza un obiect de tip tabla pt jucatorii conectati
 * actualizeaza status.joc = IN_CURS

##### 3. main
  * seteaza status.joc initial = ASTEPTARE
  * Asteapta pana se conecteaza un numar predefinit de N jucatori sau un interval maxim (de exemplu 30 de secunde)
  * Asteapta un interval predefinit (de exemplu 5 secunte), dupa ce numarul minim de clienti a fost atins pt a mai da ocazia si altor jucatori sa se conecteze
  * Start joc
  * cat timp status.joc = IN_CURS
    * asteapta un interval fix predefinit(de exemplu: 300ms delay in tre frameuri - ar putea fi customizat in functie de marimea hartii)
    * avanseaza joc apeland metoda  Tabla.'Avanseaza joc'

#### Interfete cu libraria de comunicatie
##### 1. prin care primim date:
###### 1. adauga jucator(id jucator, nume)
 * Atunci cand un client se conecteaza la server, este considerat un potential jucator

###### 2. schimba directie(id jucator, directie)
 * Apeleaza Tabla.'Actualizeaza directie'

##### 2. prin care trimitem date:
###### 1. frame nou (char *harta, int latime, int inaltime)
 * Apelata la finalul executiei Tabla.'Avanseaza joc'. Ar putea printa harta actualizata pe consola

###### 2. game over (id jucator)
 * Apelata de fiecare data cand un jucator moare.

###### 3. start_joc(latime, inaltime, lista Jucatori)
 * Apelata cand urmeaz sa inceapa un joc. Trimite clientilor informatii despre dimensiunea Hartii si detalii despre jucatori gen: nume/caracter asignat.

###### 4. status_update(lista Jucatori)
 * Apelata de fiecare data cand se modifica scorul pt un jucator, pt a informa clientii conectati in legatura cu statusul
 * Daca toti jucatorii au murit, actualizeaza status.joc = OPRIT

###### 5. eroare(id jucator, eroare)
 * Poate fi apelata daca un jucator incearca sa mute intr-o pozitie interzisa. de exemplu: anterior se misca spre dreapta, si a cerut sa schimbe directia spre stanga
 


## Utilizare interfete:
### Pt interfetele de intrare
 Callback in libraria de comunicatii. Se inregistreaza o functie care este apelata de librarie.
 
Ca idee:
In librarie exista ceva de genul:
>       typedef int (*t_functie)(int,int);
>       t_functie callback;
> 
>        librarie_com.inregistreaza_functie(t_functie functie)
>        {
>            callback = functie;
>        }
> 
>        librarie_com.undeva_intr-un_task_intern()
>        {
>             librarie_com.callback(5,7);
>        }

Local serverului:
>       int functie_localaproduct(int x, int y) {
>         // daca apare o modificare functia va fi apelata de com
>        }
> 
>        int main()
>        {
>           // inregistrare callback
>           librarie_com.inregistreaza_functie(functie_localaproduct)
>        }
* Note: clasele au si parametru this, deci functiile ar trebui sa fie statice daca partea de client server e in c, sau sa se paseza cumva si this. 

#### NOTE: Exemplu implementare alternativa/propusa in samples/comm
 
### Pt interfetele de iesire
 Implementata o clasa abstracta cu metodele dorite
 Clasa abstracta este mostenita de catre 'Manager joc'.
 Atunci cand incepe jocul, se creaza un obiect de tipul Tabla, folosint ca parametru si un pointer catre un obiect de tipul clasei abstracte / referinta la clasa curenta / 'Manager joc'.this
 Metodele pt interfetele de iesire vor fi apelate din clasa Tabla. Acestea la randul lor vor apela metode din libraria de com.

