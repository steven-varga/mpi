/*
 * Copyright (c) 2021 vargaconsulting, Toronto,ON Canada
 * Author: Varga, Steven <steven@vargaconsulting.ca>
 */
#ifndef  MPICPP_COMM_HPP
#define  MPICPP_COMM_HPP

#include <mpi.h>

#ifdef MPI_CONVERSION_IMPLICIT
	#define MPICPP__EXPLICIT
#else
	#define MPICPP__EXPLICIT explicit
#endif

namespace mpi::impl {
}
#endif
