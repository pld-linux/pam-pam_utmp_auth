/*
 * pam_utmp v1
 * (c) copyright 2003 wojtek kaniewski <wojtekka@irc.pl>
 * 
 * prosty modu³ pozwalaj±cy siê zalogowaæ bez has³a, je¶li u¿ytkownik jest
 * ju¿ wpisany do /var/run/utmp z konsoli. mo¿liwe, ¿e jest niebezpieczny,
 * dlatego nie gwarantujê niczego.
 *
 * kompilacja:
 *     gcc pam_utmp.c -o pam_utmp.so -shared
 *     cp pam_utmp.so /lib/security
 *
 * instalacja:
 *    dopisaæ na pocz±tku /etc/pam.d/login liniê:
 *    "auth    sufficient     /lib/security/pam_utmp.so"
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <stdarg.h>
#include <string.h>
#include <utmp.h>
#include <ctype.h>

#define PAM_SM_AUTH
#define PAM_MODULE_NAME "pam_wtmp"

#include <security/pam_modules.h>
#include <security/pam_misc.h>

static void _pam_log(int err, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	openlog(PAM_MODULE_NAME, LOG_PID, LOG_AUTH);
	vsyslog(err, fmt, ap);
	closelog();
	va_end(ap);
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	const char *user, *host, *tty;
	struct utmp *ut;

	if (pam_get_user(pamh, &user, NULL) != PAM_SUCCESS)
		return PAM_USER_UNKNOWN;

	if (pam_get_item(pamh, PAM_RHOST, (const void**) &host) != PAM_SUCCESS)
		return PAM_USER_UNKNOWN;

	if (pam_get_item(pamh, PAM_TTY, (const void**) &tty) != PAM_SUCCESS)
		return PAM_USER_UNKNOWN;
  
	if (!user || host || !tty)
		return PAM_AUTHTOK_ERR;

	while ((ut = getutent())) {
		if (ut->ut_type == USER_PROCESS && !strcmp(ut->ut_user, user) && ( !strncmp(ut->ut_line, "tty", 3) || !strncmp(ut->ut_line,"vc", 2) ) && isdigit(ut->ut_line[3]))
			return PAM_SUCCESS;
	}
    
	_pam_log(LOG_INFO, "user=%s, rhost=%s, tty=%s", user, host, tty);
	return PAM_AUTHTOK_ERR;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	return PAM_SUCCESS;
}
       
