var MyCalc = require('./build/Release/mycalc');

var calc = new MyCalc(10);
calc.addOne(5);
console.log(calc.getValue());//15