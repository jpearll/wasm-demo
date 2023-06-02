
// Import necessary modules from Node.js
import fs, { writeFile } from 'fs'
import util from 'util'
import path from 'path'
import { PerformanceObserver, performance } from 'perf_hooks';

// Promisify fs.readFile and fs.writeFile for easier use with async/await
const readFile = util.promisify(fs.readFile)
const writeFileAsync = util.promisify(writeFile)

// Define the main async function
async function main () {
  // Read the WebAssembly binary file
  const buf = await readFile(path.resolve('notebook/fib_2.wasm'))

  // Instantiate the WebAssembly module
  const res = await WebAssembly.instantiate(new Uint8Array(buf.buffer))

  // Extract the 'fib' function exported from the module
  const { fib } = res.instance.exports

  // Prepare an array of results, starting with the CSV headers
  const results = [['Fibonacci Number', 'WASM Time', 'JS Time']]

  // Run benchmarks on both the WebAssembly and JavaScript versions of the Fibonacci function
  for (let i = 1; i < 10; i++) {
    // Use Promise.all to run the benchmarks in parallel and wait for both to finish
    const [wasmTime, jsTime] = await Promise.all([
      bench('WASM', fib, i),
      bench('JS', fibonacci, i)
    ])

    // Add the benchmark results to the results array
    results.push([i, wasmTime, jsTime])
  }

  // Convert the results array to a CSV string
  const csvString = results.map(row => row.join(',')).join('\n')

  // Write the CSV string to a file
  await writeFileAsync('notebook/benchmark_results.csv', csvString)
}

// Benchmarking function
async function bench (label, fn, ubound) {
  
  const N_ITERS = 10; // Number of iterations for averaging results
  const observations = [] // Array to store performance observation entries

  // Instantiate the PerformanceObserver
  const obs = new PerformanceObserver(list => {
    const entries = list.getEntries()
    observations.push(...entries)
    performance.clearMarks() // Clear marks after getting entries
  })

  // Start observing 'measure' entries
  obs.observe({ entryTypes: ['measure'] })

  // Run the function N_ITERS times and measure the performance
  for (let i = 0; i <= N_ITERS; ++i) {
    performance.mark('Start') // Start performance mark
    fn(ubound)
    performance.mark('End') // End performance mark
    performance.measure('Start to End', 'Start', 'End') // Measure duration between 'Start' and 'End' marks
  }
 
  // Calculate average duration and return it
  const o = obs.takeRecords();
  const total = o.reduce((total, e) => total + e.duration, 0)
  const avg = total / o.length;
  return avg.toFixed(4)
}

// Recursive Fibonacci function in JavaScript
function fibonacci (num) {
  if (num <= 1) return 1

  return fibonacci(num - 1) + fibonacci(num - 2)
}

// Call the main function
main()
