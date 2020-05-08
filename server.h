#ifndef SERVER_H
#define SERVER_H

#include <systemc.h>
#include <data_structures.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

class serverBlock : public sc_module {
  public:
    //Inputs
    //sc_in<bool> clock;
    sc_in<bool> requestM1;
    sc_in<bool> requestM2;
    sc_in<bool> requestM3;

    sc_in<bool> startM1;
    sc_in<bool> startM2;
    sc_in<bool> startM3;

    sc_in<bool> endM1;
    sc_in<bool> endM2;
    sc_in<bool> endM3;

    //Outputs
    sc_out<bool> network_free1;
    sc_out<bool> network_free2;
    sc_out<bool> network_free3;

    sc_out<bool> server_ok1; //to mobile 1
    sc_out<bool> server_ok2; //to mobile 2
    sc_out<bool> server_ok3; //to mobile 3

    sc_out<bool> packet_start1;
		sc_out<bool> packet_start2;
		sc_out<bool> packet_start3;

		sc_out<bool> packet_end1;
		sc_out<bool> packet_end2;
		sc_out<bool> packet_end3;

    SC_HAS_PROCESS(serverBlock);
    serverBlock(sc_module_name name) : sc_module(name) {
      srand(time(NULL));
      SC_THREAD(prcRX);
      SC_THREAD(prcTX);
    };

  private:
    bool _network_free = 1;

    void prcRX(){
      while(true){
        wait(requestM1.value_changed_event()|
             requestM2.value_changed_event()| requestM3.value_changed_event());

        if(requestM1.read()== 0  && requestM2.read()==0 && requestM3.read()==0){
          continue;
        }
        if(requestM1.read()){  //if the request is from M1
          if(_network_free){
            server_ok1.write(1);
            _network_free = 0;
            network_free1.write(0);
            network_free2.write(0);
            network_free3.write(0);

            wait(startM1.posedge_event());
            wait(endM1.posedge_event());

            _network_free = 1;
            network_free1.write(1);
            network_free2.write(1);
            network_free3.write(1);
            wait(5,SC_NS);
          }

        }

        else if(requestM2.read()){//if the request is from M2
          if(_network_free){
            server_ok2.write(1);
            _network_free = 0;
            network_free1.write(0);
            network_free2.write(0);
            network_free3.write(0);

            wait(startM2.posedge_event());
            wait(endM2.posedge_event());

            _network_free = 1;
            network_free1.write(1);
            network_free2.write(1);
            network_free3.write(1);
            wait(5,SC_NS);
          }

        }

        else if(requestM3.read()){ //if the request is from M3
          if(_network_free){
            server_ok3.write(1);
            _network_free = 0;
            network_free1.write(0);
            network_free2.write(0);
            network_free3.write(0);

            wait(startM3.posedge_event());
            wait(endM3.posedge_event());

            _network_free = 1;
            network_free1.write(1);
            network_free2.write(1);
            network_free3.write(1);
            wait(5,SC_NS);
          }



        }
        //_network_free = 1;
        //network_free1.write(1);
        //network_free2.write(1);
        //network_free3.write(1);
        //wait(50,SC_NS);
      }

    }

    void prcTX(){
      while(true){

        wait(t1 - IMAGE_TRANSMIT_TIME , SC_SEC); // start transmitting image 1

        transmitToMobile1();
        transmitToMobile2();
        transmitToMobile3();

        //wait(t2-t1, SC_SEC);  // start transmittig image 2

        transmitToMobile1();
        transmitToMobile2();
        transmitToMobile3();

        //wait(t3-t2, SC_SEC); // start transmitting image 3

        transmitToMobile1();
        transmitToMobile2();
        transmitToMobile3();


        //wait(t4-t3, SC_SEC); // start transmitting image 4

        transmitToMobile1();
        transmitToMobile2();
        transmitToMobile3();


        //wait(t5-t4, SC_SEC); // start transmitting image 5

        transmitToMobile1();
        transmitToMobile2();
        transmitToMobile3();

      }
    }

    void transmitToMobile1(){
      for(int i = 0; i<MAX_RX_PACKETS; i++){
        while(true){
          if(_network_free){
            /*
            while(!_network_free){
              //cout<<"@"<<sc_time_stamp().to_seconds()<<"s Network busy for server to send image packets to mobile 1"<<endl;
              wait((rand()%6)+1 , SC_SEC);  //wait if netwrok busy
            }
            */
            _network_free = false;
            network_free1.write(0);
            network_free2.write(0);
            network_free3.write(0);

            packet_start1.write(0);
            packet_end1.write(0);
            wait(5, SC_NS);
            packet_start1.write(1);
            //cout<<"@"<<sc_time_stamp().to_seconds()<<"s Image packet transmission to mobile 1 begins"<<endl;
            wait(SERVER_TO_MOBILE_PACKET_TRANSMISSION_DELAY, SC_MS);
            //cout<<"@"<<sc_time_stamp().to_seconds()<<"s Image packet transmission to mobile 1 ends"<<endl;
            packet_end1.write(1);

            _network_free = true;
            network_free1.write(1);
            network_free2.write(1);
            network_free3.write(1);

            wait(DELTA, SC_SEC);
            break;
          }
          else{
            wait(DELTA, SC_SEC);
          }

        }

      }
    }

    void transmitToMobile2(){
      for(int i = 0; i<MAX_RX_PACKETS; i++){
        while(true){
          if(_network_free){
            /*
            while(!_network_free){
              //cout<<"@"<<sc_time_stamp().to_seconds()<<"s Network busy for server to send image packets to mobile 2"<<endl;
              wait((rand()%6)+1 , SC_SEC);  //wait if netwrok busy
            }
            */
            _network_free = false;
            network_free1.write(0);
            network_free2.write(0);
            network_free3.write(0);

            packet_start2.write(0);
            packet_end2.write(0);
            wait(5, SC_NS);
            packet_start2.write(1);
            //cout<<"@"<<sc_time_stamp().to_seconds()<<"s Image packet transmission to mobile 2 begins"<<endl;
            wait(SERVER_TO_MOBILE_PACKET_TRANSMISSION_DELAY, SC_MS);
            //cout<<"@"<<sc_time_stamp().to_seconds()<<"s Image packet transmission to mobile 2 ends"<<endl;
            packet_end2.write(1);

            _network_free = true;
            network_free1.write(1);
            network_free2.write(1);
            network_free3.write(1);

            wait(DELTA, SC_SEC);
            break;
          }
          else{
            wait(DELTA, SC_SEC);
          }
        }

      }
    }

    void transmitToMobile3(){
      for(int i = 0; i<MAX_RX_PACKETS; i++){
        while(true){
          if(_network_free){
            /*
            while(!_network_free){
              //cout<<"@"<<sc_time_stamp().to_seconds()<<"s Network busy for server to send image packets to mobile 3"<<endl;
              wait((rand()%6)+1 , SC_SEC);  //wait if netwrok busy
            }
            */
            _network_free = false;
            network_free1.write(0);
            network_free2.write(0);
            network_free3.write(0);

            packet_start3.write(0);
            packet_end3.write(0);
            wait(5, SC_NS);
            packet_start3.write(1);
            //cout<<"@"<<sc_time_stamp().to_seconds()<<"s Image packet transmission to mobile 3 begins"<<endl;
            wait(SERVER_TO_MOBILE_PACKET_TRANSMISSION_DELAY, SC_MS);
            //cout<<"@"<<sc_time_stamp().to_seconds()<<"s Image packet transmission to mobile 3 ends"<<endl;
            packet_end3.write(1);

            _network_free = true;
            network_free1.write(1);
            network_free2.write(1);
            network_free3.write(1);

            wait(DELTA, SC_SEC);
            break;
          }
          else{
            wait(DELTA, SC_SEC);
          }
        }

      }
    }
};
#endif
