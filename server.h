#ifndef SERVER_H
#define SERVER_H

#include <systemc.h>

class serverBlock : public sc_module {
  public:
    //Inputs
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
      sensitive<<requestM1<<requestM2<<requestM3;
    };

  private:
    bool _network_free = 1;
    void prcRX(){
      while(true){
        wait();
        if(requestM1.read()== 0  && requestM2.read()==0 && requestM3.read()==0){
          continue;
        }
        if(requestM1.read()){  //if the request is from M1
          cout<<sc_time_stamp()<<" mobile1"<<" requests network access"<<endl;
          if(_network_free){
            server_ok1.write(1);
            wait(5, SC_NS);
            cout<<sc_time_stamp()<<" Network access granted to mobile1"<<endl;
          }
          _network_free = 0;
          network_free1.write(0);
          network_free2.write(0);
          network_free3.write(0);
          wait(5,SC_NS);

          wait(startM1.posedge_event());
          cout<<sc_time_stamp()<<" mobile1"<<" begins packet transmission"<<endl;
          wait(endM1.posedge_event());
          cout<<sc_time_stamp()<<" mobile1"<<" ends packet transmission"<<endl;

          _network_free = 1;
          network_free1.write(1);
          network_free2.write(1);
          network_free3.write(1);
          wait(5, SC_NS);
        }

        if(requestM2.read()){//if the request is from M2
          cout<<sc_time_stamp()<<" mobile2"<<" requests network access"<<endl;
          if(_network_free){
            server_ok2.write(1);
            wait(5, SC_NS);
            cout<<sc_time_stamp()<<" Network access granted to mobile2"<<endl;
          }
          _network_free = 0;
          network_free1.write(0);
          network_free2.write(0);
          network_free3.write(0);
          wait(5,SC_NS);

          wait(startM2.posedge_event());
          cout<<sc_time_stamp()<<" mobile2"<<" begins packet transmission"<<endl;
          wait(endM2.posedge_event());
          cout<<sc_time_stamp()<<" mobile2"<<" ends packet transmission"<<endl;

          _network_free = 1;
          network_free1.write(1);
          network_free2.write(1);
          network_free3.write(1);
          wait(5, SC_NS);
        }

        if(requestM3.read()){ //if the request is from M3
          cout<<sc_time_stamp()<<" mobile3"<<" requests network access"<<endl;
          if(_network_free){
            server_ok3.write(1);
            wait(5, SC_NS);
            cout<<sc_time_stamp()<<" Network access granted to mobile3"<<endl;
          }
          _network_free = 0;
          network_free1.write(0);
          network_free2.write(0);
          network_free3.write(0);
          wait(5,SC_NS);

          wait(startM3.posedge_event());
          cout<<sc_time_stamp()<<" mobile3"<<" begins packet transmission"<<endl;
          wait(endM3.posedge_event());
          cout<<sc_time_stamp()<<" mobile3"<<" ends packet transmission"<<endl;

          _network_free = 1;
          network_free1.write(1);
          network_free2.write(1);
          network_free3.write(1);
          wait(5, SC_NS);
        }
      }
    }
};
#endif
