
package main

import (
	"fmt"
	"io"
	"os"
	"github.com/bytecodealliance/wasmtime-go"
)

func main() {

	engine := wasmtime.NewEngine()

	// Create a new store with the engine
	store := wasmtime.NewStore(engine)

	// Read the WASM module file
	moduleFile, err := os.Open("../add.wasm")
	if err != nil {
		panic(err)
	}
	defer moduleFile.Close()

	moduleBytes, err := io.ReadAll(moduleFile)
	if err != nil {
		panic(err)
	}

	// Compile the module
	module, err := wasmtime.NewModule(engine, moduleBytes)
	if err != nil {
		panic(fmt.Sprintf("cannot load the module, %s", err.Error()))
	}

	instance, err := wasmtime.NewInstance(store, module, []wasmtime.AsExtern{})
	if err != nil {
		panic(fmt.Sprintf("cannot create instance, %s", err.Error()))
	}

	add := instance.GetExport(store, "add").Func()

	val, err := add.Call(store, 6, 27)
	if err != nil {
		panic(fmt.Sprintf("cannot call function, %s", err.Error()))
	}


	fmt.Printf("Calling function from WASM module: add(6, 27) = %d\n", val.(int32))
}

