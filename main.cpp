#include <iostream>
#include <string>
#include "GeolocationWifiPoints.hpp"
#include "GeolocationRequest.hpp"

main(int argc, char * argv[]) 
{
    bool success = false;
    GeolocationWifiPoints accessPts;
    GeolocationRequest request;

    success = accessPts.FindWifiPoints();
    if (!success)
        return 0;
        
    std::string requestStr = accessPts.CreateJson();
    std::cout << std::endl << requestStr << std::endl;

    request.RequestLocation(requestStr);
    std::cout << "Location of this PC: " << std::endl;
    request.Display();

    return 0;
}