#include "GeolocationWifiPoints.hpp"
//#include <boost/json/src.hpp>

bool GeolocationWifiPoints::FindWifiPoints()
{
    HANDLE hClient = NULL;
    DWORD dwMaxClient = 2;
    DWORD dwCurVersion = 0;
    DWORD dwResult = 0;
    DWORD dwRetVal = 0;
    int iRet = 0;
    std::string macTemp;

    /* variables for WlanEnumInterfaces */

    PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
    PWLAN_INTERFACE_INFO pIfInfo = NULL;
    
    PWLAN_BSS_LIST pBssList = NULL;
    PWLAN_BSS_ENTRY pBssEntry = NULL;

    dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
    if (dwResult != ERROR_SUCCESS) 
    {
        std::cout << "WlanOpenHandle failed with error: " << dwResult << std::endl;
        return false;
    }

    dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
    if (dwResult != ERROR_SUCCESS)
    {
        std::cout << "WlanEnumInterfaces failed with error: " << dwResult << std::endl;
        return false;
    } else 
    {
        std::cout << "Number of interfaces: " << pIfList->dwNumberOfItems << std::endl;
        for (int i = 0; i < (int) pIfList->dwNumberOfItems; ++i)
        {
            pIfInfo = (WLAN_INTERFACE_INFO *) &pIfList->InterfaceInfo[i];
            dwResult = WlanGetNetworkBssList(hClient,
                                             &(pIfInfo->InterfaceGuid), 
                                             NULL,
                                            dot11_BSS_type_any,
                                            false,
                                            NULL,
                                             &pBssList);
            if (dwResult != ERROR_SUCCESS) 
            {
                std::cout << "WlanGetNetworkBssList failed with error: " << dwResult << std::endl;   
            } else 
            {
                for (int j = 0; j < (int) pBssList->dwNumberOfItems; ++j) 
                {
                    pBssEntry = (WLAN_BSS_ENTRY *) &pBssList->wlanBssEntries[j];
                    macTemp = std::format("{:0>2x}:{:0>2x}:{:0>2x}:{:0>2x}:{:0>2x}:{:0>2x}",
                         pBssEntry->dot11Bssid[0],
                         pBssEntry->dot11Bssid[1],
                         pBssEntry->dot11Bssid[2],
                         pBssEntry->dot11Bssid[3],
                         pBssEntry->dot11Bssid[4],
                         pBssEntry->dot11Bssid[5]);

                    std::cout << pBssEntry->dot11Ssid.ucSSID << std::endl;
                    std::cout << macTemp << std::endl;
                    nearbyAccessPoints.push_back(Wifipoint{(const char *)pBssEntry->dot11Ssid.ucSSID,
                                                                macTemp,
                                                                pBssEntry->lRssi});
                    //std::cout << std::hex << (int)pBssEntry->dot11Bssid[0] << std::endl;
                }
            }
        }
    }
    return true;
}

std::string GeolocationWifiPoints::CreateJson()
{
    std::string json;
    boost::json::object obj, tempObj;
    boost::json::array wifiArr;
    for (auto& wifiPt : nearbyAccessPoints)
    {
        tempObj["macAddress"] = wifiPt.MAC;
        tempObj["signalStength"] = wifiPt.Strength;
        tempObj["signalToNoiseRatio"] = 0;
        wifiArr.emplace_back(tempObj);
    }
    obj["considerIp"] = false;
    obj["wifiAccessPoints"] = wifiArr;
    json = boost::json::serialize(obj);
    return json;
}