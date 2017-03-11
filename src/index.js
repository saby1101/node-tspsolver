var addon = require('bindings')('node_tspsolver')

exports.solveTsp = function (costMatrix, roundtrip, options, callback) {
    function callbackError(message) {
        if (callback != null && typeof callback === 'function') {
            return callback(new Error(message))
        } else {
            return Promise.reject(new Error(message))
        }
    }

    if (!Array.isArray(costMatrix) || costMatrix.some(row => !Array.isArray(row) || row.length !== costMatrix.length)) {
        return callbackError("Costmatrix needs to be a 2d symmetric array!")
    }

    if (costMatrix.some(row => row.some(val => !Number.isFinite(val)))) {
        return callbackError("Costmatrix should only contain finite numbers!")
    }

    if (costMatrix.length < 1) {
        return callbackError("Empty costMatrix.")
    }

    roundtrip = roundtrip == null ? true : roundtrip

    if (typeof roundtrip !== 'boolean') { return callbackError('roundtrip expected to be a boolean!') }

    if (costMatrix.length === 1) {
        const route = roundtrip ? [0, 0] : [0]

        if (callback != null && typeof callback === 'function') { return callback(null, route) }
        else { return Promise.resolve(route) }
    }

    var opts = Object.assign({
        N: 1000000,
        T: 100,
        lambda: 0.985,
        reheatInterval: 100000
    }, Object(options))

    if (!Number.isInteger(opts.N) || opts.N < 0) { return callbackError('Invalid opts!') }
    if (!Number.isFinite(opts.T) || opts.T <= 0) { return callbackError('Invalid opts!') }
    if (!Number.isFinite(opts.lambda) || opts.lambda <= 0 || opts.lambda >= 1) { return callbackError('Invalid opts!') }

    if (Number.isInteger(opts.reheatInterval) && opts.reheatInterval <= 0) { return callbackError('Invalid opts!') }

    if (opts.reheatInterval == null) { opts.reheatInterval = opts.N / 2 }

    if (callback != null && typeof callback === 'function') {
        addon.solveTsp(costMatrix, roundtrip, opts, callback)
    } else {
        return new Promise((resolve, reject) => addon.solveTsp(costMatrix, roundtrip, opts, (err, result) => {
            if (err) { reject(err) }
            else { resolve(result) }
        }))
    }
}
