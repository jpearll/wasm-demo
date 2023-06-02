
// access the file system and load the wasm module.
import fs from 'fs' 

// call_add_from_wasm shows how to load the WASM binary module
// and call the exported function with provided parameters.
(async function () {

    // read the wasm module form the file system.
    const wasmModule = fs.readFileSync('notebook/add_ext.wasm'); 

    // This part is new, we define the import function to be 
    // called internally by the WASM
    var importObject = {
        imports: {
            log_func: function(arg) {
                console.log("Result: " + arg);
            }
        }
    };

    // Instantiate the buffer.
    // return a module named math and its exports
    await WebAssembly.instantiate(new Uint8Array(wasmModule), importObject)
        .then(result => result.instance.exports.add_log(10, 5)); 

}) ();
