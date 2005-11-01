/*
 * Copyright (c) 2005 Alex Lambert
 * Rights to this code are as documented in doc/LICENSE.
 *
 * Implements USERSERV RETURN.
 *
 * $Id: return.c 3385 2005-11-01 09:21:18Z pfish $
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"userserv/return", FALSE, _modinit, _moddeinit,
	"$Id: return.c 3385 2005-11-01 09:21:18Z pfish $",
	"Atheme Development Group <http://www.atheme.org>"
);

static void us_cmd_return(char *origin);

command_t us_return = { "RETURN", "Returns a nickname to its owner.",
			AC_IRCOP, us_cmd_return };

list_t *us_cmdtree, *us_helptree;

void _modinit(module_t *m)
{
	us_cmdtree = module_locate_symbol("userserv/main", "us_cmdtree");
	us_helptree = module_locate_symbol("userserv/main", "us_helptree");
	command_add(&us_return, us_cmdtree);
	help_addentry(us_helptree, "RETURN", "help/nickserv/return", NULL);
}

void _moddeinit()
{
	command_delete(&us_return, us_cmdtree);
	help_delentry(us_helptree, "RETURN");
}

static void us_cmd_return(char *origin)
{
	user_t *u = user_find(origin);
	char *target = strtok(NULL, " ");
	char *newmail = strtok(NULL, " ");
	char *newpass;
	char oldmail[EMAILLEN];
	myuser_t *mu;

	if (u == NULL)
		return;

	if (!target || !newmail)
	{
		notice(usersvs.nick, origin, "Insufficient parameters for \2RETURN\2.");
		notice(usersvs.nick, origin, "Usage: RETURN <nickname> <e-mail address>");
		return;
	}

	if (!(mu = myuser_find(target)))
	{
		notice(usersvs.nick, origin, "\2%s\2 is not registered.", target);
		return;
	}

	if (is_sra(mu))
	{
		notice(usersvs.nick, origin, "\2%s\2 belongs to a services root administrator; it cannot be returned.", target);
		return;
	}

	if ((strlen(newmail) > 32) || !validemail(newmail))
	{
		notice(usersvs.nick, origin, "\2%s\2 is not a valid e-mail address.", newmail);
		return;
	}

	newpass = gen_pw(12);
	strlcpy(oldmail, mu->email, EMAILLEN);
	strlcpy(mu->email, newmail, EMAILLEN);

	if (!sendemail(u, EMAIL_SENDPASS, mu, newpass))
	{
		strlcpy(mu->email, oldmail, EMAILLEN);
		notice(usersvs.nick, origin, "Sending email failed, nickname \2%s\2 remains with \2%s\2.",
				mu->name, mu->email);
		return;
	}

	strlcpy(mu->pass, newpass, NICKLEN);
	free(newpass);

	/* prevents users from "stealing it back" in the event of a takeover */
	metadata_delete(mu, METADATA_USER, "private:verify:emailchg:key");
	metadata_delete(mu, METADATA_USER, "private:verify:emailchg:newemail");
	metadata_delete(mu, METADATA_USER, "private:verify:emailchg:timestamp");

	wallops("%s returned the nickname \2%s\2 to \2%s\2", origin, target, newmail);
	notice(usersvs.nick, origin, "The e-mail address for \2%s\2 has been set to \2%s\2",
						target, newmail);
	notice(usersvs.nick, origin, "A random password has been set; it has been sent to \2%s\2.",
						newmail);
}
