var timeswipe = require('timeswipe');

timeswipe.SetMode("PRIMARY");
console.log("mode: ", timeswipe.GetMode());
timeswipe.SetSensorOffsets(0,0,0,0);
timeswipe.SetSensorGains(1,1,1,1);
timeswipe.SetSensorTransmissions(1,1,1,1);
timeswipe.SetBurstSize(24000);
timeswipe.SetSampleRate(24000);
//timeswipe.TraceSPI(true);

timeswipe.onEvent(async function (ev) {
    console.log("got event: ", ev);
});

var lines = 0
timeswipe.Start(async function (data, error) {
    if (error) {
        console.error("errors: ",error);
    } else {
            lines += data[0].length;
        data.forEach(function(entry) {
            //console.log(entry.join('\t'));
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

