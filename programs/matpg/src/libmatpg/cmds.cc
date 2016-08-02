/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: cmds.c,v $
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  91/12/28  16:58:39  yusuke
 * Final , Final revision
 * 
 * Revision 2.0  91/12/21  18:49:39  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.6  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "mcsh.h"
#include "mcsh_var.h"
#include "mcsh_args.h"
#include "CellLib.h"
#include "bn.h"
#include "matpg.h"
#include "cmds.h"
#include "network.h"
#include "fault.h"
#include "testpat.h"
#include "search_mgr.h"
#include "learn.h"
#include "timer.h"

static char RCSid[] = "$Header: /vol/eros/eros6/yusuke/src/CC/MATPG/RCS/cmds.c,v 2.2 1992/01/22 23:49:14 yusuke Exp $";

sw_timer_t sw_timer(4);
static timer_t timer_for_print_stats;
static timer_t timer_for_time;

int cmd_read_blif(int argc, Cchar** argv) {
	mcsh_args_t temp(argc, argv);
	mcsh_command_subst(temp);
	if (mcsh_file_subst(temp) == false) {
		return 0;
	}
	int argc1 = temp.argc();
	Cchar** argv1 = temp.argv();
	if (argc1 > 2) {
		fputs("read: too many arguments.\n", cur_fp2);
		return 0;
	}
	FILE* fp;
	if (argc1 == 1) {
		fp = cur_fp0;
	} else {
		if ((fp = fopen((char*) argv1[1], "r")) == NULL) {
			fprintf(cur_fp2, "%s: no such file.\n", argv1[1]);
			return 0;
		}
	}

	int old_tm_id = sw_timer.change(TM_READ);
	gn_read_blif(fp, cur_fp2);
	sw_timer.change(old_tm_id);

	if (fp != cur_fp0) {
		fclose(fp);
	}
	return 0;
}

int cmd_read_lib(int argc, Cchar** argv) {
	mcsh_args_t temp(argc, argv);
	mcsh_command_subst(temp);
	if (mcsh_file_subst(temp) == false) {
		return 0;
	}
	int argc1 = temp.argc();
	Cchar** argv1 = temp.argv();
	if (argc1 > 2) {
		fputs("readlib: too many arguments.\n", cur_fp2);
		return 0;
	}
	FILE* fp;
	if (argc1 == 1) {
		fp = cur_fp0;
	} else {
		if ((fp = fopen((char*) argv1[1], "r")) == NULL) {
			fprintf(cur_fp2, "%s: no such file or directory.\n", argv1[1]);
			return 0;
		}
	}

	int old_tm_id = sw_timer.change(TM_READ);
	cur_lib->erase();
	cur_lib->read(fp);
	sw_timer.change(old_tm_id);

	if (fp != cur_fp0) {
		fclose(fp);
	}
	return 0;
}

int cmd_write_blif(int argc, Cchar** argv) {
	mcsh_args_t temp(argc, argv);
	mcsh_command_subst(temp);
	if (mcsh_file_subst(temp) == false) {
		return 0;
	}
	int argc1 = temp.argc();
	Cchar** argv1 = temp.argv();
	if (gn_get_npi() == 0 || gn_get_npo() == 0) {
		fputs("write: current network is empty.\n", cur_fp2);
		return 0;
	}
	if (argc1 > 2) {
		fputs("write: too many arguments.\n", cur_fp2);
		return 0;
	}
	if (argc1 == 1) {
		bn_write_blif(cur_fp1);
	} else {
		FILE* fp;
		if ((fp = fopen((char*) argv1[1], "w")) == NULL) {
			fprintf(cur_fp2, "%s: can not create file.\n", argv1[1]);
			return 0;
		}
		bn_write_blif(fp);
		fclose(fp);
	}
	return 0;
}

int cmd_clr_fault(int argc0, Cchar** ) {
	if (argc0 != 1) {
		fputs("clr_fault: Too many arguments.\n", cur_fp2);
		return 0;
	}
	fault_clr();
	tp_clr();
	return 0;
}

int cmd_set_fault(int argc0, Cchar** argv0) {
	timer_t timer;
	mcsh_args_t temp(argc0, argv0);
	mcsh_command_subst(temp);
	if (mcsh_file_subst(temp) == false) {
		return 0;
	}
	int argc = temp.argc();
	Cchar** argv = temp.argv();
	bn_node_t* node;
	val3 val;
	int ipos;
	switch (argc) {
	case 1:
		fault_set_all();
		break;
	case 3:
	case 4:
		if (strcmp(argv[1], "0") == 0) {
			val = val_0;
		} else if (strcmp(argv[1], "1") == 0) {
			val = val_1;
		} else {
			fputs("Usage: set_fault [0/1 node-name [input#]]\n", cur_fp2);
			return 0;
		}
		if ((node = bn_find_node(argv[2], false)) == NULL) {
			fprintf(cur_fp2, "%s: No such node\n", argv[2]);
			return 0;
		}
		if (node->chk_del() == true) {
			node = node->proxy;
		}
		if (argc == 3) {
			ipos = -1;
		} else {
			ipos = atoi(argv[3]);
			if (ipos >= node->ni) {
				fputs("set_fault: input# exceeds # of inputs\n", cur_fp2);
				return 0;
			}
			if (node->get_pol(ipos) == 1) {
				val = neg3(val);
			}
		}
		fault_add(val, node->gate, ipos);
		break;
	default:
		fputs("Too many arguments.\n", cur_fp2);
		break;
	}
	if (log_flag == true) {
		timer.stop();
		fprintf(log_fp, "********* set_fault *********\n");
		fprintf(log_fp, "%10d: # of Total faults.\n", fault_w_num());
		fprintf(log_fp, "%10.2fu %10.2fs: CPU time\n",
			timer.get_utime(), timer.get_stime());
	}
	return 0;
}

int cmd_print_fault(int argc, Cchar** argv) {
	if (argc > 2) {
		fputs("print_fault: too many arguments.\n", cur_fp2);
		return 0;
	}
	FTYPE ftype;
	if (argc == 1 || strcmp(argv[1], "-u") == 0) {
		ftype = FTYPE_U;
	} else if (strcmp(argv[1], "-r") == 0) {
		ftype = FTYPE_R;
	} else if (strcmp(argv[1], "-d") == 0) {
		ftype = FTYPE_D;
	} else {
		fputs("usage: print_fault [-u/-d/-r]\n", cur_fp2);
	}
	fault_print(cur_fp1, ftype);
	return 0;
}

int cmd_print_pattern(int argc, Cchar** argv) {
	bool num_flag = false;
	bool hex_flag = false;
	for (int i = 1; i < argc; i ++) {
		if (strcmp(argv[i], "-hex") == 0) {
			hex_flag = true;
		} else if (strcmp(argv[i], "-num") == 0) {
			num_flag = true;
		} else if (argv[i][0] == '-') {
			fputs("usage: print_pattern [-hex]\n", cur_fp2);
			return 0;
		} else {
			break;
		}
	}
	if (i < argc) {
		fputs("print_pattern: too many arguments.\n", cur_fp2);
		return 0;
	}
	tp_print(cur_fp1, num_flag, hex_flag);
	return 0;
}

int cmd_print_stats(int argc, Cchar**) {
	if (argc != 1) {
		fputs("USAGE: print_stats\n", cur_fp2);
		return 0;
	}
	timer_for_print_stats.stop();
	double utime = timer_for_print_stats.get_utime();
	double stime = timer_for_print_stats.get_stime();
	timer_for_print_stats.start();
	fprintf(cur_fp1, "#A: # of total faults       = %7d\n", fault_w_num());
	fprintf(cur_fp1, "#B: # of detected faults    = %7d\n", fault_d_num());
	fprintf(cur_fp1, "#C: # of redundant faults   = %7d\n", fault_r_num());
	fprintf(cur_fp1, "#D: # of undetected faults  = %7d\n", fault_u_num());
	fprintf(cur_fp1, "#E: # of generated patterns = %7d\n", tp_get_num());
	fprintf(cur_fp1, "#F:   Total CPU time        = %7.2fu %7.2fs\n",
			utime, stime);
	fprintf(cur_fp1, "#G:    (read time           = %7.2f)\n",
							sw_timer.get_utime(TM_READ) + sw_timer.get_utime(TM_MISC));
	fprintf(cur_fp1, "#H:    (dtpg time           = %7.2f)\n",
													sw_timer.get_utime(TM_DTPG));
	fprintf(cur_fp1, "#I:    (fsim time           = %7.2f)\n",
													sw_timer.get_utime(TM_FSIM));
	return 0;
}

int cmd_time(int argc, Cchar**) {
	if (argc != 1) {
		fputs("time: Too many arguments.\n", cur_fp2);
		return 0;
	}
	timer_for_time.stop();
	double utime = timer_for_time.get_utime();
	double stime = timer_for_time.get_stime();
	timer_for_time.start();
	fprintf(cur_fp1, "Total %5.2fu %5.2fs\n", utime, stime);
	return 0;
}

int cmd_rtpg(int argc, Cchar** argv) {
	int DEFAULT_PAT_NUM = 10000;
	mcsh_args_t temp(argc, argv);
	mcsh_command_subst(temp);
	if (mcsh_file_subst(temp) == false) {
		return 0;
	}

	if (fault_u_num() == 0) {
		return 0;
	}

	FILE* fp = NULL;
	int max_pat = DEFAULT_PAT_NUM;
	int min_f = 0;
	int max_i = 4;
	int argc1 = temp.argc();
	Cchar** argv1 = temp.argv();
	bool n_flag = false;
	for (int i = 1; i < argc1; i ++) {
		if (strcmp(argv1[i], "-f") == 0) {
			if (i + 1 >= argc1) {
				fputs("rtpg: no filename specified.\n", cur_fp2);
				return 0;
			}
			if ((fp = fopen((char*) argv1[i + 1], "r")) == NULL) {
				fprintf(cur_fp2, "%s: No such file.\n", argv1[i + 1]);
				return 0;
			}
			i ++;
		} else if (strcmp(argv1[i], "-n") == 0) {
			if (i + 1 >= argc1) {
				fputs("rtpg: number of patterns not specified.\n", cur_fp2);
				return 0;
			}
			max_pat = atoi(argv1[i + 1]);
			n_flag = true;
			i ++;
		} else if (strcmp(argv1[i], "-s") == 0) {
			if (i + 1 >= argc1) {
				fputs("rtpg: random seed not specified.\n", cur_fp2);
				return 0;
			}
			int seed = atoi(argv1[i + 1]);
			srandom(seed);
			i ++;
		} else if (strcmp(argv1[i], "-min") == 0) {
			if (i + 1 >= argc1) {
				fputs("rtpg: mininum faults number not specified.\n", cur_fp2);
				return 0;
			}
			min_f = atoi(argv1[i + 1]);
			i ++;
		} else if (strcmp(argv1[i], "-max") == 0) {
			if (i + 1 >= argc1) {
				fputs("rtpg: maximum intervals not specified.\n", cur_fp2);
				return 0;
			}
			max_i = atoi(argv1[i + 1]);
			i ++;
		} else {
			fputs("usage: rtpg [-f filename] [-n #_of_patterns]\n", cur_fp2);
			fputs("            [-min minimum-fault]\n", cur_fp2);
			fputs("            [-max maximum-interval]\n", cur_fp2);
			fputs("            [-s random-seed]\n", cur_fp2);
			return 0;
		}
	}
	if (n_flag == true) {
		min_f = 0;
		max_i = -1;
	}

	rtpg(min_f, max_i, max_pat, fp);

	return 0;
}

int cmd_rfsim(int argc, Cchar**) {
	if (argc > 1) {
		fputs("rfsim: too many arguments.\n", cur_fp2);
		return 0;
	}

	rfsim();

	return 0;
}

int cmd_dtpg(int argc, Cchar** argv) {
	extern int us_mode;
	FSIM_MODE fsim_mode;
	bool learn_flag;
	DYN_MODE dyn_mode;
	bool scope_flag;
	char* ptr;

	if (fault_u_num() == 0) {
		return 0;
	}

	/* set fault simulation mode */
	if (my_var.chk("fsim") == true) {
		if (ptr = my_var.get("fsim")) {
			if (strcmp(ptr, "single") == 0) {
				fsim_mode = FSIM_SINGLE;
			} else if (strcmp(ptr, "parallel") == 0) {
				fsim_mode = FSIM_PARALLEL;
			} else if (strcmp(ptr, "none") == 0) {
				fsim_mode = FSIM_NONE;
			}
		} else {
			fsim_mode = FSIM_PARALLEL;
		}
	} else {
		fsim_mode = FSIM_NONE;
	}

	/* set backtrace mode */
	if (ptr = my_var.get("backtrace")) {
		if (strcmp(ptr, "PODEM") == 0) {
			set_backtrace_mode(BT_PODEM);
		} else if (strcmp(ptr, "D") == 0) {
			set_backtrace_mode(BT_D);
		} else {
			fprintf(cur_fp2, "backtrace: illegal variable value %s\n", ptr);
		}
	}

	/* set learning flag */
	learn_flag = my_var.chk("learning");

	/* set dynamic implication flag */
	if (ptr = my_var.get("dyn_imp")) {
		if (strcmp(ptr, "single") == 0) {
			dyn_mode = DYN_SINGLE;
		} else if (strcmp(ptr, "all") == 0) {
			dyn_mode = DYN_ALL;
		} else {
			fprintf(cur_fp2, "dyn_imp: illegal variable value %s\n", ptr);
		}
	} else {
		dyn_mode = DYN_NONE;
	}

	/* set unique sensitization mode */
	if (ptr = my_var.get("usense")) {
		us_mode = atoi(ptr);
	} else {
		us_mode = 2;
	}

	/* set fault-scoping flag */
	scope_flag = my_var.chk("scoping");

	/* set backtrack limit */
	if (my_var.chk("backtrack_limit") == true
	 && my_var.get("backtrack_limit") != NULL) {
		set_backtrack_limit(atoi(my_var.get("backtrack_limit")));
	} else {
		set_default_backtrack_limit();
	}

	for (int i = 1; i < argc; i ++) {
		if (strcmp(argv[i], "-bt_D") == 0) {
			set_backtrace_mode(BT_D);
		} else if (strcmp(argv[i], "-bt_PODEM") == 0) {
			set_backtrace_mode(BT_PODEM);
		} else if (strcmp(argv[i], "-fsim_single") == 0) {
			fsim_mode = FSIM_SINGLE;
		} else if (strcmp(argv[i], "-fsim_para") == 0) {
			fsim_mode = FSIM_PARALLEL;
		} else if (strcmp(argv[i], "-nofsim") == 0) {
			fsim_mode = FSIM_NONE;
		} else if (strcmp(argv[i], "-learn") == 0) {
			learn_flag = true;
		} else if (strcmp(argv[i], "-nolearn") == 0) {
			learn_flag = false;
		} else if (strcmp(argv[i], "-dyn_imp") == 0) {
			dyn_mode = DYN_ALL;
		} else if (strcmp(argv[i], "-nodyn_imp") == 0) {
			dyn_mode = DYN_NONE;
		} else if (strcmp(argv[i], "-dyn_imp1") == 0) {
			dyn_mode = DYN_SINGLE;
		} else if (strcmp(argv[i], "-scope") == 0) {
			scope_flag = true;
		} else if (strcmp(argv[i], "-noscope") == 0) {
			scope_flag = false;
		} else if (strcmp(argv[i], "-blim") == 0) {
			if (++ i >= argc) {
				fputs("dtpg: -blim backtrack limit not specified.\n", cur_fp2);
				return 0;
			}
			set_backtrack_limit(atoi(argv[i]));
		} else {
			fprintf(cur_fp2, "dtpg: illegal option %s\n", argv[i]);
			return 0;
		}
	}

	if (learn_flag == true) {
		learning();
		use_learning();
	} else {
		dont_use_learning();
	}

	dtpg(fsim_mode, dyn_mode, scope_flag);

	return 0;
}
