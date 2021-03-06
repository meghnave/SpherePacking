/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */
#include "ompi_config.h"
#include <stdio.h>

#include "ompi/mpi/c/bindings.h"
#include "ompi/win/win.h"
#include "ompi/mca/osc/osc.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Win_wait = PMPI_Win_wait
#endif

#if OMPI_PROFILING_DEFINES
#include "ompi/mpi/c/profile/defines.h"
#endif

static const char FUNC_NAME[] = "MPI_Win_wait";


int MPI_Win_wait(MPI_Win win) 
{
    int rc;

    if (MPI_PARAM_CHECK) {
        OMPI_ERR_INIT_FINALIZE(FUNC_NAME);

        if (ompi_win_invalid(win)) {
            return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, MPI_ERR_WIN, FUNC_NAME);
        } else if (0 == (ompi_win_get_mode(win) & OMPI_WIN_POSTED)) {
            return OMPI_ERRHANDLER_INVOKE(win, MPI_ERR_RMA_SYNC, FUNC_NAME);
        }
    }

    rc = win->w_osc_module->osc_wait(win);
    OMPI_ERRHANDLER_RETURN(rc, win, rc, FUNC_NAME);
}
