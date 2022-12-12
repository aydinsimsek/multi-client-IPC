## Method 
When a new client is connected to the server's socket, a child process is created with fork() system call and that client is connected to the child process' socket, in this way multiple clients are handled simultaneously.


## Screenshot of an example usage

![screenshot](https://user-images.githubusercontent.com/43919074/128604957-5397fc63-e1e4-45ef-8c42-48ad8e72ae08.png)
