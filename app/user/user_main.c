#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "udhcp/dhcpd.h"

extern char drcom_user[];
extern char drcom_pass[];
extern uint64_t drcom_mac;
extern uint64_t mac_test;

extern char host[];
extern char os[];
extern int user_len;
extern int pass_len;
extern int host_len;
extern int os_len;

void ICACHE_FLASH_ATTR waitap(void *args)
{
    while(1)
    {
        vTaskDelay(1000 / portTICK_RATE_MS);
        if(5 == wifi_station_get_connect_status())
        {
            printf("[*] Wifi-Got IP\n");
            break;
        }
    }
	
	main(NULL);

}
void ICACHE_FLASH_ATTR
user_init(void) {
    uart_init(115200, 115200);
    //printf("engine start \n");
    //led_init();
    //led_red();
	gpio_14_init();
	if (get_gpio14() == 0)
	{
        struct softap_config wificonfig;
        memset(wificonfig.ssid, 0, 32);
        memset(wificonfig.password, 0, 64);
        strcpy(wificonfig.ssid, "DrCOMDialer");
        wificonfig.channel = 1;
        wificonfig.authmode = AUTH_OPEN;
        wificonfig.ssid_hidden = 0;
        wificonfig.max_connection = 10;
        wifi_softap_set_config(&wificonfig);
        wifi_set_opmode(0x02);

        
		printf("[*] gpio_14 LOW, start httpd\n");
	    {
	        struct dhcp_info *pdhcp_info = NULL;
	        pdhcp_info = (struct dhcp_info *)zalloc(sizeof(struct dhcp_info));
	        pdhcp_info->start_ip = ipaddr_addr("192.168.145.100");
	        pdhcp_info->end_ip = ipaddr_addr("192.168.145.110"); // don't set the range too large, because it will cost memory.
	        pdhcp_info->max_leases = 10;
	        pdhcp_info->auto_time = 60;
	        pdhcp_info->decline_time = 60;
	        pdhcp_info->conflict_time = 60;
	        pdhcp_info->offer_time = 60;
	        pdhcp_info->min_lease_sec = 60;
	        dhcp_set_info(pdhcp_info);
	        udhcpd_start();
	        free(pdhcp_info);
	    }
		
		httpd_init();
	} else {
        char ssid[32];
        char wifipass[32];

        load_drcom_params();
        printf("[*] drcom params loaded, username=%s, password=%s, mac=", drcom_user, drcom_pass);
        print_data(&drcom_mac, 6);
        user_len = strlen(drcom_user);
        pass_len = strlen(drcom_pass);

        load_wifi_params(ssid, wifipass);
        printf("[*] AP params loaded, ssid=%s, password=%s\n", ssid, wifipass);
        
		struct ip_info station_ip;
		/* init wifi */
		printf("[*] wifi config\n");
		struct station_config wificonfig;
		
		memset(&wificonfig, 0 , sizeof(struct station_config));
		wifi_set_opmode(0x01);
		printf("[*] copying ssid\n");
		strcpy(wificonfig.ssid, ssid);
		strcpy(wificonfig.password, wifipass);
		printf("[*] wifi setting.\n");
		wifi_station_set_config(&wificonfig);
		printf("[*] wifi connect\n");
		wifi_station_connect();

		
		printf("[*] DrCOM start\n");
		xTaskCreate(waitap, "waitap", 8192, NULL, 2, NULL);
	}
}
