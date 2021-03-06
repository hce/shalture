/*
 * Copyright (c) 2005-2006 Atheme Development Group
 * Rights to this code are as documented in doc/LICENSE.
 *
 * Generic stuff from libathemecore.
 *
 */

#ifndef LIBATHEMECORE_H
#define LIBATHEMECORE_H

extern void shalture_bootstrap(void);
extern void shalture_init(char *execname, char *log_p);
extern void shalture_setup(void);

extern int shalture_main(int argc, char *argv[]);

#endif

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
