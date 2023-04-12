#!/bin/bash

{
    nasm -fbin main.s
    qemu-system-x86_64 -drive file=main
}