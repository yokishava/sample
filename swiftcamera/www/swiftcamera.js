var exec = require('cordova/exec');

var swiftcamera = {
    startcamera: function(recieveData, successCallback, errorCallback) {
        //exec(successCallback, errorCallback, 'swiftcamera', 'startcamera', []);
        exec(successCallback, errorCallback, 'swiftcamera', 'startcamera', recieveData);
    }
};

module.exports = swiftcamera;
