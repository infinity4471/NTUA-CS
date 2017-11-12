.ORIG x3000

IN

LD R1, NUM ; R1 is set to constantly keep the value of NUM

LD R4, msg_ptr
ADD R4, R4, #-1 ; setting our register R4 one step behind the address of the message

ITERATION

ADD R4, R4, #1
LDR R3, R4, #0 ; R3 contains the iterated character

BRz DONE

AND R5, R5, #0
ADD R5, R5, R3
NOT R5, R5
ADD R5, R5, #1

ADD R6, R5, R0

; 18-24 -> checking if the character we are currently processing is equal to the input character

BRnp ITERATION ; if the character is not equal to the input go to the next one

ADD R6, R3, #-16
ADD R6, R6, #-16
ADD R6, R6, #-16
ADD R6, R6, #-15 ; check if current character is uppercase

BRn NUMERICAL_CHECK

ADD R6, R6, #-16
ADD R6, R6, #-9 ; continuation of uppercase check

BRp NUMERICAL_CHECK ; line 33 and 38 goes to check if it is numerical if uppercase check fails

JSR TO_LOWER

STR R1, R4, #0 ; it is indeed uppercase, so change it to lowercase

BR ITERATION ; continue

NUMERICAL_CHECK

AND R6, R6, #0
ADD R6, R3, #-16
ADD R6, R6, #-16
ADD R6, R6, #-16

BRn ITERATION

ADD R6, R6, #-9

BRp ITERATION ; lines 46-57 check if our character is numerical

ADD R0, R0, #-16
ADD R0, R0, #-16
ADD R0, R0, #-16 ; transform R0 to a numerical value ( 0 - 9 )

JSR NUM_EXPR ; calculate the value x^2 and store it in R2

ADD R2, R2, #15
ADD R2, R2, #15
ADD R2, R2, #15
ADD R2, R2, #3
    
ADD R0, R0, #15
ADD R0, R0, #15
ADD R0, R0, #15
ADD R0, R0, #3 ; lines 65 - 73 change R2 and the character in R0 to ascii encoding

STR R2, R4, #0 ; replace the character in the string with x^2 % n

BR ITERATION

DONE

HALT
END

NUM_EXPR ; given x, n calculates x ^ 2
         ; X in R0, n in R1, result in R2

    AND R2, R2, #0
    AND R3, R3, #0
    ADD R3, R3, R0

    LOOP_1

        ADD R2, R2, R0
        ADD R3, R3, #-1

    BRp LOOP_1 ; implementing multiplication with adding

    ; we have to keep the value of register R7 to return from the subroutine

    ADD R6, R7, #0

    JSR MODULO
    
    JMP R6

MODULO ; given x, y calculates x mod y 
       ; x in R2, y in R1, result in R2

    NOT R1, R1
    ADD R1, R1, #1

    LOOP_2 ; this loop performs repeated substractions until we have reached the value of the modulo
        AND R3, R3, #0
        ADD R3, R2, R1

        BRn EXIT

        ADD R2, R2, R1
    BR LOOP_2

    EXIT

    NOT R1, R1
    ADD R1, R1, #1
    RET

TO_LOWER ;given an uppercase character x stored in R0 it changes it to lowercase
         ; add 32, store result in R1

    AND R1, R1, #0
    ADD R1, R0, #15
    ADD R1, R1, #15
    ADD R1, R1, #2

    RET

message .STRINGZ "M4MMM4MMAM4MAA4MAM4MMMAMAMMAM"
NUM .FILL #4
msg_ptr .FILL message

.END
