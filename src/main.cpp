#include "mbed.h"

#include "rgbpanel.h"
#include "EthernetInterface.h"
#include "rtos.h"

DigitalOut test(p21);

DigitalOut myled(LED1);
RgbPanel panel(p5,p6,p7, p8, p9, p10, p11,p12,p13,p14, p15, p16, p17);

Thread* thread;

void network_thread(void const *args){
    EthernetInterface eth;
    eth.init(); //Use DHCP
    eth.connect();
    printf("\nServer IP Address is %s\n", eth.getIPAddress());
    
    UDPSocket server;
    server.bind(49376);
    
    Endpoint client;
    char buffer[1024];
    while (true) {
        printf("\nWaiting for UDP packet...\n");
        int n = server.receiveFrom(client, buffer, sizeof(buffer));
        thread->set_priority(osPriorityHigh);
        memcpy(panel.buffer, buffer, n);
        thread->set_priority(osPriorityNormal);
    }
}

int main() {
    
/*    for(int i = 0; i < 32; i++) {
        panel.buffer[(i * 32) + i] = ((i & 3) + 1);
        panel.buffer[(i * 32) + 31 - i] = ((i & 3) + 1);
    }*/

     thread = new Thread(network_thread);



    while(1) {
        test = 1;
        panel.draw();
        test = 0;
        wait_us(10);
    }
}
