int:     .data 0
start:   jsr  %end
         bne  %start
         jmp  %start         
; 5. invalid source operand (relative mode) 
         lea  %int,r1
; 6. invalid target operand (relative mode) 
         add  #45,%int
; 7. invalid operand (relative mode) 
         inc  %int
; 8. invlid characters (%start) 
         bne  %%start
; 9. invalid operand (incomplete relative)
         jmp  % start         
; 10. invlid label (42) (or invalid characters (42))
         jsr  %42
; 11. invlid label (5xy) (or invalid characters (5xy))
         bne  %5xy
; 12. invalid operand (or invalid characters (%start))
end:     stop %start