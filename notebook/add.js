
// access the file system and load the wasm module.
import fs from 'fs' 

// call_add_from_wasm shows how to load the WASM binary module
// and call the exported function with provided parameters.
(async function () {

    // read the wasm module form the file system.
    const wasmModule = fs.readFileSync('notebook/add.wasm'); 

    // Instantiate the buffer.
    // return a module named math and its exports
    const mod = await WebAssembly.instantiate(new Uint8Array(wasmModule))
        .then(result => result.instance.exports); 

    // Interoperability between Javascript and WASM
    // The following function acts as any other function in ES6
    console.log("Calling WASM, adding two numbers: " + mod.add(10, 5)); 

})();
