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

#ifndef FEMesh_h
#define FEMesh_h 1

#include <vector>
#include "Point3D.h"

using std::vector;

namespace Clobscode
{
	class FEMesh{
		
	public:
		
		FEMesh(){};
		
		virtual void setPoints(vector<Point3D> &pts);
		
		virtual void setElements(vector<vector<unsigned int> > &els);
		
		virtual void setOutsideNodes(list<unsigned int> &outpts);
		
		virtual vector<Point3D> &getPoints();
		
		virtual vector<vector<unsigned int> > &getElements();
		
		virtual list<unsigned int> &getOutsideNodes();
        
        //For debugging:
        virtual void setColoredCells(const vector<unsigned int> &colored);
        
        virtual const vector<unsigned int> &getColoredCells();
        
        //For decoration
        virtual const vector<unsigned short> &getRefLevels() const;
        virtual void setRefLevels(const vector<unsigned short> &rl);
        
        //virtual const vector <double> &getMinAngles() const;
        //virtual void setMinAngles(const vector<double> &ma);
        
        virtual const vector<unsigned short> &getSurfState() const;
        virtual void setSurfState(const vector<unsigned short> &surf);
        
        virtual const vector<unsigned short> &getDebugging() const;
        virtual void setDebugging(const vector<unsigned short> &deb);
        
		
	protected:
		
		vector<Point3D> points;
		vector<vector<unsigned int> > elements;
		list<unsigned int> outpts;
        
        vector <unsigned short> ref_levels, surf_state, deb_state;
        //vector <double> min_angles;
        vector<unsigned int> color;
		
	};
    
    
    inline const vector<unsigned short> &FEMesh::getRefLevels() const {return ref_levels;}
    inline void FEMesh::setRefLevels(const vector<unsigned short> &rl) {ref_levels=rl;}
    
    //inline const vector<double> &FEMesh::getMinAngles() const {return min_angles;}
    //inline void FEMesh::setMinAngles(const vector<double> &ma) {min_angles=ma;}
    
    inline const vector<unsigned short> &FEMesh::getSurfState() const {return surf_state;}
    inline void FEMesh::setSurfState(const vector<unsigned short> &surf) {surf_state=surf;}
    
    inline const vector<unsigned short> &FEMesh::getDebugging() const {return deb_state;}
    inline void FEMesh::setDebugging(const vector<unsigned short> &deb) {deb_state=deb;}
    
    inline void FEMesh::setColoredCells(const vector<unsigned int> &colored) {color = colored;}
    inline const vector<unsigned int> &FEMesh::getColoredCells() {return color;}
    
	
	inline void FEMesh::setOutsideNodes(list<unsigned int> &outpts){
		list<unsigned int>::iterator iter;
		for (iter=outpts.begin(); iter!=outpts.end(); iter++) {
			this->outpts.push_back(*iter);
		}
	}
	
	inline void FEMesh::setElements(vector<vector<unsigned int> > &els){
		unsigned int n = els.size();
		elements.reserve(n);
		for (unsigned int i=0; i<n; i++) {
			elements.push_back(els[i]);
		}
	}
	
	inline void FEMesh::setPoints(vector<Point3D> &pts){
		unsigned int n = pts.size();
		points.reserve(n);
		for (unsigned int i=0; i<n; i++) {
			points.push_back(pts[i]);
		}
	}
	
	inline vector<Point3D> &FEMesh::getPoints(){
		return points;
	}
	
	inline vector<vector<unsigned int> > &FEMesh::getElements(){
		return elements;
	}
	
	inline list<unsigned int> &FEMesh::getOutsideNodes(){
		return outpts;
	}
}
#endif
