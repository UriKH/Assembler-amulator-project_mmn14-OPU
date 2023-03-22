# MMN14 - Assembler Simulation
Welcome to my final project of the course Systems Programming Lab in the Open University (final grade 100)

* This project simulate the first stage of the assembling process.
  The output files relate to the next specified assembly language and for a computer with address space of 4096.
* Compiles .as (assembly) files to .ent (entries), .ext (externals), .ob (object) files.

# Commands:
* The available commands are:
  mov, cmp, add, lea, not, clr, inc, dec, jmp, bne, jsr, red, prn, rts, stop
* saved words:
  .data, .string, .extern, .entry
* Available registers:
  r0, r1, r2, r3, r4, r5, r6

# Notes:
* usage of labels is in the usual format
* prefix for numberical values is: #

Use the makefile to compile the project and run with:
> main > [.as file]
