#include <ardupilotmega/mavlink.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Print message name to buffer acording to its ID
void print_message_name(mavlink_message_t msg, char *buf){
  switch(msg.msgid){
    case MAVLINK_MSG_ID_HEARTBEAT:
      sprintf(buf, "HEARTBEAT");
      break;
    case MAVLINK_MSG_ID_SYS_STATUS:
      sprintf(buf, "SYS_STATUS");
      break;
    case MAVLINK_MSG_ID_SYSTEM_TIME:
      sprintf(buf, "SYSTEM_TIME");
      break;
    case MAVLINK_MSG_ID_PING:
      sprintf(buf, "PING");
      break;
    case MAVLINK_MSG_ID_CHANGE_OPERATOR_CONTROL:
      sprintf(buf, "CHANGE_OPERATOR_CONTROL");
      break;
    case MAVLINK_MSG_ID_CHANGE_OPERATOR_CONTROL_ACK:
      sprintf(buf, "CHANGE_OPERATOR_CONTROL_ACK");
      break;
    case MAVLINK_MSG_ID_AUTH_KEY:
      sprintf(buf, "AUTH_KEY");
      break;
    case MAVLINK_MSG_ID_SET_MODE:
      sprintf(buf, "SET_MODE");
      break;
    case MAVLINK_MSG_ID_PARAM_REQUEST_READ:
      sprintf(buf, "PARAM_REQUEST_READ");
      break;
    case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
      sprintf(buf, "PARAM_REQUEST_LIST");
      break;
    case MAVLINK_MSG_ID_PARAM_VALUE:
      sprintf(buf, "PARAM_VALUE");
      break;
    case MAVLINK_MSG_ID_PARAM_SET:
      sprintf(buf, "PARAM_SET");
      break;
    case MAVLINK_MSG_ID_GPS_RAW_INT:
      sprintf(buf, "GPS_RAW_INT");
      break;
    case MAVLINK_MSG_ID_GPS_STATUS:
      sprintf(buf, "GPS_STATUS");
      break;
    case MAVLINK_MSG_ID_SCALED_IMU:
      sprintf(buf, "SCALED_IMU");
      break;
    case MAVLINK_MSG_ID_RAW_IMU:
      sprintf(buf, "RAW_IMU");
      break;
    case MAVLINK_MSG_ID_RAW_PRESSURE:
      sprintf(buf, "RAW_PRESSURE");
      break;
    case MAVLINK_MSG_ID_SCALED_PRESSURE:
      sprintf(buf, "SCALED_PRESSURE");
      break;
    case MAVLINK_MSG_ID_ATTITUDE:
      sprintf(buf, "ATTITUDE");
      break;
    case MAVLINK_MSG_ID_ATTITUDE_QUATERNION:
      sprintf(buf, "ATTITUDE_QUATERNION");
      break;
    case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
      sprintf(buf, "LOCAL_POSITION_NED");
      break;
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
      sprintf(buf, "GLOBAL_POSITION");
      break;
  }
}


int main(int argc, char* argv[]){

  mavlink_status_t status;
  mavlink_message_t msg;
  int chan = MAVLINK_COMM_0;
  int port;
  sscanf(argv[1], "%d", &port);
  printf("Decoding from port %d \r\n", port);

  // Parse message comming from udp socket
  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Bind socket
  bind(sock, (struct sockaddr *)&addr, sizeof(addr));

  // Receive messages continuosly and decode them
  while(1){
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    int recsize = recv(sock, (void *)buf, MAVLINK_MAX_PACKET_LEN, 0);
    if(recsize > 0){
      // Something received - print out all bytes and parse packet
      mavlink_message_t msg;
      mavlink_status_t status;
      //printf("Bytes Received: %d\n", (int)recsize);
      for(int i = 0; i < recsize; ++i){
        if(mavlink_parse_char(chan, buf[i], &msg, &status)){
          // Print packet information
          //printf("Received packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
          // Print packet info using ardupilotmega dialect
          /*if(msg.msgid == MAVLINK_MSG_ID_HEARTBEAT){
            mavlink_heartbeat_t hb;
            mavlink_msg_heartbeat_decode(&msg, &hb);
            printf("Heartbeat: %d, %d, %d, %d, %d, %d\n", hb.type, hb.autopilot, hb.base_mode, hb.custom_mode, hb.system_status, hb.mavlink_version);
          }*/ 

          // Print message name
          char msg_name[30];
          print_message_name(msg, msg_name);
          //printf("Received packet: %s\n", msg_name);

          // If message is a heartbeat, decode it and print it strictly in binary string format (1s and 0s)
          if(msg.msgid == MAVLINK_MSG_ID_HEARTBEAT){  
            mavlink_heartbeat_t hb;
            mavlink_msg_heartbeat_decode(&msg, &hb);
            printf("Heartbeat: ");
            for(int i = 0; i < 8; i++){
              printf("%d", (hb.type >> i) & 1);
            }
            printf(" ");
            for(int i = 0; i < 8; i++){
              printf("%d", (hb.autopilot >> i) & 1);
            }
            printf(" ");
            for(int i = 0; i < 8; i++){
              printf("%d", (hb.base_mode >> i) & 1);
            }
            printf(" ");
            for(int i = 0; i < 24; i++){
              printf("%d", (hb.custom_mode >> i) & 1);
            }
            printf(" ");
            for(int i = 0; i < 8; i++){
              printf("%d", (hb.system_status >> i) & 1);
            }
            printf(" ");
            for(int i = 0; i < 8; i++){
              printf("%d", (hb.mavlink_version >> i) & 1);
            }
            printf("\n");
          }

        }
      }
    }
  } 

	return 0;
}
