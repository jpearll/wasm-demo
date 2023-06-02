
// Import the necessary libraries
extern crate rand;
extern crate rand_distr;

// Import the Distribution and Normal traits for generating random numbers. 
// Normal is used because stock returns are often assumed to follow a normal (or log-normal) 
// distribution due to Central Limit Theorem.
use rand_distr::{Distribution, Normal}; 
use std::time::{Instant}; // Import the Instant struct for timing

// The compute_risk function is defined as an externally accessible function
#[no_mangle] // no_mangle: don't let the compiler generate a function name so it can be called externally
pub extern "C" fn compute_risk() {
    // Create a new portfolio object
    let mut portfolio = Portfolio::new();
    
    // Add stocks to the portfolio with their expected returns and standard deviations. 
    // Expected return is the mean of the distribution of possible returns, usually 
    // estimated based on historical data or predicted through financial models. 
    // Standard deviation measures the dispersion of returns around the mean, acting as a measure of risk.
    portfolio.add_stock(5.0, 0.02); 
    portfolio.add_stock(8.5, 0.035);
    portfolio.add_stock(3.2, 0.1);
  
    // Start a timer to track the execution time of the simulation
    let start = Instant::now();
    
    // Run a Monte Carlo simulation on the portfolio with 1000 iterations. 
    // Monte Carlo simulation is a method to estimate the risk of a portfolio 
    // through repeated random sampling.
    let (expected_return, risk) = portfolio.simulate(1000); 

    // Calculate the duration of the simulation
    let duration = start.elapsed();

    // Output the expected return, risk and execution time of the simulation
    println!("Expected returns: {}, Risk: {}, Exec time: {:?}", expected_return, risk, duration);
}

// Define a Stock struct that holds the expected 
// return and standard deviation of a stock
struct Stock {
    expected_return: f64,
    standard_deviation: f64,
}

// Define a Portfolio struct that holds a vector of Stocks
struct Portfolio {
    stocks: Vec<Stock>,
}

// Implement methods on the Portfolio struct
impl Portfolio {
    // Create a new, empty portfolio
    fn new() -> Portfolio {
        Portfolio { stocks: Vec::new() }
    }

    // Add a stock to the portfolio with a given 
    // expected return and standard deviation
    fn add_stock(&mut self, expected_return: f64, standard_deviation: f64) {
        self.stocks.push(Stock {
            expected_return,
            standard_deviation,
        });
    }

     // Run a Monte Carlo simulation on the portfolio with a specified number of iterations.
     // See this reference: https://towardsdatascience.com/best-investment-portfolio-via-monte-carlo-simulation-in-python-53286f3fe93
    fn simulate(&self, iterations: usize) -> (f64, f64) {
        let mut rng = rand::thread_rng(); // Create a random number generator
        let mut returns = Vec::with_capacity(iterations); // Pre-allocate a vector to hold the returns
        
        // For each iteration, calculate the return of the portfolio
        for _ in 0..iterations {
            let mut portfolio_return = 0.0;
            for stock in &self.stocks {
                
                // Build a Normal distribution from mean and standard deviation
                // here, from return and risk
                let normal = Normal::new(stock.expected_return, stock.standard_deviation).unwrap();
                
                // We are randomly selecting a value from the normal distribution
                portfolio_return += normal.sample(&mut rng); 
            }
            returns.push(portfolio_return);
        }
        // Portfolio expected returned = mean of the returns
        let mean: f64 = returns.iter().sum::<f64>() / iterations as f64;

        // To simplify the calculation of Risk, we are using the Standard Deviation
        let variance: f64 = returns.iter().map(|&x| (x - mean).powi(2)).sum::<f64>() / iterations as f64;
        (mean, variance.sqrt()) // Return the mean (expected return) and standard deviation (risk)
    }
}

