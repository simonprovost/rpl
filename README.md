# Rpl (FORK FROM laffeycomputer)

<p align="center">
<kbd>
  <img src="https://miro.medium.com/max/1024/1*BwEhCXFS1TetR5YtWESSOQ.png">
</kbd>
</p>

The rpl command reads standard input, replaces all occurrences of the first parameter, Expression, with the second parameter, Substitute, and then writes the result to standard output.

I Forked the source from laffey Computer services, I added a missing option which let the user skip some  directories he wanted.

# INSTALL

* Use the configure script `./configure`,
* Continue with `make` command,
* Finish by a `"sudo"(if necessary) make install` command.

# How to use then new method

It's a bit simple: the regular command is `rpl [-iwRspfdtx [-q|-v]] <old_str> <new_str> <target_file(s)>`.

The new one is `rpl [-iwRspfdtx [-q|-v]] <old_str> <new_str> <target_file(s)> --exclude-directories <all_paths> --end-excludes-directories`

After this one, the directories wrote will not be scanning by the command in order to skip some of them sometimes useless.

# Feedbacks/Issues

Please feel free to open an issue or contact me.

Thank you, Simon.
