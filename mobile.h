/*Author : Riwaz Awasthi
 *Description: The following code implements a mobile module that collects
 * a person's gaze behavior and appropriately converts that data into a tuple.
 * Each tuple is stored in a data structure with the start and end times for "gaze"
 * at a certain ROI of image is adjusted. Once there are 20 tuples, the module packetizes it
 * and tries to transfer it to the server module. Two signals are used to indicate the packet transmission
 * beginning and end. The bandwidth of transmission is 0.5MHz and since a tuple has three elements,
 * (ROI_index, t_start, t_end), with each element of tuple being 64bits, the packet size if 64x3 bits.
 * The packet transmission delay is equal to the packet size/bandwidth which comes out to be about 8ms.
 *Input/Outputs:-
  * network_free -> network status
  * server_ack -> ACK from server if it's okay
  * request -> request network access
  * start -> positive edge indicates packet transfer beginning
  * end -> positive edge indicates packet transfer end
 */

#ifndef MOBILE_H
#define MOBILE_H

#include<systemc.h>
#include<data_structures.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string>

class mobileBlock : public sc_module {
	public:
		// Inputs
		sc_in<bool> clock;
		sc_in<bool> network_free; // From Server
		sc_in<bool> server_ack; // From Server

		// Outputs
		sc_out<bool> request; // To Server
		sc_out<bool> start; // To Server
		sc_out<bool> end; // To Server


		SC_HAS_PROCESS(mobileBlock);
		mobileBlock(sc_module_name name) : sc_module(name) {
      s = name;
			SC_THREAD(prcSensor);
			sensitive<<clock.pos();
			//SC_THREAD(prcTx);
			//sensitive<<clock.pos();

		};

	private:
		int tupleCounter = 0;
		int imagePacketCounter = 0;
		int transmitPacketCounter = 0;
		int gazeDataIndex = 0;
		std::vector<roiTuple> tupleMap;
		std::string s;

		void prcSensor () {
			while(true){
				wait();
				int x, y;
				if(gazeDataIndex >= _gazeMap.size()){
					gazeDataIndex = 0;
				}
				//read and store x,y;
				x = _gazeMap[gazeDataIndex].x;
				y = _gazeMap[gazeDataIndex].y;
				gazeDataIndex++;
				prcConvert(point(x,y));  //transfer the flow
			}

		}

		void prcConvert (struct point _p) {
			int _roi = 0;
			for(int i = 1 ; i <= 5 ; i++) {
				if(_p.x >= _roiMap[i][0].x && _p.x <= _roiMap[i][1].x && _p.y >= _roiMap[i][0].y && _p.y <= _roiMap[i][1].y) {
				_roi = i;
				}
			}
			prcCompress(_roi); //transfer the flow
		}

		void prcCompress (int _roi) {

			if(tupleMap.size()==0) {

				prcPacketize(roiTuple(_roi , 0 , 1)); //transfer the flow
				return;
			}
			double temp = tupleMap.back().t_end;
			if(tupleMap.back().roi == _roi) {
				tupleMap.back().t_end = temp + 0.01; //increment the end time if new roi matches last roi

			} else {
				prcPacketize(roiTuple(_roi , temp + 0.01 , temp + 0.01));
			}

		}

		void prcPacketize (const roiTuple &_r) {
			tupleMap.push_back(_r);
			tupleCounter++;
			if(tupleCounter==20){
				imagePacketCounter++;
				transmitPacketCounter++;
				tupleCounter = 0;
				//cout<<"@"<<sc_time_stamp().to_seconds()<<"s "<<s<<" packets = "<<imagePacketCounter<<endl;
				prcTx();
			}

		}

		void prcTx () {
			//while(true){
			//	wait();
				while(transmitPacketCounter>0){
					while(!network_free.read()){
						cout<<"@"<<sc_time_stamp().to_seconds()<<"s "<<"Network busy for "<<s<<endl;
						wait((rand()%6) , SC_SEC);  //wait if netwrok busy
					}
					bool success = false;
					while(!success){
						request.write(1); //request network access
						cout<<"@"<<sc_time_stamp().to_seconds()<<"s "<<s<<" requests network access"<<endl;
						wait(5,SC_NS);
						if(server_ack.read()){
							cout<<"@"<<sc_time_stamp().to_seconds()<<"s Network access granted to "<<s<<endl;
							start.write(0);
							end.write(0);
							wait(5,SC_NS);
							start.write(1);
							cout<<"@"<<sc_time_stamp().to_seconds()<<"s "<<s<<" begins packet transmission"<<endl;
							wait(8,SC_MS); //packet delay = packet size/bandwidth
							end.write(1);
							wait(5,SC_NS);
							cout<<"@"<<sc_time_stamp().to_seconds()<<"s "<<s<<" ends packet transmission"<<endl;
							success = true;
							transmitPacketCounter--;
						}
						else{
							cout<<"@"<<sc_time_stamp().to_seconds()<<"s Server didn't acknowledge "<<s<<endl;
							wait(rand()%6 , SC_SEC);
							break;
						}
					}
					request.write(0);
					wait(5,SC_NS);
				}

			//}
		}

};

#endif
