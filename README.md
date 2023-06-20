# Minishell

![Logo](https://cdn.discordapp.com/attachments/742825059122413768/1120809718332543057/image.png)

The minishell project is designed to handle simple and more complex tasks with custom features. The project works out of the box and creates a folder inside the user's home folder (/home/user/.minishell) that contains all the files created by the shell (history, .minishellrc, .minipasswd).
## Dependencies

In order to run, the projet needs the sodium library (https://doc.libsodium.org/).
In order to compile the C files with gcc, you need to use the -lsodium flag (everything is setup by default in the makefile).
## Run Locally

Run the make file

```bash
  make
```

Move the binary to /bin

```bash
  sudo cp minishell /bin
```


Clean files

```bash
  make fclean
```

Execute the binary

```bash
  /bin/minishell
```
## Commands

- cd - ls - pwd - date - echo - history - head
- cat - mkdir - rmdir - help - clear - PS1
- dump - searchdump - adduser - exit
## Features
- History file encryption with ChaCha20.
- Signal handling (ctrl-c , ctrl+ALT+\ , window closing).
- Prompt variable, history, users created and logs.
- Security features : dump, searchdump...
(All features were tested and work well).

## Appendix

Project has been tested on Ubuntu 22. You may have gcc and sodium in order to run the project. Please be aware that it may contain some errors.

