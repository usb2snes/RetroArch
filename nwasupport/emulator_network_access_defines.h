/*
 * Copyright (c) 2022 Sylvain "Skarsnik" Colinet.
 *
 * This file is orignaly part of the NWA project.
 * (see https://github.com/usb2snes/emulator-networkaccess).
 * Integrated in the RetroArch project
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

#ifndef H_EMULATOR_NETWORK_ACCESS_DEFINES_H
#define H_EMULATOR_NETWORK_ACCESS_DEFINES_H

const unsigned int EMULATOR_NETWORK_ACCESS_STARTING_PORT = 0xBEEF;

typedef enum  {
    EMULATOR_INFO = 1,
    EMULATION_STATUS = 2,
    EMULATION_PAUSE = 3,
    EMULATION_STOP = 4,
    EMULATION_RESET = 5,
    EMULATION_RESUME = 6,
    EMULATION_RELOAD = 7,

    MY_NAME_IS = 8,

    LOAD_GAME = 9,
    GAME_INFO = 10,

    CORES_LIST = 11,
    CORE_INFO = 12,
    CORE_CURRENT_INFO = 13,
    LOAD_CORE = 14,
    CORE_MEMORIES = 15,
    CORE_READ = 16,
    bCORE_WRITE = 17,

    DEBUG_BREAK = 18,
    DEBUG_CONTINUE = 19,

    LOAD_STATE = 20,
    SAVE_STATE = 21,

    CUSTOM = 128
} emulator_network_access_command;

#define EMU_NWA_COMMAND_STRING(cmd) {cmd, #cmd}

typedef struct {
    emulator_network_access_command command;
    const char* string;
} emulator_network_access_command_string_entry;

const emulator_network_access_command_string_entry emulator_network_access_command_strings[] = {
    EMU_NWA_COMMAND_STRING(EMULATOR_INFO),
    EMU_NWA_COMMAND_STRING(EMULATION_STATUS),
    EMU_NWA_COMMAND_STRING(EMULATION_PAUSE),
    EMU_NWA_COMMAND_STRING(EMULATION_STOP),
    EMU_NWA_COMMAND_STRING(EMULATION_RESET),
    EMU_NWA_COMMAND_STRING(EMULATION_RESUME),
    EMU_NWA_COMMAND_STRING(EMULATION_RELOAD),
    EMU_NWA_COMMAND_STRING(MY_NAME_IS),
    EMU_NWA_COMMAND_STRING(LOAD_GAME),
    EMU_NWA_COMMAND_STRING(GAME_INFO),
    EMU_NWA_COMMAND_STRING(CORES_LIST),
    EMU_NWA_COMMAND_STRING(CORE_INFO),
    EMU_NWA_COMMAND_STRING(CORE_CURRENT_INFO),
    EMU_NWA_COMMAND_STRING(LOAD_CORE),
    EMU_NWA_COMMAND_STRING(CORE_MEMORIES),
    EMU_NWA_COMMAND_STRING(CORE_READ),
    EMU_NWA_COMMAND_STRING(bCORE_WRITE),
    EMU_NWA_COMMAND_STRING(DEBUG_BREAK),
    EMU_NWA_COMMAND_STRING(DEBUG_CONTINUE),
    EMU_NWA_COMMAND_STRING(LOAD_STATE),
    EMU_NWA_COMMAND_STRING(SAVE_STATE)
};

const unsigned int emulator_network_access_number_of_command = sizeof(emulator_network_access_command_strings) / sizeof(emulator_network_access_command_strings[0]);

#undef EMU_NWA_COMMAND_STRING

typedef enum {
    protocol_error,
    invalid_command,
    invalid_argument,
    command_error,
    not_allowed
} emulator_network_access_error_type;

typedef struct {
    emulator_network_access_error_type error_type;
    const char* string;
} emulator_network_access_error_type_string_entry;

#define EMU_NWA_ERROR_STRING(cmd) {cmd, #cmd}

const emulator_network_access_error_type_string_entry emulator_network_access_error_type_strings[] = {
    EMU_NWA_ERROR_STRING(protocol_error),
    EMU_NWA_ERROR_STRING(invalid_command),
    EMU_NWA_ERROR_STRING(invalid_argument),
    EMU_NWA_ERROR_STRING(command_error),
    EMU_NWA_ERROR_STRING(not_allowed)
};

#undef EMU_NWA_ERROR_STRING
#endif
