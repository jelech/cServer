# cServer
a personal server with c code.


### Have a nice fun

### you need:

- xinetd
- download the file to your linux
- create ```/etc/xinetd.d/OneName```
- add

```
        service OneName
        {
                socket_type      = stream
                protocol         = tcp
                wait             = no
                user             = nobody
                server           = yourServicePath/OneName
                server_args      = yourServicePath/mysite
                disable          = no
                flags            = IPv4
        }
```

 - edit ```/etc/services``` -> add a port for yourself
 - run ```make```
 - run ```sudo service xinetd restart```
 - check ```127.0.0.1:yourPort```


### future:

I will let it more functional that a service should be. And the xinetd I will try to change it into myself way to complete it's mission.
