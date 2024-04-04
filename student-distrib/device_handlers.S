.globl RTC_HANDLER,KEYBOARD_HANDLER

;/* void RTC_HANLDER();
; * Inputs: None
; * Return Value: None
; * Function: Saves flags and registers, calls rtc_handler,
; *           restores flags and registers
; */
RTC_HANDLER:
    pushal
    pushfl

    call rtc_handler

    popfl
    popal

    iret

;/* void KEYBOARD_HANLDER();
; * Inputs: None
; * Return Value: None
; * Function: Saves flags and registers, calls keyboard_handler,
; *           restores flags and registers
; */
KEYBOARD_HANDLER:
    pushal
    pushfl

    call keyboard_handler

    popfl
    popal

    iret


.end
