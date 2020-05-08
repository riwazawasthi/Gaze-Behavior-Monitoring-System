#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <systemc.h>
#include <map>
#include <fstream>
#include <iostream>
#include <cmath>

#define PACKET_SIZE 1000000
#define TUPLES_PER_PACKETS 20
#define MAX_RX_PACKETS (8000000/PACKET_SIZE)

#define BANDWIDTH 1000000
#define SERVER_TO_MOBILE_PACKET_TRANSMISSION_DELAY ((float)((PACKET_SIZE * 1000)/BANDWIDTH))
#define MOBILE_TO_SERVER_PACKET_TRANSMISSION_DELAY ((float)((3840 * 1000)/BANDWIDTH))

#define IMAGE_TRANSMIT_TIME ( (((float)(8000000/BANDWIDTH))*3.0f) + 40.0f )

#define DELTA (float)0.25
/*
//case 1
#define t1 200.0f
#define t2 220.0f
#define t3 225.0f
#define t4 230.0f
#define t5 250.0f
*/

//case 2
#define t1 150.0f
#define t2 175.0f
#define t3 200.0f
#define t4 225.0f
#define t5 250.0f


struct roiTuple {
    int roi;
	  double t_start;
    double t_end;
    roiTuple();
    roiTuple(int _r , double _s , double _e) : roi(_r) , t_start(_s) , t_end(_e) {};
    roiTuple(const roiTuple &_r) : roi(_r.roi) , t_start(_r.t_start) , t_end(_r.t_end) {};
};

struct point {
    int x;
    int y;
    point(int _x , int _y) : x(_x) , y(_y) {};
};

std::map<int , std::vector<point> > roiMap1 ();
std::map<int , std::vector<point> > roiMap2 ();
std::map<int , std::vector<point> > roiMap3 ();
std::map<int , std::vector<point> > roiMap4 ();
std::map<int , std::vector<point> > roiMap5 ();
std::vector<point> gazeMap();

std::map<int , std::vector<point> > _roiMap1 = roiMap1();
std::map<int , std::vector<point> > _roiMap2 = roiMap2();
std::map<int , std::vector<point> > _roiMap3 = roiMap3();
std::map<int , std::vector<point> > _roiMap4 = roiMap4();
std::map<int , std::vector<point> > _roiMap5 = roiMap5();
std::vector<point> _gazeMap = gazeMap();

std::map<int , std::vector<point> > roiMap1 () { //image 1
  	std::map<int , std::vector<point> > _map;
    // ROI 1
    _map[1].push_back(point(50 , 20));
    _map[1].push_back(point(400 , 320));
    // ROI 2
    _map[2].push_back(point(50 , 370));
    _map[2].push_back(point(450 , 1000));
    // ROI 3
    _map[3].push_back(point(470 , 20));
    _map[3].push_back(point(600 , 900));
    // ROI 4
    _map[4].push_back(point(670 , 40));
    _map[4].push_back(point(950 , 550));
    // ROI 5
    _map[5].push_back(point(680 , 700));
    _map[5].push_back(point(1000 , 1000));
  	return _map;
}

std::map<int , std::vector<point> > roiMap2 () { //image 1
  	std::map<int , std::vector<point> > _map;
    // ROI 1
    _map[1].push_back(point(10 , 10));
    _map[1].push_back(point(150 , 700));
    // ROI 2
    _map[2].push_back(point(300 , 10));
    _map[2].push_back(point(980 , 250));
    // ROI 3
    _map[3].push_back(point(300 , 270));
    _map[3].push_back(point(980 , 700));
    // ROI 4
    _map[4].push_back(point(10 , 740));
    _map[4].push_back(point(950 , 1000));

  	return _map;
}

std::map<int , std::vector<point> > roiMap3 () { //image 1
  	std::map<int , std::vector<point> > _map;
    // ROI 1
    _map[1].push_back(point(10 , 10));
    _map[1].push_back(point(260 , 900));
    // ROI 2
    _map[2].push_back(point(270 , 10));
    _map[2].push_back(point(520 , 1000));
    // ROI 3
    _map[3].push_back(point(570, 20));
    _map[3].push_back(point(700 , 950));
    // ROI 4
    _map[4].push_back(point(730 , 10));
    _map[4].push_back(point(950 , 950));

  	return _map;
}

std::map<int , std::vector<point> > roiMap4 () { //image 1
  	std::map<int , std::vector<point> > _map;
    // ROI 1
    _map[1].push_back(point(10 , 10));
    _map[1].push_back(point(160 , 1000));
    // ROI 2
    _map[2].push_back(point(170 , 10));
    _map[2].push_back(point(220 , 1000));
    // ROI 3
    _map[3].push_back(point(230 , 10));
    _map[3].push_back(point(380 , 1000));
    // ROI 4
    _map[4].push_back(point(390 , 10));
    _map[4].push_back(point(540 , 1000));
    // ROI 5
    _map[5].push_back(point(550 , 10));
    _map[5].push_back(point(700 , 1000));
    // ROI 6
    _map[6].push_back(point(710 , 10));
    _map[6].push_back(point(860 , 1000));
    // ROI 7
    _map[7].push_back(point(870 , 10));
    _map[7].push_back(point(1010 , 1000));
  	return _map;
}

std::map<int , std::vector<point> > roiMap5 () { //image 1
  	std::map<int , std::vector<point> > _map;
    // ROI 1
    _map[1].push_back(point(10 , 10));
    _map[1].push_back(point(1000 , 160));
    // ROI 2
    _map[2].push_back(point(10 , 170));
    _map[2].push_back(point(1000 , 220));
    // ROI 3
    _map[3].push_back(point(10 , 230));
    _map[3].push_back(point(1000 , 380));
    // ROI 4
    _map[4].push_back(point(100 , 390));
    _map[4].push_back(point(1000 , 540));
    // ROI 5
    _map[5].push_back(point(10 , 550));
    _map[5].push_back(point(1000 , 700));
    // ROI 6
    _map[6].push_back(point(10 , 710));
    _map[6].push_back(point(1000 , 860));
    // ROI 7
    _map[7].push_back(point(10 , 870));
    _map[7].push_back(point(1000 , 1010));
  	return _map;
}

std::vector<point> gazeMap(){
  std::vector<point> _map1;
  std::vector<int> temp;
  int x;
  std::ifstream file ("gazeData.txt");
  while(file>>x){
    temp.push_back(x);
  }
  file.close();

  for(int i = 0; i<temp.size() ; i = i+2){
    _map1.push_back(point(temp[i], temp[i+1]));
  }

  return _map1;

}


#endif
