/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2006 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2006      Cisco Systems, Inc.  All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */
#include "ompi_config.h"
#include <stdio.h>

#include "ompi/mpi/c/bindings.h"
#include "ompi/mca/pml/pml.h"
#include "ompi/request/request.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Waitall = PMPI_Waitall
#endif

#if OMPI_PROFILING_DEFINES
#include "ompi/mpi/c/profile/defines.h"
#endif

static const char FUNC_NAME[] = "MPI_Waitall";


int MPI_Waitall(int count, MPI_Request *requests, MPI_Status *statuses) 
{
    if ( MPI_PARAM_CHECK ) {
        int i, rc = MPI_SUCCESS;
        OMPI_ERR_INIT_FINALIZE(FUNC_NAME);
        if( (NULL == requests) && (0 != count) ) {
            rc = MPI_ERR_REQUEST;
        }
        for (i = 0; i < count; i++) {
            if (NULL == requests[i]) {
                rc = MPI_ERR_REQUEST;
            }
        }
        OMPI_ERRHANDLER_CHECK(rc, MPI_COMM_WORLD, rc, FUNC_NAME);
    }

    if (OMPI_SUCCESS == ompi_request_wait_all(count, requests, statuses)) {
        return MPI_SUCCESS;
    }
    if (MPI_SUCCESS != 
        ompi_errhandler_request_invoke(count, requests, FUNC_NAME)) {
        return MPI_ERR_IN_STATUS;
    }
    return MPI_SUCCESS;
}
