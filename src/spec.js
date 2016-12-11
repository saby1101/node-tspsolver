const test = require('ava')
const fs = require('fs')

const tsplibReader = require('./tsplib_reader')
const solver = require('./index')

function tourCost(points, costMatrix) {
    let sum = 0
    for (let i = 0; i < points.length - 1; i++) {
        sum = sum + costMatrix[points[i]][points[i + 1]]
    }
    return sum
}

function validateTour(tour, numPoints) {
    if (tour[0] !== 0 || tour[tour.length - 1] !== 0) { return false }

    const waypoints = tour.slice(1, tour.length - 1)

    if (new Set(waypoints).size !== waypoints.length) { return false }

    for (let i = 0; i < numPoints; i++) {
        if (tour.filter(p => p === i)[0] == null) { return false }
    }

    return true
}

function withinError(value, expected) {
    const errorVal = Math.abs(value - expected)
    return errorVal / expected
}

function testDataset(name, optimalTourCost, allowedError) {
    test(name, t => {
        const data = fs.readFileSync(__dirname + '/testcases/' + name, 'utf8')

        const costMatrix = tsplibReader.read(data)

        return solver.solveTsp(costMatrix, true, {})
            .then(tour => {
                t.true(validateTour(tour), 'Invalid tour!')
                const cost = tourCost(tour, costMatrix)
                t.true(cost >= optimalTourCost, 'Cannot be better than optimal!')
                t.true(withinError(cost, optimalTourCost) < allowedError || 0.1)
            })
    })
}

[
    ['wi29', 27603, 0.05],
    ['d3j8', 6656, 0.1],
    ['qa194', 9352, 0.2]
].forEach(val => testDataset(val[0], val[1], val[2]))
