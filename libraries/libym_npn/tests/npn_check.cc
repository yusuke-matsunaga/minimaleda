/* 
 * tkAppInit.c --
 *
 *	Provides a default version of the Tcl_AppInit procedure for
 *	use in wish and similar Tk-based applications.
 *
 * Copyright (c) 1993 The Regents of the University of California.
 * Copyright (c) 1994-1997 Sun Microsystems, Inc.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * RCS: @(#) $Id: npn_check.cc 1594 2008-07-18 10:26:12Z matsunaga $
 */

#include <tk.h>
#include <locale.h>

#include "FuncCmd.h"
#include "TvFuncTestCmd.h"
#include "TvFuncTimeCmd.h"

#ifdef TK_TEST
extern int		Tktest_Init _ANSI_ARGS_((Tcl_Interp *interp));
#endif /* TK_TEST */

/*
 * The following #if block allows you to change the AppInit
 * function by using a #define of TCL_LOCAL_APPINIT instead
 * of rewriting this entire file.  The #if checks for that
 * #define and uses Tcl_AppInit if it doesn't exist.
 */

#ifndef TK_LOCAL_APPINIT
#define TK_LOCAL_APPINIT Tcl_AppInit    
#endif
extern int TK_LOCAL_APPINIT _ANSI_ARGS_((Tcl_Interp *interp));


/*
 *----------------------------------------------------------------------
 *
 * main --
 *
 *	This is the main program for the application.
 *
 * Results:
 *	None: Tk_Main never returns here, so this procedure never
 *	returns either.
 *
 * Side effects:
 *	Whatever the application does.
 *
 *----------------------------------------------------------------------
 */

int
main(int argc, char** argv)
{
    
    /*
     * The following #if block allows you to change how Tcl finds the startup
     * script, prime the library or encoding paths, fiddle with the argv,
     * etc., without needing to rewrite Tk_Main()
     */
    
#ifdef TK_LOCAL_MAIN_HOOK
    extern int TK_LOCAL_MAIN_HOOK _ANSI_ARGS_((int *argc, char ***argv));
    TK_LOCAL_MAIN_HOOK(&argc, &argv);
#endif

    Tk_Main(argc, argv, TK_LOCAL_APPINIT);
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
  using namespace nsYm::nsTclpp;
  using namespace nsYm;

  if (Tcl_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
    if (Tk_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
    Tcl_StaticPackage(interp, "Tk", Tk_Init, Tk_SafeInit);

#ifdef TK_TEST
    if (Tktest_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
    Tcl_StaticPackage(interp, "Tktest", Tktest_Init,
            (Tcl_PackageInitProc *) NULL);
#endif /* TK_TEST */


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

    /*
     * Call Tcl_CreateCommand for application-specific commands, if
     * they weren't already created by the init procedures called above.
     */

    TclCmdBinder<ForallFuncCmd>::reg(interp, "ym_npn::forall_func");
    TclCmdBinder<RandomFuncCmd>::reg(interp, "ym_npn::random_func");
    TclCmdBinder<ReadFuncCmd>::reg(interp,   "ym_npn::read_func");
    TclCmdBinder<DumpFuncCmd>::reg(interp,   "ym_npn::dump_func");
    TclCmdBinder<TvFuncTestCmd>::reg(interp, "ym_npn::tvfunc_test");
    TclCmdBinder<TvFuncTimeCmd>::reg(interp, "ym_npn::tvfunc_time");
    TclCmdBinder<TvFuncNpnCmd>::reg(interp,  "ym_npn::tvfunc_npn");

    // これらのコマンドをエクスポートしておく
    const char* cmds =
      "namespace eval ym_npn {\n"
      "    namespace export forall_func\n"
      "    namespace export random_func\n"
      "    namespace export read_func\n"
      "    namespace export dump_func\n"
      "    namespace export tvfunc_test\n"
      "    namespace export tvfunc_time\n"
      "    namespace export tvfunc_npn\n"
      "}";
    int code = Tcl_Eval(interp, cmds);
    if ( code != TCL_OK ) {
      return code;
    }

    /*
     * Specify a user-specific startup file to invoke if the application
     * is run interactively.  Typically the startup file is "~/.apprc"
     * where "app" is the name of the application.  If this line is deleted
     * then no user-specific startup file will be run under any conditions.
     */

    Tcl_SetVar(interp, "tcl_rcFileName", "~/.ym_npnrc", TCL_GLOBAL_ONLY);
    
    return TCL_OK;
}
