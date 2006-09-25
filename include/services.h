/*
 * Copyright (C) 2005 William Pitcock, et al.
 * Rights to this code are as documented in doc/LICENSE.
 *
 * Data structures related to services psuedo-clients.
 *
 * $Id: services.h 6479 2006-09-25 16:41:02Z jilles $
 */

#ifndef SERVICES_H
#define SERVICES_H

typedef struct chansvs_ chansvs_t;
typedef struct globsvs_ globsvs_t;
typedef struct opersvs_ opersvs_t;
typedef struct memosvs_ memosvs_t;
typedef struct helpsvs_ helpsvs_t;
typedef struct nicksvs_ nicksvs_t;
typedef struct usersvs_ usersvs_t;
typedef struct saslsvs_ saslsvs_t;

/* don't forget to define the extern variables below,
   and then the real definitions in atheme.c ! */

/* core services */
struct chansvs_
{
  char *nick;                   /* the IRC client's nickname  */
  char *user;                   /* the IRC client's username  */
  char *host;                   /* the IRC client's hostname  */
  char *real;                   /* the IRC client's realname  */
  char *disp;			/* the IRC client's dispname  */

  boolean_t fantasy;		/* enable fantasy commands    */

  uint32_t ca_vop;		/* xop access levels */
  uint32_t ca_hop;
  uint32_t ca_aop;
  uint32_t ca_sop;

  boolean_t changets;		/* use TS to better deop people */

  service_t *me;                /* our user_t struct          */
};

struct globsvs_
{
  char *nick;
  char *user;
  char *host;
  char *real;
  char *disp;			/* the IRC client's dispname  */
   
  service_t *me;
};

struct opersvs_
{
  char *nick;
  char *user;
  char *host;
  char *real;
  char *disp;			/* the IRC client's dispname  */
   
  service_t *me;
};

struct memosvs_
{
  char   *nick;
  char   *user;
  char   *host;
  char   *real;
  char   *disp;

  service_t *me;
};

struct helpsvs_
{
  char   *nick;
  char   *user;
  char   *host;
  char   *real;
  char   *disp;

  service_t *me;
};

/* authentication services */
struct nicksvs_
{
  boolean_t  spam;
  boolean_t  no_nick_ownership;

  char   *nick;
  char   *user;
  char   *host;
  char   *real;
  char   *disp;			/* the IRC client's dispname  */

  service_t *me;
};

struct usersvs_
{
  char   *nick;
  char   *user;
  char   *host;
  char   *real;
  char   *disp;			/* the IRC client's dispname  */

  service_t *me;
};

struct saslsvs_
{
  list_t pending;

  char   *nick;
  char   *user;
  char   *host;
  char   *real;
  char   *disp;			/* the IRC client's dispname  */

  service_t *me;
};

/* help us keep consistent messages */
#define STR_INSUFFICIENT_PARAMS "Insufficient parameters for \2%s\2."
#define STR_INVALID_PARAMS "Invalid parameters for \2%s\2."

/* for command_fail() */
enum faultcode_
{
	fault_needmoreparams = 1,
	fault_badparams = 2,
	fault_nosuch_source = 3,
	fault_nosuch_target = 4,
	fault_authfail = 5,
	fault_noprivs = 6,
	fault_nosuch_key = 7,
	fault_alreadyexists = 8,
	fault_toomany = 9,
	fault_emailfail = 10,
	fault_notverified = 11,
	fault_nochange = 12,
	fault_already_authed = 13,
	fault_unimplemented = 14
};
typedef enum faultcode_ faultcode_t;

/* extern declarations */
E chansvs_t chansvs;
E globsvs_t globsvs;
E opersvs_t opersvs;
E memosvs_t memosvs;
E helpsvs_t helpsvs;
E nicksvs_t nicksvs;
E usersvs_t usersvs;
E saslsvs_t saslsvs;

/* servtree.c */
E service_t *fcmd_agent;

/* services.c */
E int authservice_loaded;

E int ban(user_t *source, channel_t *chan, user_t *target);
E int remove_ban_exceptions(user_t *source, channel_t *chan, user_t *target);
E void join(char *chan, char *nick);
E void joinall(char *name);
E void partall(char *name);
E void verbose(mychan_t *mychan, char *fmt, ...);
E void snoop(char *fmt, ...);
E void notice(char *from, char *to, char *message, ...);
E void command_fail(sourceinfo_t *si, faultcode_t code, const char *fmt, ...);
E void command_success_nodata(sourceinfo_t *si, const char *fmt, ...);
E void command_success_string(sourceinfo_t *si, const char *result, const char *fmt, ...);
E void verbose_wallops(char *, ...);

/* ptasks.c */
E void handle_topic(channel_t *, char *, time_t, char *);
E int floodcheck(user_t *, user_t *);

#endif
