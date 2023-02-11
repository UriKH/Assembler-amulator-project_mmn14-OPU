# MMN14 - Assembler Simulation
Welcome to my final project of the course Systems Programming Lab in the Open University

Compiles .as (assembly) files to .ent (entries), .ext (externals), .ob (bobject) files.

# Commands:
* The available commands are:
  mov, cmp, add, lea, not, clr, inc, dec, jmp, bne, jsr, red, prn, rts, stop
* saved words:
  .data, .string, .extern, .entry
* Available registers:
  r0, r1, r2, r3, r4, r5, r6

# Notes:
* usage of labels is in the usual format
* before using a number first use: #

Use the makefile to compile the project and run with:
> main > [.as file]

By Uri K.H and Lihi B.H.
