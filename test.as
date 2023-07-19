.entry LENGTH
.extern W
MAIN: mov @r3, LENGTH
LOOP: jmp L1
K: .data 22
mcro m1
sub @r1, @r4
bne END
endmcro
STR: .string "abcdef"


; test
prn -5
bne LOOP
LABEL: .extern Another
.string "test"
m1
L1: inc K
bne LOOP
END: stop
LENGTH: .data 6,-9 ,15
