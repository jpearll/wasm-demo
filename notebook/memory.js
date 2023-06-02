
// access the file system and load the wasm module.
import fs from 'fs' 

// call_add_from_wasm shows how to load the WASM binary module
// and call the exported function with provided parameters.
(async function () {

    // read the wasm module form the file system.
    const wasmModule = fs.readFileSync('notebook/memory.wasm'); 

    // This part is new, we define the import function to be 
    // called internally by the WASM
    var importObject = {
        imports: {
            log_func: function(arg) {
                console.log("We are trigger this function internally, here is the result: " + arg);
            },
        },
        env: {
            // initiatize the memory block to be passed to WASM
            memory: new WebAssembly.Memory({initial: 10, maximum: 100 })
        }
    };

    // Instantiate the buffer.
    // return a module named math and its exports
    await WebAssembly.instantiate(new Uint8Array(wasmModule), importObject)
        .then(result => {
            
            // We initiatize the array with values
            const maxNumber = 10;
            const arr = new Int32Array(importObject.env.memory.buffer);

            for (let i = 0; i < maxNumber; i++) {
                arr[i] = i * 10;
            }           

            // We call the WASM function to process the data
            result.instance.exports.add_and_log(maxNumber, 1, 2)
        }); 

})();
