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

#ifndef IntersectionsVisitor_h
#define IntersectionsVisitor_h 1

#include "../MeshPoint.h"
#include "../OctreeEdge.h"
#include "../Point3D.h"
#include "../SurfTriangle.h"
#include "../TriMesh.h"

#include <list>
#include <set>
#include <vector>

#include "Visitor.h"

/* Since checkIntersections was an overloaded function,
 * visitor behavior depends on the select_faces parameter.
 * The parameter with which the visitor is created should
 * match the references given to the visitor, else, it will
 * throw an exception.
 *
 * check intersections with selected input faces: needs mesh, faces, coords.
 * checkIntersections(TriMesh &mesh,list<unsigned int> &faces,vector<Point3D> &coords);
 *
 * check intersections with all input faces: needs mesh, points.
 * checkIntersections(TriMesh &mesh, vector<MeshPoint> &pts)
 */


namespace Clobscode
{
    class IntersectionsVisitor : public Visitor{

    public:
        IntersectionsVisitor();
        IntersectionsVisitor(bool select_faces);

        bool visit(Octant *o);

        void setTriMesh(TriMesh &mesh);
        void setPoints(vector<MeshPoint> &points);
        void setFaces(list<unsigned int> &faces);
        void setCoords(vector<Point3D> &coords);

    protected:
        //variables
        TriMesh *mesh;
        vector<MeshPoint> *points;
        list<unsigned int> *faces;
        vector<Point3D> *coords;
        bool select_faces;

        //auxiliary functions
        bool intersectsTriangle(SurfTriangle &st, vector<Point3D> &input_pts, const Point3D &pmin, const Point3D &pmax);

        bool clipGeneralCase(const Point3D &p1, const Point3D &p2, const Point3D &pmin, const Point3D &pmax);

        bool edgeTriangleIntersection(SurfTriangle &st, vector<Point3D> &input_pts,
                                      const Point3D &pmin, const Point3D &pmax);

        unsigned int computePosition(const Point3D &p, const Point3D &pmin, const Point3D &pmax);

        vector<vector<Point3D>> getEdges(const Point3D &pmin, const Point3D &pmax);


    };
}


#endif //MESHER_ROI_INTERSECTIONSVISITOR_H
