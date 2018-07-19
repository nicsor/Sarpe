/*
 * Manager_joc.h
 *
 * Author: Nicolae Natea
 */

#ifndef MANAGER_JOC_H_
#define MANAGER_JOC_H_

#include <string>
#include <vector>

#include "Jucator.h"
#include "Status_update_itf.h"
#include "Tabla.h"

class Manager_joc : public Status_update_itf {
    enum Stare_joc
    {
        STARE_JOC_ASTEPTARE,   ///< Joc in asteptare
        STARE_JOC_PORNIT,      ///< Joc in curs
        STARE_JOC_OPRIT        ///< Joc terminat
    };

    // Construction
public:
    Manager_joc(int port);
    virtual ~Manager_joc();

    // Metode
public:
    /** Verifica numarul de jucatori inscrisi */
    int get_nr_jucatori();

    /** Start joc */
    void start_joc(int dimSarpe);

    /** Avanseaza joc */
    void muta();

    /** Printeaza o tabla */
    void printeaza(const char * const tabla, size_t latime, size_t inaltime);

    /** Verifica daca jocul a inceput */
    bool e_pornit();

    //
    //  Incoming Interface
    //

public:
    /** Adauga jucator nou */
    void adauga_jucator(int id, std::string nume);

    /** Schimba Directie */
    void schimba_directie(int id, Tabla::Directie directie);

    //
    //  Outgoing Interface
    //
protected:
    void frame_nou(const char * const tabla, size_t latime, size_t inaltime);
    void game_over(const Jucator_ptr &jucator);
    void start_joc(size_t latime, size_t inaltime, const std::vector<Jucator_ptr> &jucatori);
    void status_update(const std::vector<Jucator_ptr> &jucatori);
    void eroare(const Jucator_ptr &jucator, int cod, const std::string &eroareStr);

private:
    std::vector<Jucator_ptr> m_jucatori;    ///< Lista de jucatori inregistrati
    Stare_joc m_stare;                      ///< Stare joc
    Tabla_ptr m_tabla;                      ///< Tabla de joc
};

#endif /* MANAGER_JOC_H_ */
