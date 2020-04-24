/*
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


class mobileBlock : public sc_module {
	public:
		// Inputs
		sc_in<bool> clock;
		sc_out<bool> network_free; // From Server
		sc_out<bool> server_ack; // From Server

		// Outputs
		sc_out<bool> request; // To Server
		sc_out<bool> start; // To Server
		sc_out<bool> end; // To Server


		SC_HAS_PROCESS(mobileBlock);
		mobileBlock(sc_module_name name) : sc_module(name) {
			SC_CTHREAD(prcSensor, clock.pos());

		};

	private:
		int tupleCounter = 0;
		int packetCounter = 0;
		int gazeDataIndex = 0;
		std::vector<roiTuple> tupleMap;

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
			if(!tupleMap.size()) {
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
			if(tupleCounter==TUPLES_PER_PACKETS){
				packetCounter++;
				tupleCounter = 0;
			}
			prcTx();
		}

		void prcTx () {
			while(packetCounter>0){
				while(!network_free.read()){
					cout<<sc_time_stamp()<<" Network busy"<<endl;
					srand(time(NULL));
					wait(rand()%6 , SC_SEC);  //wait if netwrok busy
				}
				bool success = false;
				while(!success){
					request.write(1); //request network access
					wait(5,SC_NS);
					if(server_ack.read()){
						start.write(0);
						end.write(0);
						wait(5,SC_NS);
						start.write(1);
						wait(8,SC_MS); //packet delay = packet size/bandwidth
						end.write(1);

						success = true;
						packetCounter--;
					}
					else{
						srand(time(NULL));
						wait(rand()%6 , SC_SEC);
						break;
					}
				}
				request.write(0);
			}

		}
};

#endif
