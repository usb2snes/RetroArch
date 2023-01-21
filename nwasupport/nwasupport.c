#include "nwasupport/generic_poll_server.h"
#include "nwasupport/nwasupport.h"


#define start_hash_reply(S) generic_poll_server_start_hash_reply(S)
#define send_full_hash_reply(S, K, ...) generic_poll_server_send_full_hash_reply(S, K, __VA_ARGS__)
#define end_hash_reply(S) generic_poll_server_end_hash_reply(S)
#define send_hash_reply(S, K, ...) generic_poll_server_send_hash_reply(S, K, __VA_ARGS__)


const generic_emu_nwa_commands_map_t generic_emu_nwa_map = {
    {EMULATOR_INFO, nwasupport_emu_info}
};

const unsigned int generic_emu_nwa_map_size = 1;
const custom_emu_nwa_commands_map_t custom_emu_nwa_map = {};
const unsigned int custom_emu_nwa_map_size = 0;


static void generic_poll_server_write_function(SOCKET socket, char* data, unsigned data_size)
{

}

#include "nwasupport/generic_poll_server.c"



static void nwa_support_poll(command_t* command)
{
   generic_poll_server_poll_code(0);
}

static void nwa_support_destroy(command_t* command)
{

}

static void nwa_support_replier(command_t* command, const char* data, size_t len)
{

}

command_t*  nwa_support_new()
{
      command_t* command = (command_t*) malloc(sizeof(command_t));
      command->poll = nwa_support_poll;
      command->destroy =  nwa_support_destroy;
      command->replier = nwa_support_replier;

      generic_poll_server_start(0);
      return command;
}

int64_t nwasupport_emu_info(SOCKET sock, char** ag, int ac)
{
    send_full_hash_reply(socket, 5,
                            "name", "Retroarch",
                            "version", "0.1",
                            "id", "I am dumb",
                            "nwa_version", "1.0",
                            "commands", "EMULATOR_INFO,EMULATION_STATUS,GAME_INFO,CORES_LIST,CORE_CURRENT_INFO,CORE_INFO,EMULATION_PAUSE,EMULATION_RESUME,EMULATION_RESET,EMULATION_STOP");
     return 0;
}
