/*
Modification based on the source code from Gustavo Pezzi, co-founder of Pikuma.com
*/

#include "CollisionDetection.h"
#include <iostream>
bool CollisionDetection::isColliding(Body* a, Body* b, Contact& contact) {
	bool aIsDisk = a->shape->getType() == DISK;
	bool bIsDisk = b->shape->getType() == DISK;
	bool aIsBox = a->shape->getType() == BOX;
	bool bIsBox = b->shape->getType() == BOX;

	if (aIsDisk && bIsDisk) {
		return isCollidingDiskDisk(a, b, contact);
	}

	if (aIsBox && bIsBox) {
		return isCollidingBoxBox(a, b, contact);
	}

	if (aIsBox && bIsDisk) {
		return isCollidingBoxDisk(a, b, contact);
	}

	if (aIsDisk && bIsBox) {
		return isCollidingBoxDisk(b, a, contact);
	}

	return false;
}

bool CollisionDetection::isCollidingDiskDisk(Body* a, Body* b, Contact& contact) {
    Disk* diskA = (Disk*) a->shape;
    Disk* diskB = (Disk*) b->shape;

    const Vec2 ab = b->position - a->position;
    const float radiusSum = diskA->radius + diskB->radius;

    bool isColliding = ab.magnitudeSquared() <= (radiusSum * radiusSum);

    if (!isColliding) {
        return false;
    }

    contact.a = a;
    contact.b = b;

    contact.normal = ab;
    contact.normal.normalize();

    contact.start = b->position - contact.normal * diskB->radius;
    contact.end = a->position + contact.normal * diskA->radius;

    contact.depth = (contact.end - contact.start).magnitude();
    
    return true;
}

// TODO: Implement other methods to check collision between different shapes
bool CollisionDetection::isCollidingBoxBox(Body* a, Body* b, Contact& contact) {
	const Box* boxA = (Box*)a->shape;
	const Box* boxB = (Box*)b->shape;
	Vec2 aAxis, bAxis;
	Vec2 aPoint, bPoint;
	float abSeparation = boxA->findMinSeparation(boxB, aAxis, aPoint);
	if (abSeparation >= 0) {
		return false;
	}
	float baSeparation = boxB->findMinSeparation(boxA, bAxis, bPoint);
	if (baSeparation >= 0) {
		return false;
	}

	//------normal is from start (inside a) to end (inside b); 
	contact.a = a;
	contact.b = b;
	if (abSeparation > baSeparation) {
		contact.depth = -abSeparation;
		contact.normal = aAxis.normal();
		contact.start = aPoint;					// the penetrating vertex of b
		contact.end = aPoint + contact.normal * contact.depth;	// a point on the penetrated edge of a
	}
	else {
		contact.depth = -baSeparation;
		contact.normal = -bAxis.normal();
		contact.start = bPoint - contact.normal * contact.depth;	// a point on the penetrated edge of b
		contact.end = bPoint;					// the penetrating vertex of a
	}
	return true;
}

bool CollisionDetection::isCollidingBoxDisk(Body* boxBody, Body* diskBody, Contact& contact) {
	const Box* box = (Box*)boxBody->shape;
	const Disk* disk = (Disk*)diskBody->shape;
	const std::vector<Vec2>& boxVertices = box->worldVertices;

	bool isOutside = false;
	Vec2 minCurrVertex;
	Vec2 minNextVertex;
	float distanceDiskCenterToEdge = std::numeric_limits<float>::lowest();

	// Loop all the edges of the polygon/box finding the nearest edge to the circle center
	for (int i = 0; i < boxVertices.size(); i++) {
		int currVertex = i;
		int nextVertex = (i + 1) % boxVertices.size();
		Vec2 edge = box->edgeAt(currVertex);
		Vec2 normal = edge.normal();

		// Compare the circle center with the rectangle vertex
		Vec2 vertexToDiskCenter = diskBody->position - boxVertices[currVertex];
		float projection = vertexToDiskCenter.dot(normal);

		// If we found a dot product projection that is in the positive/outside side of the normal
		if (projection > 0) {
			// Circle center is outside the polygon
			distanceDiskCenterToEdge = projection;
			minCurrVertex = boxVertices[currVertex];
			minNextVertex = boxVertices[nextVertex];
			isOutside = true;
			break;
		}
		else {
			// Circle center is inside the rectangle, find the min edge (the one with the least negative projection)
			if (projection > distanceDiskCenterToEdge) {
				distanceDiskCenterToEdge = projection;
				minCurrVertex = boxVertices[currVertex];
				minNextVertex = boxVertices[nextVertex];
			}
		}
	}

	if (isOutside) {
		///////////////////////////////////////
		// Check if we are inside region A:
		///////////////////////////////////////
		Vec2 v1 = diskBody->position - minCurrVertex; // vector from the nearest vertex to the circle center
		Vec2 v2 = minNextVertex - minCurrVertex; // the nearest edge (from curr vertex to next vertex)
		if (v1.dot(v2) < 0) {
			// Distance from vertex to circle center is greater than radius... no collision
			if (v1.magnitude() > disk->radius) {
				return false;
			}
			else {
				// Detected collision in region A:
				contact.a = boxBody;
				contact.b = diskBody;
				contact.depth = disk->radius - v1.magnitude();
				contact.normal = v1.normalize();
				contact.start = diskBody->position + (contact.normal * -disk->radius);
				contact.end = contact.start + (contact.normal * contact.depth);
			}
		}
		else {
			///////////////////////////////////////
			// Check if we are inside region B:
			///////////////////////////////////////
			v1 = diskBody->position - minNextVertex; // vector from the next nearest vertex to the circle center
			v2 = minCurrVertex - minNextVertex;   // the nearest edge
			if (v1.dot(v2) < 0) {
				// Distance from vertex to circle center is greater than radius... no collision
				if (v1.magnitude() > disk->radius) {
					return false;
				}
				else {
					// Detected collision in region B:
					contact.a = boxBody;
					contact.b = diskBody;
					contact.depth = disk->radius - v1.magnitude();
					contact.normal = v1.normalize();
					contact.start = diskBody->position + (contact.normal * -disk->radius);
					contact.end = contact.start + (contact.normal * contact.depth);
				}
			}
			else {
				///////////////////////////////////////
				// We are inside region C:
				///////////////////////////////////////
				if (distanceDiskCenterToEdge > disk->radius) {
					// No collision... Distance between the closest distance and the circle center is greater than the radius.
					return false;
				}
				else {
					// Detected collision in region C:
					contact.a = boxBody;
					contact.b = diskBody;
					contact.depth = disk->radius - distanceDiskCenterToEdge;
					contact.normal = (minNextVertex - minCurrVertex).normal();
					contact.start = diskBody->position - (contact.normal * disk->radius);
					contact.end = contact.start + (contact.normal * contact.depth);
				}
			}
		}
	}
	else {
		// The center of circle is inside the polygon... it is definitely colliding!
		contact.a = boxBody;
		contact.b = diskBody;
		contact.depth = disk->radius - distanceDiskCenterToEdge;
		contact.normal = (minNextVertex - minCurrVertex).normal();
		contact.start = diskBody->position - (contact.normal * disk->radius);
		contact.end = contact.start + (contact.normal * contact.depth);
	}

	return true;
}