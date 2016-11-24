# node-tspsolver
Travelling salesman solver for nodejs

This solver uses Simulated Annealing with optional periodic reheating.

Initial solution is constructed using Nearest Neighbour heuristic.

Tour transformations used in the local search step: 2-opt, translation and swapping.

<strong>Signature:</strong>

function solveTsp(costMatrix, roundtrip, options, callback)

function solveTsp(costMatrix, roundtrip, options) returns a promise

<strong>Arguments:</strong>

costMatrix- 2d array of costs .. costMatrix[i][j] gives cost between ith and jth points

roundtrip- whether salesman needs to get back to starting point, ie point at index 0. If false, point at n - 1 is treated as the end point

options- {<br/>
    N - 'number of iterations' default: 80000, <br/>
    T - 'Initial temperature' default: 110, <br/>
    lambda - 'Annealing parameter' default: 0.999, <br/>
    reheatInterval - 'period after which to reset temperature' default: N / 2, <br/>
} <br/>

callback- function (err, result) {}

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

solver.solveTsp(costMatrix, true, {}, function (err, result)) {
    console.log(result) // result is an array of indices specifying the route.
}

</code></pre>