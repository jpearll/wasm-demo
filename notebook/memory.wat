
;; This Module demonstrate the usage of an external function 
(module
    ;; This part declares that there is a dependency on an external function to
    ;; be called; either from another WASM module, or from JS.
    (func $log (import "imports" "log_func") (param i32))

    ;; Imports an external Memory initiated by JS
    ;; creates a memory object with a size of 1 page (64KiB)
    (memory (import "env" "memory") 1)

    ;; Input
    ;;  param i32
    ;;  param i32
    ;; Output
    ;;  result i32
    (func $add (param i32 i32) (result i32)
        local.get 0
        local.get 1
        i32.add
    )

    (func $add_and_log (param $max i32) (param $n1 i32) (param $n2 i32)
        ;; Loop index
        (local $i i32)
        (local $maxPtr i32)

        (local.set $i (i32.const 0))  

        ;; initiatize at the max offset
        (local.set $maxPtr 
            (i32.mul (local.get $max) (i32.const 4))
        )
    
        (loop $loop 
            local.get $n1
            local.get $n2
            call $add
            (i32.load (local.get $i))
            i32.mul
            call $log

            ;; Increment $i
            (local.set $i
                (i32.add (local.get $i) (i32.const 4))
            )
            ;; i < max
            local.get $i
            local.get $maxPtr
            i32.lt_s
            br_if $loop
        )
    )

    ;; export the function so JS can call it
    (export "add_and_log" (func $add_and_log))

)
