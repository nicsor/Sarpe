/*
 * Implementarea clasei Manager_joc.
 *
 * Author: Nicolae Natea
 */

#include <iostream>
#include "Manager_joc.h"

Manager_joc::Manager_joc(int port) :
    m_stare(STARE_JOC_ASTEPTARE)
{
}

Manager_joc::~Manager_joc() {
}

/**
 * @return numarul de jucatori inscrisi
 */
int Manager_joc::get_nr_jucatori()
{
    return m_jucatori.size();
}

/**
 * @param[in] dimSarpe dimensiune initiala serpi
 */
void Manager_joc::start_joc(int dimSarpe)
{
    std::cout << "Start joc nou" << std::endl;

    int nrJucatori = get_nr_jucatori();

    if ((m_stare != STARE_JOC_PORNIT) and (nrJucatori > 0))
    {
        m_stare = STARE_JOC_PORNIT;
        m_tabla = std::make_shared<Tabla>(m_jucatori, dimSarpe, this);
    }
    else
    {
        // prea putini jucatori
    }
}

/**
 * @return adevarat daca jocul este porit, altfel fals
 */
bool Manager_joc::e_pornit()
{
   return (m_stare == STARE_JOC_PORNIT);
}

/**
 * @param[in] tabla     tabla de joc
 * @param[in] latime    latime tabla
 * @param[in] inaltime  inaltime tabla
 */
void Manager_joc::printeaza(const char * const tabla, size_t latime, size_t inaltime)
{
    int offset = 0;
    std::cout << std::endl;

    std::cout << "\t ";

    // Printeaza delimitator sus
    for (size_t j = 0; j < latime; ++j)
    {
        std::cout << "_";
    }

    for (size_t i = 0; i < inaltime; ++i)
    {
        // Printeaza delimitator stanga si dreapta
        std::cout << "|" << std::endl << (i+1) << "\t|";

        // Printeaza continut matrice
        for (size_t j = 0; j < latime; ++j)
        {
            std::cout << tabla[offset];
            ++offset;
        }
    }

    std::cout << "|" << std::endl << "\t ";

    // Printeaza delimitator subsol
    for (size_t j = 0; j < latime; ++j)
    {
        std::cout << "-";
    }

    std::cout << std::endl;
}

//
//  Task periodic
//

void Manager_joc::muta()
{
    if ((m_stare == STARE_JOC_PORNIT) and (m_tabla.get() != nullptr))
    {
        m_tabla->avanseaza();
    }
}


//
//  Incoming Interface
//

/**
 * @param[in] id     ID jucator
 * @param[in] nume   nume jucator
 */
void Manager_joc::adauga_jucator(int id, std::string nume)
{
    std::cout<< "adauga_jucator: " << id << ", " <<  nume << std::endl;
    Jucator_ptr jucatorPtr = std::make_shared<Jucator>(id, nume);

    m_jucatori.push_back(jucatorPtr);
}

/**
 * @param[in] id       ID jucator
 * @param[in] directie noua directie aleasa
 */
void Manager_joc::schimba_directie(int id, Tabla::Directie directie)
{
    if ((m_stare == STARE_JOC_PORNIT) and (m_tabla.get() != nullptr))
    {
        m_tabla->actualizeaza_directie(id, directie);
    }
}



//
//  Outgoing Interface
//

void Manager_joc::frame_nou(const char * const tabla, size_t latime, size_t inaltime)
{
    printeaza(tabla,latime,inaltime);
}

void Manager_joc::game_over(const Jucator_ptr &jucator)
{
    std::cout << "Jucatorul " << jucator->nume << " a pierdut; Scor: " << jucator->scor;
}

void Manager_joc::start_joc(size_t latime, size_t inaltime, const std::vector<Jucator_ptr> &jucatori)
{
    std::cout << "A inceput un joc nou ! Tabla de "
            << latime << "x" << inaltime << " cu " << jucatori.size() << " jucatori." << std::endl;

    for (auto jucator : jucatori)
    {
        std::cout << "\t" << jucator->id << ": " << jucator->nume;
    }
}

void Manager_joc::status_update(const std::vector<Jucator_ptr> &jucatori)
{
   if (jucatori.size())
   {
      std::cout << "Jucatori:" << std::endl;
      for (auto jucator : jucatori)
      {
         std::cout << jucator->id << ". " << jucator->nume;
      }
   }
   else
   {
      m_stare = STARE_JOC_OPRIT;
   }
}

void Manager_joc::eroare(const Jucator_ptr &jucator, int cod, const std::string &eroareStr)
{
    std::cout << "Eroare detectata pt " << jucator->nume << ": " << eroareStr;
}
