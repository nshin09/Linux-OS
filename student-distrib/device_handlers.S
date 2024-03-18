.globl RTC_HANDLER,KEYBOARD_HANDLER

RTC_HANDLER:
    pushal
    pushfl

    call rtc_handler

    popfl
    popal

    iret

KEYBOARD_HANDLER:
    pushal
    pushfl

    call keyboard_handler

    popfl
    popal

    iret


.end