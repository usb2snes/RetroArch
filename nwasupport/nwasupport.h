/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2021 - Daniel De Matteis
 *  Copyright (C) 2012-2015 - Michael Lelli
 *  Copyright (C) 2014-2017 - Jean-Andr� Santoni
 *  Copyright (C) 2016-2019 - Brad Parker
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef H_NWA_SUPPORT_H
#define H_NWA_SUPPORT_H

#include "command.h"

#ifdef _WIN32
#include <winsock2.h>
#else
typedef int SOCKET;
#endif

int64_t nwasupport_my_name_is(SOCKET sock, char** arg, int ac);
int64_t nwasupport_emulator_info(SOCKET sock, char** arg, int ac);
int64_t nwasupport_emulation_status(SOCKET sock, char** arg, int ac);
int64_t nwasupport_game_info(SOCKET sock, char** arg, int ac);
int64_t nwasupport_cores_list(SOCKET sock, char** arg, int ac);
int64_t nwasupport_core_info(SOCKET sock, char** arg, int ac);
int64_t nwasupport_core_current_info(SOCKET sock, char** arg, int ac);
int64_t nwasupport_emulation_pause(SOCKET sock, char** arg, int ac);
int64_t nwasupport_emulation_resume(SOCKET sock, char** arg, int ac);
int64_t nwasupport_emulation_reset(SOCKET sock, char** arg, int ac);
int64_t nwasupport_save_state(SOCKET sock, char** arg, int ac);
int64_t nwasupport_load_state(SOCKET sock, char** arg, int ac);
int64_t nwasupport_retroarch_event_command(SOCKET sock, char** arg, int ac);
int64_t nwasupport_retroarch_read_core_memory(SOCKET sock, char** arg, int ac);
int64_t nwasupport_retroarch_core_write_memory(SOCKET sock, char** arg, int ac);

command_t* nwa_support_new();


typedef struct {
      enum event_command   event;
      const char*          string;
} cmd_event_string_map_entry_t;

#endif
