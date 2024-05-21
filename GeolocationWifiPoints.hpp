#ifndef GEOLOCWIFIPTS_H
#define GEOLOCWIFIPTS_H

#include <string>
#include <vector>
#include <windows.h>
#include <wlanapi.h>
#include <iostream>
#include <format>
#include <boost/json.hpp>


class GeolocationWifiPoints 
{
    private:
        struct Wifipoint
        {
            std::string SSID;
            std::string MAC;
            int Strength;
            /* data */
        };

        std::vector<GeolocationWifiPoints::Wifipoint> nearbyAccessPoints;
    public:
       // GeolocationWifiPoints();

        bool FindWifiPoints();

        std::string CreateJson();    

};

#endif