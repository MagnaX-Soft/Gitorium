#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

#include <string.h>

#define CMD_VALID(name) if(!strcmp(cmd, name)) return 1;
#define CALL_CMD(name, fnc) if (!strcmp(cmd, name)) { fnc(argv); return; }
#define CALL_ARGV(cmd, name, fnc) if (!strcmp(cmd, name)) { fnc(argv); return; }

#define CALL_HELP(name, cmd) if (!strcmp(argv[0], name)) { command_help_##cmd(argv); return; }

#define HELP_PRINT(string) puts(string);
#define HELP_USAGE(usage_string) HELP_PRINT("usage: "usage_string)
#define HELP_BLANK HELP_PRINT("")

extern int is_remote_command_valid(char *cmd);
extern void call_remote_command(char *cmd, char **argv);

extern void command_help(char **argv);
extern void command_info(char **argv);
extern void command_list(char **argv);
extern void command_list_repos(char **argv);
extern void command_list_commands(char **argv);

extern void command_help_info(char **argv);
extern void command_help_list(char **argv);
extern void command_help_list_repos(char **argv);
extern void command_help_list_commands(char **argv);

#endif // COMMANDS_H_INCLUDED
