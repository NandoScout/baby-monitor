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

        $scope.toggleLight = function () { 
            //Se envia comando al server de luz=true para prender y luz=false para apagar
            RestService.doGet('/luz=' + $scope.light.checked)
            .then(function (result) {
                console.log("El estado de la luz es " + $scope.light.checked);
            }, function (reason) {
                console.log("Ocurrió un error:" + reason);
                $scope.msg = {error: reason};
                //si ocurre un error se corrige valor del interruptor
                $scope.light.checked = !$scope.light.checked;
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
                $scope.music.checked = !$scope.music.checked;
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
                $scope.fan.checked = !$scope.fan.checked;
            });
        };
    }
})();