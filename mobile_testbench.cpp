#include<systemc.h>
#include<mobile.h>

// template < >
class gazeMonitor : public sc_module {
	public:
		/*Signals for Module Interconnections*/
		sc_signal<bool> clock;
		sc_signal<bool> request1, start1, end1;

    sc_signal<bool> network_free1;

    sc_signal<bool> server_ok1; //to mobile 1


		/*Module Instantiation*/

		mobileBlock* m1;


		SC_HAS_PROCESS(gazeMonitor);
		gazeMonitor(sc_module_name name) : sc_module(name) {
			/**
			 * Module Instances Created
			 **/

			m1 = new mobileBlock ("_mobile1_");


      network_free1.write(1);
      server_ok1.write(1);

      m1->clock(clock);
      m1->network_free(network_free1);
      m1->server_ack(server_ok1);
      m1->request(request1);
      m1->start(start1);
      m1->end(end1);

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
	cout<<endl<<endl<<"@ "<<sc_time_stamp()<<"----------End Simulation---------"<<endl<<endl<<endl;
	return 0;
}
