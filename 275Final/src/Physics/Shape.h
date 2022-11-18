/*
Modification based on the the source code from Gustavo Pezzi, co-founder of Pikuma.com
*/

#ifndef SHAPE_H
#define SHAPE_H

#include "Vec2.h"
#include <vector>

enum ShapeType {
    DISK,
	BOX
	// other shapes will be added later
};

struct Shape {
    virtual ~Shape() = default;
    virtual ShapeType getType() const = 0;
    virtual Shape* clone() const = 0;
    virtual float getMomentOfInertia() const = 0;
};

struct Disk: public Shape {
    float radius;

    Disk(const float radius);
    virtual ~Disk();
    ShapeType getType() const override;
    Shape* clone() const override;
    float getMomentOfInertia() const override;

	float getArea() const;
};
struct Box : public Shape {
	float width;
	float height;
	std::vector<Vec2> localVertices;	// design the box by vertices	
	std::vector<Vec2> worldVertices;	// the box in the scene

	Box(float width, float height);
	virtual ~Box();
	ShapeType getType() const override;
	Shape* clone() const override;
	Vec2 edgeAt(int index) const;
	float findMinSeparation(const Box* other, Vec2& axis, Vec2& point) const;
	float getMomentOfInertia() const override;
	void updateVertices(float angle, const Vec2& position);	// the new position and orientation of the box
};
// structures of other shapes will be added later
#endif
