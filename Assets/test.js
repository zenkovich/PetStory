"use strict";

class Greeter {
    constructor(message) {
        this.greeting = message;
        print('using fucking class')
    }
    greet() {
        return "Hello, " + this.greeting;
    }
}

var greeter = new Greeter("world");
print(greeter.greet());