/*
 * tclAppInit.c --
 *
 *	Provides a default version of the main program and Tcl_AppInit
 *	procedure for Tcl applications (without Tk).
 *
 * Copyright (c) 1993 The Regents of the University of California.
 * Copyright (c) 1994-1997 Sun Microsystems, Inc.
 * Copyright (c) 1998-1999 by Scriptics Corporation.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * RCS: @(#) $Id: tclAppInit.cc 2274 2009-06-10 07:45:29Z matsunaga $
 */

#include "magus_nsdef.h"

#define ERRORCODE_HACK 1

#include <tcl.h>

#ifdef HAVE_TCLREADLINE
#include <tclreadline.h>
#endif


#ifdef TCL_TEST

#include "tclInt.h"

extern int		Procbodytest_Init _ANSI_ARGS_((Tcl_Interp *interp));
extern int		Procbodytest_SafeInit _ANSI_ARGS_((Tcl_Interp *interp));
extern int		TclObjTest_Init _ANSI_ARGS_((Tcl_Interp *interp));
extern int		Tcltest_Init _ANSI_ARGS_((Tcl_Interp *interp));
#ifdef TCL_THREADS
extern int		TclThread_Init _ANSI_ARGS_((Tcl_Interp *interp));
#endif

#endif /* TCL_TEST */

#ifdef TCL_XT_TEST
extern void		XtToolkitInitialize _ANSI_ARGS_((void));
extern int		Tclxttest_Init _ANSI_ARGS_((Tcl_Interp *interp));
#endif

/*
 * The following #if block allows you to change the AppInit
 * function by using a #define of TCL_LOCAL_APPINIT instead
 * of rewriting this entire file.  The #if checks for that
 * #define and uses Tcl_AppInit if it doesn't exist.
 */

#ifndef TCL_LOCAL_APPINIT
#define TCL_LOCAL_APPINIT Tcl_AppInit
#endif
extern
int
TCL_LOCAL_APPINIT _ANSI_ARGS_((Tcl_Interp *interp));

extern
int
magus_init(Tcl_Interp* interp);

/*
 *----------------------------------------------------------------------
 *
 * main --
 *
 *	This is the main program for the application.
 *
 * Results:
 *	None: Tcl_Main never returns here, so this procedure never
 *	returns either.
 *
 * Side effects:
 *	Whatever the application does.
 *
 *----------------------------------------------------------------------
 */

int
main(int argc,
     char** argv)
{
  /*
   * The following #if block allows you to change how Tcl finds the startup
   * script, prime the library or encoding paths, fiddle with the argv,
   * etc., without needing to rewrite Tcl_Main()
   */

#ifdef TCL_LOCAL_MAIN_HOOK
  extern int TCL_LOCAL_MAIN_HOOK _ANSI_ARGS_((int *argc, char ***argv));
#endif

#ifdef TCL_XT_TEST
  XtToolkitInitialize();
#endif

#ifdef TCL_LOCAL_MAIN_HOOK
  TCL_LOCAL_MAIN_HOOK(&argc, &argv);
#endif

  Tcl_Main(argc, argv, TCL_LOCAL_APPINIT);

  return 0;			/* Needed only to prevent compiler warning. */
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_AppInit --
 *
 *	This procedure performs application-specific initialization.
 *	Most applications, especially those that incorporate additional
 *	packages, will have their own version of this procedure.
 *
 * Results:
 *	Returns a standard Tcl completion code, and leaves an error
 *	message in the interp's result if an error occurs.
 *
 * Side effects:
 *	Depends on the startup script.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_AppInit(Tcl_Interp* interp)
{
  using namespace std;

  if (Tcl_Init(interp) == TCL_ERROR) {
    return TCL_ERROR;
  }

#ifdef TCL_TEST
#ifdef TCL_XT_TEST
  if (Tclxttest_Init(interp) == TCL_ERROR) {
    return TCL_ERROR;
  }
#endif
  if (Tcltest_Init(interp) == TCL_ERROR) {
    return TCL_ERROR;
  }
  Tcl_StaticPackage(interp, "Tcltest", Tcltest_Init,
		    (Tcl_PackageInitProc *) NULL);
  if (TclObjTest_Init(interp) == TCL_ERROR) {
    return TCL_ERROR;
  }
#ifdef TCL_THREADS
  if (TclThread_Init(interp) == TCL_ERROR) {
    return TCL_ERROR;
  }
#endif
  if (Procbodytest_Init(interp) == TCL_ERROR) {
    return TCL_ERROR;
  }
  Tcl_StaticPackage(interp, "procbodytest", Procbodytest_Init,
		    Procbodytest_SafeInit);
#endif /* TCL_TEST */

#ifdef HAVE_TCLREADLINE
  if (TCL_ERROR == Tclreadline_Init(interp)) {
    return TCL_ERROR;
  }
  Tcl_StaticPackage(interp, "tclreadline",
		    Tclreadline_Init, Tclreadline_SafeInit);
  string file = TCLRL_LIBRARY;
  file += "/tclreadlineInit.tcl";
  int status = Tcl_EvalFile(interp, file.c_str());
  if ( status != TCL_OK ) {
    cerr << "(TclreadlineAppInit) unable to eval " << file << endl;
    exit (1);
  }
#ifdef ERRORCODE_HACK
  // よくわからないけど errorCode と errorInfo を定義しておかないと
  // おこられるみたい．
  {
    const char* tmp_script = "set ::errorCode NONE; set ::errorInfo NONE";
    if ( Tcl_Eval(interp, tmp_script) != TCL_OK ) {
      cerr << "(Tcl_AppInit) unable to eval \"" << tmp_script
	   << "\"" << endl;
      exit(1);
    }
  }
#endif // ERRORCODE_HACK
#endif

  /*
   * Call the init procedures for included packages.  Each call should
   * look like this:
   *
   * if (Mod_Init(interp) == TCL_ERROR) {
   *     return TCL_ERROR;
   * }
   *
   * where "Mod" is the name of the module.
   */
  if ( magus_init(interp) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  /*
   * Call Tcl_CreateCommand for application-specific commands, if
   * they weren't already created by the init procedures called above.
   */


  /*
   * Specify a user-specific startup file to invoke if the application
   * is run interactively.  Typically the startup file is "~/.apprc"
   * where "app" is the name of the application.  If this line is deleted
   * then no user-specific startup file will be run under any conditions.
   */

  Tcl_SetVar(interp, "tcl_rcFileName", "~/.magusrc", TCL_GLOBAL_ONLY);

  return TCL_OK;
}
