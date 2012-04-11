A int64 library for lua

In 64 bit architecture , we can use lightuserdata type for 64bit number operation. It's a simple way to support int64 in lua.

This library only offer one api : create a int64 number , see test.lua .

It can create a int64 number from lua number or a string (A little-endian 8 bytes string) . you can also push a lightuserdata as a int64 number from C library.
