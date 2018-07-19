/*
 * Tabla.h
 *
 * Author: Nicolae Natea
 */

#ifndef TABLA_H_
#define TABLA_H_

#include <list>
#include <string>
#include <map>
#include <memory>
#include <vector>

#include "Jucator.h"
#include "Status_update_itf.h"

class Tabla {
    // Type declarations
public:
    enum Directie
    {
        DIRECTIE_SUS,
        DIRECTIE_JOS,
        DIRECTIE_DREAPTA,
        DIRECTIE_STANGA,
        DIRECTIE_MAX,
    };

private:
    struct Sarpe {
        Jucator_ptr           jucator;   ///< detalii jucator
        int                   pozCapX;   ///< pozitie cap x
        int                   pozCapY;   ///< pozitie cap y
        Directie              directie;  ///< directie
        bool                  kaput;     ///< mort
        std::vector<Directie> corp;      ///< Corp sarpe ... probabil list ar fi mai ok, dar crapa.

      Sarpe(Jucator_ptr jucatorPtr, int x, int y, int dimensiune);
      ~Sarpe();
    };

    typedef std::shared_ptr<Sarpe> Sarpe_ptr;

    // Constante
public:
    const char SPATIU_MANCARE = '*';
    const char SPATIU_GOL     = ' ';
    const char SPATIU_OCUPAT  = 'x';

    // Construction
public:
    Tabla(const std::vector<Jucator_ptr> &jucatori, size_t dimSarpe, Status_update_itf *itf);
    virtual ~Tabla();

    // Metode
public:
    /** Avanseaza jocul cu o mutare */
    void avanseaza();

    /** Actualizeaza directia unui utilizator, cunoscand id-ul */
    bool actualizeaza_directie(int id, Directie dir);

   /** Calculeaza inversul unei directii */
   static Directie inversul_directiei(Directie dir);

   /** Genereaza o directie aleatorie */
   static Directie directie_aleatorie();

protected:
    /** Genereaza serpi */
    void genereaza_serpi(const std::vector<Jucator_ptr> &jucatori, int dimSarpe);

    /** Genereaza mancare */
    void genereaza_mancare(int maxBobite);

    /** Deseneaza serpii pe tabla */
    void deseneaza_serpi();

    /** Sterge serpii de pe tabla */
    void sterge_serpi();

    /** Afla coordona urmatoare cunoscand o pozitie si o directie */
    bool coordonata_urmatoare(int &x, int &y, Directie dir);

private:
    int m_width;                      ///< Latime tabla
    int m_height;                     ///< Inaltime tabla
    std::string m_tabla;              ///< Tabla de joc.
    std::map<int, Sarpe_ptr> m_serpi; ///< Participanti vii
    Status_update_itf *m_itf;         ///< Status update itf
};

typedef std::shared_ptr<Tabla> Tabla_ptr;

#endif /* TABLA_H_ */
