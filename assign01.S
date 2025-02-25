#include "hardware/regs/addressmap.h"
#include "hardware/regs/io_bank0.h"
#include "hardware/regs/timer.h"
#include "hardware/regs/m0plus.h"

.syntax unified
.cpu    cortex-m0plus
.thumb
.global main_asm
.align  4
.extern printf

.equ    DFLT_STATE_STRT, 1              @ Specify the value to start flashing
.equ    DFLT_STATE_STOP, 0              @ Specify the value to stop flashing
.equ    DFLT_ALARM_TIME, 1000000        @ Specify the default alarm timeout

.equ    GPIO_BTN_DN_MSK, 0x00040000     @ Bit-18 for falling-edge event on GP20
.equ    GPIO_BTN_EN_MSK, 0x00400000     @ Bit-22 for falling-edge event on GP21
.equ    GPIO_BTN_UP_MSK, 0x04000000     @ Bit-26 for falling-edge event on GP22

.equ    GPIO_BTN_DN,  20                @ Specify pin for the "down" button
.equ    GPIO_BTN_EN,  21                @ Specify pin for the "enter" button
.equ    GPIO_BTN_UP,  22                @ Specify pin for the "up" button
.equ    GPIO_LED_PIN, 25                @ Specify pin for the built-in LED
.equ    GPIO_DIR_IN,   0                @ Specify input direction for a GPIO pin
.equ    GPIO_DIR_OUT,  1                @ Specify output direction for a GPIO pin

.equ    LED_VAL_ON,    1                @ Specify value that turns the LED "on"
.equ    LED_VAL_OFF,   0                @ Specify value that turns the LED "off"

.equ    GPIO_ISR_OFFSET, 0x74           @ GPIO is int #13 (vector table entry 29)
.equ    ALRM_ISR_OFFSET, 0x40           @ ALARM0 is int #0 (vector table entry 16)

main_asm:   
    bl      init_leds                   @ Same as previous labs
    bl      init_btns                   @ Use asm_gpio_set_irq C function to setup falling-edge interrutps
    bl      install_alrm_isr            @ See below
    bl      install_gpio_isr            @ See below
loop:       
    bl      set_alarm                   @ Set a new alarm
    wfi                                 @ Wait here until any interrupt fires
    ldr     r0, =led_state              @ get the string at "led_state"
    ldr     r1, =lstate                 @ get the LED's current flashing state
    ldr     r1, [r1]                    @ 
    bl      printf                      @ print the string "led_state"
    ldr     r0, =timer_state            @ get the string at "timer_state"
    ldr     r1, =ltimer                 @ get the LED's flashing delay
    ldr     r1, [r1]                    @ 
    bl      printf                      @ print the string
    b       loop                        @ Always branch back to loop


init_leds:
    push    {lr}                        @ Store the link register
    movs    r0, #GPIO_LED_PIN           @ This value is the GPIO LED pin on the PI PICO board
    bl      asm_gpio_init               @ Call the subroutine to initialise the GPIO pin specified by r0
    movs    r0, #GPIO_LED_PIN           @ This value is the GPIO LED pin on the PI PICO board
    movs    r1, #GPIO_DIR_OUT           @ We want this GPIO pin to be setup as an output pin
    bl      asm_gpio_set_dir            @ Call the subroutine to set the GPIO pin specified by r0 to state specified r1
    pop     {pc}                        @ Pop the link register to the program counter
    
init_btns:
    push    {lr}                        @ Store the link register
    movs    r0, #GPIO_BTN_DN            @ This value is the GPIO button 20 pin on the PI PICO board
    bl      asm_gpio_init               @ Call the subroutine to initialise the GPIO pin specified by r0
    movs    r0, #GPIO_BTN_DN            @ This value is the GPIO button 20 pin on the PI PICO board
    bl      asm_gpio_set_irq            @ set the irq for this button
    movs    r0, #GPIO_BTN_DN            @ This value is the GPIO button 20 pin on the PI PICO board
    movs    r1, #GPIO_DIR_IN            @ We want this GPIO pin to be setup as an input pin
    bl      asm_gpio_set_dir            @ Call the subroutine to set the GPIO pin specified by r0 to state specified r1
        
    movs    r0, #GPIO_BTN_EN            @ This value is the GPIO button 21 pin on the PI PICO board
    bl      asm_gpio_init               @ Call the subroutine to initialise the GPIO pin specified by r0
    movs    r0, #GPIO_BTN_EN            @ This value is the GPIO button 21 pin on the PI PICO board
    bl      asm_gpio_set_irq            @ set the irq for this button
    movs    r0, #GPIO_BTN_EN            @ This value is the GPIO button 21 pin on the PI PICO board
    movs    r1, #GPIO_DIR_IN            @ We want this GPIO pin to be setup as an input pin
    bl      asm_gpio_set_dir            @ Call the subroutine to set the GPIO pin specified by r0 to state specified r1
        
    movs    r0, #GPIO_BTN_UP            @ This value is the GPIO button 22 pin on the PI PICO board
    bl      asm_gpio_init               @ Call the subroutine to initialise the GPIO pin specified by r0
    movs    r0, #GPIO_BTN_UP            @ This value is the GPIO button 22 pin on the PI PICO board
    bl      asm_gpio_set_irq            @ set the irq for this button
    movs    r0, #GPIO_BTN_UP            @ This value is the GPIO button 22 pin on the PI PICO board
    movs    r1, #GPIO_DIR_IN            @ We want this GPIO pin to be setup as an input pin
    bl      asm_gpio_set_dir            @ Call the subroutine to set the GPIO pin specified by r0 to state specified r1
    
    pop     {pc}                        @ Pop the link register to the program counter


//
// Enable alarm timer interrupts and set an alarm
//
set_alarm:
    // Enable alarm timer interrupts using the (TIMER_BASE + TIMER_INTE_OFFSET) register
    // Get the current timer count from (TIMER_BASE + TIMER_TIMELR_OFFSET) register
    // Add the time delay you want to wait for to the current timer count
    // Push the updated value to (TIMER_BASE + TIMER_ALARM0_OFFSET)
    // Exit subroutine

    ldr     r2, =(TIMER_BASE + TIMER_INTE_OFFSET)       @ Get the register at the timer at the INTE offset
    ldr     r3, =1                                      @ Get the value to enable timer interrupts
    str     r3, [r2]                                    @ Enable timer interrupts

    ldr     r2, =(TIMER_BASE + TIMER_TIMELR_OFFSET)     @ Get the register at the timer at the TIMELR offset
    ldr     r1, [r2]                                    @ 
    ldr     r3, =ltimer                                 @ Get the value in ltimer
    ldr     r3, [r3]                                    @ 
    adds    r1, r3                                      @ Add the time delay in ltimer to the timer
    ldr     r2, =(TIMER_BASE + TIMER_ALARM0_OFFSET)     @ 
    str     r1, [r2]                                    @ Push the updated value back

    bx      lr                                          @ Exit subroutine

//
// Sets up the alrm_isr in the RAM vector table and enables the correct interrupt
//
install_alrm_isr:
    // Get the address of the RAM vector table using the (PPB_BASE + M0PLUS_VTOR_OFFSET) register
    // Store the address of the alrm_isr handler to the correct offset for ALARM0 in the vector table
    // Disable the ALARM0 IRQ by writing the correct value to (PPB_BASE + M0PLUS_NVIC_ICPR_OFFSET)
    // Enable the ALARM0 IRQ by writing the correct value to (PPB_BASE + M0PLUS_NVIC_ISER_OFFSET)
    // Exit subroutine
    ldr     r2, =(PPB_BASE + M0PLUS_VTOR_OFFSET)        @ load in the ALARM timer into r2
    ldr     r1, [r2]                                    @ load r2 into r1 
    movs    r2, #ALRM_ISR_OFFSET                        @ load in the ISR offset
    add     r2, r1                                      @ offset the ALARM timer by the offset
    ldr     r0, =alrm_isr                               @ Get the address of the alrm_isr handler
    str     r0, [r2]                                    @ Store SVC number

    movs    r3, #1                                      @ Load in the value to disable and anable the alarm
    ldr     r2, =(PPB_BASE + M0PLUS_NVIC_ICPR_OFFSET)   @ Get the alarm offset by the ICPR offset
    str     r3, [r2]                                    @ Disable the ALARM0 IRQ
    ldr     r2, =(PPB_BASE + M0PLUS_NVIC_ISER_OFFSET)   @ Get the alarm offset by the ISER offset
    str     r3, [r2]                                    @ Enable the ALARM0 IRQ

    bx      lr                                          @ Return from subroutine

//
// Service the pending interrupt from the ALARM0 TIMER
//
.thumb_func
alrm_isr:
    // Perform required functionality (e.g. toggle the LED)
    // Disable pending interrupt from TIMER by writing correct value to (TIMER_BASE + TIMER_INTR_OFFSET)
    // Exit ISR
    push {lr}

    ldr r2, =(TIMER_BASE + TIMER_INTR_OFFSET)       @ Get timer register
    movs r1, #1                                     @ 
    str r1, [r2]                                    @ Disable alarm interrupt

    ldr r1, =lstate                                 @ Get flashing state of the LED
    ldr r1, [r1]                                    @ 
    cmp r1, #DFLT_STATE_STOP                        @ If the LED isn't flashing,
    beq end_alarm                                   @ leave it off.
    bl sub_toggle                                   @ Otherwise, toggle the LED
end_alarm:
    pop {pc}


// 
// Toggle the LED either off (if it's on) or on (if it's off)
// 
sub_toggle:
    push    {lr}                                @ Store the link register to the stack as we will call nested subroutines
    ldr     r2, =(TIMER_BASE+TIMER_INTR_OFFSET) @ Load timer base and offset it
    movs    r1, #1                              @ 
    str     r1, [r2]                            @ Disable timer interrupt
    
    ldr    r0, =GPIO_LED_PIN                    @ Set the LED GPIO pin number to r0 for use by asm_gpio_get
    bl      asm_gpio_get                        @ Get current the value of the LED GPIO pin (returns to r0)
    cmp     r0, #LED_VAL_OFF                    @ Check if the LED GPIO pin value is "off"
    beq     led_set_on                          @ If it is "off" then then jump code to to turn it on
led_set_off:            
    ldr    r1, =LED_VAL_OFF                     @ The LED is currently "on" so we want to turn it "off"
    b       led_set_state                       @ Jump to portion of code where we set the state of the LED
led_set_on:         
    ldr    r1, =LED_VAL_ON                      @ The LED is currently "off" so we want to turn it "on"
led_set_state:          
    ldr    r0, =GPIO_LED_PIN                    @ Set the LED GPIO pin number to r0 for use by asm_gpio_put
    bl      asm_gpio_put                        @ Update the the value of the LED GPIO pin (based on value in r1)
    pop     {pc}                                @ Pop the link register from the stack to the program counter

    

//
// Sets up the gpio_isr in the RAM vector table and enables the correct interrupt
//
install_gpio_isr:
    // Get the address of the RAM vector table using the (PPB_BASE + M0PLUS_VTOR_OFFSET) register
    // Store the address of the gpio_isr handler to the correct offset for GPIO in the vector table
    // Disable the GPIO IRQ by writing the correct value to (PPB_BASE + M0PLUS_NVIC_ICPR_OFFSET)
    // Enable the GPIO IRQ by writing the correct value to (PPB_BASE + M0PLUS_NVIC_ISER_OFFSET)
    // Exit subroutine
    ldr     r2, =(PPB_BASE + M0PLUS_VTOR_OFFSET)        @ load in the ALARM timer into r2 
    ldr     r1, [r2]                                    @ load r2 into r1 
    movs    r2, #GPIO_ISR_OFFSET                        @ load in the ISR offset
    adds    r2, r1                                      @ offset the ALARM timer by the offset
    ldr     r0, =gpio_isr                               @ Get the address of the gpio_isr handler
    str     r0, [r2]                                    @ Store SVC number

    ldr     r3, =1
    lsls    r3, #13
    ldr     r2, =(PPB_BASE + M0PLUS_NVIC_ICPR_OFFSET)   @ load in the base plus the ICPR offset
    str     r3, [r2]
    ldr     r2, =(PPB_BASE + M0PLUS_NVIC_ISER_OFFSET)   @ load in the base plus the ISER offset
    str     r3, [r2]

    bx      lr                                          @ Return from subroutine

//
// Service the pending interrupt from the GPIO
//
.thumb_func                     @ Required for all interrupt service routines
gpio_isr:
    // Read the interrupt status event from the (IO_BANK0_BASE + IO_BANK0_PROC0_INTS2_OFFSET) register
    // Detect which button was pressed by comparing to GPIO_BTN_DN_MSK, GPIO_BTN_EN_MSK and GPIO_BTN_UP_MSK
    // Perform required functionality based on the button press event that was detected
    // Disable the pending interrupt from GPIO by writing the correct value to (IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)
    // Exit ISR
    push    {lr}                                                    @ Store the link register to the stack as we will call nested subroutines
	ldr     r2, =(IO_BANK0_BASE + IO_BANK0_PROC0_INTS2_OFFSET)      @ load in the interrupt status into r2
    ldr     r2, [r2]                                                @ 
	ldr 	r5, =GPIO_BTN_DN_MSK	                                @ GP20 is pressed -  halve interval
	ldr 	r6, =GPIO_BTN_EN_MSK	                                @ GP21 is pressed -  toggle flashing
	ldr 	r7, =GPIO_BTN_UP_MSK	                                @ GP22 is pressed -  double interval

@ Check if GPIO20 was pressed
    movs    r1, r2                                                  @ Create a copy of r2
    ands    r1, r5                                                  @ Check if r1 AND r5 returns r5
	cmp 	r1, r5                                                  @ If it does, then GP20 was pressed
	bne 	checkDouble				                                @ if GP20 is pressed,
	ldr		r3, =lstate                                             @ Get the LED's flashing state
    ldr     r0, [r3]                                                @ 
    cmp     r0, #DFLT_STATE_STRT                                    @ Check if the LED is flashing or not
    beq     halve_delay                                             @ If it ism jump to 'halve_delay'
    ldr     r2, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)            @ Otherwise, 
    str     r5, [r2]                                                @ disable the pending interrupt

    ldr     r0, =gpio20_pressed_flashing_off                        @ If the GP20 is pressed while LED isn't flashing,
    bl      printf                                                  @ print text to the serial output,

    b       reset_delay                                             @ and (jump to) reset the delay.
halve_delay:
	ldr		r3, =ltimer	                                            @ load the alarm interval
    ldr     r0, [r3]                                                @ 
	lsrs 	r0, r0, #1                                              @ lsr (halve) the interval once
	str 	r0, [r3]				                                @ store the value back into the interval
    ldr     r2, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)            @ 
    str     r5, [r2]                                                @ disable the pending interrupt

    ldr     r0, =gpio20_pressed_flashing_on                         @ If GP20 is pressed while the LED is flashing,
    ldr     r1, =ltimer                                             @ get the delay
    ldr     r1, [r1]                                                @ put the delay into r1
    bl      printf                                                  @ print a formatted string using r1

    b       gpio_return                                             @ exit the subroutine.

@ Check if GPIO22 was pressed
checkDouble:
    movs    r1, r2                                                  @ Create a copy of r2
    ands    r1, r7                                                  @ Check if r1 AND r7 returns r7
	cmp 	r1, r7                                                  @ If it does, then GP22 was pressed
	bne 	checkToggle				                                @ if GP20 is pressed,
    ldr		r3, =lstate                                             @ Get the LED's flashing state
    ldr     r0, [r3]                                                @ 
    cmp     r0, #DFLT_STATE_STRT                                    @ Check if the LED is flashing or not
    beq     double_delay                                            @ If it ism jump to 'halve_delay'
    ldr     r2, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)            @ Otherwise, 
    str     r7, [r2]                                                @ disable the pending interrupt

    ldr     r0, =gpio22_pressed_flashing_off                        @ If the GP22 is pressed while LED isn't flashing,
    bl      printf                                                  @ print text to the serial output,

    b       reset_delay                                             @ and (jump to) reset the delay.
double_delay:
	ldr		r3, =ltimer	                                            @ load the alarm interval
    ldr     r0, [r3]                                
	lsls 	r0, r0, #1				                                @ lsl (double) the interval once
	str 	r0, [r3]				                                @ store the value back into the interval
    ldr     r2, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)            @ 
    str     r7, [r2]                                                @ disable the pending interrupt
                                                                
    ldr     r0, =gpio22_pressed_flashing_on                         @ If GP22 is pressed while the LED is flashing,
    ldr     r1, =ltimer                                             @ get the delay
    ldr     r1, [r1]                                                @ put the delay into r1
    bl      printf                                                  @ print a formatted string using r1
    
    b       gpio_return                                             @ exit the subroutine.

@ Check if GPIO21 was pressed
checkToggle:
    movs    r1, r2                                                  @ Create a copy of r2
    ands    r1, r6                                                  @ Check if r1 AND r6 returns r6
	cmp 	r1, r6                                                  @ If it does, then GP21 was pressed
	bne 	gpio_return				                                @ if GP21 is pressed,
    ldr		r4, =lstate                                             @ get the LED's current state
    ldr     r0, [r4]                                                @ 
    cmp     r0, #DFLT_STATE_STRT                                    @ if the LED is on
    beq     gpio_led_set_off                                        @ turn it off
gpio_led_set_on:                                                    @ otherwise, turn it on
    movs    r1, #LED_VAL_ON                                         @ The LED is currently "off" so we want to turn it "on"
    str     r1, [r4]                                                @ store the "on" value into the LED's address

    ldr     r0, =gpio21_pressed_turned_on                           @ If the GP21 is pressed while LED isn't flashing,
    bl      printf                                                  @ print text to the serial output, 

    b       disable_led_interrupt                                   @ and jump to disable LED interrupt.
gpio_led_set_off:                                                   @ 
    movs    r1, #LED_VAL_OFF                                        @ The LED is currently "on" so we want to turn it "off"
    str     r1, [r4]                                                @ store the "off" value into the LED's address

    ldr     r0, =gpio21_pressed_turned_off                          @ If the GP21 is pressed while LED is flashing,
    bl      printf                                                  @ print text to the serial output.
disable_led_interrupt:                                              @
    ldr     r2, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)            @ get the address
    str     r6, [r2]                                                @ store the gpio21 mask into the address

    b       gpio_return                                             @ exit the subroutine

reset_delay:
    ldr r1, =ltimer                                                 @ Get the address of the timer delay.
    ldr r2, =DFLT_ALARM_TIME                                        @ Get the default timer delay.
    str r2, [r1]                                                    @ Store the default delay into the timer.

    ldr     r0, =timer_state                                        @ Get the state of the timer.
    ldr     r1, [r1]                                                @ 
    bl      printf                                                  @ Print a formatted string using r1.
    
gpio_return:
    pop     {pc}                                                    @ Pop the link register from the stack to the program counter

// Strings to print to the serial output on button/alarm events. They are formatted and work with 'printf' from the C language.
.align 4
msg:    
    .asciz "Hello World!\n"
led_state:      
    .asciz "LED Flashing state: %d\n"
timer_state:      
    .asciz "Delay: %d microseconds\n\n"
gpio20_pressed_flashing_on:
    .asciz "Button GPIO20 was pressed while flashing. Delay halved.\nDelay: %d microseconds.\n\n"
gpio20_pressed_flashing_off:
    .asciz "Button GPIO20 was pressed while not flashing. Delay reset.\n"
gpio21_pressed_turned_off:
    .asciz "Button GPIO21 was pressed. LED flashing stopped.\n\n"
gpio21_pressed_turned_on:
    .asciz "Button GPIO21 was pressed. LED flashing resumed.\n\n"
gpio22_pressed_flashing_on:
    .asciz "Button GPIO22 was pressed while flashing. Delay doubled.\nDelay: %d microseconds.\n\n"
gpio22_pressed_flashing_off:
    .asciz "Button GPIO22 was pressed while not flashing. Delay reset.\n"

.data
lstate: .word   DFLT_STATE_STRT
ltimer: .word   DFLT_ALARM_TIME