#ifndef IPC_H
#define IPC_H

#include <stddef.h>
#include <stdint.h>

typedef enum __ipcstat {
    g_ipc_success = 1,
    g_ipc_fatal = -1,
} ipc_stat;

ipc_stat ipc_init(void);
ipc_stat ipc_register(const char* name, int32_t id);
ipc_stat ipc_detach(void);
ipc_stat ipc_send(const char* target_name, int32_t target_id,
                  const char* data, size_t len);
ipc_stat ipc_receive(char* data, size_t* len);

#endif // IPC_H
