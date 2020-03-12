![](https://github.com/panda-official/TimeSwipe.js/workflows/Workflow/badge.svg)

# [TimeSwipe](https://github.com/panda-official/TimeSwipe) wrapper for NodeJs [![npm version](https://badge.fury.io/js/timeswipe.svg)](https://badge.fury.io/js/timeswipe)

## Prerequisites
- Driver installation ([instructions](https://github.com/panda-official/TimeSwipe/tree/master/driver)) - latest driver for [download](https://github.com/panda-official/TimeSwipe/releases)
- Run module with root permission (`sudo`)

## Installation
```
npm install timeswipe
```

## Usage
Example:
```
var timeswipe = require('timeswipe');

// Settings
timeswipe.SetBridge(0);
timeswipe.SetSensorOffsets(0,0,0,0);
timeswipe.SetSensorGains(1,1,1,1);
timeswipe.SetSensorTransmissions(1,1,1,1);

// Event Listener for Button
timeswipe.onButton(async function (pressed, counter) {
    console.error("onButton pressed: ", pressed, " counter: ", counter);
});

// Event Listener for Errors
timeswipe.onError(async function (errors) {
    console.error("onError errors: ", errors);
});

var lines = 0

// Start measurement
timeswipe.Start(async function (data, error) {
    if (error) {
        console.error("errors: ",error);
    } else {
        data.forEach(function(entry) {
            lines = lines + 1;
        });
    }
});

const promise = new Promise((resolve, reject) => {
  setTimeout(() => {
    resolve()
  }, 10000)
})

function onComplete () {
  timeswipe.Stop()
  console.log('got lines: ', lines)
  process.exit(0)
}

promise.then(onComplete)
```

## Functions

Settings for Offset, Gain and Transmission can be applied for each signal separated.

### SetSensorOffsets (int offset1, int offset2, int offset3, int offset4)
Set Offset for measured data (bit).
 
### SetSensorGains (int gain1, int gain2, int gain3, int gain4)
Set board internal measurement-gain (bit --> V).
 
### SetSensorTransmissions (double trans1, double trans2, double trans3, double trans4)
Set sensor specific transmission (V --> real unit). You get those from their datasheet.
 
### SetSecondary (int number)
Choose primary or secondary measurement type. 
For TimeSwipe pHAT IEPE this is 0 for IEPE, 1 for voltage signal (+-10V).
For TimeSwipe pHAT DMS this is 0 for bridge circuits, 1 for current signal (4-20mA).
 
### Start (function(data,error))
With this you can start measurement. Add callback with input data (array) and error (count of errors occured while record.
 
### onButton (function(pressed, counter))
Event Listener for button pressed/released.
 
### onError (OnErrorCallback cb)
Event Listener for errors.
 
### Stop ()
Stop measurement.


## Build instruction

### Make binary packages locally using docker
```
docker run --rm -v "$PWD/":/usr/src -w /usr/src pandaofficial/timeswipe_js /usr/src/build.sh
```

## Release new version on master branch
```
npm version patch
npm publish
git push --tags
```
