JMP START

TERMS: DB 10 ; terms = 10
TOGGLE: DB 1 ; toggle = 1
RESULT: DB 80H, 00H, 00H, 01H ; OUTPUT

NUM: DB 86H, 0AH, 91H, 02H ; x = pi/3

X_2: DB 00H, 00H, 00H, 00H ; храним квадрат NUM'а
X_N: DB 00H, 00H, 00H, 00H ; NUM в н-ной степени

FAC_INC: DB 80H, 00H, 00H, 02H     ; инкремент для факториала
FAC: DB 80H, 00H, 00H, 02H   ; хранение факториала
JUSTONE: DB 80H, 00H, 00H, 01H ;

ACC: DB 00H, 00H, 00H, 00H   ; аккумулятор

FIRSTOP:  DB 00H, 00H, 00H, 00H ; первый вспомогательный операнд
SECONDOP: DB 00H, 00H, 00H, 00H ; второй вспомогательный операнд
SCRATCH:  DB 00H, 00H, 00H, 00H ; нулевой операнд для суммы


START:
  MVI H, 0 ; SET PAGE 0

  ; ---- X^2 ----
  LXI B, NUM            ; Копируем X в первый операнд
  LXI D, FIRSTOP        ;
  CALL ASSIGN           ;

  LXI B, NUM            ; Также дублиурем X во второй операнд
  LXI D, SECONDOP       ;
  CALL ASSIGN           ;

  LXI H, X_2            ; Положим результат вычислений в X_2
  MOV C, L
  LXI H, SECONDOP       ; Кладем первый X
  MOV B, L              ;
  LXI H, FIRSTOP        ; Кладем второй X
  CALL LMUL             ; X_2 = X * X(получаем на выходе квадрат)

  LXI B, X_2            ; Копируем в X_N
  LXI D, X_N
  CALL ASSIGN
  ; ---- X^2 ----


LOOP:
  ; ---- ACC = X_N / FAC ----
  LXI B, X_N
  LXI D, FIRSTOP
  CALL ASSIGN

  LXI B, FAC
  LXI D, SECONDOP
  CALL ASSIGN

  ; Когда делим, в регистре C результат, в регистре B делитель, в регистре H делимое
  LXI H, ACC
  MOV C, L
  LXI H, SECONDOP
  MOV B, L
  LXI H, FIRSTOP
  CALL LDIV
  ; ---- ACC = X_N / FAC ----

  ; ---- RES +-= ACC ----
  LXI B, SCRATCH
  LXI D, FIRSTOP
  CALL ASSIGN

  LXI H, FIRSTOP
  MOV C, L
  LXI H, ACC
  MOV B, L

  LXI H, TOGGLE
  MOV A, M
  ANA A ; Обновляем бит Zero

  JZ TOGGLE0
  ; Если toggle == 1, значит вычитаем
  ; res -= x_n / fac
  LXI H, RESULT
  CALL LSUB

  LXI H, TOGGLE
  DCR M

  JMP CONTINUE
TOGGLE0: ; иначе прибавляем
  ; res += x_n / fac
  LXI H, RESULT
  CALL LADD

  LXI H, TOGGLE
  INR M
  ; ---- RES +-= ACC ----
CONTINUE:

  ; ---- NEXT FACTORIAL ----
  CALL NEXT_FACTORIAL
  CALL NEXT_FACTORIAL
  ; ---- NEXT FACTORIAL ----

  ; ---- NEXT X_N ----
  LXI B, X_2
  LXI D, FIRSTOP
  CALL ASSIGN

  LXI B, X_N
  LXI D, SECONDOP
  CALL ASSIGN

  LXI H, X_N
  MOV C, L
  LXI H, FIRSTOP
  MOV B, L
  LXI H, SECONDOP
  CALL LMUL
  ; ---- NEXT X_N ----

  ; ---- LOOP CONDITIONS ----
  LXI H, TERMS
  DCR M
  MOV A, M
  ANA A ; Обновляем бит Zero
  JNZ LOOP
  ; ---- LOOP CONDITIONS ----

  ; sec(x) = 1 / cos(x)
  LXI B, RESULT
  LXI D, FIRSTOP
  CALL ASSIGN

  LXI H, RESULT
  MOV C, L
  LXI H, FIRSTOP
  MOV B, L
  LXI H, JUSTONE
  CALL LDIV

  HLT

NEXT_FACTORIAL:
  ; i += 1
  ; fac *= i
  ; i += 1
  ; fac *= i
  ; 4! = 2! * 3 * 4...
  LXI B, SCRATCH
  LXI D, FIRSTOP
  CALL ASSIGN

  LXI B, JUSTONE
  LXI D, SECONDOP
  CALL ASSIGN

  LXI H, FIRSTOP
  MOV C, L
  LXI H, SECONDOP
  MOV B, L
  LXI H, FAC_INC
  CALL LADD

  LXI B, FAC
  LXI D, FIRSTOP
  CALL ASSIGN

  LXI B, FAC_INC
  LXI D, SECONDOP
  CALL ASSIGN

  ; Когда умножаем, в регистре C результат, в B и H множители
  LXI H, FAC ; FAC = FAC * I
  MOV C, L
  LXI H, FIRSTOP
  MOV B, L
  LXI H, SECONDOP
  CALL LMUL

  RET

ASSIGN:
  LDAX B
  STAX D
  INX B
  INX D
  LDAX B
  STAX D
  INX B
  INX D
  LDAX B
  STAX D
  INX B
  INX D
  LDAX B
  STAX D
  RET
;
;
;******************************************************
;       //// DIVIDE SUBROUTINE
;******************************************************
;
;
LDIV:   CALL    CSIGN           ;COMPUTE SIGN OF RESULT
        CALL    ZCHK            ;CHECK IF DIVIDEND = ZERO
        JNZ     DTST2           ;IF DIVIDEND .NE. 0 CHECK DIVISOR
        CALL    BCHK            ;CHECK FOR ZERO/ZERO
        JZ      INDFC           ;ZERO/ZERO = INDEFINITE
        JMP     WZERC           ;ZERO/NONZERO = ZERO
DTST2:  CALL    BCHK            ;COME HERE IF DIVIDEND .NE. 0
        JZ      OFLWC           ;NONZERO/ZERO = OVERFLOW
                                ;IF WE GET HERE, THINGS LOOK OKAY
        MOV     E,L             ;SAVE BASE IN E
        MOV     L,C             ;BASE 6 TO L
        CALL    DCLR            ;CLEAR QUOTIENT MANTISSA SLOT
        MOV     L,E             ;RESTORE BASE IN L
        CALL    ENT1            ;DO FIRST CYCLE
        MOV     L,C             ;BASE 6 TO L
        CALL    DLST            ;MOVE QUOTIENT OVER ONE PLACE
        MVI     D,23            ;NUMBER OF ITERATIONS TO D
REP3:   MOV     L,E
        CALL    ENT2
        DCR     D               ;DEC D
        JZ      GOON
        MOV     A,L
        MOV     L,C             ;BASE 6 TO L
        MOV     C,A
        CALL    DLST            ;MOVE QUOTIENT MANT OVER
        MOV     A,L             ;CPTR TO A
        MOV     E,C             ;LPTR TO E
        MOV     C,A             ;CPTR TO C
        JMP     REP3
;
GOON:   CALL    AORS            ;CHECK IF RESULT IS NORMALIZED
        JM      CRIN
        MOV     A,L             ;LPTR TO A
        MOV     L,C             ;CPTR TO L
        MOV     C,A             ;LPTR TO C
        CALL    DLST            ;SHIFT QUOTIENT LEFT
        MOV     C,L
        MOV     L,E
        CALL    LDCP            ;COMPUTE THE CHARACTERISTIC OF RESULT
        RET
;
CRIN:   CALL    CFCHE           ;GET A=CHAR(H,L), E=CHAR(H,B)
        SUB     E               ;NEW CHAR = CHAR(DIVIDEND) - CHAR(DVISIOR)
        CPI     7FH             ;CHECK MAX POSITIVE NUMBER
        JZ      OFLWC           ;JUMP ON OVERFLOW
        ADI     1               ;ADD 1 SINCE WE DID NOT LEFTSHIFT
        CALL    CCHK            ;CHECK AND STORE CHARACTERISTIC
        RET                     ;RETURN
;
;
;
;******************************************************
;       //// ADDITION SUBROUTINE
;******************************************************
;
;
LADD:   XRA     A               ;/***SET UP TO ADD
        JMP     LADS            ;NOW DO IT
;
;
;******************************************************
;       //// SUBTRACTION SUBROUTINE
;******************************************************
;
;
LSUB:   MVI     A,80H           ;/****SET UP TO SUBTRACT
;   SUBROUTINE LADS
;
;       FLOATING POINT ADD OR SUB
;       A 128 ON ENTRY SUB
;       A 0 ON ENTRY ADD
;       F-S F,FIRST OPER DESTROYED
;       BASE 11 USED FOR SCRATCH
LADS:   CALL    ACPR            ;SAVE ENTRY PNT AT BASE 6
        CALL    BCHK            ;CHECK ADDEND/SUBTRAHEND = ZERO
        RZ                      ;IF SO, RESULT=ARG SO RETURN
                                ;THIS WILL PREVENT UNDERFLOW INDICATION ON
                                ;ZERO + OR - ZERO
        CALL    CCMP
        JZ      EQ02            ;IF EQUAL, GO ON
        MOV     D,A             ;SAVE LPTR CHAR IN D
        JC      LLTB
        SUB     E               ;L.GT.B IF HERE
        ANI     127
        MOV     D,A             ;DIFFERENCE TO D
        MOV     E,L             ;SAVE BASE IN E
        MOV     L,C             ;C PTR TO L
        INR     L               ;C PTR 1 TO L
        MOV     M,E             ;SAVE BASE IN C PTR\1
        MOV     L,B             ;B PTR TO L
        JMP     NCHK
LLTB:   MOV     A,E             ;L.LT.B IF HERE,BPTR TO A
        SUB     D               ;SUBTRACT LPTR CHAR FROM BPTR CHAR
        ANI     127
        MOV     D,A             ;DIFFERENCE TO D
NCHK:   MVI     A,24
        CMP     D
        JNC     SH10
        MVI     D,24
SH10:   ORA     A
        CALL    DRST
        DCR     D
        JNZ     SH10
EQUL:   MOV     A,L
        CMP     B
        JNZ     EQ02            ;F.GT.S IF L.NE.B
        MOV     L,C             ;C PTR TO L
        INR     L               ;C PTR\1 TO L
        MOV     L,M             ;RESTORE L
EQ02:   CALL    LASD            ;CHECK WHAT TO
        CALL    ACPR            ;SAVE ANSWER
        CPI     2               ;TEST FOR ZERO ANSWER
        JNZ     NOT0
        JMP     WZER            ;WRITE FLOATING ZERO AND RETURN
;
NOT0:   MVI     D,1             ;WILL TEST FOR SUB
        ANA     D
        JZ      ADDZ            ;LSB 1 INPLIES SUB
        CALL    TSTR            ;CHECK NORMAL/REVERSE
        JZ      SUBZ            ;IF NORMAL,GO SUBZ
        MOV     A,L             ;OTHERWISE REVERSE
        MOV     L,B             ;ROLES
        MOV     B,A             ;OF L AND B
;
SUBZ:   CALL    DSUB            ;SUBTRACT SMALLER FROM BIGGER
        CALL    MANT            ;SET UP SIGN OF RESULT
        CALL    TSTR            ;SEE IF WE NEED TO INTERCHANGE
                                ;BPTR AND LPTR
        JZ      NORM            ;NO INTERCHANGE NECESSARY, SO NORMALIZE
                                ;AND RETURN
        MOV     A,L             ;INTERCHANGE
        MOV     L,B             ;L
        MOV     B,A             ;AND B
        MOV     A,C             ;CPTR  TO A
        MOV     C,B             ;BPTR TO C
        MOV     E,L             ;LPTR TO E
        MOV     B,A             ;CPTR TO B
        CALL    LXFR            ;MOVE BPTR> TO LPTR>
        MOV     A,B
        MOV     B,C
        MOV     C,A
        MOV     L,E
        JMP     NORM            ;NORMALIZE RESULT AND RETURN
;
;   COPY THE LARGER CHARACTERISTIC TO THE RESULT
;
ADDZ:   CALL    CCMP            ;COMPARE THE CHARACTERISTICS
        JNC     ADD2            ;IF CHAR(H,L) .GE. CHAR(H,B) CONTINUE
        CALL    BCTL            ;IF CHAR(H,L) .LT. CHAR(H,B) THE COPY
                                ;CHAR(H,B) TO CHAR(H,L)
ADD2:   CALL    MANT            ;COMPUTE SIGN OF RESULT
        CALL    DADD            ;ADD MANTISSAS
        JNC     SCCFG           ;IF THERE IS NO OVFLW - DONE
        CALL    DRST            ;IF OVERFLOW SHIFT RIGHT
        CALL    INCR            ;AND INCREMENT CHARACTERISTIC
        RET                     ;ALL DONE, SO RETURN
;
;   THIS ROUTINE STORES THE MANTISSA SIGN IN THE RESULT
;   THE SIGN HAS PREVIOUSLY BEEN COMPUTED BY LASD.
;
MANT:   MOV     E,L             ;SAVE L PTR
        MOV     L,C             ;C PTR TO L
        MOV     A,M             ;LOAD INDEX WORD
        ANI     128             ;SCARF SIGN
        MOV     L,E             ;RESTORE L PTR
        INR     L               ;L PTR 2
        INR     L
        INR     L               ;TO L
        MOV     E,A             ;SAVE SIGN IN E
        MOV     A,M
        ANI     127             ;SCARF CHAR
        ADD     E               ;ADD SIGN
        MOV     M,A             ;STORE IT
        DCR     L               ;RESTORE
        DCR     L
        DCR     L               ;L PTR
        RET
;
;
;   SUBROUTINE LASD
;
;       UTILITY ROUTINE FOR LADS
;       CALCULATES TRUE OPER AND SGN
;       RETURNS ANSWER IN
LASD:   CALL    MSFH            ;FETCH MANT SIGNS, F IN A,D
        CMP     E               ;COMPARE SIGNS
        JC      ABCH            ;F\,S- MEANS GO TO A BRANCH
        JNZ     BBCH            ;F- S\ MEANS GO TO B BRANCH
        ADD     E               ;SAME SIGN IF HERE, ADD SIGNS
        JC      BMIN            ;IF BOTH MINUS, WILL OVERFLOW
        CALL    AORS            ;BOTH POS IF HERE
        JP      L000            ;IF AN ADD, LOAD 0
COM1:   CALL    DCMP            ;COMPARE F WITH S
        JC      L131            ;S.GT.F,SO LOAD 131
        JNZ     L001            ;F.GT.S,SO LOAD 1
L002:   MVI     A,2             ;ERROR CONDITION, ZERO ANSWER
        RET
BMIN:   CALL    AORS            ;CHECK FOR ADD OR SUB
        JP      L128            ;ADD, SO LOAD 128
COM2:   CALL    DCMP            ;COMPARE F WITH S
        JC      L003            ;S.GT.F,SO LOAD 3
        JNZ     L129            ;FGT.S.SO LOAD 129
        JMP     L002            ;ERROR
ABCH:   CALL    AORS            ;FT,S- SO TEST FOR A/S
        JM      L000            ;SUBTRACT, SO LOAD 0
        JMP     COM1            ;ADD, SO GO TO DCMP
BBCH:   CALL    AORS            ;F-,S\,SO TEST FOR A/S
        JM      L128            ;SUB
        JMP     COM2            ;ADD
L000:   XRA     A
        RET
L001:   MVI     A,1
        RET
L003:   MVI     A,3
        RET
L128:   MVI     A,128
        RET
L129:   MVI     A,129
        RET
L131:   MVI     A,131
        RET
;
;       SUBROUTINE LMCM
;       COMPARES THE MAGNITUDE OF
;       TWO FLOATING PNT NUMBERS
;       Z[1 IF [,C[1 IF F.LT.S.
LMCM:   CALL    CCMP            ;CHECK CHARS
        RNZ                     ;RETURN IF NOT EQUAL
        CALL    DCMP            ;IF EQUAL, CHECK MANTS
        RET
;
;
;
;***************************************************
;       //// MULTIPLY SUBROUTINE
;***************************************************
;
;   SUBROUTINE LMUL
;
;       FLOATING POINT MULTIPLY
;       L PTR X B PTR TO C PTR
;
LMUL:   CALL    CSIGN           ;COMPUTE SIGN OF RESULT AND STORE IT
        CALL    ZCHK            ;CHECK FIRST OPERAND FOR ZERO
        JZ      WZERC           ;ZERO * ANYTHING = ZERO
        CALL    BCHK            ;CHECK SECOND OPERAND FOR ZERO
        JZ      WZERC           ;ANYTHING * ZERO = ZERO
        MOV     E,L             ;SAVE L PTR
        MOV     L,C             ;C PTR TO L
        CALL    DCLR            ;CLR PRODUCT MANT LOCS
        MOV     L,E             ;L PTR TO L
        MVI     D,24            ;LOAD NUMBER ITERATIONS
KPGO:   CALL    DRST            ;SHIFT L PTR RIGHT
        JC      MADD            ;WILL ADD B PTR IF C[1
        MOV     A,L             ;INTERCHANGE
        MOV     L,C             ;L AND
        MOV     C,A             ;C PTRS
INTR:   CALL    DRST            ;SHIFT PRODUCT OVER
        MOV     A,L             ;INTERCHANGE
        MOV     L,C             ;L AND C PTRS_BACK TO
        MOV     C,A             ;ORIGINAL>
        DCR     D
        JNZ     KPGO            ;MORE CYCLES IF Z[0
        CALL    AORS            ;TEST IF RESULT IS NORMALIZED
        JM      LMCP            ;IF NORMALIZED GO COMPUTE CHAR
        MOV     E,L             ;SAVE LPTR IN E
        MOV     L,C             ;SET L=CPTR
        CALL    DLST            ;LEFT SHIFT RESULT TO NORMALIZE
        MOV     L,E             ;RESTORE LPTR
        CALL    CFCHE           ;OTHERWISE SET A=CHAR(H,L), E=CHAR(H,B)
        ADD     E               ;CHAR(RESULT) = CHAR(H,L) + CHAR(H,B)
        CPI     80H             ;CHECK FOR SMALLEST NEGATIVE NUMBER
        JZ      UFLWC           ;IF SO THEN UNDERFLOW
        SUI     1               ;SUBTRACT 1 TO COMPENSATE FOR NORMALIZE
        CALL    CCHK            ;CHECK CHARACTERISTIC AND STORE IT
        RET                     ;RETURN
;
MADD:   MOV     A,L             ;INTERCHANGE
        MOV     L,C             ;L AND
        MOV     C,A             ;C PTRS
        CALL    DADD            ;ACCUMULATE PRODUCT
        JMP     INTR
;
;   SUBROUTINE NORM
;
;       THIS SUBROUTINE WILL NORMALIZE A FLOATING POINT
;       NUMBER, PRESERVING ITS ORIGINAL SIGN.
;       WE CHECK FOR UNDERFLOW AND SET THE CONDITION
;       FLAG APPROPRIATELY.  (SEE ERROR RETURNS).
;       THER IS AN ENTRY POINT TO FLOAT A SIGNED INTEGER
;       (FLOAT) AND AN ENTRY POINT TO FLOAT AN UNSIGNED
;       INTEGER.
;
;   ENTRY POINTS:
;
;       NORM  - NORMALIZE FLOATING PT NUMBER AT (H,L)
;       FLOAT - FLOAT TRIPLE PRECISION INTEGER AT (H,L)
;               PRESERVING SIGN BIT IN (H,L)+3
;       DFXL  - FLOAT UNSIGNED (POSITIVE) TRIPLE PRECISION
;               AT (H,L)
;
;   REGISTERS ON EXIT:
;
;       A = CONDITION FLAG (SEE ERROR RETURNS)
;       D,E = GARBAGE
;       B,C,H,L = SAME AS ON ENTRY
;
NORM:   MOV     E,L             ;SAVE L IN E
NORM1:  CALL    GCHAR           ;GET CHAR(H,L) IN A WITH SIGN EXTENDED
        MOV     D,A             ;SAVE CHAR IN D
FXL1:   MOV     L,E             ;RESTORE L
FXL2:   CALL    ZMCHK           ;CHECK FOR ZERO MANTISSA
        JZ      WZER            ;IF ZERO MANTISSA THEN ZERO RESULT
REP6:   MOV     A,M             ;GET MOST SIGNIFICANT BYTE OF
                                ;MANTISSA
        ORA     A               ;SET FLAGS
        JM      SCHAR           ;IF MOST SIGNFICANT BIT = 1 THEN
                                ;NUMBER IS NORMALIZED AND WE GO TO
                                ;STORE THE CHARACTERISTIC
        MOV     A,D             ;OTHERWISE CHECK FOR UNDERFLOW
        CPI     C0H             ;COMPARE WITH MINIMUM CHAR
        JZ      WUND            ;IF EQUAL THEN UNDERFLOW
        CALL    DLST            ;SHIFT MANTISSA LEFT
        DCR     D               ;DECREMENT CHARACTERSTIC
        JMP     REP6            ;LOOP AN TEST NEXT BIT
SCHAR:  JMP     INCR3           ;STORE THE CHARACTERISTIC USING
                                ;THE SAME CODE AS THE INCREMENT
;
DFXL:   MOV     E,L             ;ENTER HERE TO FLOAT UNSIGNED
                                ;INTEGER
                                ;FIRT SAVE L IN E
        INR     L               ;MAKE (H,L) POINT TO CHAR
        INR     L               ;MAKE (H,L) POINT TO CHAR
        INR     L               ;MAKE (H,L) POINT TO CHAR
        XRA     A               ;ZERO ACCUMULATOR
        MOV     M,A             ;STORE A PLUS (+) SIGN
        MOV     L,E             ;RESTORE L
FLOAT:  MVI     D,24            ;ENTER HERE TO FLOAT INTEGER
                                ;PRESERVING ORIGINAL SIGN IN (H,L)+3
                                ;SET UP CHARACTERISTIC
        JMP     FXL2            ;GO FLOAT THE NUMBER
;
;
;
;
;   SUBROUTINE ZCHK
;
;       THIS ROUTINE SETS THE ZERO FLAG IF IT DETECTS
;       A FLOATING ZERO AT (H,L).
;
;   SUBROUTINE ZMCHK
;
;       THIS ROUTINE SETS THE ZERO FLAG IF IT DETECTS A
;       ZERO MANTISSA AT (H,L)
;
ZCHK:
ZMCHK:  INR     L               ;SET L TO POINT LAST BYTE OF MANTISSA
        INR     L               ;SET L TO POINT TO LAST BYTE OF MANTISSA
        MOV     A,M             ;LOAD LEAST SIGNIFICANT BYTE
        DCR     L               ;L POINTS TO MIDDLE BYTE
        ORA     M               ;OR WITH LEAST SIGNFICANT BYTE
        DCR     L               ;L POINTS TO MOST SIGNFICANT BYTE
                                ;OF MANTISSA (ORIGINAL VALUE)
        ORA     M               ;OR IN MOST SIGNFICANT BYTE
        RET                     ;RETURNS WITH ZERO FLAG SET APPROPRIATELY
;
;  SUBROUTINE BCHK
;
;       THIS ROUTINE CHECKS (H,B) FOR FLOATING PT ZERO
;
BCHK:   MOV     E,L             ;SAVE LPTR IN E
        MOV     L,B             ;SET L=BPTR
        CALL    ZCHK            ;CHECK FOR ZERO
        MOV     L,E             ;RESTORE L=LPTR
        RET                     ;RETURN
;
;
;   SUBROUTINE DLST
;
;       SHIFTS DBL WORD ONE PLACE LF
DLST:   INR     L
        INR     L               ;/***TP
        MOV     A,M             ;LOAD IT
        ORA     A               ;KILL CARRY
        RAL                     ;SHIFT IT LEFT
        MOV     M,A             ;STORE IT
        DCR     L
        MOV     A,M             ;LOAD IT
        RAL                     ;SHIFT IT LEFT
;       IF CARRY SET BY FIRST SHIFT
;       IT WILL BE IN LSB OF SECOND
        MOV     M,A
        DCR     L               ;/***TP EXTENSION
        MOV     A,M
        RAL
        MOV     M,A             ;/***ALL DONE TP
        RET
;
;
;   SUBROUTINE DRST
;
;       SHIFTS DOUBLE WORD ONE PLACE
;       TO THE RIGHT
;           DOES NOT AFFECT D
;
DRST:   MOV     E,L             ;/***TP MODIFIED RIGHT SHIFT TP
        MOV     A,M             ;LOAD FIRST WORD
        RAR                     ;ROTATE IT RIGHT
        MOV     M,A             ;STORE IT
        INR     L               ;/*** TP
        MOV     A,M             ;LOAD SECOND WORD
        RAR                     ;SHIFT IT RIGHT
        MOV     M,A             ;STORE IT
        INR     L               ;/*** TP EXTENSION
        MOV     A,M
        RAR
        MOV     M,A
        MOV     L,E             ;/***TP - ALL DONE TP
        RET
;
;
;   SUBROUTINE DADD
;
;       ADDS TWO DOUBLE PRECISION
;       WORDS, C=1 IF THERE IS OVRFLW
;
DADD:   MOV     E,L             ;SAVE BASE IN E
        MOV     L,B             ;BASE \3 TO L
        INR     L               ;BASE \4 TO L
        INR     L               ;/***TP
        MOV     A,M             ;LOAD S MANTB
        MOV     L,E             ;BASE TO L
        INR     L               ;BASE \1 TO L
        INR     L               ;/***TP
        ADD     M               ;ADD TWO MANTB]S
        MOV     M,A             ;STORE ANSWER
        MOV     L,B             ;/***TP EXTENSION
        INR     L
        MOV     A,M
        MOV     L,E
        INR     L
        ADC     M
        MOV     M,A             ;/***TP - ALL DONE
        MOV     L,B             ;BASE \3 TO L
        MOV     A,M             ;MANTA OF S TO A
        MOV     L,E             ;BASE TO L
        ADC     M               ;ADD WITH CARRY
        MOV     M,A             ;STORE ANSWER
        RET
;
;
;   SUBROUTINE DCLR
;
;       CLEARS TWO SUCCESSIVE
;       LOCATIONS OF MEMORY

DCLR:   XRA     A
        MOV     M,A
        INR     L
        MOV     M,A
        INR     L               ;/***TP EXTENSION
        MOV     M,A             ;/***TP ZERO 3
        DCR     L               ;/***TP - ALL DONE
        DCR     L
        RET
;
;
;       /*****ALL NEW DSUB - SHORTER***
;
;   SUBROUTINE DSUB
;
;       DOUBLE PRECISION SUBTRACT
;
DSUB:   MOV     E,L             ;SAVE BASE IN E
        INR     L               ;/***TP EXTENSION
        INR     L               ;/START WITH LOWS
        MOV     A,M             ;/GET ARG
        MOV     L,B             ;/NOW SET UP TO SUB
        INR     L
        INR     L
        SUB     M               ;/NOW DO IT
        MOV     L,E             ;/NOW MUST PUT IT BACK
        INR     L
        INR     L
        MOV     M,A             ;/PUT BACK
        DCR     L               ;/***TP - ALL DONE
        MOV     A,M             ;/GET LOW OF LOP
        MOV     L,B             ;/SET TO BOP
        INR     L               ;/SET TO BOP LOW
        SBB     M               ;/GET DIFF. OF LOWS
        MOV     L,E             ;/SAVE IN LOP LOW
        INR     L               ;/TO LOP LOW
        MOV     M,A             ;/INTO RAM
        DCR     L               ;/BACK UP TO LOP HIGH
        MOV     A,M             ;/GET LOP HIGH
        MOV     L,B             ;/SET TO BOP HIGH
        SBB     M               ;/SUB. WITH CARRY
        MOV     L,E             ;/SAVE IN LOP HIGH
        MOV     M,A             ;/INTO RAM
        RET                     ;/ALL DONE - MUCH SHORTER
;
;   SUBROUTINE GCHAR
;
;       THIS SUBROUTINE RETURNS THE CHARACTERISTIC OF
;       THE FLOATING POINT NUMBER POINTED TO BY (H,L)
;       IN THE A REGISTER WITH ITS SIGN EXTENDED INTO THE
;       LEFTMOST BIT.
;
;   REGISTERS ON EXIT:
;
;       A = CHARACTERISTIC OF (H,L) WITH SIGN EXTENDED
;       L = (ORIGINAL L) + 3
;       B,C,D,E,H = SAME AS ON ENTRY
;
GCHAR:  INR     L               ;MAKE (H,L) POINT TO CHAR
        INR     L               ;MAKE (H,L) POINT TO CHAR
        INR     L               ;MAKE (H,L) POINT TO CHAR
        MOV     A,M             ;SET A=CHAR + MANTISSA SIGN
        ANI     7FH             ;GET RID OF MANTISSA SIGN BIT
        ADI     40H             ;PROPAGATE CHAR SIGN INTO LEFTMOST BIT
        XRI     40H             ;RESTORE ORIGINAL CHAR SIGN BIT
        RET                     ;RETURN WITH (H,L) POINTING TO THE
                                ;CHAR = ORIGINAL (H,L)+3
                                ;SOMEONE ELSE WILL CLEAN UP
;
;
;   SUBROUTINE CFCHE
;
;       THIS SUBROUTINE RETURNS THE CHARACTERISTICS OF THE
;       FLOATING POINT NUMBERS POINTED TO BY (H,L) AND
;       (H,B) IN THE A AND E REGISTERS RESPECTIVELY,
;       WITH THEIR SIGNS EXTENDED INTO THE LEFTMOST BIT.
;
;   REGISTERS ON EXIT:
;
;       A = CHARACTERISTIC OF (H,L) WITH SIGN EXTENDED
;       E = CHARACTERISTIC OF (H,B) WITH SIGN EXTENDED
;       B,C,H,L = SAME AS ON ENTRY
;       D = A
;
CFCHE:  MOV     E,L             ;SAVE LPTR IN E
        MOV     L,B             ;SET L = BPTR
        CALL    GCHAR           ;GET CHAR(H,B) WITH SIGN EXTENDED IN A
        MOV     L,E             ;RESTORE L = LPTR
        MOV     E,A             ;SET E=CHAR(H,B) WITH SIGN EXTENDED
        CALL    GCHAR           ;SET A=CHAR(H,L) WITH SIGN EXTENDED
        DCR     L               ;RESTORE L = LPTR
        DCR     L               ;RESTORE L = LPTR
        DCR     L               ;RESTORE L = LPTR
        MOV     D,A             ;SET D=A=CHAR(H,L) WITH SIGN EXTENDED
        RET
;
;
;   SUBROUTINE CCMP
;
;       THIS SUBROUTINE COMPARES THE CHARACTERISTICS OF
;       FLOATING POINT NUMBERS POINTED TO BY (H,L) AND (H,B).
;       THE ZERO FLIP-FLOP IS SET IF CHAR(H,L) EQUALS
;       CHAR(H,B).  IF CHAR(H,L) IS LESS THAN CHAR(H,B) THEN
;       THE CARRY BIT WILL BE SET.
;
;   REGISTERS ON EXIT:
;
;       A = CHARACTERISTIC OF (H,L) WITH SIGN EXTENDED
;       E = CHARACTERISTIC OF (H,B) WITH SIGN EXTENDED
;       D = A
;       B,C,H,L = SAME AS ON ENTRY
;
CCMP:   CALL    CFCHE           ;FETCH CHARACTERTISTICS WITH SIGN EXTENDED
                                ;INTO A (CHAR(H,L)) AND E (CHAR(H,B)) REGISTERS
        MOV     D,A             ;SAVE CHAR (H,L)
        SUB     E               ;SUBTRACT E (CHAR(H,B))
        RAL                     ;ROTATE SIGN BIT INTO CARRY BIT
        MOV     A,D             ;RESTORE A=CHAR(H,L)
        RET                     ;RETURN
;
;   ERROR RETURNS
;
;       THE FOLLOWING CODE IS USED TO RETURN VARIOUS
;       ERROR CONDITIONS.  IN EACH CASE A FLOATING POINT
;       NUMBER IS STORED IN  THE 4 WORDS POINTED TO BY (H,L)
;       AND A FLAG IS STORED IN THE ACCUMULATOR.
;
;       CONDITION   FLAG   RESULT (+)        RESULT (-)
;
;       UNDERFLOW    377   000 000 000 100   000 000 000 300
;       OVERFLOW     177   377 377 377 077   377 377 377 277
;       INDEFINITE   077   377 377 377 077   377 377 377 277
;       NORMAL       000   XXX XXX XXX XXX   XXX XXX XXX XXX
;       NORMAL ZERO  000   000 000 000 100   (ALWAYS RETURNS +0)
;
;   ENTRY POINTS:
;
;       WUND - WRITE UNDERFLOW
;       WOVR - WRITE OVERFLOW
;       WIND - WRITE INDEFINITE
;       WZER - WRITE NORMAL ZERO
;
;###S
;WFLT     MACRO   VMANT,VCHAR,VFLAG,LABEL  ;WRITE FLOATING NUMBER
;
;         MVI     D,VCHAR     ;LOAD CHARACTERISTIC INTO D REGISTER
;         CALL    WCHAR       ;WRITE CHARACTERISTIC
;LABEL::  MVI     A,VMANT     ;LOAD MANTISSA VALUE
;                             ;WE ASSUME HERE THAT ALL BYTES OF MANTISSA
;                             ;ARE THE SAME
;         CALL    WMANT       ;WRITE THE MANTISSA
;         MVI     A,VFLAG     ;SET ACCUMULATOR TO FLAG
;         ORA     A           ;SET FLAGS PROPERLY
;         RET                 ;RETURN (WMANT RESTORED (H,L))
;         ENDM
;
;WUND:   WFLT    0,40H ,FFH ,UFLW1  ;WRITE UNDERFLOW
WUND:   MVI     D,40H           ;LOAD CHARACTERISTIC INTO D REGISTER
        CALL    WCHAR           ;WRITE CHARACTERISTIC
UFLW1:  MVI     A,0             ;LOAD MANTISSA VALUE
                                ;WE ASSUME HERE THAT ALL BYTES OF MANTISSA
                                ;ARE THE SAME
        CALL    WMANT           ;WRITE THE MANTISSA
        MVI     A,FFH           ;SET ACCUMULATOR TO FLAG
        ORA     A               ;SET FLAGS PROPERLY
        RET                     ;RETURN (WMANT RESTORED (H,L))
;WOVR:   WFLT    FFH ,3FH ,7FH ,OFLW1  ;WRITE OVERFLOW
WOVR:   MVI     D,3FH            ;LOAD CHARACTERISTIC INTO D REGISTER
        CALL    WCHAR           ;WRITE CHARACTERISTIC
OFLW1:  MVI     A,FFH           ;LOAD MANTISSA VALUE
                                ;WE ASSUME HERE THAT ALL BYTES OF MANTISSA
                                ;ARE THE SAME
        CALL    WMANT           ;WRITE THE MANTISSA
        MVI     A,7FH           ;SET ACCUMULATOR TO FLAG
        ORA     A               ;SET FLAGS PROPERLY
        RET                     ;RETURN (WMANT RESTORED (H,L))
;WIND:   WFLT    FFH ,3FH ,3FH ,INDF1  ;WRITE INDEFINITE
WIND:   MVI     D,3FH            ;LOAD CHARACTERISTIC INTO D REGISTER
        CALL    WCHAR           ;WRITE CHARACTERISTIC
INDF1:  MVI     A,FFH           ;LOAD MANTISSA VALUE
                                ;WE ASSUME HERE THAT ALL BYTES OF MANTISSA
                                ;ARE THE SAME
        CALL    WMANT           ;WRITE THE MANTISSA
        MVI     A,3FH            ;SET ACCUMULATOR TO FLAG
        ORA     A               ;SET FLAGS PROPERLY
        RET                     ;RETURN (WMANT RESTORED (H,L))
;###E
;
WZER:   INR     L               ;WRITE NORMAL ZERO
        INR     L               ;
        INR     L               ;
        MVI     M,40H           ;STORE CHARACTERISTIC FOR ZERO
        XRA     A               ;ZERO ACCUMULATOR
        CALL    WMANT           ;STORE ZERO MANTISSA
        ORA     A               ;SET FLAGS PROPERLY
        RET                     ;RETURN
;
; ROUTINE TO WRITE MANTISSA FOR ERROR RETURNS
;
WMANT:  DCR     L               ;POINT LEAST SIGNIFICANT BYTE
                                ;OF MANTISSA
        MOV     M,A             ;STORE LSBYTE OF MANTISSA
        DCR     L               ;POINT TO NEXT LEAST SIGNIFICANT BYTE
                                ;OF MANTISSA
        MOV     M,A             ;STORE NLSBYTE OF MANTISSA
        DCR     L               ;POINT TO MOST SIGNIFICANT BYTE
                                ;OF MANTISSA
        MOV     M,A             ;STORE MSBYTE OF MANTISSA
        RET                     ;RETURN (H,L) POINTS TO BEGINNING OF
                                ;FLOATING POINT RESULT
;
; ROUTINE TO WRITE CHARACTERTIC FOR ERROR RETURNS
; NOTE:  WE PRESERVE ORIGINAL MANTISSA SIGN
; ON ENTRY D CONTAINS NEW CHARACTERTISTIC TO BE STORED.
;
WCHAR:  INR     L               ;SET (H,L) TO POINT TO CHARACTERISTIC
        INR     L               ;PART OF ABOVE
        INR     L               ;PART OF ABOVE
        MOV     A,M             ;LOAD CHARACTERISTIC A
                                ;AND MANTISSA SIGN
        ANI     80H             ;JUST KEEP MANTISSA SIGN
        ORA     D               ;OR IN NEW CHARACTERISTIC
        MOV     M,A             ;STORE IT BACK
        RET                     ;RETURN WITH (H,L) POINT TO CHARACTERISTIC
                                ;OF RESULT
                                ;SOMEONE ELSE WILL FIX UP (H,L)
;
;   SUBROUTINE INDFC
;
;       THIS ROUTINE WRITES A FLOATING INDEFINITE, SETS
;       THIS WRITES WRITES A FLOATING POINT INDEFINITE
;       AT (H,C), SETS THE CONDITION FLAG AND RETURNS
;
;
INDFC:  MOV     E,L             ;SAVE LPTR IN E
        MOV     L,C             ;SET L=CPTR SO (H,L)-ADDR OF RESULT
        CALL    WIND            ;WRITE INDEFINITE
        MOV     L,E             ;RESTORE L=LPTR
        RET                     ;RETURN
;
;
;   SUBROUTINE WZERC
;
;       THIS ROUTINE WRITES A NORMAL FLAOTING POINT ZERO
;       AT (H,C), SETS THE CONDITION FLAG AND RETURNS
;
WZERC:  MOV     E,L             ;SAVE LPTR IN E
        MOV     L,C             ;SETL=CPTR SO (H,L)=ADDR OF RESULT
        CALL    WZER            ;WRITE NORMAL ZERO
        MOV     L,E             ;RESTORE L=LPTR
        RET                     ;RETURN
;
;   SUBROUTINE INCR
;
;       THIS SUBROUTINE INCREMENTS THE CHARACTERISTIC
;       OF THE FLOATING POINT NUMBER POINTED TO BY (H,L).
;       WE TEST FOR OVERFLOW AND SET APPROPRIATE FLAG.
;       (SEE ERRROR RETURNS).
;
;   REGISTERS ON EXIT:
;
;        A = CONDITION FLAG (SEE ERROR RETURNS)
;        D = CLOBBERED
;        B,C,H,L = SAME AS ON ENTRY
;
INCR:   CALL    GCHAR           ;GET CHAR WITH SIGN EXTENDED
        CPI     4FH             ;COMPARE WITH MAX CHAR PERMITTED
        JZ      OFLW1           ;INCREMENT WOULD CAUSE OVERFLOW
        MOV     D,A             ;SAVE IT IN D
        INR     D               ;INCREMENT IT
        JMP     INCR2           ;JUMP AROUND ALTERNATE ENTRY POINT
INCR3:  INR     L               ;COME HERE TO STORE CHARACTERISTIC
        INR     L               ;POINT (H,L) TO CHAR
        INR     L               ;POINT (H,L) TO CHAR
INCR2:  MVI     A,7FH 
        ANA     D               ;KILL SIGN BIT
        MOV     D,A             ;BACK TO D
        MOV     A,M             ;NOW SIGN IT
        ANI     80H             ;GET MANTISSA SIGN
        ORA     D               ;PUT TOGETHER
        MOV     M,A             ;STORE IT BACK
        DCR     L               ;NOW BACK TO BASE
        DCR     L               ;/***TP
        DCR     L
SCCFG:  XRA    A                ;SET SUCCESS FLAG
        RET
;
;   SUBROUTINE DECR
;
;       THIS SUBROUTINE DECREMENTS THE CHARACTERISTIC
;       OF THE FLOATING POINT NUMBER POINTED TO BY (H,L).
;       WE TEST FOR UNDERFLOW AND SET APPROPRIATE FLAG.
;       (SEE ERRROR RETURNS).
;
;   REGISTERS ON EXIT:
;
;        A = CONDITION FLAG (SEE ERROR RETURNS)
;        D = CLOBBERED
;        B,C,H,L = SAME AS ON ENTRY
;
DECR:   CALL    GCHAR           ;GET CHAR WITH SIGN EXTENDED
        CPI     C0H             ;COMPARE WITH MIN CHAR PERMITTED
        JZ      UFLW1           ;DECREMENT WOULD CAUSE UNDERFLOW
        MOV     D,A             ;SAVE CHARACTERSTIC IN D
        DCR     D               ;DECREMENT CHARACTERISTIC
        JMP     INCR2           ;GO STORE IT BACK
;
;   SUBROUTINE AORS
;
;       RETURN S=1 IF BASE 6
;       HAS A 1 IN MSB
;
AORS:   MOV     E,L             ;SAVE BASE
        MOV     L,C             ;BASE 6 TO L
        MOV     A,M             ;LOAD IT
        ORA     A               ;SET FLAGS
        MOV     L,E             ;RESTORE BASE
        RET
;
;
;   SUBROUTINE TSTR
;
;       CHECKS C PTR TO SEE IF
;       NLSB !
;       RETURNS Z=1 IF NOT
;       DESTROYS E,D
;
TSTR:   MOV     E,L             ;SAVE BASE
        MOV     L,C             ;C PTR TO L
        MVI     D,2             ;MASK TO D
        MOV     A,M             ;LOAD VALUE
        MOV     L,E             ;RESTORE BASE
        ANA     D               ;AND VALUE WITH MASK
        RET
;
;
;   SUBROUTINE ACPR
;
;       STORES A IN LOCATION OF CPTR
;       LPTR IN E
;
ACPR:   MOV     E,L             ;SAVE LPTR
        MOV     L,C             ;CPTR TO L
        MOV     M,A             ;STORE A
        MOV     L,E             ;RESTORE BASE
        RET
;
;
;   SUBROUTINE DCMP
;
;       COMPARES TWO DOUBLE LENGTH
;       WORDS
;
DCMP:   MOV     A,M             ;NUM MANTA TO A
        MOV     E,L             ;SAVE BASE IN E
        MOV     L,B             ;BASE 3 TO L
        CMP     M               ;COMPARE WITH DEN MANTA
        MOV     L,E             ;RETURN BASE TO L
        RNZ                     ;RETURN IF NOT THE SAME
        INR     L               ;L TO NUM MANTB
        MOV     A,M             ;LOAD IT
        MOV     L,B             ;DEN MANTB ADD TO L
        INR     L               ;BASE 4 TO L
        CMP     M
        MOV     L,E
        RNZ                     ;/***TP EXTENSION
        INR     L               ;NOW CHECK BYTE 3
        INR     L
        MOV     A,M             ;GET FOR COMPARE
        MOV     L,B
        INR     L
        INR     L               ;BYTE 3 NOW
        CMP     M               ;COMPARE
        MOV     L,E             ;/***TP - ALL DONE
        RET
;
;
;   SUBROUTINE DIVC
;
;       PERFORMS ONE CYCLE OF DOUBLE
;       PRECISION FLOATING PT DIVIDE
;       ENTER AT ENT1 ON FIRST CYCLE
;       ENTER AT ENT2 ALL THEREAFTER
;
ENT2:   CALL    DLST            ;SHIFT MOVING DIVIDEND
        JC      OVER            ;IF CARRY=1,NUM.GT.D
ENT1:   CALL    DCMP            ;COMPARE NUM WITH DEN
        JNC     OVER            ;IF CARRY NOT SET,NUM.GE.DEN
        RET
OVER:   CALL    DSUB            ;CALL DOUBLE SUBTRACT
        MOV     E,L             ;SAVE BASE IN E
        MOV     L,C             ;BASE 6 TO L
        INR     L               ;BASE 7 TO L
        INR     L               ;/***TP
        MOV     A,M
        ADI     1               ;ADD 1
        MOV     M,A             ;PUT IT BACK
        MOV     L,E             ;RESTORE BASE TO L
        RET
;
;
;   SUBROUTINE LXFR
;
;       MOVES CPTR TO EPTR
;       MOVES 3 WORDS IF ENTER AT LXFR
;
LXFR:   MVI     D,4             ;MOVE 4 WORDS
REP5:   MOV     L,C             ;CPTR TO L
        MOV     A,M             ;CPTR> TO A
        MOV     L,E             ;EPTR TO L
        MOV     M,A
        INR     C               ;INCREMENT C
        INR     E               ;INCREMENT E TO NEXT
        DCR     D               ;TEST FOR DONE
        JNZ     REP5            ;GO FOR FOR TILL D=0
        MOV     A,E             ;NOW RESET C AND E
        SUI     4               ;RESET BACK BY 4
        MOV     E,A             ;PUT BACK IN E
        MOV     A,C             ;NOW RESET C
        SUI     4               ;BY 4
        MOV     C,A             ;BACK TO C
        RET                     ;DONE
;
;   SUBROUTINE LDCP
;
;       THIS SUBROUTINE COMPUTES THE CHARACTERISTIC
;       FOR THE FLOATING DIVIDE ROUTINE
;
;   REGISTERS ON EXIT:
;
;       A = CONDITION FLAG (SEE ERROR RETURNS)
;       D,E = GARBAGE
;       B,C,H,L = SAME AS ON ENTRY
;
;   REGISTERS ON ENTRY:
;
;       (H,B) = ADDRESS OFF DIVISOR
;       (H,C) = ADDRESS OF QUOTIENT
;       (H,L) = ADDRESS OF DIVIDEND
;
LDCP:   CALL    CFCHE           ;SET E=CHAR(H,B), A=CHAR(H,L)
        SUB     E               ;SUBTRACT TO GET NEW CHARACTERISTIC
        JMP     CCHK            ;GO CHECK FOR OVER/UNDERFLOW
                                ;AND STORE CHARACTERTISTIC
;
;
;   SUBROUTINE LMCP
;
;       THIS SUBROUTINE COMPUTES THE CHARACTERISTIC
;       FOR THE FLOATING MULTIPLY ROUTINE.
;
;   REGISTERS ON EXIT:
;
;       A = CONDITION FLAG (SEE ERROR RETURNS)
;       D,E = GARBAGE
;       B,C,H,L = SAME AS ON ENTRY
;
;   REGISTERS ON ENTRY:
;
;       (H,B) = ADDRESS OFF MULTIPLICAND
;       (H,C) = ADDRESS OF PRODUCT
;       (H,L) = ADDRESS OF MULTIPLIER
;
LMCP:   CALL    CFCHE           ;SET E=CHAR(H,B), A=CHAR(H,L)
        ADD     E               ;ADD TO GET NEW CHARACTERISTIC
                                ;NOW FALL INTO THE ROUTINE
                                ;WHICH CHECKS FOR OVER/UNDERFLOW
                                ;AND STORE CHARACTERTISTIC
;
;
;   SBUROUTINE CCHK
;
;       THIS SUBROUTINE CHECKS A CHARACTERISTIC IN
;       THE ACCUMULATOR FOR OVERFLOW OR UNDERFLOW.
;       IT THEN STORES THE CHARACTERISTIC, PRESERVING
;       THE PREVIOUSLY COMPUTED MANTISSA SIGN.
;
;  REGISTERS ON ENTRY:
;
;       (H,L) = ADDRESS OF ONE OPERAND
;       (H,B) = ADDRESS OF OTHER OPERAND
;       (H,C) = ADDRESS OF RESULT
;       A     = NEW CHARACTERISTIC OF  RESULT
;
;   REGISTERS ON EXIT:
;
;       A = CONDITION FLAG (SEE ERROR RETURNS)
;       D,E = GARBAGE
;       B,C,H,L = SAME AS ON ENTRY
;
CCHK:                           ;ENTER HERE TO CHECK CHARACTERISTIC
        CPI     40H             ;CHECK FOR 0 TO +63
        JC      STORC           ;JUMP IF OKAY
        CPI     80H             ;CHECK FOR +64 TO +127
        JC      OFLWC           ;JUMP IF OVERFLOW
        CPI     C0H             ;CHECK FOR -128 TO -65
        JC      UFLWC           ;JUMP IF UNDERFLOW
STORC:  MOV     E,L             ;SAVE L IN E
        MOV     L,C             ;LET L POINT TO RESULT
        MOV     D,A             ;SAVE CHARACTERISTIC IN D
        CALL    INCR3           ;STORE CHARACTERISTIC
        MOV     L,E             ;RESTORE L
        RET                     ;RETURN
;
;   SUBROUTINE OFLWC
;
;       THIS ROUTINE WRITES A FLOATING POINT OVERFLOW AT (H,C)
;       SETS THE CONDITION FLAG, AND RETURNS.
;
OFLWC:  MOV     E,L             ;SAVE L IN E
        MOV     L,C             ;SET L=CPTR, SO (H,L)=ADDR OF RESULT
        CALL    WOVR            ;WRITE OUT OVERFLOW
        MOV     L,E             ;RESTORE L
        RET                     ;RETURN
;
;   SUBROUTINE UFLWC
;
;       THIS ROUTINE WRITES A FLOATING POINT UNDERFLOW AT (H,C)
;       SETS THE CONDITION FLAG, AND RETURNS.
;
UFLWC:  MOV     E,L             ;SAVE L IN E
        MOV     L,C             ;SET L=CPTR, SO (H,L)=ADDR OF RESULT
        CALL    WUND            ;WRITE OUT UNDEFLOW
        MOV     L,E             ;RESTORE L
        RET                     ;RETURN
;
;
;   SUBROUTINE CSIGN
;
;       THIS SUBROUTINE COMPUTES AND STORE THE MANTISSA
;       SIGN FOR THE FLOATING MULTIPLY AND DIVIDE ROUTINES
;
;   REGISTERS ON ENTRY:
;
;       (H,L) = ADDRESS OF ONE OPERAND
;       (H,B) = ADDRESS OF OTHER OPERAND
;       (H,C) = ADDRESS OF RESULT
;
;   REGISTERS ON EXIT:
;
;       A,D,E = GARBAGE
;       B,C,H,L = SAME AS ON ENTRY
;
;
CSIGN:  CALL    MSFH            ;SET A=SIGN(H,L), E=SIGN(H,B)
        XRA     E               ;EXCLUSIVE OR SIGNS TO GET NEW SIGN
        CALL    CSTR            ;STORE SIGN INTO RESULT
        RET                     ;RETURN
;
;
;   SUBROUTINE CSTR
;
;       STORES VALUE IN A IN
;       CPTR 2
;       PUTS LPTR IN E
;
CSTR:   MOV     E,L             ;SAVE LPTR IN E
        MOV     L,C             ;CPTR TO L
        INR     L               ;CPTR\2
        INR     L               ;TO L
        INR     L               ;/***TP
        MOV     M,A             ;STORE ANSWER
        MOV     L,E             ;LPTR BACK TO L
        RET
;
;   SUBROUTINE MSFH
;
;       THIS SUBROUTINE FETCHES THE SIGNS OF THE MANTISSAS
;       OF THE FLOATING POINT NUMBERS POINTED TO BY (H,L)
;       AND (H,B) INTO THE A AND E REGISTERS RESPECTIVELY.
;
;   REGISTERS ON EXIT:
;
;       A = SIGN  OF MANTISSA OF (H,L)
;       E = SIGN OF MANTISSA OF (H,B)
;       B,C,D,H,L = SAME AS ON ENTRY
;
MSFH:   MOV     E,L             ;SAVE LPTR
        MOV     L,B             ;BPTR TO L
        INR     L               ;BPTR\2
        INR     L               ;/***TP
        INR     L               ;TO L
        MOV     A,M             ;BPTR 2>TO A
        ANI     128             ;SAVE MANT SIGN
        MOV     L,E             ;LPTR BACK TO L
        MOV     E,A             ;STORE BPTR MANT SIGN
        INR     L               ;LPTR\2
        INR     L               ;/***TP
        INR     L               ;TO L
        MOV     A,M             ;LPTR\2>TO A
        ANI     128             ;SAVE LPTR MANT SIGN
        DCR     L               ;LPTR BACK
        DCR     L               ;TO L
        DCR     L               ;/***TP
        RET
;
;
;   SUBROUTINE BCTL
;
;           MOVES BPTR CHAR TO LPTR CHAR
;           DESTROYS E
;
BCTL:   MOV     E,L             ;LPTR TO E
        MOV     L,B             ;BPTR TO L
        INR     L               ;BPTR 2
        INR     L               ;/***TP
        INR     L               ;TO L
        MOV     A,M             ;BPTR CHAR TO A
        MOV     L,E             ;LPTR TO L
        INR     L               ;LPTR 2
        INR     L               ;TO L
        INR     L               ;/***TP
        MOV     M,A             ;STORE BPTR CHAR IN LPTR CHAR
        MOV     L,E             ;LPTR TO L
        RET
