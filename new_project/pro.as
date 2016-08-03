.entry LOOP
.entry LENGTH
.extern L3
.extern W
MAIN: mov K[2-4],W
add r2,STR
LOOP: jmp W
prn #-5
sub r1, r4
inc K
 mov LOOP[1-13],r3
bne L3
END: stop
STR: .string “abcdef”
LENGTH: .data 6,-9,
