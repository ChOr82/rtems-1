/*
 *  BSP startup
 *
 *  This routine starts the application.  It includes application,
 *  board, and monitor specific initialization and configuration.
 *  The generic CPU dependent initialization has been performed
 *  before this routine is invoked.
 *
 *  Copyright (C) 2000 OKTET Ltd., St.-Petersburg, Russia
 *  Author: Victor V. Vengerov <vvv@oktet.ru>
 *
 *  Based on work:
 *  Author:
 *    David Fiddes, D.J@fiddes.surfaid.org
 *    http://www.calm.hw.ac.uk/davidf/coldfire/
 *
 *  COPYRIGHT (c) 1989-1998.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *
 *  http://www.OARcorp.com/rtems/license.html.
 * 
 *  $Id$
 */

#include <bsp.h>
#include <rtems/libio.h>
 
#include <rtems/libcsupport.h>
 
#include <string.h>

/*
 *  The original table from the application and our copy of it with
 *  some changes.
 */

extern rtems_configuration_table Configuration;
rtems_configuration_table  BSP_Configuration;

rtems_cpu_table Cpu_table;

extern unsigned int _RamSize;


char *rtems_progname;

/*
 *  Use the shared implementations of the following routines
 */
 
void bsp_postdriver_hook(void);
void bsp_libc_init( void *, uint32_t, int );
void bsp_pretasking_hook(void);               /* m68k version */

/* bsp_start --
 *     This routine does the bulk of the system initialisation.
 */
void bsp_start( void )
{
    extern void *_WorkspaceBase;
    extern unsigned long _M68k_Ramsize;
    _M68k_Ramsize = (unsigned long)&_RamSize;   /* size set in linker script */


    /*
     *  Need to "allocate" the memory for the RTEMS Workspace and
     *  tell the RTEMS configuration where it is.  This memory is
     *  not malloc'ed.  It is just "pulled from the air".
     */

    BSP_Configuration.work_space_start = (void *)&_WorkspaceBase;

    /*
     *  initialize the CPU table for this BSP
     */
    Cpu_table.pretasking_hook = bsp_pretasking_hook;  /* init libc, etc. */

    Cpu_table.postdriver_hook = bsp_postdriver_hook;

    Cpu_table.do_zero_of_workspace = TRUE;

    Cpu_table.interrupt_stack_size = 4096;

    Cpu_table.interrupt_vector_table = (m68k_isr *)0; /* vectors at start of RAM */

}
