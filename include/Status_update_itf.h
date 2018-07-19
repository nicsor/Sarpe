/*
 * Status_update_itf.h
 *
 * Author: Nicolae Natea
 */

#ifndef STATUS_UPDATE_ITF_H_
#define STATUS_UPDATE_ITF_H_


// Forward declaration
class Jucator;

class Status_update_itf {
public:
    virtual ~Status_update_itf()
    {

    }

    /**
     * Callback pt a indica o eroare pe partea de logica cauzata de un jucator
     *
     * @param[in] jucator   instanta jucator
     * @param[in] cod       cod de eroare
     * @param[in] eroareStr eroare
     */
    virtual void eroare(const Jucator_ptr &jucator, int cod, const std::string &eroareStr) = 0;

    /**
     * Callback pt a indica avans joc.
     *
     * @param[in] tabla     tabla de joc
     * @param[in] latime    latime tabla
     * @param[in] inaltime  inaltime tabla
     */
    virtual void frame_nou(const char * const tabla, size_t latime, size_t inaltime) = 0;

    /**
     * Callback pt a indica ca un jucator a pierdut
     *
     * @param[in] jucator   jucatorul care a pierdut
     */
    virtual void game_over(const Jucator_ptr &jucator) = 0;

    /**
     * Callback pt a indica startul unei partide de joc si pt a indica jucatorii inscrisi
     *
     * @param[in] latime    latime tabla
     * @param[in] inaltime  inaltime tabla
     * @param[in] jucatori  jucatori inscrisi
     */
    virtual void start_joc(size_t latime, size_t inaltime, const std::vector<Jucator_ptr> &jucatori) = 0;

    /**
     * Callback pt a indica acutualizarea scorului
     *
     * @param[in] jucatori  jucatori existenti
     */
    virtual void status_update(const std::vector<Jucator_ptr> &jucatori) = 0;
};

#endif /* STATUS_UPDATE_ITF_H_ */
