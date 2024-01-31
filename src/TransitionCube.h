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
 * TransitionCube.h
 *
 *  Created on: May 30, 2012
 *      Author: jsepulve
 */

#ifndef TransitionCube_H_
#define TransitionCube_H_

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include "TransitionPoint.h"

using namespace std;

namespace patterns {
	
	/**
	 * Creates a basic TransitionCube
	 *         External                           Internal
	 *       4--------19-------7              6--------7--------8
	 *      /|                /|             /|                /|
	 *     / |               / |            / |               / |
	 *   16  |     24       18 |          15  |     16       17 |
	 *   /   12       4    /   15         /   3        4    /   5
	 *  5----+--17--------6   3|        24----+--25--------26   |
	 *  | 21 |        13  |  2 |  <-->   | 12 |        13  |  14|
	 *  |    0--------11--|----3         |    0--------1---|- --2
	 *  |   /             |   /          |   /             |   /
	 *  |  8        20    |  10          |  9        10    |  11
	 * 13 /       22      14/           21 /       22      23/
	 *  |/                |/             |/                |/
	 *  1--------9--------2             18--------19-------20
	 *
	 */
	class TransitionCube {
	public:
		//typedef unsigned int Uint;
		/**
		 * Default constructor.
		 */
		TransitionCube();
		
		virtual ~TransitionCube() { };
		
		/**
		 * Creates a TransitionCube assigning edge and mid points.
		 */
		TransitionCube(const vector<Uint>& edges);
		
		/**
		 * Copy constructor
		 */
		TransitionCube(const TransitionCube & c): m_MapVertices(c.m_MapVertices), m_edges(c.m_edges) { };
		
		/**
		 * Assignment operator
		 */
		TransitionCube &operator =(const TransitionCube &c)  
		{
			if (this != &c) { m_MapVertices = c.m_MapVertices; m_edges = c.m_edges; }
			return *this;
		};
		
		/**
		 * Equals operator
		 */ 
		bool operator ==(const TransitionCube &c) const {
			return ( (m_MapVertices == c.m_MapVertices) || (m_edges==c.m_edges) );
		};
		
		bool operator !=(const TransitionCube &c) const {
			return ( (m_MapVertices != c.m_MapVertices) || (m_edges!=c.m_edges) );
		};
		
		
		/**
		 * TransitionCube start over
		 * Sets all hexahedron points to init condition, all rotations will lost.
		 */
		void resetTransitionCube(); 
		
		/**
		 * Adds on a new edge or face vertices to hexahedron.
		 * Vertices already present in the TransitionCube are not appended.
		 */
		void addNewTransitionPoint(int);
		
		/**
		 * Removes a edge or face point currently in the hexahedron
		 */
		void deleteTransitionPoint(int);
		
		/**
		 * Rotates TransitionCube in 90 degree steps.
		 * Positive step is right hand direction rotation.
		 * Rotation step of +1 unit is a 90 degree clockwise.
		 */
		void rotX(int);
		
		/**
		 * Rotates Y TransitionCube in 90 degree steps.
		 * Positive step is right hand direction rotation.
		 * Rotation step of +1 unit is a 90 degree clockwise.
		 */
		void rotY(int);
		
		/**
		 * Rotates Z TransitionCube in 90 degree steps.
		 * Positive step is right hand direction rotation.
		 * Rotation step of +1 unit is a 90 degree clockwise.
		 */
		void rotZ(int);
		
		/**
		 * Return init points
		 */
		void getInitTransitionPoints(vector<Uint> & );
		
		/**
		 * Return vector of points
		 */
		void getCurrentTransitionPoints(vector<Uint> & );
		
		/**
		 * Return map of init and rotated value of each point.
		 */
		void getCurrentTransitionPoints(map<Uint, Uint>& );
		
		/**
		 * Return vector of points
		 */
		void getEdgeTransitionPoints(vector<Uint> & );
		
		/**
		 * Return mask of edge points
		 */
		Uint getEdgeTransitionPointsMask() { return m_mask; };
		
		/**
		 * Return a string with node rotations performed.
		 */
		string reportRotation();
		
	private:
		/**
		 * Creates a default eight corner points.
		 * Populates internal map with a vertice and object point.
		 * Each point is defined by a vector of three dimensions (x,y,z).
		 */
		void addCornerTransitionPointsMap();
		
		/**
		 * Inserts edge vectors into the internal map
		 */
		void addEdgeTransitionPointsMap();
		
		/**
		 * Calculates number of step repetions
		 */
		int getRotationSteps(int);
		
		void rotate (int, int);
		
		typedef vector<TransitionPoint> TransitionPointVector;
		typedef vector<TransitionPoint>::iterator TransitionPointVectorIter;
		
		/**
		 * Map of a vertice and object point (x,y,z)
		 */
		typedef map<Uint,TransitionPoint> PointMap_t;
		typedef PointMap_t::iterator PointMapIter;
		
		PointMap_t  m_MapVertices;
		
		vector<Uint> m_edges;
		vector<Uint> m_rotated_edges;
		
		// total number of TransitionCube points
		static const Uint TOTAL_POINTS = 27;
		
		/**
		 * Public points defined in the patterns.
		 */
		static const Uint ExternalToInternal[];
		
		/**
		 * Container of internal points defined in TransitionCube class
		 */
		static const Uint InternalToExternal[];
		
		Uint m_mask;
		
		
		
	};
	
}

#endif /* TransitionCube_H_ */
