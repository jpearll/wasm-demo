
;; This is a simple module declaration that takes two numbers
;; in parameters, adds them and return the result.
(module
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
    ;; export the function so JS can call it
     (export "add" (func $add))
)
