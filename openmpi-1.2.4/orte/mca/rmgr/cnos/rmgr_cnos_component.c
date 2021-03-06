/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */

#include "orte_config.h"

#ifdef HAVE_CNOS_PM_BARRIER
#include <catamount/cnos_mpi_os.h>
#endif

#include "orte/orte_constants.h"
#include "orte/util/proc_info.h"
#include "opal/util/output.h"
#include "orte/mca/errmgr/errmgr.h"

#include "orte/mca/rds/base/base.h"
#include "orte/mca/ras/base/base.h"
#include "orte/mca/rmaps/base/base.h"
#include "orte/mca/pls/base/base.h"
#include "rmgr_cnos.h"

/*
 * Local functions
 */

static int orte_rmgr_cnos_open(void);
static int orte_rmgr_cnos_close(void);
static orte_rmgr_base_module_t* orte_rmgr_cnos_init(int *priority);


orte_rmgr_base_component_t mca_rmgr_cnos_component = {
      /* First, the mca_base_component_t struct containing meta
         information about the component itself */

      {
        /* Indicate that we are a rmgr v2.0.0 component (which also
           implies a specific MCA version) */

        ORTE_RMGR_BASE_VERSION_2_0_0,

        "cnos", /* MCA component name */
        ORTE_MAJOR_VERSION,  /* MCA component major version */
        ORTE_MINOR_VERSION,  /* MCA component minor version */
        ORTE_RELEASE_VERSION,  /* MCA component release version */
        orte_rmgr_cnos_open,  /* component open  */
        orte_rmgr_cnos_close  /* component close */
      },

      /* Next the MCA v1.0.0 component meta data */
      {
        /* Whether the component is checkpointable or not */
        false
      },

      orte_rmgr_cnos_init
};


/**
  * component open/close/init function
  */
static int orte_rmgr_cnos_open(void)
{
    return ORTE_SUCCESS;
}


static orte_rmgr_base_module_t *orte_rmgr_cnos_init(int* priority)
{
    /* set a priority higher than the proxy component */
    *priority = 10;

#ifdef HAVE_CNOS_PM_BARRIER
    /* register with the process manager so that everyone aborts if
       any one process aborts.  This is a bit slower than it needs to
       be, but useful. */
    cnos_pm_barrier(0);
#endif

    return &orte_rmgr_cnos_module;
}


/**
 *  Close all subsystems.
 */
static int orte_rmgr_cnos_close(void)
{
    return ORTE_SUCCESS;
}
