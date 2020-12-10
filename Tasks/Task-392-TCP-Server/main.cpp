/*#if !FEATURE_LWIP
    #error [NOT_SUPPORTED] LWIP not supported for this target
#endif
 */
#include "mbed.h"
#include "EthernetInterface.h"
#include "TCPSocket.h"
 
#define HTTP_STATUS_LINE "HTTP/1.0 200 OK"
#define HTTP_HEADER_FIELDS "Content-Type: text/html; charset=utf-8"
#define HTTP_MESSAGE_BODY ""                                     \
"<html>" "\r\n"                                                  \
"  <body style=\"display:flex;text-align:center\">" "\r\n"       \
"    <div style=\"margin:auto\">" "\r\n"                         \
"      <h1>Hello World</h1>" "\r\n"                              \
"      <p>It works !</p>" "\r\n"                                 \
"    </div>" "\r\n"                                              \
"  </body>" "\r\n"                                               \
"</html>" "\r\n"
    
#define HTTP_RESPONSE HTTP_STATUS_LINE "\r\n"   \
                      HTTP_HEADER_FIELDS "\r\n" \
                      "\r\n"                    \
                      HTTP_MESSAGE_BODY "\r\n"
#define HTTP_TITLE     "<head><title> Plymouth Uni Weather Page </title></head>" "\r\n"
#define HTTP_FORMAT_1 "<body style=\"display:flex;text-align:center\">" "\r\n" \
                      "<div style=\"margin:auto\">" "\r\n"
#define HTTP_BOTTOM "</html>" "\r\n"

 
//For the static IP version
#define IP        "10.0.0.10"
#define NETMASK   "255.255.255.0"
#define GATEWAY   "10.0.0.1"

//const char *const myHTTP = HTTP_RESPONSE; //This if you wish to set above Compiler defines into Flash Silicon

EthernetInterface net;

int main()
{
    printf("\r\nBasic HTTP server example\r\n");
    
    // Connect the ethernet interface
    //net.set_network(IP, NETMASK, GATEWAY);  //For static IP
    net.connect();

    // Show the network address
    SocketAddress a;
    net.get_ip_address(&a);
    printf("IP address: %s\n", a.get_ip_address() ? a.get_ip_address() : "None");

    // Open a socket on the network interface, and create a TCP connection on port 80
    TCPSocket socket;
    socket.open(&net);
    socket.bind(80);

    
    //Set socket to listening mode
    int err=socket.listen(5);
    if(err==0) {
        printf("Listening OK\n\r");
    }
    else {
        printf("Listen error=%d\n\r",err);
        socket.close();
        while(1);
    }

    while (true)
    {
        // ACCEPT Accepting connections
        TCPSocket* clt_sock=socket.accept();    //Blocking

        //Unblocks with each connection

        //Send response (blocking until completed)
        printf("%s STRING LENGTH is: %d\n\r", HTTP_RESPONSE, strlen(HTTP_RESPONSE)); // the rest of this line to use Flash Silicon *see notes above line number 35" myHTTP,strlen(myHTTP));
        nsapi_size_or_error_t ret = clt_sock->send(HTTP_RESPONSE, strlen(HTTP_RESPONSE));  //myHTTP,mydatasize)the rest of this line to use Flash Silicon *see notes above line number 35" myHTTP,strlen(myHTTP));
        
        //Echo how many bytes were sent
        printf("Sent %d bytes\n", ret);

        //You are responsible to close this
        clt_sock->close();

        wait_us(1000000);//Delay 1 second
    }


}
 
//have to close socket to re-enter