var fs = require("fs");

var tsplibReader = require("./tsplib_reader");
var solver = require("./index");

function tourCost(points, costMatrix) {
  var sum = 0;
  for (var i = 0; i < points.length - 1; i++) {
    sum = sum + costMatrix[points[i]][points[i + 1]];
  }
  return sum;
}

function validateTour(tour, numPoints) {
  if (tour[0] !== 0 || tour[tour.length - 1] !== 0) {
    return false;
  }

  var waypoints = tour.slice(1, tour.length - 1);

  if (new Set(waypoints).size !== waypoints.length) {
    return false;
  }

  for (var i = 0; i < numPoints; i++) {
    if (tour.filter(p => p === i)[0] == null) {
      return false;
    }
  }

  return true;
}

function prettyNumber(num) {
  return parseFloat(Math.round(num * 100) / 100).toFixed(2);
}

function testDataset(name, optimalTourCost) {
  test(name, function() {
    var data = fs.readFileSync(__dirname + "/testcases/" + name, "utf8");

    var costMatrix = tsplibReader.read(data);

    var start = Date.now();
    return solver
      .solveTsp(costMatrix, true, {})
      .then(tour => {
        var end = Date.now();
        expect(validateTour(tour)).toBe(true);
        var cost = tourCost(tour, costMatrix);
        expect(cost).toBeGreaterThanOrEqual(optimalTourCost);
        console.log(
          name,
          "-->",
          "Cost: ",
          cost,
          "Opt:",
          optimalTourCost,
          "Error:",
          prettyNumber(((cost - optimalTourCost) * 100) / optimalTourCost),
          "%",
          "Time:",
          end - start,
          "ms"
        );
      })
      .catch(function(err) {
        expect(err).toBeNull();
      });
  }, 20000);
}

[
  ["wi29", 27603],
  ["d3j8", 6656],
  ["qa194", 9352],
  ["uy734", 79114],
  ["zi929", 95345],
  ["rw1621", 26051]
].forEach(function(val) {
  testDataset(val[0], val[1]);
});

test("1 point tsp", function() {
  var cm = [[0]];

  return Promise.all([
    solver
      .solveTsp(cm, false)
      .then(function(tour) {
        expect(tour).toEqual([0]);
      })
      .catch(function(err) {
        expect(err).toBeNull();
      }),
    solver
      .solveTsp(cm, true)
      .then(function(tour) {
        expect(tour).toEqual([0, 0]);
      })
      .catch(function(err) {
        expect(err).toBeNull();
      })
  ]);
});

test("2 point tsp", function() {
  var cm = [[0, 1], [1, 0]];

  return Promise.all([
    solver
      .solveTsp(cm, false)
      .then(function(tour) {
        expect(tour).toEqual([0, 1]);
      })
      .catch(function(err) {
        expect(err).toBeNull();
      }),
    ,
    solver
      .solveTsp(cm, true)
      .then(function(tour) {
        expect(tour).toEqual([0, 1, 0]);
      })
      .catch(function(err) {
        expect(err).toBeNull();
      })
  ]);
});

test("3 point tsp", function() {
  var cm = [[0, 1, 2], [1, 0, 2], [3, 2, 0]];

  return Promise.all([
    solver
      .solveTsp(cm, false)
      .then(function(tour) {
        expect(tour).toEqual([0, 1, 2]);
      })
      .catch(function(err) {
        expect(err).toBeNull();
      }),
    ,
    solver
      .solveTsp(cm, true)
      .then(function(tour) {
        expect(tour).toEqual([0, 2, 1, 0]);
      })
      .catch(function(err) {
        expect(err).toBeNull();
      })
  ]);
});
