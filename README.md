# rpl

The rpl command reads standard input, replaces all occurrences of the first parameter, Expression, with the second parameter, Substitute, and then writes the result to standard output.
I added something in order to add a new method to this command which will skipped the directories wanted.

# INSTALL

Use the configure script `./configure`,
Continue with `make` command,
To finished by a `"sudo"(if necessary) make install` command.

# HOW TO USE NEW METHOD

It's a bit simple the regular command is `rpl [-iwRspfdtx [-q|-v]] <old_str> <new_str> <target_file(s)>`
The new one is `rpl [-iwRspfdtx [-q|-v]] <old_str> <new_str> <target_file(s)> --exclude-directories <all_paths> --end-excludes-directories`

After this one the directories wrote will not be scanning by the command in order to skip some of them sometimes useless.

# Requests/Issues

If you have something to tell me, open an issue.

Thank you, Simon.
