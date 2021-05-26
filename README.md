# 42 project: ft_ping

The goal of this project is to recode the `ping` command in C. This command tests the reachability of a host on an IP network, by sending a ICMP Echo request packet, and waiting for a response. The time it takes for the "ping" to come back is recorded.

## How to use

* Compile the project with `make`
* Launch it with superuser rights (`sudo ./ft_ping [address/hostname]`)

## Options available

* `-c`: packet count
* `-i`: interval between packets
* `-t`: packet Time-to-live
* `-f`: flood mode
* `-h`: help (usage)
* `-v`: verbose mode
