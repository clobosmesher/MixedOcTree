/*
 <Mix-mesher: region type. This program generates a mixed-elements mesh>
 
 Copyright (C) <2013,2017>  <Claudio Lobos>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/gpl.txt>
 */

#ifndef MeshPoint_h
#define MeshPoint_h 1

#include <iostream>
#include <math.h>
#include "Point3D.h"
#include <vector>
#include <list>

using Clobscode::Point3D;
using std::list;
using std::vector;

namespace Clobscode
{	
	class MeshPoint{
		
	public:
		
		MeshPoint();
		
		MeshPoint(const Point3D &p);
		
		//MeshPoint(const Point3D p, const un
		
		virtual ~MeshPoint();
		
		virtual void setPoint(Point3D &p);
		
		//acces method:
		virtual Point3D &getPoint();
		
		virtual void addElement(unsigned int idx);
		
		virtual list<unsigned int> &getElements();
		
		virtual void clearElements();
		
		virtual bool wasOutsideChecked();
		
		virtual void outsideChecked();
		
		virtual void setMaxDistance(double md);
		
		virtual double getMaxDistance();
		
		virtual void updateMaxDistanceByFactor(const double &per);
		
		virtual void setProjected();
		
		virtual bool wasProjected();
		
		//state methods
		virtual void setOutside();
		
		virtual void setInside();
		
		//returns true if node is inside any input mesh
		virtual bool isInside();
		
		//returns true if node is outside every input mesh
		virtual bool isOutside();
		
		virtual void setIOState(bool state);
		
		virtual bool getIOState();
		
	protected:
		
		Point3D point;
		//this flag avoids to re-check if node is inside, 
		//which is an expensive task
		bool outsidechecked, projected;
		//inside is a flag to shrink elements to the surface.
		//it is a vector to know the state w.r.t. every input
		//geometry
		bool inside;//, projected;
		list<unsigned int> elements;
		
		double maxdistance;
		
	};
	
	inline void MeshPoint::outsideChecked(){
		outsidechecked = true;
	}
	
	inline bool MeshPoint::wasOutsideChecked(){
		return outsidechecked;
	}
	
	inline void MeshPoint::setMaxDistance(double md){
		if (md<maxdistance) {
			return;
		}
		maxdistance = md;
	}
	
	inline double MeshPoint::getMaxDistance(){
		return maxdistance;
	}
	
	inline void MeshPoint::updateMaxDistanceByFactor(const double &per){
		maxdistance *= per;
	}
	
	inline void MeshPoint::setOutside(){
		inside = false;
	}
	
	inline void MeshPoint::setInside(){
		inside = true;
	}
	
	inline void MeshPoint::setIOState(bool state){
		inside = state;
	}
	
	inline bool MeshPoint::getIOState(){
		return inside;
	}
	
	//returns true if node is inside any input mesh
	inline bool MeshPoint::isInside(){
		return inside;
	}
	
	//returns true if node is outside every input mesh
	inline bool MeshPoint::isOutside(){
		return !inside;
	}
	
	inline void MeshPoint::setProjected(){
		projected = true;
        inside = false;
	}
	
	inline bool MeshPoint::wasProjected(){
		return projected;
	}
	
	inline Point3D &MeshPoint::getPoint(){
		return point;
	}
	
	inline void MeshPoint::setPoint(Point3D &p){
		point = p;
	}
	
	inline void MeshPoint::addElement(unsigned int e){
		elements.push_back(e);
	}
	
	inline void MeshPoint::clearElements(){
		elements.clear();
	}
	
	inline list<unsigned int> &MeshPoint::getElements(){
		return elements;
	}
	
	std::ostream& operator<<(std::ostream& o,MeshPoint &p);
	
}
#endif
