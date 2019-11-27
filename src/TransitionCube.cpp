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

/*
 * TransitionCube.cpp
 *
 *  Created on: May 30, 2012
 *      Author: jsepulve
 */

#include <math.h>
#include <algorithm>
#include "TransitionCube.h"


using namespace std;


namespace patterns {
	
	const Uint TransitionCube::ExternalToInternal[TOTAL_POINTS] = {
		0,  18, 20, 2, 6, 24, 26, 8,
		9,  19, 11, 1, 3, 21, 23, 5,
		15, 25, 17, 7,10, 12, 22,14,
		4,  16, 13
	};
	
	const Uint TransitionCube::InternalToExternal[TOTAL_POINTS] = {
		0, 11, 3,  12, 24, 15,  4, 19, 7,
		8, 20, 10, 21, 26, 23, 16, 25,18,
		1,  9, 2,  13, 22, 14,  5, 17, 6
	};
	
	
	TransitionCube::TransitionCube(): m_mask(0) { 
		//Default eight corner point.
		addCornerTransitionPointsMap(); 
	}
	
	TransitionCube::TransitionCube(const vector<Uint>& edges ): m_mask(0) {
		
		if (edges.size() > 0) {        
			
			for (int i=0; i<edges.size(); i++)
				// Convert input edge points to local internal mapping.
				m_edges.push_back(ExternalToInternal[edges[i]]);
			
			// Sort points ascending.
			sort (m_edges.begin(), m_edges.begin() + m_edges.size() );
			
			//Inserts input edge points into the same map of corner points.
			addEdgeTransitionPointsMap();
		}
		
		//Insert eight default corner points into the map.
		addCornerTransitionPointsMap();
	}
	
	void TransitionCube::resetTransitionCube() {
		m_MapVertices.clear();
		addCornerTransitionPointsMap();
		addEdgeTransitionPointsMap();
	}
	
	void TransitionCube::addNewTransitionPoint(int point) {
		TransitionPoint tmpPoint = TransitionPoint(point);
		
		if (m_MapVertices.find(tmpPoint.getInitTransitionPoint()) == m_MapVertices.end())
			m_MapVertices.insert(std::make_pair(tmpPoint.getInitTransitionPoint(), tmpPoint));
	}
	
	void TransitionCube::deleteTransitionPoint(int point)
	{
		m_MapVertices.erase (m_MapVertices.find(point));
	}
	
	void TransitionCube::rotate(int axis, int rot) {
		
		if ((axis > -1) && (axis < 3)) {
			
			//Mask of rotated edge points.
			Uint mask = 0;
			
			// Get edge points iterator already configured in the constructor.
			vector<Uint>::iterator Iter = m_edges.begin();
			
			// Clean up all previous rotation
			m_rotated_edges.clear();
			
			/// Gets a number of 90 degree steps.
			/// If negative rotation converts to similar positive rotation.
			int step = this->getRotationSteps(rot);
			
			/// This loop rotates toward axis direction.
			/// Rotates all points of the map (corner and edge points)
			for (PointMapIter it  = m_MapVertices.begin(); it != m_MapVertices.end(); 
				 ++it) {
				if      (axis == 0)
					(*it).second.rotX(step);
				
				else if (axis == 1)
					(*it).second.rotY(step);
				
				else if (axis == 2)
					(*it).second.rotZ(step);
				
				if (!m_edges.empty()) 
				{
					if ( (*it).first == (*Iter) && (Iter != m_edges.end()) ) {
						
						// Shift a left value edge point times to build a mask.
						mask |=1<<InternalToExternal[(*it).second.getRotatedTransitionPoint()];
						
						// Puts into local vector the point number the result 
						// of rotated edge points.
						m_rotated_edges.push_back((*it).second.getRotatedTransitionPoint());
						++Iter;
						
						//This display rotation of points.
						//cout << " " << (*it).first << "-->" << (*it).second.getRotatedPoint() << " ";
					}
					
				}
				
			}
			/// Save obtained mask in public variable.
			if (mask)
				m_mask = mask;
		}
	}
	
	void TransitionCube::rotX(int rotation) {
		this->rotate(0,rotation);
	}
	
	void TransitionCube::rotY(int rotation) {
		this->rotate(1,rotation);
	}
	
	void TransitionCube::rotZ(int rotation) {
		this->rotate(2,rotation);
	}
	
	void TransitionCube::getInitTransitionPoints(vector<Uint> & nodes) {
		nodes.clear();
		
		for (PointMapIter it  = m_MapVertices.begin(); 
			 it != m_MapVertices.end(); 
			 ++it) {
			nodes.push_back( InternalToExternal[(*it).first] );
		}
	}
	
	
	void TransitionCube::getCurrentTransitionPoints(vector<Uint> & nodes) {
		nodes.clear();
		
		for (PointMapIter it  = m_MapVertices.begin(); 
			 it != m_MapVertices.end(); 
			 ++it) {
			nodes.push_back( InternalToExternal[(*it).second.getRotatedTransitionPoint()] );
		}
	}
	
	void TransitionCube::getCurrentTransitionPoints(map<Uint, Uint> & nodes) {
		nodes.clear();
		
		for (PointMapIter it  = m_MapVertices.begin(); 
			 it != m_MapVertices.end(); 
			 ++it) {
			nodes.insert( make_pair(InternalToExternal[(*it).second.getRotatedTransitionPoint()],
									InternalToExternal[(*it).second.getInitTransitionPoint()]));
		}
	}
	
	
	
	void TransitionCube::getEdgeTransitionPoints(vector<Uint> & edges) {
		
		if (m_rotated_edges.empty()) {
			for (Uint i=0; i<m_edges.size(); i++)
				edges.push_back( InternalToExternal[ m_edges[i] ] );
		}
		else {
			for (Uint i=0; i<m_rotated_edges.size(); i++)
				edges.push_back( InternalToExternal[ m_rotated_edges[i] ] );
		}
		
	}
	
	void TransitionCube::addCornerTransitionPointsMap() {
		
		// Map of defaultvertices.
		m_MapVertices.insert(std::make_pair(0, TransitionPoint(0,0,0)));/// 0
		m_MapVertices.insert(std::make_pair(18,TransitionPoint(0,0,2)));/// 18
		m_MapVertices.insert(std::make_pair(20,TransitionPoint(2,0,2)));/// 20
		m_MapVertices.insert(std::make_pair(2, TransitionPoint(2,0,0)));/// 2
		m_MapVertices.insert(std::make_pair(6, TransitionPoint(0,2,0)));/// 6
		m_MapVertices.insert(std::make_pair(24,TransitionPoint(0,2,2)));/// 24
		m_MapVertices.insert(std::make_pair(26,TransitionPoint(2,2,2)));/// 26
		m_MapVertices.insert(std::make_pair(8, TransitionPoint(2,2,0)));/// 8
		
	}
	
	void TransitionCube::addEdgeTransitionPointsMap() {
		if ( m_edges.size() > 0 ) {
			
			for (vector<Uint>::iterator it  = m_edges.begin(); 
				 it != m_edges.end(); 
				 ++it) 
				m_MapVertices.insert(std::make_pair( TransitionPoint(*it).getInitTransitionPoint(), TransitionPoint(*it)));
		}
		
	}
	
	int TransitionCube::getRotationSteps(int rotation) {
		int step = 0;
		
		/// Check positive or negative 90 degree steps.
		if (rotation < 0) {
			step = abs(rotation) % 4;
			if      (step == 3) step = 1;
			else if (step == 1) step = 3;
		}
		else
			step = rotation % 4;
		
		return step;
	}
	
	
	string TransitionCube::reportRotation() {
		stringstream report ;
		
		report << "|SIZE: "<< m_edges.size() << " NODES:";
		
		for (vector<Uint>::iterator it = m_edges.begin(); it != m_edges.end(); ++it)
			report << " " << InternalToExternal[(*it)] ;
		
		for (PointMapIter it  = m_MapVertices.begin(); 
			 it != m_MapVertices.end(); 
			 ++it) {
			report << "| " << InternalToExternal[(*it).second.getInitTransitionPoint()] << 
            "-->" << InternalToExternal[(*it).second.getRotatedTransitionPoint()] ;
		}
		return report.str();
	}
	
	
	
}
