
function readLocations(data) {
    var lines = data.split('\n')

    var i = 0
    var dimensions, edgeWeightType
    for (; i < lines.length; i++) {
        var line = lines[i]

        if (line.startsWith('DIMENSION')) {
            dimensions = parseInt(line.split(':')[1].trim())

        } else if (line.startsWith('EDGE_WEIGHT_TYPE')) {

            edgeWeightType = line.split(':')[1].trim()
        }
        else if (line === 'NODE_COORD_SECTION') { break }
    }
    i++

    if (edgeWeightType !== 'EUC_2D') {
        throw new Error('Only support EUC_2D for now!')
    }

    var locations = []

    for (; i < lines.length; i++) {
        var line = lines[i]
        var objs = line.split(' ')
        locations.push({
            id: parseInt(objs[0]),
            x: parseFloat(objs[1]),
            y: parseFloat(objs[2])
        })
    }

    if (dimensions !== locations.length) {
        throw new Error('Dimensions dont match number of locations')
    }

    var matrix = []
    for (var i = 0; i < dimensions; i++) {
        matrix[i] = []
        for (var j = 0; j < dimensions; j++) {
            matrix[i][j] = Math.round(euc2D(locations[i], locations[j]))
        }
    }


    return matrix
}

function euc2D(l1, l2) {
    return Math.sqrt(Math.pow(l1.x - l2.x, 2) + Math.pow(l1.y - l2.y, 2))
}

module.exports = {
    read: readLocations
}
