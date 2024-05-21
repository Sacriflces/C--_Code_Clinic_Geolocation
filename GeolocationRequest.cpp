#include "GeolocationRequest.hpp"


size_t GeolocationRequest::WriteCallback(char * data, size_t size, size_t nmemb, void * userdata) 
{
    size_t realsize = size * nmemb;
    struct MemoryStruct * mem = (struct MemoryStruct *)userdata;

    char * ptr = (char *)realloc(mem->response, mem->size + realsize + 1);
    if (!ptr)
    {
        std::cout << "Problem allocating memory for the POST return." << std::endl;
        return 0;
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

bool GeolocationRequest::RequestLocation(const std::string &json) 
{
    curl_global_init(CURL_GLOBAL_ALL);
    CURL * handle = curl_easy_init();
    if (handle)
    {
        std::string fullURL = GEO_LOC_URL;
        fullURL +=  API_KEY;
        struct curl_slist *slist1 = NULL;
        slist1 = curl_slist_append(slist1, "Content-Type: application/json");
        slist1 = curl_slist_append(slist1, "Accept: application/json");
        curl_easy_setopt(handle, CURLOPT_URL, fullURL.c_str());
        curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist1);
        curl_easy_setopt(handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, json.c_str());
        curl_easy_setopt(handle,  CURLOPT_SSL_VERIFYPEER, 0L);

        CURLcode res = curl_easy_perform(handle);
        if (res != CURLE_OK) 
		{
			std::cerr << "ERROR: curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		} else 
        {
            std::cout << chunk.response << std::endl;       
            boost::json::parser p;
            size_t n = p.write(chunk.response);
            boost::json::value jv = p.release();
            boost::json::object obj = jv.as_object();

            for (auto it = obj.begin(); it != obj.end(); ++it)
            {
                std::cout << it->key_c_str() << ": " << it->value() << " "<< it->value().is_double() << std::endl;      
            }
            
            if (obj.if_contains("location"))
            {
            location.latitude = obj["location"].as_object()["lat"].as_double();
            location.longitude = obj["location"].as_object()["lng"].as_double();
            }

            if (obj.if_contains("accuracy"))
            {
                if(obj["accuracy"].is_double())
                    location.radius = obj["accuracy"].as_double();
                else if (obj["accuracy"].is_int64())
                    location.radius = obj["accuracy"].as_int64();
            }            
        }
        curl_easy_cleanup(handle);
    }
    
    return true;
}

void GeolocationRequest::Display()
{
    std::cout << std::setprecision(4) <<"Within a " << location.radius << " meter radius of (" << location.latitude << ", " << location.longitude << ")" << std::endl; 
}

GeolocationRequest::~GeolocationRequest() 
{
    if (chunk.response != nullptr){
        free(chunk.response);
    }
}