
(module
 ;; The module exports the fib function
 (export "fib" (func $fib)) 

 ;; Function definition of 'fib'
 ;; It takes a 32-bit integer as a parameter and returns a 32-bit integer result
 (func $fib (param $n i32) (result i32)
 
  ;; 'if' statement checks if the parameter $n is less than 2
  (if
   (i32.lt_s
    (local.get $n)  ;; get the value of local variable $n
    (i32.const 2)  ;; compare with constant integer 2
   )
   ;; If $n is less than 2, the function returns 1
   (return
    (i32.const 1)  ;; return constant integer 1
   )
  )
  ;; If $n is not less than 2, the function returns the sum of fib(n-2) and fib(n-1)
  (return
   (i32.add  ;; add the following two function calls
    ;; First recursive call: fib(n-2)
    (call $fib  ;; call function $fib with argument (n-2)
     (i32.sub  ;; subtract 2 from $n
      (local.get $n)  ;; get the value of local variable $n
      (i32.const 2)  ;; subtract constant integer 2
     )
    )
    ;; Second recursive call: fib(n-1)
    (call $fib  ;; call function $fib with argument (n-1)
     (i32.sub  ;; subtract 1 from $n
      (local.get $n)  ;; get the value of local variable $n
      (i32.const 1)  ;; subtract constant integer 1
     )
    )
   )
  )
 )
)

