#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include<systemc.h>
#include <map>
#include <fstream>
#include<iostream>

#define TUPLES_PER_PACKETS 20

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

std::map<int , std::vector<point> > roiMap ();
std::vector<point> gazeMap();

std::map<int , std::vector<point> > _roiMap = roiMap();
std::vector<point> _gazeMap = gazeMap();

std::map<int , std::vector<point> > roiMap () { //image 1
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
