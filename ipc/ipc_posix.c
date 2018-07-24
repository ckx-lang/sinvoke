#include "ipc.h"
#include <mqueue.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>

typedef enum __chknamestat {
    g_chkname_success = 1,
    g_chkname_error = -1,
    g_chkname_nonalpha = -2,
    g_chkname_exceed = -3
} chkname_stat;

static chkname_stat check_n_encode_ipc_name(const char* org_name, int32_t id,
                                            char* encoded_name) {
    size_t size = 0;
    while (*org_name != '\0') {
        if (!isalpha(*org_name))
            return g_chkname_nonalpha;
        if (size > 28) {
            return g_chkname_exceed;
        }
        *encoded_name = *org_name;
        ++encoded_name;
        ++org_name;
        ++size;
    }
    *(encoded_name) = id % 26 + 97;
    *(encoded_name+1) = id % 10 +48;
    *(encoded_name+2) = '\0';
    return g_chkname_success;
}

static bool bother_chkname(chkname_stat stat) {
    switch (stat) {
    case g_chkname_success:
        return true;
    case g_chkname_exceed:
        fprintf(stderr, "[!LOG] [IPC-POSIX] ipc name length exceeds.\n");
        break;
    case g_chkname_nonalpha:
        fprintf(stderr, "[!LOG] [IPC-POSIX] ipc contains non-alpha char.\n");
        break;
    case g_chkname_error:
    default:
        fprintf(stderr, "[!LOG] [IPC-POSIX] failed checking ipc name.\n");
    }
    return false;
}

ipc_stat ipc_init() {
    return g_ipc_success;
}

static bool registered = false;
static char self_encoded_name[32] = {0};
static mqd_t self_mqd;

ipc_stat ipc_register(const char *name, int32_t id) {
    if (registered) {
        fprintf(stderr, "[!LOG] [IPC-POSIX] self already registered.");
        return g_ipc_fatal;
    }

    chkname_stat stat0 = check_n_encode_ipc_name(name, id, self_encoded_name);
    if (!bother_chkname(stat0)) {
        return g_ipc_fatal;
    }

    self_mqd = mq_open(self_encoded_name, O_CREAT, 0600);
    if (self_mqd < 0) {
        fprintf(stderr, "[!LOG] [IPC-POSIX] failed creating message queue.\n");
        return g_ipc_fatal;
    }

    registered = true;
    return g_ipc_success;
}

ipc_stat ipc_send(const char *target_name, int32_t target_id,
                  const char *data, size_t len) {
    if (!registered) {
        fprintf(stderr, "![LOG] [IPC-POSIX] unregistered target cannot"
                        "send messages.\n");
        return g_ipc_fatal;
    }

    /// @todo not implemented.
    return g_ipc_fatal;
}
