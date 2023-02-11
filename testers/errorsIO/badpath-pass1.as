; 1. this line is ok (label DATA4 is declared later)
    Start:  dec DATA4
; 2. missing operand
        sub #5
; 3. missing operand
        sub #5,
; 4. missing operand
        red
; 5. this line is ok (label declared as external)
       .extern DATA4
; 6. invalid target operand (immediate)
        add  #5,#6
; 7. this line is ok (immediate target operand allowed for cmp)
    Next:   cmp  #5,#6
; 8. invalid operand (immediate)
        inc  #50
; 9. undefined instruction name
        and  r1,r2
; 10. undefined instruction name (case sensitivity)
        jSr  Start
; 11. invalid characters (,r2)
        cmp  r1,,r2
; 12. invalid characters (,r3)
        add  #5,r1,r3
; 13. invalid characters (blabla)
        prn  r1 blabla
; 14. invalid operand (undefined addressing mode)
        prn  48
; 15. invalid operand (incomplete immediate #)  
        prn  # 48
; 16. invalid characters (r3)
        cmp  r1,#r3
; 17. label was already declared earlier
    Next:   clr  r2
; 18. label does not start in first column (optional error!)
     label1:   sub  r1,r7
; 19. invalid source operand (register)   
        lea  r3,r1
; 20. invalid source operand (immediate)   
        lea  #3,r5
; 21. invalid operand (or invalid characters (Start))
        stop Start
; 22. this line is ok (.data directive can be used without a label)
        .data 400
; 23. this line is ok (.string directive can be used without a label)
        .string "maman14"
; 24. invalid characters (400) 
    Z1:     .data   200 400
; 25. this line is ok (arbitrary spaces/tabs between fields or at end of line)
    DATA1:  .data   100,    	200 ,  -500,300, 500   				   
; 26. this line is ok (arbitrary spaces/tabs between fields or at end of line)
        cmp     r1  ,      DATA1   				   
; 27. this line is ok (no spaces/tabs between operands)
        add     #1,r2   
; 28. invalid characters (,3)
    Z2:     .data   1, ,3
; 29. invalid character (,)
    Z3:     .data   4,
; 30. invalid characters (#123)
    Z4:     .data   #123
; 31. invalid characters (.4)
    Z5:     .data   12.4
; 32. invalid characters (-5)
    Z6:     .data   --5
; 33. invalid label (cannot be an instruction name)
    mov:    .data   5
; 34. invalid label (cannot be a register name)
    r6:     .data   200
; 35. label was already declared earlier
    DATA1:  .data   300
; 36. this line is ok (new label, as labels are case sensitive)
    Data1:  .data   100, +200, -300
; 37. invalid label(non-alphabetic first character)
    1DATA:  .data   300
; 38. this line is ok (label X declaration ignored - warning may be issued)
    X:   .entry  DATA1
; 39. this line is ok (it is ok to declare the same external more than once)
        .extern DATA4
; 40. local label cannot be declared as external
        .extern Start
; 41. this line is ok (label Y declaration ignored - warning may be issued)
    Y:   .extern DATA8
; 42. this line is ok (label STOP is declared later)
        .entry  STOP
; 43 label already designated as external (possibly detected only in 2nd pass)
        .entry  DATA4
; 44. invalid symbol (DATA2 is not an instruction name and not a valid label)
    DATA2   .data   4
; 45. undefined directive (case sensitivity of directive name)
        .DATA   5
; 46. This line is ok (it is ok to designate the same entry more than once)
        .entry  DATA1
; 47. invalid characters (blabla is not a string)
    Z7:     .string blabla
; 48. invalid characters (blabla)
    Z8:     .string "abcdefg" blabla
; 49. this line is ok (comma within string is not a separator)
    STR1:   .string "abc, ,defg"
; 50. invalid label (too long)
    SuperCalifragilisticExpiAlidocious: .data 4
; 51. missing argument in directive
    Z9:     .data
; 52. missing argument in directive
        .entry
; 53. undefined directive
    Z10:    .invalid 85, 90
; 54. this line is ok
        rts
; 55. invalid operand (or invalid characters (Next))
        rts Next
; 56. this line is ok (case sensitivity)
STOP:   stop