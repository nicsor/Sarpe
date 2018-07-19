/*
 * Jucator.h
 *
 * Author: Nicolae Natea
 */

#ifndef JUCATOR_H_
#define JUCATOR_H_

#include <memory>
#include <string>

struct Jucator {
    int         id;     ///< ID jucator
	std::string nume;   ///< Nume jucator
    int         scor;   ///< Scor jucator

	Jucator(int id, std::string nume) :
	    id(id),
	    nume(nume),
	    scor(0)
    {}
};

typedef std::shared_ptr<Jucator> Jucator_ptr;

#endif /* JUCATOR_H_ */
