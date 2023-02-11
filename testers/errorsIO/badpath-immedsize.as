DATA1: .data 100
       cmp #2047,DATA1
       cmp #2048,DATA1
       cmp r2,#-2048
       cmp r2,#-2049
       stop
