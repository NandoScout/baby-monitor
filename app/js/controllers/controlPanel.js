(function() {
'use strict';

    angular
        .module('App')
        .controller('ControlPanelController', ControlPanelController);

    ControlPanelController.$inject = ['$scope', '$state', 'RestService'];
    function ControlPanelController($scope, $state, RestService) {
        $scope.light = { checked: false };
        $scope.music = { checked: false };
        $scope.fan = { checked: false };
        $scope.lightSensor = { checked: false };
        $scope.shakeSensor = { checked: false };


        //---------SHAKE------------------------------------
        
        var onShake = function () {
            $scope.toggleMusic();
            alert("detected shake");
        // Fired when a shake is detected
        };
        var onError = function () {
        // Fired when there is an accelerometer error (optional)
        };
        $scope.toggleShakeSensor = function () {
            // Start watching for shake gestures and call "onShake"
            // with a shake sensitivity of 40 (optional, default 30)
            if($scope.shakeSensor.checked == true)
            {
                shake.startWatch(onShake, 40 /*, onError */);
            }
            else
            {
                shake.stopWatch();
            }
        };

        //---------END-SHAKE---------------------------------

        //--------FINGERPRINT--------------------------------
        var successCallback = function () {
            alert ("la autenticacion por huella digital esta disponible");
        };
        var errorCallback = function () {
            console.log ("la autenticacion no esta disponible");
        };  
        FingerprintAuth.isAvailable(successCallback, errorCallback);
        //--------END-FINGERPRINT-----------------------------

        //--------LIGHT-SENSOR--------------------------------
        $scope.toggleLightSensor = function () {
            if($scope.lightSensor.checked == true)
            {
                window.plugin.lightsensor.watchReadings(
                    function success(reading){
                        console.log(JSON.stringify(reading));
                        if((reading.intensity<50 && !$scope.light.checked)||(reading.intensity>=50 && $scope.light.checked))
                        {
                            $scope.toggleLight();
                        }
                    }, 
                    function error(message){
                        console.log(message);
                    }
                    );
            }
            else
            {
                window.plugin.lightsensor.stop();
            }
        };
        //--------END-LIGHT-SENSOR-----------------------------

        //-----------ACTUADORES--------------------------------
        

        $scope.toggleLight = function () { 
            //Se envia comando al server de luz=true para prender y luz=false para apagar
            RestService.doGet('/luz=' + $scope.light.checked)
            .then(function (result) {
                console.log("El estado de la luz es " + $scope.light.checked);
            }, function (reason) {
                console.log("Ocurrió un error:" + reason);
                $scope.msg = {error: reason};
                //si ocurre un error se corrige valor del interruptor
                // $scope.light.checked = !$scope.light.checked;
            });
        };

        $scope.toggleMusic = function () { 
            //Se envia comando al server de music=true para prender y music=false para apagar
            RestService.doGet('/music=' + $scope.music.checked)
            .then(function (result) {
                console.log("El estado de la música es " + $scope.music.checked);
            }, function (reason) {
                console.log("Ocurrió un error:" + reason);
                $scope.msg = {error: reason};
                //si ocurre un error se corrige valor del interruptor
                // $scope.music.checked = !$scope.music.checked;
            });
        };

        $scope.toggleFan = function () { 
            //Se envia comando al server de fan=true para prender y fan=false para apagar
            RestService.doGet('/fan=' + $scope.fan.checked)
            .then(function (result) {
                console.log("El estado del ventilador es " + $scope.fan.checked);
            }, function (reason) {
                console.log("Ocurrió un error:" + reason);
                $scope.msg = {error: reason};
                //si ocurre un error se corrige valor del interruptor
                // $scope.fan.checked = !$scope.fan.checked;
            });
        };
        //-----------END-ACTUADORES---------------------------
    }
})();