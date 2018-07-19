/*
 * Tabla.cpp
 *
 * Author: Nicolae Natea
 */
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Tabla.h"
#include <stdexcept>


const Tabla::Directie directii[Tabla::DIRECTIE_MAX] =
{
   Tabla::DIRECTIE_SUS,
   Tabla::DIRECTIE_JOS,
   Tabla::DIRECTIE_DREAPTA,
   Tabla::DIRECTIE_STANGA
};

// Helper pt printare directie
const char * const dirStr[Tabla::DIRECTIE_MAX] =
{
        [Tabla::DIRECTIE_SUS]="SUS",
        [Tabla::DIRECTIE_JOS]="JOS",
        [Tabla::DIRECTIE_DREAPTA]="DREAPTA",
        [Tabla::DIRECTIE_STANGA]="STANGA"
};

Tabla::Sarpe::Sarpe(Jucator_ptr j, int x, int y, int dimensiune)  :
    jucator(j),
    pozCapX(x),
    pozCapY(y),
    kaput(false)
{
    directie = directie_aleatorie();

    Directie next = Tabla::inversul_directiei(directie);

    for (int i = 0; i <dimensiune-1; i++)
    {
        corp.push_back(next);
    }
}

Tabla::Sarpe::~Sarpe()
{
   //corp.clear(); // pt ciudatenie daca 'corp' e de tip list
}

Tabla::Tabla(const std::vector<Jucator_ptr> &jucatori, size_t dimSarpe, Status_update_itf *itf) :
        m_itf(itf)
{
    //std::cout << "Creaza tabla" << std::endl;

    // Setam un seed pt random
    srand (time(NULL));

    // Sarpele va avea dimensiunea de macar 2 caractere
    if (dimSarpe < 2)
    {
        dimSarpe = 2;
    }

    size_t nrJucatori = jucatori.size();

    if ((nrJucatori == 0) or (nrJucatori > ('z' - 'a')) or (m_itf == nullptr))
    {
        std::string logErr = "Numar invalid de jucatori: " + nrJucatori;

        throw std::invalid_argument(logErr.c_str());
    }

    // setam valoare de minim (3* dim sarpe) pt a putea genera in
    // faza initiala un singur sarpe intr-un cadran
    m_height = 3 * nrJucatori * dimSarpe;
    m_width  = 5 * nrJucatori * dimSarpe;

    // Alocam spatiul necesar pt tabla de joc
    m_tabla = std::string(m_width * m_height, SPATIU_GOL);

    // Initializeaza tabla
    genereaza_serpi(jucatori, dimSarpe);
    genereaza_mancare(nrJucatori * 3);

    // Informeaza ca jocul urmeaza sa inceapa
    m_itf->start_joc(m_width, m_height, jucatori);
}

/**
 * @param[in] maxBobite numarul maxim de bobite ce poate fi generat pe tabla
 */
void Tabla::genereaza_mancare(int maxBobite)
{
    //std::cout << "Genereaza mancare" << std::endl;

    for (int i = 0; i < maxBobite; i++)
    {
        int offset = rand() % (m_height * m_width);

        if (m_tabla[offset] == SPATIU_GOL)
        {
            m_tabla[offset] = SPATIU_MANCARE;
        }
    }
}

void Tabla::genereaza_serpi(const std::vector<Jucator_ptr> &jucatori, int dimSarpe)
{
    //std::cout << "Genereaza serpi" << std::endl;

    // Imparte in cadrane egale: if (n*n == nrJucatori) => n randuri si n coloane
    size_t dimCadranY = m_height / sqrt(jucatori.size());
    size_t dimCadranX = m_width  / sqrt(jucatori.size());

    // Offset initial setat la mijlocul cadranului
    size_t offsetX = dimCadranX / 2;
    size_t offsetY = dimCadranY / 2;

    // Pentru fiecare jucator ...
    for (auto jucatorPtr : jucatori)
    {
        // Genereaza cate un sarpe in centrul cadranului
        Sarpe_ptr sarpePtr = std::make_shared<Sarpe>(jucatorPtr, offsetX, offsetY, dimSarpe);

        m_serpi.insert(std::make_pair(jucatorPtr->id, sarpePtr));

        if (offsetX >= m_width - dimCadranX)
        {
            offsetX  = dimCadranX / 2; // Salt in centrul urmatorului cadran
            offsetY += dimCadranY;     // De pe randul urmator
        }
        else
        {
            // Salt in centrul urmatorului cadran
            offsetX += dimCadranX;
        }
    }

    deseneaza_serpi();
}

/**
 * @param[in] id       id utilizator
 * @param[in] directie noua directie
 *
 * @return true daca directia a fost actualizata
 */
bool Tabla::actualizeaza_directie(int id, Directie directie)
{
    bool status = false;
    auto it = m_serpi.find(id);

    if (it != m_serpi.end())
    {
        if (Tabla::inversul_directiei(directie) != it->second->directie)
        {
            it->second->directie = directie;
            status = true;
        }
        else
        {
            m_itf->eroare(it->second->jucator, 0, "Directie invalida");
        }
    }
    else
    {
        //m_itf->eroare(nullptr, 0, "Directie invalida");
    }

    return status;
}

void Tabla::sterge_serpi()
{
    //std::cout << "Sterge serpi " << std::endl;
    for (auto &sarpe : m_serpi)
    {
        int x = sarpe.second->pozCapX;
        int y = sarpe.second->pozCapY;
        int offset = y * m_width + x;

        // Sterge cap
        m_tabla[offset] = SPATIU_GOL;

        // Sterge corp
        for (auto directie : sarpe.second->corp)
        {
            if (coordonata_urmatoare(x, y, directie))
            {
                offset = y * m_width + x;
                m_tabla[offset] = SPATIU_GOL;
            }
        }
    }
}

/**
 * @param[in/out] x        coordonata x initiala/rezultata
 * @param[in/out] y        coordonata y initiala/rezultata
 * @param[in/out] directie directia dorita
 *
 * @return true daca mutarea este in interiorul tablei, false altfel
 */
bool Tabla::coordonata_urmatoare(int &x, int &y, Directie directie) {
    switch (directie) {
        case DIRECTIE_DREAPTA:
            ++x;
            break;
        case DIRECTIE_STANGA:
            --x;
            break;
        case DIRECTIE_SUS:
            --y;
            break;
        case DIRECTIE_JOS:
            ++y;
            break;
        default:
        {
            std::string logErr = "Directie invalida: " + std::to_string(directie);
            throw std::invalid_argument(logErr.c_str());
            break;
        }
    }

    return ((x >= 0) and (x < m_width) and (y >= 0) and (y < m_height)
            and (y * m_width + x) < (m_width * m_height));
}

void Tabla::avanseaza()
{
    std::string tabla(m_width * m_height, SPATIU_GOL);

    bool statusUpdate = false;

    // Elimina serpii de pe tabla
    sterge_serpi();

    // Calculeaza noile pozitii ale serpilor
    for (auto &sarpe : m_serpi)
    {
        int x             = sarpe.second->pozCapX;
        int y             = sarpe.second->pozCapY;
        Directie directie = sarpe.second->directie;

        // Afiseaza mutarea facuta
        std::cout << "Sarpe: " << (char)('A' + sarpe.second->jucator->id)
                << " " << x << " " << y << ": " << dirStr[directie] << std::endl;

        // Valideaza noua pozitie - verifica sa nu fie in afara matricii
        bool mutareValida = coordonata_urmatoare(x, y, directie);

        bool creste = true;

        if (mutareValida)
        {
            int offset = y * m_width + x;
            // Seteaza coordonatele noului capat
            sarpe.second->pozCapX = x;
            sarpe.second->pozCapY = y;

            // Adauga pozitie noua la inceput
            //sarpe.second.corp.push_front(inversul_directiei(directie)); // pt list
            sarpe.second->corp.insert(sarpe.second->corp.begin(), inversul_directiei(directie));

             // Decide daca sarpele va creste cu o pozitie
             creste = (m_tabla[offset] == SPATIU_MANCARE);
        }
        else
        {
            sarpe.second->kaput = true;
        }

        x = sarpe.second->pozCapX;
        y = sarpe.second->pozCapY;

        // Setam spatiu ocupat si pt serpii ce urmeaza sa moara pt a forta
        // si jucatorii ce intra in ei sa piarda
        for (auto directie : sarpe.second->corp)
        {
           if (coordonata_urmatoare(x, y, directie))
           {
               int offset = y * m_width + x;
               tabla[offset] = SPATIU_OCUPAT;
           }
        }

        if ((not creste) and (not sarpe.second->corp.empty()))
        {
            // Sterge din coada
            sarpe.second->corp.pop_back();
        }
        else
        {
            // Indica cum ca statusul s-a actualizat pt un sarpe
            statusUpdate = true;
        }

        // Actualizeaza scor sarpe
        sarpe.second->jucator->scor = sarpe.second->corp.size() + 1;
    }

    // TODO: verifica coliziune capete std::multiset?
    auto sarpePtrIt = m_serpi.cbegin();

    // Elimina serpii pe cale sa dea ortul popii
    while (sarpePtrIt != m_serpi.cend())
    {
        int offset = sarpePtrIt->second->pozCapY * m_width + sarpePtrIt->second->pozCapX;
        // Verificam daca sarpele a iesit din cadru sau
      // daca muta intr-o pozitie in care avem un alt sarpe
        if (
                sarpePtrIt->second->kaput or
                tabla[offset] == SPATIU_OCUPAT)
        {
            // Informeaza ca sarpele a murit
            m_itf->game_over(sarpePtrIt->second->jucator);

            // Sterge-l din lista curenta
            sarpePtrIt = m_serpi.erase(sarpePtrIt);

            statusUpdate = true;
        }
        else
        {
            ++sarpePtrIt;
        }
    }

    deseneaza_serpi();

    // Genereaza maxim o bobita
    genereaza_mancare(1);

    // Informeaza ca un frame nou este disponibil
    m_itf->frame_nou(m_tabla.data(), m_width, m_height);

    if (statusUpdate)
    {
        std::vector<Jucator_ptr> jucatori;

        for (auto &sarpe : m_serpi)
        {
            jucatori.push_back(sarpe.second->jucator);
        }

        m_itf->status_update(jucatori);
    }
}

void Tabla::deseneaza_serpi()
{
    // Redeseneaza serpi
    for (auto &sarpe : m_serpi)
    {
        int id = sarpe.second->jucator->id;
        int x  = sarpe.second->pozCapX;
        int y  = sarpe.second->pozCapY;
        int offset = y * m_width + x;

        // Cap sarpe reprezentat prin majuscule
        m_tabla[offset] = 'A' + id;

        for (auto directie : sarpe.second->corp)
        {
            // Corp sarpe reprezentat prin 'minuscule' ? does it refer to letters? hmmm
             if (coordonata_urmatoare(x, y, directie))
             {
                 offset = y * m_width + x;
                 m_tabla[offset] = 'a' + id;
             }
        }
    }
}

/**
 * @param[in] dir - directia pt care dorim sa aflam inversul
 *
 * @return inversul unei directii primite ca parametru
 */
Tabla::Directie Tabla::inversul_directiei(Directie dir)
{
    Directie directie = DIRECTIE_STANGA;

    switch (dir)
    {
    case (DIRECTIE_DREAPTA):
        directie = DIRECTIE_STANGA;
        break;
    case (DIRECTIE_STANGA):
        directie = DIRECTIE_DREAPTA;
        break;
    case (DIRECTIE_SUS):
        directie = DIRECTIE_JOS;
        break;
    case (DIRECTIE_JOS):
        directie = DIRECTIE_SUS;
        break;
    default:
        break;
    }

    return directie;
}

/**
 * @return returneaza o directie aleatorie
 */
Tabla::Directie Tabla::directie_aleatorie()
{
   size_t directie  = rand();
   return directii[directie % Tabla::DIRECTIE_MAX];
}

Tabla::~Tabla() {
}
