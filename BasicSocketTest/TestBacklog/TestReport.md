### Testing Environment: macOS

Compile the code into a `server` program, then run:
```
./server 127.0.0.1 12345 3
```

Next, execute the following command three times (in three separate terminals):
```
telnet 127.0.0.1 12345
```

After that, check the network status:
```
netstat -nt | grep 12345
```

Since both the server and clients are on the same machine, there are six connections:
```
tcp4       0      0  127.0.0.1.12345        127.0.0.1.53593        ESTABLISHED
tcp4       0      0  127.0.0.1.53593        127.0.0.1.12345        ESTABLISHED
tcp4       0      0  127.0.0.1.12345        127.0.0.1.53536        ESTABLISHED
tcp4       0      0  127.0.0.1.53536        127.0.0.1.12345        ESTABLISHED
tcp4       0      0  127.0.0.1.12345        127.0.0.1.53489        ESTABLISHED
tcp4       0      0  127.0.0.1.53489        127.0.0.1.12345        ESTABLISHED
```

When starting a fourth `telnet` session, you’ll notice it sends a SYN packet but fails to establish a connection with the server:
```
tcp4       0      0  127.0.0.1.53658        127.0.0.1.12345        SYN_SENT   
tcp4       0      0  127.0.0.1.12345        127.0.0.1.53593        ESTABLISHED
tcp4       0      0  127.0.0.1.53593        127.0.0.1.12345        ESTABLISHED
tcp4       0      0  127.0.0.1.12345        127.0.0.1.53536        ESTABLISHED
tcp4       0      0  127.0.0.1.53536        127.0.0.1.12345        ESTABLISHED
tcp4       0      0  127.0.0.1.12345        127.0.0.1.53489        ESTABLISHED
tcp4       0      0  127.0.0.1.53489        127.0.0.1.12345        ESTABLISHED
```

It times out because the server has no remaining queue slots for it:
```
telnet 127.0.0.1 12345
Trying 127.0.0.1...
telnet: connect to address 127.0.0.1: Operation timed out
telnet: Unable to connect to remote host
```