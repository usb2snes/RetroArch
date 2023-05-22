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

#include "emulator_network_access_defines.h"
#include "nwasupport.h"
#include "nwasupport/generic_poll_server.h"
#include "nwasupport/nwasupport.h"
#include "version.h"
#include "core_info.h"
#include "content.h"
#include "paths.h"
#include "retroarch.h"
#include "runloop.h"



#define RETROARCH_EVENT_STRING(cmd) {cmd, #cmd}

const cmd_event_string_map_entry_t cmd_event_string_map[] = {
   /* Resets RetroArch. */
   RETROARCH_EVENT_STRING(CMD_EVENT_RESET),
   RETROARCH_EVENT_STRING(CMD_EVENT_SET_PER_GAME_RESOLUTION),
   RETROARCH_EVENT_STRING(CMD_EVENT_SET_FRAME_LIMIT),
   /* Loads core. */
   RETROARCH_EVENT_STRING(CMD_EVENT_LOAD_CORE),
   RETROARCH_EVENT_STRING(CMD_EVENT_LOAD_CORE_PERSIST),
#if defined(HAVE_RUNAHEAD) && (defined(HAVE_DYNAMIC) || defined(HAVE_DYLIB))
   RETROARCH_EVENT_STRING(CMD_EVENT_LOAD_SECOND_CORE),
#endif
   RETROARCH_EVENT_STRING(CMD_EVENT_UNLOAD_CORE),
   /* Closes content. */
   RETROARCH_EVENT_STRING(CMD_EVENT_CLOSE_CONTENT),
   /* Swaps the current state with what's on the undo load buffer. */
   RETROARCH_EVENT_STRING(CMD_EVENT_UNDO_LOAD_STATE),
   /* Rewrites a savestate on disk. */
   RETROARCH_EVENT_STRING(CMD_EVENT_UNDO_SAVE_STATE),
   /* Save state hotkeys. */
   RETROARCH_EVENT_STRING(CMD_EVENT_LOAD_STATE),
   RETROARCH_EVENT_STRING(CMD_EVENT_SAVE_STATE),
   RETROARCH_EVENT_STRING(CMD_EVENT_SAVE_STATE_DECREMENT),
   RETROARCH_EVENT_STRING(CMD_EVENT_SAVE_STATE_INCREMENT),
   /* Save state actions. */
   RETROARCH_EVENT_STRING(CMD_EVENT_SAVE_STATE_TO_RAM),
   RETROARCH_EVENT_STRING(CMD_EVENT_LOAD_STATE_FROM_RAM),
   RETROARCH_EVENT_STRING(CMD_EVENT_RAM_STATE_TO_FILE),
   /* Takes screenshot. */
   RETROARCH_EVENT_STRING(CMD_EVENT_TAKE_SCREENSHOT),
   /* Quits RetroArch. */
   RETROARCH_EVENT_STRING(CMD_EVENT_QUIT),
   /* Reinitialize all drivers. */
   RETROARCH_EVENT_STRING(CMD_EVENT_REINIT_FROM_TOGGLE),
   /* Reinitialize all drivers. */
   RETROARCH_EVENT_STRING(CMD_EVENT_REINIT),
   /* Toggles cheevos hardcore mode. */
   RETROARCH_EVENT_STRING(CMD_EVENT_CHEEVOS_HARDCORE_MODE_TOGGLE),
   /* Deinitialize rewind. */
   RETROARCH_EVENT_STRING(CMD_EVENT_REWIND_DEINIT),
   /* Initializes rewind. */
   RETROARCH_EVENT_STRING(CMD_EVENT_REWIND_INIT),
   /* Toggles rewind. */
   RETROARCH_EVENT_STRING(CMD_EVENT_REWIND_TOGGLE),
   /* Initializes autosave. */
   RETROARCH_EVENT_STRING(CMD_EVENT_AUTOSAVE_INIT),
   /* Stops audio. */
   RETROARCH_EVENT_STRING(CMD_EVENT_AUDIO_STOP),
   /* Starts audio. */
   RETROARCH_EVENT_STRING(CMD_EVENT_AUDIO_START),
   /* Mutes audio. */
   RETROARCH_EVENT_STRING(CMD_EVENT_AUDIO_MUTE_TOGGLE),
   /* Volume adjustments. */
   RETROARCH_EVENT_STRING(CMD_EVENT_VOLUME_UP),
   RETROARCH_EVENT_STRING(CMD_EVENT_VOLUME_DOWN),
   RETROARCH_EVENT_STRING(CMD_EVENT_MIXER_VOLUME_UP),
   RETROARCH_EVENT_STRING(CMD_EVENT_MIXER_VOLUME_DOWN),
   /* Toggles FPS counter. */
   RETROARCH_EVENT_STRING(CMD_EVENT_FPS_TOGGLE),
   /* Toggles statistics display. */
   RETROARCH_EVENT_STRING(CMD_EVENT_STATISTICS_TOGGLE),
   /* Initializes overlay. */
   RETROARCH_EVENT_STRING(CMD_EVENT_OVERLAY_INIT),
   /* Deinitializes overlay. */
   RETROARCH_EVENT_STRING(CMD_EVENT_OVERLAY_DEINIT),
   /* Sets current scale factor for overlay. */
   RETROARCH_EVENT_STRING(CMD_EVENT_OVERLAY_SET_SCALE_FACTOR),
   /* Sets current alpha modulation for overlay. */
   RETROARCH_EVENT_STRING(CMD_EVENT_OVERLAY_SET_ALPHA_MOD),
   /* Sets diagonal sensitivities of overlay eightway areas. */
   RETROARCH_EVENT_STRING(CMD_EVENT_OVERLAY_SET_EIGHTWAY_DIAGONAL_SENSITIVITY),
   /* Deinitializes overlay. */
   RETROARCH_EVENT_STRING(CMD_EVENT_DSP_FILTER_INIT),
   /* Initializes recording system. */
   RETROARCH_EVENT_STRING(CMD_EVENT_RECORD_INIT),
   /* Deinitializes recording system. */
   RETROARCH_EVENT_STRING(CMD_EVENT_RECORD_DEINIT),
   /* Deinitializes history playlist. */
   RETROARCH_EVENT_STRING(CMD_EVENT_HISTORY_DEINIT),
   /* Initializes history playlist. */
   RETROARCH_EVENT_STRING(CMD_EVENT_HISTORY_INIT),
   /* Deinitializes core information. */
   RETROARCH_EVENT_STRING(CMD_EVENT_CORE_INFO_DEINIT),
   /* Initializes core information. */
   RETROARCH_EVENT_STRING(CMD_EVENT_CORE_INFO_INIT),
   /* Deinitializes core. */
   RETROARCH_EVENT_STRING(CMD_EVENT_CORE_DEINIT),
   /* Initializes core. */
   RETROARCH_EVENT_STRING(CMD_EVENT_CORE_INIT),
   /* Apply video state changes. */
   RETROARCH_EVENT_STRING(CMD_EVENT_VIDEO_APPLY_STATE_CHANGES),
   /* Set video blocking state. */
   RETROARCH_EVENT_STRING(CMD_EVENT_VIDEO_SET_BLOCKING_STATE),
   /* Sets current aspect ratio index. */
   RETROARCH_EVENT_STRING(CMD_EVENT_VIDEO_SET_ASPECT_RATIO),
   /* Restarts RetroArch. */
   RETROARCH_EVENT_STRING(CMD_EVENT_RESTART_RETROARCH),
   /* Shutdown the OS */
   RETROARCH_EVENT_STRING(CMD_EVENT_SHUTDOWN),
   /* Reboot the OS */
   RETROARCH_EVENT_STRING(CMD_EVENT_REBOOT),
   /* Resume RetroArch when in menu. */
   RETROARCH_EVENT_STRING(CMD_EVENT_RESUME),
   /* Add a playlist entry to favorites. */
   RETROARCH_EVENT_STRING(CMD_EVENT_ADD_TO_FAVORITES),
   /* Reset playlist entry associated core to DETECT */
   RETROARCH_EVENT_STRING(CMD_EVENT_RESET_CORE_ASSOCIATION),
   /* Toggles pause. */
   RETROARCH_EVENT_STRING(CMD_EVENT_PAUSE_TOGGLE),
   /* Pauses RetroArch. */
   RETROARCH_EVENT_STRING(CMD_EVENT_MENU_PAUSE_LIBRETRO),
   RETROARCH_EVENT_STRING(CMD_EVENT_PAUSE),
   /* Unpauses RetroArch. */
   RETROARCH_EVENT_STRING(CMD_EVENT_UNPAUSE),
   /* Toggles menu on/off. */
   RETROARCH_EVENT_STRING(CMD_EVENT_MENU_TOGGLE),
   /* Configuration saving. */
   RETROARCH_EVENT_STRING(CMD_EVENT_MENU_RESET_TO_DEFAULT_CONFIG),
   RETROARCH_EVENT_STRING(CMD_EVENT_MENU_SAVE_CURRENT_CONFIG),
   RETROARCH_EVENT_STRING(CMD_EVENT_MENU_SAVE_CURRENT_CONFIG_OVERRIDE_CORE),
   RETROARCH_EVENT_STRING(CMD_EVENT_MENU_SAVE_CURRENT_CONFIG_OVERRIDE_CONTENT_DIR),
   RETROARCH_EVENT_STRING(CMD_EVENT_MENU_SAVE_CURRENT_CONFIG_OVERRIDE_GAME),
   RETROARCH_EVENT_STRING(CMD_EVENT_MENU_SAVE_CONFIG),
   /* Applies shader changes. */
   RETROARCH_EVENT_STRING(CMD_EVENT_SHADERS_APPLY_CHANGES),
   /* A new shader preset has been loaded */
   RETROARCH_EVENT_STRING(CMD_EVENT_SHADER_PRESET_LOADED),
   /* Shader hotkeys. */
   RETROARCH_EVENT_STRING(CMD_EVENT_SHADER_NEXT),
   RETROARCH_EVENT_STRING(CMD_EVENT_SHADER_PREV),
   RETROARCH_EVENT_STRING(CMD_EVENT_SHADER_TOGGLE),
   /* Apply cheats. */
   RETROARCH_EVENT_STRING(CMD_EVENT_CHEATS_APPLY),
   /* Cheat hotkeys. */
   RETROARCH_EVENT_STRING(CMD_EVENT_CHEAT_TOGGLE),
   RETROARCH_EVENT_STRING(CMD_EVENT_CHEAT_INDEX_PLUS),
   RETROARCH_EVENT_STRING(CMD_EVENT_CHEAT_INDEX_MINUS),
   /* Initializes network system. */
   RETROARCH_EVENT_STRING(CMD_EVENT_NETWORK_INIT),
   /* Initializes netplay system with a string or no host specified. */
   RETROARCH_EVENT_STRING(CMD_EVENT_NETPLAY_INIT),
   /* Initializes netplay system with a direct host specified. */
   RETROARCH_EVENT_STRING(CMD_EVENT_NETPLAY_INIT_DIRECT),
   /* Initializes netplay system with a direct host specified after loading content. */
   RETROARCH_EVENT_STRING(CMD_EVENT_NETPLAY_INIT_DIRECT_DEFERRED),
   /* Deinitializes netplay system. */
   RETROARCH_EVENT_STRING(CMD_EVENT_NETPLAY_DEINIT),
   /* Switch between netplay gaming and watching. */
   RETROARCH_EVENT_STRING(CMD_EVENT_NETPLAY_GAME_WATCH),
   /* Open a netplay chat input menu. */
   RETROARCH_EVENT_STRING(CMD_EVENT_NETPLAY_PLAYER_CHAT),
   /* Toggle chat fading. */
   RETROARCH_EVENT_STRING(CMD_EVENT_NETPLAY_FADE_CHAT_TOGGLE),
   /* Start hosting netplay. */
   RETROARCH_EVENT_STRING(CMD_EVENT_NETPLAY_ENABLE_HOST),
   /* Disconnect from the netplay host. */
   RETROARCH_EVENT_STRING(CMD_EVENT_NETPLAY_DISCONNECT),
   /* Toggle ping counter. */
   RETROARCH_EVENT_STRING(CMD_EVENT_NETPLAY_PING_TOGGLE),
   /* Toggles netplay hosting. */
   RETROARCH_EVENT_STRING(CMD_EVENT_NETPLAY_HOST_TOGGLE),
   /* Reinitializes audio driver. */
   RETROARCH_EVENT_STRING(CMD_EVENT_AUDIO_REINIT),
   /* Resizes windowed scale. Will reinitialize video driver. */
   RETROARCH_EVENT_STRING(CMD_EVENT_RESIZE_WINDOWED_SCALE),
   /* Toggles disk eject. */
   RETROARCH_EVENT_STRING(CMD_EVENT_DISK_EJECT_TOGGLE),
   /* Cycle to next disk. */
   RETROARCH_EVENT_STRING(CMD_EVENT_DISK_NEXT),
   /* Cycle to previous disk. */
   RETROARCH_EVENT_STRING(CMD_EVENT_DISK_PREV),
   /* Switch to specified disk index */
   RETROARCH_EVENT_STRING(CMD_EVENT_DISK_INDEX),
   /* Appends disk image to disk image list. */
   RETROARCH_EVENT_STRING(CMD_EVENT_DISK_APPEND_IMAGE),
   /* Stops rumbling. */
   RETROARCH_EVENT_STRING(CMD_EVENT_RUMBLE_STOP),
   /* Toggles mouse grab. */
   RETROARCH_EVENT_STRING(CMD_EVENT_GRAB_MOUSE_TOGGLE),
   /* Toggles game focus. */
   RETROARCH_EVENT_STRING(CMD_EVENT_GAME_FOCUS_TOGGLE),
   /* Toggles desktop menu. */
   RETROARCH_EVENT_STRING(CMD_EVENT_UI_COMPANION_TOGGLE),
   /* Toggles fullscreen mode. */
   RETROARCH_EVENT_STRING(CMD_EVENT_FULLSCREEN_TOGGLE),
   /* Toggle recording. */
   RETROARCH_EVENT_STRING(CMD_EVENT_RECORDING_TOGGLE),
   /* Toggle streaming. */
   RETROARCH_EVENT_STRING(CMD_EVENT_STREAMING_TOGGLE),
   /* Toggle BSV recording. */
   RETROARCH_EVENT_STRING(CMD_EVENT_BSV_RECORDING_TOGGLE),
   /* Toggle Run-Ahead. */
   RETROARCH_EVENT_STRING(CMD_EVENT_RUNAHEAD_TOGGLE),
   /* Toggle Preemtive Frames. */
   RETROARCH_EVENT_STRING(CMD_EVENT_PREEMPT_TOGGLE),
   /* Deinitialize or Reinitialize Preemptive Frames. */
   RETROARCH_EVENT_STRING(CMD_EVENT_PREEMPT_UPDATE),
   /* Force Preemptive Frames to refill its state buffer. */
   RETROARCH_EVENT_STRING(CMD_EVENT_PREEMPT_RESET_BUFFER),
   /* Toggle VRR runloop. */
   RETROARCH_EVENT_STRING(CMD_EVENT_VRR_RUNLOOP_TOGGLE),
   /* AI service. */
   RETROARCH_EVENT_STRING(CMD_EVENT_AI_SERVICE_TOGGLE),
   RETROARCH_EVENT_STRING(CMD_EVENT_AI_SERVICE_CALL),
   /* Misc. */
   RETROARCH_EVENT_STRING(CMD_EVENT_SAVE_FILES),
   RETROARCH_EVENT_STRING(CMD_EVENT_CONTROLLER_INIT),
   RETROARCH_EVENT_STRING(CMD_EVENT_DISCORD_INIT),
   RETROARCH_EVENT_STRING(CMD_EVENT_PRESENCE_UPDATE),
   RETROARCH_EVENT_STRING(CMD_EVENT_OVERLAY_NEXT),
   RETROARCH_EVENT_STRING(CMD_EVENT_OSK_TOGGLE),
};

const unsigned int retroarch_event_number_of_command = sizeof(cmd_event_string_map) / sizeof(cmd_event_string_map[0]);
#undef RETROARCH_EVENT_STRING


#define start_hash_reply(S) generic_poll_server_start_hash_reply(S)
#define send_full_hash_reply(S, K, ...) generic_poll_server_send_full_hash_reply(S, K, __VA_ARGS__)
#define end_hash_reply(S) generic_poll_server_end_hash_reply(S)
#define send_hash_reply(S, K, ...) generic_poll_server_send_hash_reply(S, K, __VA_ARGS__)


const generic_emu_nwa_commands_map_t generic_emu_nwa_map = {
    {MY_NAME_IS, nwasupport_my_name_is},
    {EMULATOR_INFO, nwasupport_emulator_info},
    {EMULATION_STATUS, nwasupport_emulation_status},
    {GAME_INFO, nwasupport_game_info},
    {CORES_LIST, nwasupport_cores_list},
    {CORE_CURRENT_INFO, nwasupport_core_current_info},
    {CORE_INFO, nwasupport_core_info},
    {EMULATION_PAUSE, nwasupport_emulation_pause},
    {EMULATION_RESUME, nwasupport_emulation_resume},
    {EMULATION_RESET, nwasupport_emulation_reset},
    {LOAD_STATE, nwasupport_load_state},
    {SAVE_STATE, nwasupport_save_state}
};

const unsigned int generic_emu_nwa_map_size = 10;
const custom_emu_nwa_commands_map_t custom_emu_nwa_map = {
   {"RETROARCH_EVENT_COMMAND", nwasupport_retroarch_event_command},
   {"RETROARCH_READ_CORE_MEMORY", nwasupport_retroarch_read_core_memory},
   {"bRETROARCH_CORE_WRITE_MEMORY", nwasupport_retroarch_core_write_memory}
};
const unsigned int custom_emu_nwa_map_size = 3;


static void generic_poll_server_write_function(SOCKET socket, char* data, unsigned data_size)
{

}

static char*	hexString(const char* str, const unsigned int size)
{
    char* toret = malloc(size * 3 + 1);

    unsigned int i;
    for (i = 0; i < size; i++)
    {
        sprintf(toret + i * 3, "%02X ", (unsigned char) str[i]);
    }
    toret[size * 3] = 0;
    return toret;
}


#define SKARSNIK_DEBUG 1

#include "nwasupport/generic_poll_server.c"



static void nwa_support_poll(command_t* command)
{
   generic_poll_server_poll_code(0);
}

static command_t* m_command = NULL;


// Loading a content seems to restart all of this, this is not acceptable
static void nwa_support_destroy(command_t* command)
{
   return ;
   free(command);
   generic_poll_server_close();
   m_command = NULL;
}

static void nwa_support_replier(command_t* command, const char* data, size_t len)
{

}

command_t*  nwa_support_new()
{
   if (m_command != NULL)
      return m_command;
   command_t* command = (command_t*) malloc(sizeof(command_t));
   command->poll = nwa_support_poll;
   command->destroy =  nwa_support_destroy;
   command->replier = nwa_support_replier;
   m_command = command;
   generic_poll_server_start(0);
   return command;
}

int64_t nwasupport_my_name_is(SOCKET sock, char** arg, int ac)
{
   if (ac != 1)
   {
      send_error(sock, invalid_argument, "MY_NAME_IS expect 1 argument <client-name>");
      return 0;
   }
   send_full_hash_reply(sock, 1, "name", arg[0]);
   return 0;
}

int64_t nwasupport_emulator_info(SOCKET sock, char** ag, int ac)
{
    send_full_hash_reply(sock, 5,
                            "name", "RetroArch",
                            "version", PACKAGE_VERSION,
                            "id", "Too fix",
                            "nwa_version", "1.0",
                            "commands", "EMULATOR_INFO,EMULATION_STATUS,GAME_INFO,CORES_LIST,CORE_CURRENT_INFO,CORE_INFO,MY_NAME_IS,"
                            "EMULATION_PAUSE,EMULATION_RESUME,EMULATION_RESET,"
                            "LOAD_STATE,SAVE_STATE,"
                            "RETROARCH_EVENT_COMMAND,RETROARCH_READ_CORE_MEMORY,bRETROARCH_CORE_WRITE_MEMORY");
     return 0;
}

int64_t nwasupport_emulation_status(SOCKET sock, char** ag, int ac)
{
   uint8_t flags = content_get_flags();
   if (flags & CONTENT_ST_FLAG_IS_INITED)
   {
      /* add some content info */
      runloop_state_t *runloop_st = runloop_state_get_ptr();
      const char *status          = "running";
      const char *content_name    = path_basename(path_get(RARCH_PATH_BASENAME));  /* filename only without ext */
      if (runloop_st->flags & RUNLOOP_FLAG_PAUSED)
         status                   = "paused";
      send_full_hash_reply(sock, 2,
                           "state", status,
                           "game", content_name);
   }
   else
       send_full_hash_reply(sock, 1,
                           "state", "no_game");
   return 0;
}

int64_t nwasupport_game_info(SOCKET sock, char** ag, int ac)
{
   uint8_t flags = content_get_flags();
   if (flags & CONTENT_ST_FLAG_IS_INITED)
   {
      const char *content_name    = path_basename(path_get(RARCH_PATH_BASENAME));  /* filename only without ext */
      char crcbuff[255];
      snprintf(crcbuff, 255, "%d", content_get_crc());
      send_full_hash_reply(sock, 3,
                           "name", content_name,
                           "file", path_get(RARCH_PATH_BASENAME),
                           "crc", crcbuff);
   }
   else
       send_error(sock, command_error, "No game running");
   return 0;
}

int64_t nwasupport_cores_list(SOCKET sock, char** arg, int ac)
{
   core_info_list_t*    core_list;
   size_t               core_number;
   bool                 all_core = true;

   if (!core_info_get_list(&core_list))
   {
      send_error(sock, command_error, "Can't get core list");
      return 0;
   }
   if (ac == 1)
      all_core = false;
   //core_number = core_info_count();
   start_hash_reply(sock);
   for (unsigned int i = 0; i < core_list->count; i++)
   {
      core_info_t* core_info = core_info_get(core_list, i);
      if (all_core || strncmp(core_info->systemname, arg[0], strlen(arg[0])) == 0)
      {
         send_hash_reply(sock, 2, "name", core_info->core_name,
                               "platform", core_info->systemname);
      }
   }
   end_hash_reply(sock);
   return 0;
}

static char* nwaplatform_name(const char* str)
{
   if (strcmp(str, "Super Nintendo Entertainment System") == 0)
      return "SNES";
}

static void nwasupport_send_core_info(SOCKET sock, core_info_t* core_info)
{
   s_debug("SE : %s\n", core_info->supported_extensions);
   char* supported_extensions = core_info->supported_extensions;
   if (core_info->supported_extensions == NULL)
      supported_extensions = "none";
   send_full_hash_reply(sock, 9, "name", core_info->core_name,
                                  "display_name", core_info->display_name,
                                  "platform", nwaplatform_name(core_info->systemname),
                                  "system_name", core_info->systemname,
                                  "supported_extensions", supported_extensions,
                                  "version", core_info->display_version,
                                  "authors", core_info->authors,
                                  "licences", core_info->licenses,
                                  "has_savestate", core_info->savestate_support_level > CORE_INFO_SAVESTATE_DISABLED ? "true" : "false",
                                  "description", core_info->description
                                  );
}

int64_t nwasupport_core_info(SOCKET sock, char** arg, int ac)
{
   if (ac != 1)
   {
      send_error(sock, invalid_argument, "CORE_INFO need only one argument: <core_name>");
      return 0;
   }
   core_info_list_t*    core_list;
   size_t               core_number;

   if (!core_info_get_list(&core_list))
   {
      send_error(sock, command_error, "Can't get core list");
      return 0;
   }
   for (unsigned int i = 0; i < core_list->count; i++)
   {
      core_info_t* core_info = core_info_get(core_list, i);
      if (strncmp(core_info->core_name, arg[0], strlen(core_info->core_name)) == 0)
      {
         s_debug("Core name:%s\n", core_info->core_name);
         nwasupport_send_core_info(sock, core_info);
         return 0;
      }
   }
   send_error(sock, invalid_argument, "No core found with the name specified");
   return  0;
}

int64_t nwasupport_core_current_info(SOCKET sock, char** arg, int ac)
{
   core_info_t* core_info;
   bool success = core_info_get_current_core(&core_info);
   if (!success || core_info == NULL)
   {
      send_error(sock, command_error, "No current core loaded");
      return 0;
   }
   nwasupport_send_core_info(sock, core_info);
   return 0;
}

int64_t nwasupport_emulation_pause(SOCKET sock, char** arg, int ac)
{
   if (!command_event(CMD_EVENT_PAUSE, NULL))
   {
      send_error(sock, command_error, "Failed to pause the emulator");
   } else {
      write(sock, "\n\n", 2);
   }
   return 0;
}

int64_t nwasupport_emulation_resume(SOCKET sock, char** arg, int ac)
{
   if (!command_event(CMD_EVENT_UNPAUSE, NULL))
   {
      send_error(sock, command_error, "Failed to unpause the emulator");
   } else {
      write(sock, "\n\n", 2);
   }
   return 0;
}

int64_t nwasupport_emulation_reset(SOCKET sock, char** arg, int ac)
{
   if (!command_event(CMD_EVENT_RESET, NULL))
   {
      send_error(sock, command_error, "Failed to pause the emulator");
   } else {
      write(sock, "\n\n", 2);
   }
   return 0;
}

int64_t nwasupport_load_state(SOCKET sock, char** arg, int ac)
{
   if (!content_load_state(arg[0], false, false))
   {
      send_error(sock, command_error, "Failed to load the savestate");
   } else {
      write(sock, "\n\n", 2);
   }
   return 0;
}

int64_t nwasupport_save_state(SOCKET sock, char** arg, int ac)
{
   if (!content_save_state(arg[0], true, false))
   {
      send_error(sock, command_error, "Failed to save the savestate");
   } else {
      write(sock, "\n\n", 2);
   }
   return 0;
}


int64_t nwasupport_retroarch_event_command(SOCKET sock, char** arg, int ac)
{
   char* event_arg = NULL;
   if (ac == 1)
      event_arg = arg[0];
   enum event_command cmd = CMD_EVENT_NONE;
   for (unsigned int i = 0; i < retroarch_event_number_of_command; i++)
   {
      if (strncmp(arg[0], cmd_event_string_map[i].string, strlen(cmd_event_string_map[i].string)) == 0)
      {
         cmd = cmd_event_string_map[i].event;
         break;
      }
   }
   if (cmd == CMD_EVENT_NONE)
   {
      send_error(sock, command_error, "Invalid command event given");
      return 0;
   }
   if (!command_event(cmd, event_arg))
   {
      send_error(sock, command_error, "Failed to execute the command event");
   } else {
      write(sock, "\n\n", 2);
   }
   return 0;
}

static const rarch_memory_descriptor_t* command_memory_get_descriptor(const rarch_memory_map_t* mmap, unsigned address, size_t* offset)
{
   const rarch_memory_descriptor_t* desc = mmap->descriptors;
   const rarch_memory_descriptor_t* end  = desc + mmap->num_descriptors;

   for (; desc < end; desc++)
   {
      if (desc->core.select == 0)
      {
         /* if select is 0, attempt to explicitly match the address */
         if (address >= desc->core.start && address < desc->core.start + desc->core.len)
         {
            *offset = address - desc->core.start;
            return desc;
         }
      }
      else
      {
         /* otherwise, attempt to match the address by matching the select bits */
         if (((desc->core.start ^ address) & desc->core.select) == 0)
         {
            /* adjust the address to the start of the descriptor */
            unsigned desc_offset = address - (unsigned)desc->core.start;

            /* address is unsigned. we only need that much of the disconnect mask */
            unsigned mask = (unsigned)desc->core.disconnect;

            /* this magic logic is copied from mmap_reduce. it removes any bits from
             * address that are non-zero in the disconnect field. bits above the
             * removed bits are shifted down to fill the gap. */
            while (mask)
            {
               const unsigned tmp = (mask - 1) & ~mask;
               desc_offset = (desc_offset & tmp) | ((desc_offset >> 1) & ~tmp);
               mask = (mask & (mask - 1)) >> 1;
            }

            /* we've calculated the actual offset of the data within the descriptor */
            *offset = desc_offset;

            /* sanity check - make sure the descriptor is large enough to hold the target address */
            if (desc_offset < desc->core.len)
               return desc;
         }
      }
   }

   return NULL;
}

int64_t nwasupport_retroarch_read_core_memory(SOCKET sock, char** arg, int ac)
{
   if (ac != 2)
   {
      send_error(sock, invalid_argument, "RETROARCH_READ_CORE_MEMORY need 2 arguments <address>;<size>");
      return 0;
   }
   char*    end_strtoll;
   size_t   address;
   size_t   size;

   address = generic_poll_server_get_offset(arg[0]);
   size = strtoll(arg[1], &end_strtoll, 10);
   if (arg[1] == end_strtoll)
   {
      send_error(sock, invalid_argument, "RETROARCH_CORE_READ_MEMORY, invalid first argument, no number found");
      return 0;
   }
   runloop_state_t *runloop_st       = runloop_state_get_ptr();
   const rarch_system_info_t* system = &runloop_st->system;

   if (!system || system->mmaps.num_descriptors == 0)
   {
      send_error(sock, command_error, "No memory map defined");
      return 0;
   }
   size_t offset;
   const rarch_memory_descriptor_t* desc = command_memory_get_descriptor(&system->mmaps, address, &offset);
   if (!desc)
   {
      send_error(sock, command_error, "No description for address");
   } else if (!desc->core.ptr)
   {
      send_error(sock, command_error, "No data for descriptor");
   } else {
      size_t sent_size = MIN((unsigned int)(desc->core.len - offset), size);
      char header[5];
      header[0] = 0;
      uint32_t network_size = htonl(sent_size);
      memcpy(header + 1, (void*)&network_size, 4);
      write(sock, header, 5);
      write(sock, (uint8_t*)desc->core.ptr + desc->core.offset + offset, sent_size);
   }
   return 0;
}

int64_t nwasupport_retroarch_core_write_memory(SOCKET sock, char** arg, int ac)
{
   return 0;
}


