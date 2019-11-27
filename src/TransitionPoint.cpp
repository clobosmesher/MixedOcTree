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
 * TransitionPoint.cpp
 *
 *  Created on: May 30, 2012
 *      Author: jsepulve
 */

#include "TransitionPoint.h"


namespace patterns {
	
	const int TransitionPoint::TOTAL_TransitionPointS = 27;
	const int TransitionPoint::CoordinatesToTransitionPointConversionMatrix [3] = {1,3,9};
	const int TransitionPoint::TransitionPointToCoordinatesConversionMatrix [TOTAL_TransitionPointS][3] = {
		{0,0,0}, //0
		{1,0,0}, //1
		{2,0,0}, //2
		{0,1,0}, //3
		{1,1,0}, //4
		{2,1,0}, //5
		{0,2,0}, //6
		{1,2,0}, //7
		{2,2,0}, //8
		{0,0,1}, //9
		{1,0,1}, //10
		{2,0,1}, //11
		{0,1,1}, //12
		{1,1,1}, //13
		{2,1,1}, //14
		{0,2,1}, //15
		{1,2,1}, //16
		{2,2,1}, //17
		{0,0,2}, //18
		{1,0,2}, //19
		{2,0,2}, //20
		{0,1,2}, //21
		{1,1,2}, //22
		{2,1,2}, //23
		{0,2,2}, //24
		{1,2,2}, //25
		{2,2,2}  //26
	};
	
	const int TransitionPoint::RotationMatrix_X [4][4][4] = {
		{ /// 0 degree rotation
			{ 1, 0, 0, 0},
			{ 0, 1, 0, 0},
			{ 0, 0, 1, 0},
			{ 0, 0, 0, 0}
		},
		{ /// 90 degree rotation
			{1, 0, 0, 0},
			{0, 0,-1, 2},
			{0, 1, 0, 0},
			{0, 0, 0, 1}
		},
		{ /// 180 degree rotation
			{1, 0, 0, 0},
			{0,-1, 0, 2},
			{0, 0,-1, 2},
			{0, 0, 0, 1}
		},
		{ /// 270 degree rotation
			{1, 0, 0, 0},
			{0, 0, 1, 0},
			{0,-1, 0, 2},
			{0, 0, 0, 1}
		}
	};
	
	const int TransitionPoint::RotationMatrix_Y [4][4][4] =
	{
		{ /// 0 degree rotation
			{ 1, 0, 0, 0},
			{ 0, 1, 0, 0},
			{ 0, 0, 1, 0},
			{ 0, 0, 0, 0}
		},
		{ /// 90 degree rotation
			{ 0, 0, 1, 0},
			{ 0, 1, 0, 0},
			{-1, 0, 0, 2},
			{ 0, 0, 0, 1}
		},
		{ /// 180 degree rotation
			{-1, 0, 0, 2},
			{ 0, 1, 0, 0},
			{ 0, 0,-1, 2},
			{ 0, 0, 0, 1}
		},
		{ /// 270 degree rotation
			{ 0, 0,-1, 2},
			{ 0, 1, 0, 0},
			{ 1, 0, 0, 0},
			{ 0, 0, 0, 1}
		}
	};
	
	const int TransitionPoint::RotationMatrix_Z [4][4][4] =
	{
		{ /// 0 degree rotation
			{ 1, 0, 0, 0},
			{ 0, 1, 0, 0},
			{ 0, 0, 1, 0},
			{ 0, 0, 0, 0}
		},
		{ /// 90 degree rotation
			{ 0,-1, 0, 2},
			{ 1, 0, 0, 0},
			{ 0, 0, 1, 0},
			{ 0, 0, 0, 1}
		},
		{ /// 180 degree rotation
			{-1, 0, 0, 2},
			{ 0,-1, 0, 2},
			{ 0, 0, 1, 0},
			{ 0, 0, 0, 1}
		},
		{ /// 270 degree rotation
			{ 0, 1, 0, 0},
			{-1, 0, 0, 2},
			{ 0, 0, 1, 0},
			{ 0, 0, 0, 1}
		}
	};
	
	TransitionPoint::TransitionPoint(int id, int x, int y, int z)
    : m_id(id), m_X(x), m_Y(y), m_Z(z), m_T(1) { 
		m_TransitionPoint = CoordinatesToTransitionPoint(); 
	}
	
	TransitionPoint::TransitionPoint(int x, int y, int z): m_X(x), m_Y(y), m_Z(z), m_T(1) { 
		m_TransitionPoint = CoordinatesToTransitionPoint(); 
		m_id    = m_TransitionPoint;
	}
	
	
	
	int TransitionPoint::CoordinatesToTransitionPoint() {
		return (m_X*CoordinatesToTransitionPointConversionMatrix[0] +
				m_Y*CoordinatesToTransitionPointConversionMatrix[1] +
				m_Z*CoordinatesToTransitionPointConversionMatrix[2]);
	}
	
	void TransitionPoint::TransitionPointToCoordinates(int TransitionPoint) {
		if ( (TransitionPoint >= 0) && (TransitionPoint <TOTAL_TransitionPointS) ) {
			m_X = TransitionPointToCoordinatesConversionMatrix[TransitionPoint][0];
			m_Y = TransitionPointToCoordinatesConversionMatrix[TransitionPoint][1];
			m_Z = TransitionPointToCoordinatesConversionMatrix[TransitionPoint][2];
			m_TransitionPoint = TransitionPoint;
			m_id = m_TransitionPoint;
		}
		else {
			m_X=0;
			m_Y=0;
			m_Z=0;
			m_TransitionPoint = 0;
			m_id=0;
		}
	}
	
	int TransitionPoint::rotX(int input_step) {
		int step = this->getSteps(input_step);
		
		const int (*rot)[4]=RotationMatrix_X[step];
		
		int temp[4] = {0, 0, 0, 0};
		
		for (int i=0; i<4; i++)
		{
			temp[i] = rot[i][0]*m_X + rot[i][1]*m_Y + rot[i][2]*m_Z + rot[i][3]*m_T;
		}
		m_X = temp[0];
		m_Y = temp[1];
		m_Z = temp[2];
		m_TransitionPoint = this->CoordinatesToTransitionPoint();
		
		return (m_TransitionPoint);
	}
	
	int TransitionPoint::rotate(int axis, int number_steps) {
		/// Do nothing just return current position
		if ( (axis > -1) && (axis < 4) )
		{
			int step = this->getSteps(number_steps);
			const int (*rot)[4];
			if (axis == 0)
				rot=RotationMatrix_X[step];
			else if (axis == 1)
				rot=RotationMatrix_Y[step];
			else if (axis == 2)
				rot=RotationMatrix_Z[step];
			
			int temp[4] = {0, 0, 0, 0};
			
			for (int i=0; i<4; i++)
			{
				temp[i] = rot[i][0]*m_X + rot[i][1]*m_Y + rot[i][2]*m_Z + rot[i][3]*m_T;
			}
			
			m_X = temp[0];
			m_Y = temp[1];
			m_Z = temp[2];
			m_TransitionPoint = this->CoordinatesToTransitionPoint();
		}
		
		return (m_TransitionPoint);
	}
	
	
	void TransitionPoint::setPosition(int _x, int _y, int _z) {
		m_X = _x;
		m_Y = _y;
		m_Z = _z;
		m_TransitionPoint = this->CoordinatesToTransitionPoint();
	}
	
	void TransitionPoint::setPosition(int TransitionPoint) {
		this->TransitionPointToCoordinates(TransitionPoint);
		m_TransitionPoint = this->CoordinatesToTransitionPoint();
	}
	
	
	int TransitionPoint::getSteps(int rotation) {
		if (rotation < 0)
		{
			int step = abs(rotation) % 4;
			if (step == 1)
				step = 3; // -90 degrees --> +270 degrees
			else if (step == 3)
				step = 1; // -270 degrees --> +90 degrees
			
			return step;
		}
		return (rotation % 4);
	}
	
	int TransitionPoint::coordinatesToID(int x, int y, int z) {
		for (int i=0; i< TOTAL_TransitionPointS; i++)
		{
			if ( (TransitionPointToCoordinatesConversionMatrix[i][0] == x) && 
                (TransitionPointToCoordinatesConversionMatrix[i][1] == y) && 
                (TransitionPointToCoordinatesConversionMatrix[i][2] == z) )
			{
				return i;
			}
		}
		return 0;
	}
	
	
}
