.org 0250

    CLR ACC
L2: CALL L1
    CMP X
    JN L2
    OUT ACC
    MOV X, Y
    MOV X, ACC
    MOV Y, X
    MOV Y, ACC
    MOV ACC, X
    MOV ACC, Y
    ADD 50
    AND X
    OR 20
    NOT ACC
    XOR Y
    NOP
    HLT

L1:
    LDR X, $0102
    SUB 20
    ADD X
    RET

L5: LS:
L8:
Ldsadas: L1: