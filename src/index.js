var addon = require('bindings')('node_tspsolver')

exports.solveTsp = function (costMatrix, roundtrip, options, callback) {
    function callbackError(message) {
        return callback(new Error(message))
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

    if (costMatrix.length === 1) {
        if (callback == null) { return Promise.resolve([0]) }
        else { return callback(null, [0]) }
    }

    if (typeof roundtrip !== 'boolean') { return callbackError('roundtrip expected to be a boolean!') }

    var opts = Object.assign({
        N: 200000,
        T: 110,
        lambda: 0.999,
        reheatInterval: 30000
    }, Object(options))

    if (!Number.isInteger(opts.N) || opts.N < 0) { return callbackError('Invalid opts!') }
    if (!Number.isFinite(opts.T) || opts.T <= 0) { return callbackError('Invalid opts!') }
    if (!Number.isFinite(opts.lambda) || opts.lambda <= 0 || opts.lambda >= 1) { return callbackError('Invalid opts!') }

    if (Number.isInteger(opts.reheatInterval) && opts.reheatInterval <= 0) { return callbackError('Invalid opts!') }

    if (opts.reheatInterval == null) { opts.reheatInterval = opts.N / 2 }

    if (callback != null && typeof callback === 'function') {
        addon.solveTsp(costMatrix, roundtrip, opts, callback)
    } else if (callback == null) {
        return new Promise((resolve, reject) => exports.solveTsp(costMatrix, roundtrip, opts, (err, result) => {
            if (err) { reject(err) }
            else { resolve(result) }
        }))
    } else {
        return callbackError('callback needs to be function if provided!')
    }
}
