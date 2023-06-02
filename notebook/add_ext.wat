
;; This Module demonstrate the usage of an external function 
(module
    ;; This part declares that there is a dependency on an external function to
    ;; be called; either from another WASM module, or from JS.
    (func $log (import "imports" "log_func") (param i32))

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

    (func $add_and_log (param i32 i32)
        local.get 0
        local.get 1
        call $add 
        call $log
    )

    ;; export the function so JS can call it
     (export "add" (func $add))
     (export "add_log" (func $add_and_log))

)
