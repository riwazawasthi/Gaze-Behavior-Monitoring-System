#include<systemc.h>
#include<server.h>
#include<mobile.h>

// template < >
class gazeMonitor : public sc_module {
	public:
		/*Signals for Module Interconnections*/
		sc_signal<bool> clock;
		sc_signal<bool> request1, start1, end1, request2, start2, end2, request3, start3, end3 ;

    sc_signal<bool> network_free1;
    sc_signal<bool> network_free2;
    sc_signal<bool> network_free3;

    sc_signal<bool> server_ok1; //to mobile 1
    sc_signal<bool> server_ok2; //to mobile 2
    sc_signal<bool> server_ok3; //to mobile 3

		/*Module Instantiation*/
		serverBlock* s;
		mobileBlock* m1;
		mobileBlock* m2;
    mobileBlock* m3;

		SC_HAS_PROCESS(gazeMonitor);
		gazeMonitor(sc_module_name name) : sc_module(name) {
			/**
			 * Module Instances Created
			 **/
			s = new serverBlock ("_server_");
			m1 = new mobileBlock ("_mobile1_");
			m2 = new mobileBlock ("_mobile2_");
      m3 = new mobileBlock ("_mobile3_");

      network_free1.write(1);

      s->requestM1(request1);
      s->requestM2(request2);
      s->requestM3(request3);
      s->startM1(start1);
      s->startM2(start2);
      s->startM3(start3);
      s->endM1(end1);
      s->endM2(end2);
      s->endM3(end3);
      s->network_free1(network_free1);
      s->network_free2(network_free2);
      s->network_free3(network_free3);
      s->server_ok1(server_ok1);
      s->server_ok2(server_ok2);
      s->server_ok3(server_ok3);

      m1->clock(clock);
      m1->network_free(network_free1);
      m1->server_ack(server_ok1);
      m1->request(request1);
      m1->start(start1);
      m1->end(end1);

      m2->clock(clock);
      m2->network_free(network_free2);
      m2->server_ack(server_ok2);
      m2->request(request2);
      m2->start(start2);
      m2->end(end2);

      m3->clock(clock);
      m3->network_free(network_free3);
      m3->server_ack(server_ok3);
      m3->request(request3);
      m3->start(start3);
      m3->end(end3);

			SC_THREAD(clockSignal);
		};

		void clockSignal() {
			while (true){
        wait(5 , SC_MS);
		    clock = false;
		    wait(5 , SC_MS);
		    clock = true;
			}
		};

};

int sc_main(int argc , char* argv[]) {
	cout<<"@ "<<sc_time_stamp()<<"----------Start---------"<<endl<<endl<<endl;
	gazeMonitor gaze("_gazeMonitor_");

	cout<<"@ "<<sc_time_stamp()<<"----------Start Simulation---------"<<endl<<endl<<endl;
	sc_start(30000 , SC_MS);
	cout<<"@ "<<sc_time_stamp()<<"----------End Simulation---------"<<endl<<endl<<endl;
	return 0;
}
