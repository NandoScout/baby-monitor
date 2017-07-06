(function () {
    angular
		.module('App')
		.factory('RestService', RestService);

	RestService.$inject = ['$q', '$http', '$rootScope'];
	function RestService($q, $http, $rootScope) {

        var apiURL = "http://192.168.10.144";
        
        return {
            doGet: function (url) {
                var deferred = $q.defer();
                $rootScope.servicePromise = $http({
                    method: 'GET',
                    url: apiURL + url,
                    headers: {
                        'Accept': 'image/webp,image/,/*'
                    }
                }).success(function (data) {
                    deferred.resolve(data);
                }).error(function (data, status, headers, config) {
                    deferred.reject("Ha ocurrido un error " + status);
                });

                return deferred.promise;
            },

            doPut: function (url, params) {
                var deferred = $q.defer();
                $rootScope.servicePromise = $http({
                    method: 'PUT',
                    url: apiURL + url,
                    data: params,
                    headers: {
                        'Accept': 'application/json',
                        'Content-Type': 'application/json'
                    }
                })
                .success(function (data) {
                    deferred.resolve(data);
                })
                .error(function (data) {
                    if(data.reason)
                    {
                        deferred.reject(data.reason);
                    }
                    else
                    {
                        deferred.reject("Ha ocurrido un error");
                    }
                });

                return deferred.promise;
            },

            doPost: function (url, params) {

                var deferred = $q.defer();
                $rootScope.servicePromise = $http({
                method: 'POST',
                url: apiURL + url,
                data: params,
                headers: {
                    'Accept': 'application/json',
                    'Content-Type': 'application/json'
                }
            })
            .success(function (data) {
                deferred.resolve(data);
            })
            .error(function (reason,status) {
                if (status == 405){
                    deferred.reject(new String(status));
                }else {
                    deferred.reject("Ha ocurrido un error");
                }
            });

            return deferred.promise;
            },

            doDelete: function (url) {
                    var deferred = $q.defer();
                    $rootScope.servicePromise = $http({
                    method: 'DELETE',
                    url: apiURL + url,
                    headers: {
                    }
                })

                .success(function (data) {
                    deferred.resolve(data);
                })
                .error(function (reason) {
                    deferred.reject("Ha ocurrido un error");
                });

                return deferred.promise;
            }
        };
    }
})();