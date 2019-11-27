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

#ifndef Octant_h
#define Octant_h 1

#include <iostream>
#include <vector>
#include <list>
#include <set>

#include "Visitors/Visitor.h"

#include "MeshPoint.h"
#include "Point3D.h"
#include "OctreeEdge.h"
#include "TriMesh.h"

using Clobscode::MeshPoint;
using Clobscode::OctreeEdge;
using Clobscode::Point3D;
using Clobscode::TriMesh;
using SurfMesh::SurfTriangle;
using std::vector;
using std::list;
using std::set;
using std::pair;

namespace Clobscode
{
	
	class Octant{
        friend class IntersectionsVisitor;
        friend class OneIrregularVisitor;
        friend class PointMovedVisitor;
        friend class SplitVisitor;
        friend class EdgeVisitor;
        friend class TransitionPatternVisitor;
        friend class SurfaceTemplatesVisitor;
        friend class RemoveSubElementsVisitor;

	public:
		
		Octant(vector<unsigned int> &epts, 
			   const unsigned short &ref_level);
		
		virtual ~Octant();

        bool accept(Visitor *v);
		
		virtual void addProjectionInfluence(const Point3D &dir);
		
		virtual const Point3D &getProjectionInfluence();
		
		virtual void noMoreProjectionInfluences();
				
		//access methods
		virtual vector<unsigned int> &getPoints();
		
		virtual bool isInside();
		
		virtual bool intersectsSurface();
		
		virtual unsigned short &getRefinementLevel();
		
		virtual bool wasShrink();
		
		virtual bool wasConsideredInProjection();
		
		virtual void resetProjectionInfluence();

		virtual list<unsigned int> &getIntersectedFaces();
		
		virtual vector<vector<unsigned int> > &getSubElements();
		
		virtual void computeMaxDistance(vector<MeshPoint> &mp);
		
        virtual double getMaxDistance();
		
		//flag for inside octants that due to "inside node" moved
		//to the input domain, it must be treated as a surface
		//element by the surfacePatterns
		virtual void setSurface();
		
		virtual bool isSurface();
		
		virtual void setIntersectedFaces(list<unsigned int> &ifcs);

    protected:
        
		//protected:
		vector<unsigned int> pointindex;
		vector<vector<unsigned int> > sub_elements, possibles, continuity;
		list<unsigned int> intersected_faces;
		//the level at which this octant is found in the
		//the tree structure (octree). Used for optimization
		unsigned short ref_level;
		
		Point3D projection_influence;
		unsigned short n_influences;
		bool influence_commit;
		bool surface;
		
		double max_dis;
	};
	
	
	inline vector<unsigned int> &Octant::getPoints(){
		return pointindex;
	}
	
	inline bool Octant::isInside(){
		return intersected_faces.empty();
	}
	
	inline bool Octant::intersectsSurface(){
		return !intersected_faces.empty();
	}
	
	inline list<unsigned int> &Octant::getIntersectedFaces(){
		return intersected_faces;
	}
	
	inline unsigned short &Octant::getRefinementLevel() {
		return ref_level;
	}
	
	inline vector<vector<unsigned int> > &Octant::getSubElements() {
		return sub_elements;
	}
	
	inline void Octant::addProjectionInfluence(const Point3D &dir) {
		projection_influence += dir;
		n_influences++;
	}
	
	inline bool Octant::wasShrink() {
		return n_influences!=0;
	}
	
	inline const Point3D &Octant::getProjectionInfluence(){
		return projection_influence;
	}
	
	inline void Octant::noMoreProjectionInfluences(){
		if (n_influences==0) {
			return;
		}
		influence_commit = true;
		projection_influence = projection_influence / n_influences;
	}
	
	inline void Octant::resetProjectionInfluence() {
		projection_influence = projection_influence * 0;
		n_influences = 0;
	}
	
	inline bool Octant::wasConsideredInProjection(){
		return influence_commit;
	}
	
	inline void Octant::computeMaxDistance(vector<MeshPoint> &mp){
		Point3D p0 = mp[pointindex[0]].getPoint();
		Point3D p1 = mp[pointindex[6]].getPoint();
		max_dis = 0.3 * (p0 - p1).Norm();
	}
	
	inline double Octant::getMaxDistance(){
		return max_dis;
	}
	
	inline void Octant::setSurface(){
		surface = true;
	}
	
	inline bool Octant::isSurface(){
		return surface || !intersected_faces.empty();
	}
	
	inline void Octant::setIntersectedFaces(list<unsigned int> &ifcs){
		intersected_faces = ifcs;
	}
	
	std::ostream& operator<<(ostream& o,Octant &e);
}
#endif
