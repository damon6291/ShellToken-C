Shell supports seven operators

Redirect input: sort < foo.txt
Redirect output: sort foo.txt > output.txt
Pipe: sort foo.txt | uniq
Background: sleep 10 &
And: true && echo one
Or: true || echo one
Semicolon: echo one; echo two

The “cd” command changes the current directory. This means calling chdir() to change the state of your shell process.
The “exit” command exits the shell.
