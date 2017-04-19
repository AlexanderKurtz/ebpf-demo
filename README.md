# Summary

This is a little test project to play with eBPF in general and
[bcc](https://github.com/iovisor/bcc) in particular.

# Requirements

You'll need a C compiler and bcc. On Debian, you can simply install the
[libbpfcc](https://packages.debian.org/sid/libbpfcc) and
[libbpfcc-dev](https://packages.debian.org/sid/libbpfcc-dev) packages.

# Files

## client.c

A simple client program which will continuously send UDP packets to
```[::1]:4242```. The amount of data in each packet must be specified as the
first argument.

Example:

	./client 1000 # Continuously sends 1000-byte packets

## server.c

The server part which listens on ```[::1]:4242``` (UDP only) and dumps all
received packets on stdout. Packets larger than the static buffer size of
```4096``` bytes will simply be truncated.

Please note that this program expects one command line argument: If ```0``` is
specified, all packets are read. If ```1``` (or any other non-zero integer) is
specified, an eBPF filter program is installed on the receiving socket which
filters out all empty packets.

Example:

	./server 0 # Run without the eBPF filter

	./server 1 # Run with the eBPF filter

## common.c

A few simple helper functions.

## filter.c

The filter program to drop empty packets. It uses bcc's internal library, so it
will only work with it. It will be compiled dynamically by the server program
during runtime using ```bcc``` (which in turn uses ```clang/LLVM```).

## Makefile

The makefile tying all this together. Nothing special here.

# Idea

The idea of this test was to start multiple clients, some of which send
non-empty and some empty (i.e. ```size == 0```) packets and measure the data
reception at the server part, for example by piping the output of the server
program to [pv](http://www.ivarch.com/programs/pv.shtml).

# Caveats

JIT-compilition of eBPF programs must be enabled in
```/proc/sys/net/core/bpf_jit_enable```.

# Expected result

When run with the eBPF filter, the server program does not have to handle empty
packets at all and should therefore be able to handle more non-empty packets
leading to an increase in data throughput.

# Actual result

Empty packets are correctly dropped and never reach the server program. If only
one client sends data (and only one byte per packet), but many clients send
empty packets, the data throughput increases significantly with the eBPF filter
program enabled.