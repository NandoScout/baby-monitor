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
        $scope.togglingLight = false;
        $scope.cmd=0;

        //---------SHAKE------------------------------------
        
        var onShake = function () {
            $scope.toggleMusic();
            //alert("detected shake "+$scope.music.checked);

            RestService.doGet('/comando=5')
            .then(function (result) {
                console.log("El estado de la música es " + $scope.music.checked);
            }, function (reason) {
                console.log("Ocurrió un error:" + reason);
                $scope.msg = {error: reason};
                //si ocurre un error se corrige valor del interruptor
                // $scope.music.checked = !$scope.music.checked;
            });
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

      /*  var successCallback = function () {
            alert ("la autenticacion por huella digital esta disponible");
        };
        var errorCallback = function () {
            console.log ("la autenticacion no esta disponible");
        };  
        FingerprintAuth.isAvailable(successCallback, errorCallback);*/

        var encryptConfig = {
            clientId: "myAppName"
        };


       

        function successCallback2(result) {
            console.log("successCallback(): " + JSON.stringify(result));
            if (result.withFingerprint) {
                // alert("Successfully encrypted credentials.");
                // alert("Encrypted credentials: " + result.token);  
            } else if (result.withBackup) {
                console.log("Authenticated with backup password");
            }

            RestService.doGet('/comando=' + ($scope.fan.checked?3:2))
            .then(function (result) {
                console.log("El estado del ventilador es " + $scope.fan.checked);
            }, function (reason) {
                console.log("Ocurrió un error:" + reason);
                $scope.msg = {error: reason};
                //si ocurre un error se corrige valor del interruptor
                // $scope.fan.checked = !$scope.fan.checked;
            });
        }

        function errorCallback2(error) {
            if (error === FingerprintAuth.ERRORS.FINGERPRINT_CANCELLED) {
                console.log("FingerprintAuth Dialog Cancelled!");
            } else {
                console.log("FingerprintAuth Error: " + error);
            }
            $scope.fan.checked = false;
            alert ("No se ha autenticado correctamente");
        }
        //--------END-FINGERPRINT-----------------------------

    

        //-----------ACTUADORES--------------------------------
        

        $scope.toggleLight = function () { 
            //Se envia comando al server de luz=true para prender y luz=false para apagar
            RestService.doGet('/comando='+($scope.light.checked?1:0))
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
            RestService.doGet('/comando=5')
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
            FingerprintAuth.encrypt(encryptConfig, successCallback2, errorCallback2);
            
        };
        //-----------END-ACTUADORES---------------------------

            //--------LIGHT-SENSOR--------------------------------
        $scope.toggleLightSensor = function () {
            if($scope.lightSensor.checked == true)
            {
                window.plugin.lightsensor.watchReadings(
                    function success(reading){
                        console.log(reading.intensity + " luz " + $scope.light.checked );
                        if(reading.intensity<50 && !$scope.light.checked)
                        {
                            $scope.light.checked = true;
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
    }
})();