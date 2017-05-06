[![Build Status](https://travis-ci.org/saby1101/node-tspsolver.svg?branch=master)](https://travis-ci.org/saby1101/node-tspsolver)

# node-tspsolver
Travelling salesman solver for nodejs

This solver uses Simulated Annealing with optional periodic reheating.

Initial solution is constructed using Nearest Neighbour heuristic.

Tour transformations used in the local search step: Stochastic 2-opt, translation and swapping.

The solver is implemented in C++ and doesn't use the nodejs main event loop for running, hence non-blocking.

<strong>Signature:</strong>

`function solveTsp(costMatrix, roundtrip, options)` returns a promise

<strong>Arguments:</strong>

`costMatrix` - 2d array of costs .. costMatrix[i][j] gives cost between ith and jth points

`roundtrip` - whether salesman needs to get back to starting point, ie point at index 0. If false, point at n - 1 is treated as the end point

`options` - {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;N - 'number of iterations' default: 1000000, <br/>
&nbsp;&nbsp;&nbsp;&nbsp;T - 'Initial temperature' default: 100, <br/>
&nbsp;&nbsp;&nbsp;&nbsp;lambda - 'Annealing parameter' default: 0.985, <br/>
&nbsp;&nbsp;&nbsp;&nbsp;reheatInterval - 100000, <br/>
} <br/>

<strong>Install:</strong>
<code>npm install node-tspsolver</code>

<strong>Examples:</strong>
<pre><code>

var solver = require('node-tspsolver')

var costMatrix = [
    [0, 1, 3, 4],
    [1, 0, 2, 3],
    [3, 2, 0, 5],
    [4, 3, 5, 0]
]

solver
    .solveTsp(costMatrix, true, {})
    .then(function (result)) {
        console.log(result) // result is an array of indices specifying the route.
    })

</code></pre>
