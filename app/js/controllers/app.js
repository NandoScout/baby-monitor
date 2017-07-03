(function () {
    'use strict';

    angular
        .module('App')
        .controller('AppController', AppController);

    AppController.$inject = ['$scope', '$ionicPopover', '$interval', 'RestService'];
    function AppController($scope, $ionicPopover, $interval, RestService) {
    
          
        var sound_value = 0;
        $scope.aux = 1;
        
        $interval( function(){ $scope.obtenerMediciones(); }, 10000);
        $scope.obtenerMediciones = function(){
            RestService.doGet('/led=0')
            .then(function (result) {
                $scope.items[0].value = result.id;
            }, function (reason) {
                $scope.msg = {error: reason};
            });
            $scope.aux ++;
        };

        $scope.items = [
            {
                name:"heart-rate-item",
                color: "#5AD863",
                icon: "ion-thermometer",
                title: "Temperatura",
                value: 0,
                unit: "ºC"
            },
            {
                color: "#1E90FF",
                icon: "ion-waterdrop",
                title: "Humedad",
                value: 0,
                unit: "%"
            },
            {
                name:"sound-item",
                color: "gray",
                icon: "ion-android-volume-up",
                title: "Sonido",
                value: 0,
                unit: "dB"
            },
            {
                color: "#D86B67",
                icon: "ion-ios-heart",
                title: "Pulso Cardiaco",
                value: 0,
                unit: "BPM"
            },
            {
                color: "#AD5CE9",
                icon: "ion-ios-sunny",
                title: "Luminosidad",
                value: 0,
                unit: "lx"
            },
            {
                color: "#3DBEC9",
                icon: "ion-arrow-graph-up-right",
                title: "Movimiento",
                value: "Poco",
                unit: ""

            }
        ];

        $scope.exitApp = function () {
            ionic.Platform.exitApp();
        };

        $ionicPopover.fromTemplateUrl('templates/modals/popover.html', {
            scope: $scope
        }).then(function (popover) {
            $scope.popover = popover;
        });

        $scope.openPopover = function ($event) {
            $scope.popover.show($event);
        };
        
        $scope.$on('$destroy', function() {
            $scope.popover.remove();
        });

        $scope.goControlPanel = function () {
            console.log("yendo al panel de contorl");
            $state.go("app.controlPanel");
        };
    }
})();