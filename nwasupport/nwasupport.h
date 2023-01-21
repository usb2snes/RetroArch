#ifndef H_NWA_SUPPORT_H
#define H_NWA_SUPPORT_H

#include "command.h"

#ifdef _WIN32
#include <winsock2.h>
#else
typedef int SOCKET;
#endif

int64_t nwasupport_emu_info(SOCKET sock, char** ag, int ac);
command_t* nwa_support_new();

#endif
