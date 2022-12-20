#ifndef ENV_H
#define ENV_H

char *generate_memory_name(void);
char *generate_env_string(char *key, char *value);
char **generate_envp(char **base_envp, char *to_add, char *to_add2);

#endif