# Custom Shell

## Features

1. **Basic Command Set**:
   - This shell provides custom commands similar to basic terminal commands:
     - `warp` for `cd`
     - `peek` for `ls`
     - `seek` for `find`
     - `proclore` for `ps`
     - `pastevents` for `history`

2. **Prompt**:
   - The shell displays a prompt that includes the username, system name, and the current working directory.

3. **Home Directory**:
   - The home directory is represented by `~`, just like in typical shells.

4. **System Commands**:
   - In addition to custom commands, the shell supports system commands using `execvp`.

5. **Flag Support**:
   - Commands like `peek` (ls) and `seek` (find) accept flags and behave similarly to their standard terminal counterparts.

6. **I/O Redirection**:
   - Input/output redirection is supported:
     - `<` to provide input to a command
     - `>` to redirect output to a file
     - `>>` to append output to a file
   - Redirection can be combined with pipes (`|`), allowing the output of one command to serve as input for another.

7. **Pipes**:
   - The shell supports pipes, where the output of one command can be used as the input for the next command in a chain.

8. **Activity Monitoring**:
   - Running processes spawned by the shell can be viewed by typing `activities`.

9. **Signal Handling**:
   - Signals such as `Ctrl + Z`, `Ctrl + C`, and `Ctrl + D` are properly handled:
     - `Ctrl + Z` stops the running foreground process and sends it to the background.
     - `Ctrl + C` stops the currently running foreground process.
     - `Ctrl + D` exits the shell.

10. **Foreground/Background Process Management**:
    - You can switch processes between foreground and background using `fg` and `bg` commands.

11. **Neonate Command**:
    - The `neonate` command continuously monitors and displays the latest process IDs.

12. **iMan Command**:
    - Use `iMan` to get the manual for any given command.

---

## How to Use

1. Navigate to the main directory of the project.
2. Run the `make` command to compile all the files.
3. Once compiled, an executable file named `a.out` will be created.
4. Run `./a.out` in the terminal to launch the shell.
5. To exit the shell, either press `Ctrl + D` or type `exit`.
6. Use `Ctrl + C` to terminate a running foreground process.
7. Use `Ctrl + Z` to pause and send a foreground process to the background.
