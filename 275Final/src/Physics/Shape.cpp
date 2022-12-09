/*
Modification based on the the source code from Gustavo Pezzi, co-founder of Pikuma.com
*/

#include "Shape.h"
#include <iostream>
#include <limits>	// for numeric limits of different types of values

Disk::Disk(float radius) {
    this->radius = radius;
    std::cout << "Disk constructor called!" << std::endl;
}

Disk::~Disk() {
   // std::cout << "Disk destructor called!" << std::endl;
}

Shape* Disk::clone() const {
    return new Disk(radius);
}

ShapeType Disk::getType() const {
    return DISK;
}

float Disk::getMomentOfInertia() const {
    // For solid circles, the moment of inertia is 1/2 * r^2
    // But this still needs to be multiplied by the rigidbody's mass
    return 0.5 * (radius * radius);
}

float Disk::getArea() const {
	return 3.14159265 * radius * radius;
}
//======================= implementation of Box =======================
Box::Box(float width, float height) {
	this->width = width;
	this->height = height;

	// Load the vertices of the box
	localVertices.push_back(Vec2(-width / 2.0, -height / 2.0));
	localVertices.push_back(Vec2(+width / 2.0, -height / 2.0));
	localVertices.push_back(Vec2(+width / 2.0, +height / 2.0));
	localVertices.push_back(Vec2(-width / 2.0, +height / 2.0));

	// initialize the word vertices of the box
	worldVertices.push_back(Vec2(-width / 2.0, -height / 2.0));
	worldVertices.push_back(Vec2(+width / 2.0, -height / 2.0));
	worldVertices.push_back(Vec2(+width / 2.0, +height / 2.0));
	worldVertices.push_back(Vec2(-width / 2.0, +height / 2.0));
}

Box::~Box() {
	//std::cout << "Box destructor called!" << std::endl;
}

ShapeType Box::getType() const {
	return BOX;
}

Shape* Box::clone() const {
	return new Box(width, height);
}

float Box::getMomentOfInertia() const {
	// For a solid 2D box, the moment of inertia is 1/12 * (w^2 + h^2)
	// But this still needs to be multiplied by the rigidbody's mass
	return (0.083333) * (width * width + height * height);
}

Vec2 Box::edgeAt(int index) const {
	int currVertex = index;
	int nextVertex = (index + 1) % worldVertices.size();
	return worldVertices[nextVertex] - worldVertices[currVertex];
}

//------------ the method of Separating Axis Theorem (SAT) ------------
float Box::findMinSeparation(const Box* other, Vec2& axis, Vec2& point) const {
	float separation = std::numeric_limits<float>::lowest();	// ~ -inf
	// Loop all the vertices of "this" box
	for (int i = 0; i < this->worldVertices.size(); i++) {
		Vec2 va = this->worldVertices[i];
		Vec2 normal = this->edgeAt(i).normal();
		// Loop all the vertices of the "other" polygon
		float minSep = std::numeric_limits<float>::max();	// ~ +inf
		Vec2 minVertex;		// the vertex of the other polygon with the smallest projection (in numerical value) to this normal
		for (int j = 0; j < other->worldVertices.size(); j++) {
			Vec2 vb = other->worldVertices[j];
			float proj = (vb - va).dot(normal);
			if (proj < minSep) {
				minSep = proj;		// the smallest projection (in numerical value)
				minVertex = vb;
			}
		}
		if (minSep > separation) {
			separation = minSep;	// the largest of the smallest projections
			axis = this->edgeAt(i);	// the edge of this polygon that separates the polygons
			point = minVertex;		// the vertex of the other polygon where the separation is obtained
		}
	}
	return separation;
}

void Box::updateVertices(float angle, const Vec2& position) {
	// Loop all the vertices, transforming from local to world space
	for (int i = 0; i < localVertices.size(); i++) {
		// First rotate, then we translate
		worldVertices[i] = localVertices[i].rotate(angle);	// find the new orientation
		worldVertices[i] += position;	// shift to the new position
	}
}