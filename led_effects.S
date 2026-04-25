.global led_animation

.equ DDRD, 0x0A
.equ PORTD, 0x0B

led_animation:
    sbi DDRD, 2     ; Set D2 (PD2) as output
    sbi DDRD, 3     ; Set D3 (PD3) as output
    sbi DDRD, 4     ; Set D4 (PD4) as output
    sbi DDRD, 5     ; Set D5 (PD5) as output

    ; LED animation sequence
    sbi PORTD, 2
    rcall delay
    cbi PORTD, 2
    sbi PORTD, 3
    rcall delay
    cbi PORTD, 3
    sbi PORTD, 4
    rcall delay
    cbi PORTD, 4
    sbi PORTD, 5
    rcall delay
    cbi PORTD, 5
    ret

delay:
    ldi r18, 200
loop1:
    ldi r19, 255
loop2:
    nop
    dec r19
    brne loop2
    dec r18
    brne loop1
    ret
