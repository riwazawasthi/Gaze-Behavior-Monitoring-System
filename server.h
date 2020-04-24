#ifndef SERVER_H
#define SERVER_H

#include <systemc.h>

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

    SC_HAS_PROCESS(serverBlock);
    serverBlock(sc_module_name name) : sc_module(name) {
      SC_THREAD(prcRX);
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
          }
          _network_free = 0;
          network_free1.write(0);
          network_free2.write(0);
          network_free3.write(0);
          wait(5,SC_NS);

          wait(startM1.posedge_event());
          wait(endM1.posedge_event());
        }

        else if(requestM2.read()){//if the request is from M2
          if(_network_free){
            server_ok2.write(1);
          }
          _network_free = 0;
          network_free1.write(0);
          network_free2.write(0);
          network_free3.write(0);
          wait(5,SC_NS);

          wait(startM2.posedge_event());
          wait(endM2.posedge_event());
        }

        else if(requestM3.read()){ //if the request is from M3
          if(_network_free){
            server_ok3.write(1);
          }
          _network_free = 0;
          network_free1.write(0);
          network_free2.write(0);
          network_free3.write(0);
          wait(5,SC_NS);

          wait(startM3.posedge_event());
          wait(endM3.posedge_event());


        }
        _network_free = 1;
        network_free1.write(1);
        network_free2.write(1);
        network_free3.write(1);
        wait(50,SC_NS);
      }

    }
};
#endif
