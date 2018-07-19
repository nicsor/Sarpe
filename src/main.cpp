/**
 *
 * Author: Nicolae Natea
 */

#include <Manager_joc.h>
#include <iostream>
#include <unistd.h>

using namespace std;


int main() {
    Manager_joc joc(32);

   int nrjucatori;

   cout << "Introdu numarul de jucatori:";
   cin >> nrjucatori;

    for (int i =0; i< nrjucatori; ++i)
    {
       std::string jucator = "Jucator_" + std::to_string(i);
        joc.adauga_jucator(i, jucator);
    }

    joc.start_joc(5);

    while (joc.e_pornit())
    {
        usleep(100000);

        // Schimba aleator directia
        int jucator = rand() % nrjucatori;
        Tabla::Directie directie = Tabla::directie_aleatorie();
        joc.schimba_directie(jucator, directie);

        // Clear screen
        cout << "\033[2J\033[1;1H";

        joc.muta();
    }

    return 0;
}
