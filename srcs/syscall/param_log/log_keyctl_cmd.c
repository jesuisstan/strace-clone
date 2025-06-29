#include "param_log.h"

/* keyctl commands */
#define KEYCTL_GET_KEYRING_ID 0		  /* ask for a keyring's ID */
#define KEYCTL_JOIN_SESSION_KEYRING 1 /* join or start named session keyring */
#define KEYCTL_UPDATE 2				  /* update a key */
#define KEYCTL_REVOKE 3				  /* revoke a key */
#define KEYCTL_CHOWN 4				  /* set ownership of a key */
#define KEYCTL_SETPERM 5			  /* set perms on a key */
#define KEYCTL_DESCRIBE 6			  /* describe a key */
#define KEYCTL_CLEAR 7				  /* clear contents of a keyring */
#define KEYCTL_LINK 8				  /* link a key into a keyring */
#define KEYCTL_UNLINK 9				  /* unlink a key from a keyring */
#define KEYCTL_SEARCH 10			  /* search for a key in a keyring */
#define KEYCTL_READ 11				  /* read a key or keyring's contents */
#define KEYCTL_INSTANTIATE 12		  /* instantiate a partially constructed key */
#define KEYCTL_NEGATE 13			  /* negate a partially constructed key */
#define KEYCTL_SET_REQKEY_KEYRING 14  /* set default request-key keyring */
#define KEYCTL_SET_TIMEOUT 15		  /* set timeout on a key */
#define KEYCTL_ASSUME_AUTHORITY 16	  /* assume authority to instantiate key */
#define KEYCTL_GET_SECURITY 17		  /* get key security label */
#define KEYCTL_SESSION_TO_PARENT 18	  /* set my session keyring on my parent process */
#define KEYCTL_REJECT 19			  /* reject a partially constructed key */
#define KEYCTL_INSTANTIATE_IOV 20	  /* instantiate a partially constructed key */
#define KEYCTL_INVALIDATE 21		  /* invalidate a key */
#define KEYCTL_GET_PERSISTENT 22	  /* get a user's persistent keyring */

static const flag_str_t keyctl_cmds[] = {
	FLAG_STR(KEYCTL_GET_KEYRING_ID),
	FLAG_STR(KEYCTL_JOIN_SESSION_KEYRING),
	FLAG_STR(KEYCTL_UPDATE),
	FLAG_STR(KEYCTL_REVOKE),
	FLAG_STR(KEYCTL_CHOWN),
	FLAG_STR(KEYCTL_SETPERM),
	FLAG_STR(KEYCTL_DESCRIBE),
	FLAG_STR(KEYCTL_CLEAR),
	FLAG_STR(KEYCTL_LINK),
	FLAG_STR(KEYCTL_UNLINK),
	FLAG_STR(KEYCTL_SEARCH),
	FLAG_STR(KEYCTL_READ),
	FLAG_STR(KEYCTL_INSTANTIATE),
	FLAG_STR(KEYCTL_NEGATE),
	FLAG_STR(KEYCTL_SET_REQKEY_KEYRING),
	FLAG_STR(KEYCTL_SET_TIMEOUT),
	FLAG_STR(KEYCTL_ASSUME_AUTHORITY),
	FLAG_STR(KEYCTL_GET_SECURITY),
	FLAG_STR(KEYCTL_SESSION_TO_PARENT),
	FLAG_STR(KEYCTL_REJECT),
	FLAG_STR(KEYCTL_INSTANTIATE_IOV),
	FLAG_STR(KEYCTL_INVALIDATE),
	FLAG_STR(KEYCTL_GET_PERSISTENT),
};

int log_KEYCTL_CMD(uint64_t value)
{
	return option_log(value, keyctl_cmds, sizeof(keyctl_cmds) / sizeof(keyctl_cmds[0]), "KEYCTL_???");
}