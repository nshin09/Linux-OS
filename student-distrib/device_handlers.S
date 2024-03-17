.globl RTC_HANDLER

RTC_HANDLER:
    pushal
    pushfl

    call rtc_handler

    popfl
    popal

    iret

.end