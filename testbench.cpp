#include "top.h"

int sc_main(int argc , char* argv[]) {
  srand(time(NULL));
	cout<<"@ "<<sc_time_stamp()<<"----------Start---------"<<endl<<endl<<endl;
	gazeMonitor gaze("_gazeMonitor_");

	cout<<"@ "<<sc_time_stamp()<<"----------Start Simulation---------"<<endl<<endl<<endl;
	sc_start(100000 , SC_MS);
	cout<<"@ "<<sc_time_stamp()<<"----------End Simulation---------"<<endl<<endl<<endl;
	return 0;
}
