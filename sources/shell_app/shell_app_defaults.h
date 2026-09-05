#ifndef SHELL_APP_DEFAULTS_H
#define SHELL_APP_DEFAULTS_H

int default_help(struct behavior *this, char **args, size_t argc);
int shell_app_exit(struct behavior *_this, char **_args, size_t _argc);
void default_on_start(struct behavior *_this);
void default_on_exit(struct behavior *_this);

#endif  // !SHELL_APP_DEFAULTS_H
