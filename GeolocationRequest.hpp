#ifndef GEOLOCREQUEST_H
#define GEOLOCREQUEST_H
#include <string>
#include <curl\curl.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <boost/json.hpp>

static const char * API_KEY = "Your API Key";
static const char * GEO_LOC_URL = "https://www.googleapis.com/geolocation/v1/geolocate?key=";
class GeolocationRequest 
{
    private:
        struct Location {
            float latitude;
            float longitude;
            float radius;
        };

        struct MemoryStruct {
            char * response = nullptr;
            size_t size = 0;
        };

        GeolocationRequest::Location location;
        GeolocationRequest::MemoryStruct chunk;

        static size_t WriteCallback(char * ptr, size_t size, size_t nmemb, void * userdata);
    
    public:
   //GeolocationRequest();
    
    bool RequestLocation(const std::string &json);

    void Display();

    ~GeolocationRequest();


};

#endif