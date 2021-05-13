/*
 * Copyright (c) 2021 vargaconsulting, Toronto,ON Canada
 * Author: Varga, Steven <steven@vargaconsulting.ca>
*/
#ifndef MPICPP_CONFIG_H
#define MPICPP_CONFIG_H

/*
@example collective.cpp
*/

/** @defgroup io mpi::send<T>( ... ); 
 * \brief Templated full or partial mpi calls
 */

namespace mpi {
    template <class T> struct name {
        static constexpr char const * value = "n/a";
    };
}
#endif





