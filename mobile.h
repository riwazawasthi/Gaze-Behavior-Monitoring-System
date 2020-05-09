/*Author : Riwaz Awasthi
 *Description: The following code implements a mobile module that collects
 * a person's gaze behavior and appropriately converts that data into a tuple.
 * Each tuple is stored in a data structure with the start and end times for "gaze"
 * at a certain ROI of image is adjusted. Once there are 20 tuples, the module packetizes it
 * and tries to transfer it to the server module. Two signals are used to indicate the packet transmission
 * beginning and end. The bandwidth of transmission is 0.5MHz and since a tuple has three elements,
 * (ROI_index, t_start, t_end), with each element of tuple being 64bits, the packet size is 64x3 bits.
 * The packet transmission delay is equal to the packet size/bandwidth.
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
		sc_in<bool> packet_start; //From Server, indicates an incoming image packet
		sc_in<bool> packet_end; //From Server, indicates end of incoming image packet

		sc_in<bool> print_clock;

		// Outputs
		sc_out<bool> request; // To Server
		sc_out<bool> start; // To Server
		sc_out<bool> end; // To Server


		SC_HAS_PROCESS(mobileBlock);
		mobileBlock(sc_module_name name) : sc_module(name) {
      s = name;
			srand(time(NULL));
			SC_THREAD(prcSensor);
			sensitive<<clock.pos();
			SC_THREAD(prcRx);
			sensitive<<packet_start.pos();
			SC_THREAD(prcImageChange);
			SC_THREAD(printTotalMemory);
			sensitive<<print_clock.pos();

		};

	private:
		int tupleCounter = 0;
		int imagePacketCounter = 0;
		int transmitPacketCounter = 0;
		int receivePacketCounter = 0;
		int image_counter = 0;
		int gazeDataIndex = 0;


		std::map<int , std::vector<point> > _roiMap;
		std::vector<roiTuple> tupleMap;
		std::string s;

		void prcSensor () {
			while(true){
				wait();
				int x, y;
				if(gazeDataIndex >= _gazeMap.size()){
					gazeDataIndex = 0;
				}
				if(_roiMap.size() != 0){
					//read and store x,y;
					x = _gazeMap[gazeDataIndex].x;
					y = _gazeMap[gazeDataIndex].y;
					gazeDataIndex++;

					prcConvert(point(x,y));  //transfer the flow
				}

			}

		}

		void prcConvert (struct point _p) {
			int _roi = 0;
			for(int i = 1 ; i <= _roiMap.size(); i++) {
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
				//cout<<"@"<<sc_time_stamp().to_seconds()<<"s "<<s<<" has a packet ready"<<endl;
				//cout<<"@"<<sc_time_stamp().to_seconds()<<"s "<<s<<" packets = "<<imagePacketCounter<<endl;
				prcTx();
			}

		}

		void prcTx () {
			while(transmitPacketCounter>0){
					while(!network_free.read()){
						//cout<<"@"<<sc_time_stamp().to_seconds()<<"s "<<"Network busy for "<<s<<endl;
						wait((rand()%6)+1 , SC_SEC);  //wait if netwrok busy
					}
					bool success = false;
					while(!success){
						request.write(1); //request network access
						//cout<<"@"<<sc_time_stamp().to_seconds()<<"s "<<s<<" requests network access"<<endl;
						wait(5,SC_NS);
						if(server_ack.read()){
							//cout<<"@"<<sc_time_stamp().to_seconds()<<"s Network access granted to "<<s<<endl;
							start.write(0);
							end.write(0);
							wait(5,SC_NS);
							start.write(1);
							//cout<<"@"<<sc_time_stamp().to_seconds()<<"s "<<s<<" begins packet transmission"<<endl;
							wait(MOBILE_TO_SERVER_PACKET_TRANSMISSION_DELAY,SC_MS); //packet delay = packet size/bandwidth
							end.write(1);
							wait(5,SC_NS);
							//cout<<"@"<<sc_time_stamp().to_seconds()<<"s "<<s<<" ends packet transmission"<<endl;
							success = true;
							transmitPacketCounter--;
						}
						else{
							//cout<<"@"<<sc_time_stamp().to_seconds()<<"s Server didn't acknowledge "<<s<<endl;
							wait(rand()%6 , SC_SEC);
							break;
						}
					}
					request.write(0);
					wait(5,SC_NS);
				}


		}

		void prcRx(){
			while(true){
				wait();
				wait(packet_end.posedge_event());
				//cout<<"@"<<sc_time_stamp().to_seconds()<<"s An image packet received by "<<s<<endl;
				receivePacketCounter++;
				if(receivePacketCounter == MAX_RX_PACKETS){
					image_counter++;
					//cout<<"@"<<sc_time_stamp().to_seconds()<<"s Full image received by "<<s<<" , image_counter = "<<image_counter<<endl;
					receivePacketCounter = 0;
				}

			}
		}

		void prcImageChange(){
			while(true){
				wait(t1, SC_SEC); //image 1 at t1 sec
				_roiMap = _roiMap1;
				tupleCounter = 0;
				transmitPacketCounter = 0;
				imagePacketCounter = 0;
				//cout<<"@"<<sc_time_stamp().to_seconds()<<"s Image 1 is being processed by "<<s<<" , image_counter = "<<image_counter<<endl;

				wait(t2-t1, SC_SEC);  //image 2 at t2 sec
				image_counter--;
				_roiMap = _roiMap2;
				tupleCounter = 0;
				transmitPacketCounter = 0;
				imagePacketCounter = 0;
				//cout<<"@"<<sc_time_stamp().to_seconds()<<"s Image 2 is being processed by "<<s<<" , image_counter = "<<image_counter<<endl;

				wait(t3-t2, SC_SEC);   //image 3 at t3 sec
				image_counter--;
				_roiMap = _roiMap3;
				tupleCounter = 0;
				transmitPacketCounter = 0;
				imagePacketCounter = 0;
				//cout<<"@"<<sc_time_stamp().to_seconds()<<"s Image 3 is being processed by "<<s<<" , image_counter = "<<image_counter<<endl;

				wait(t4-t3, SC_SEC);   //image 4 at t4 sec
				image_counter--;
				_roiMap = _roiMap4;
				tupleCounter = 0;
				transmitPacketCounter = 0;
				imagePacketCounter = 0;
				//cout<<"@"<<sc_time_stamp().to_seconds()<<"s Image 4 is being processed by "<<s<<" , image_counter = "<<image_counter<<endl;

				wait(t5-t4, SC_SEC);  //image 5 at t5 sec
				image_counter--;
				_roiMap = _roiMap5;
				tupleCounter = 0;
				transmitPacketCounter = 0;
				imagePacketCounter = 0;
				//cout<<"@"<<sc_time_stamp().to_seconds()<<"s Image 5 is being processed by "<<s<<" , image_counter = "<<image_counter<<endl;

			}
		}

		void printTotalMemory(){
      while(true){
				wait();
				int temp = (transmitPacketCounter * 3840) + (tupleCounter*64*3)
				           + (receivePacketCounter*PACKET_SIZE) + (image_counter*8000000);
				std::string s1 ("_mobile3_");  //change to respective mobile name to print the corresponding data
				if(s.compare(s1) == 0){
				  cout<<temp<<endl;
				}

			}

		}

};

#endif
