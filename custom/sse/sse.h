#include "esphome.h"


#ifdef __cplusplus
extern "C"
{
#endif
    #include "sh2lib.h"

    int handle_get_response(struct sh2lib_handle *handle, const char *data, size_t len, int flags)
    {
        ESP_LOGD("SSE", "Recivo");
        delay(500);
        // if (len > 0) {
        //     ESP_LOGD("SSE", "%.*s\n", len, data);
        // }

        // if (flags == DATA_RECV_RST_STREAM) {
        //     ESP_LOGD("SSE", "STREAM CLOSED");
        // }
            return 0;
    }

    void http2_task(void *args)
    {

        ESP_LOGI("SSE", "http2_task => Entro");         

        struct sh2lib_handle hd;
        struct sh2lib_config_t conf = {
            .uri = "https://openhab.iraeka.duckdns.org",
            .cacert_buf = nullptr,
            .cacert_bytes = 0
        };

        //     // , "https://http2.golang.org"


        //https://openhab.iraeka.duckdns.org/rest/events?topics=openhab/items/TermometroEkainTemperature/*


        if (sh2lib_connect(&conf, &hd) != ESP_OK) {
            ESP_LOGE("SSE", "http2_task => Error connecting to HTTP2 server"); 
            delay(1000);
            vTaskDelete(NULL);
        }

        ESP_LOGD("SSE", "http2_task => Connected to %s", hd.hostname);         
        delay(500);

        sh2lib_do_get(&hd, "/rest/events", handle_get_response);
        ESP_LOGD("SSE", "http2_task => sh2lib_do_get executed");         
        delay(500);

                
        bool finish =  false;

        while (!finish) {
            ESP_LOGI("SSE", "http2_task => loop!");         
                        
            if (sh2lib_execute(&hd) != ESP_OK) {
                ESP_LOGE("SSE", "http2_task => Error in send/receive");
                finish = true;
            } 

            delay(10);
        }
        

        sh2lib_free(&hd);
            
        ESP_LOGI("SSE", "http2_task => Disconnected");

        vTaskDelete(NULL);
    }



#ifdef __cplusplus
}
#endif

//root/.platformio/packages/framework-espidf/components/nghttp/nghttp2/lib/nghttp2_session.c




class SSE : public Component, public Sensor {
    public:
        void setup() override {
            ESP_LOGD("SSE", "creating http2_task...");
            xTaskCreate(http2_task, "http2_task", (1024 * 32), NULL, 5, NULL);
        }

        void loop() override {
             ESP_LOGD("SSE", "loop()");
           // This will be called by App.loop()
            //vTaskDelete(NULL);
        }

        float get_setup_priority() const override { return esphome::setup_priority::LATE; }

    private:
    


     
};






