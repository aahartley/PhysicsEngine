/*
Modifications based on the source code from Gustavo Pezzi, co-founder of Pikuma.com
*/

#include "Contact.h"

void Contact::resolvePenetration() {
	float da = 0, db = 0;

	if (a->isStatic()) {
		da = 0;
		db = depth;
	}
	else if (b->isStatic()) {
		da = depth;
		db = 0;
	}
	else {
		da = depth * b->mass / (a->mass + b->mass);
		db = depth * a->mass / (a->mass + b->mass);
	}

    a->position -= normal * da;
    b->position += normal * db;
}

void Contact::resolveCollision() {
	// Apply positional correction using the projection method
	resolvePenetration();

	// Define elasticity (coefficient of restitution e)
	float e = std::min(a->restitution, b->restitution);

	// Calculate the relative velocity between the two objects
	const Vec2 vRel = (a->velocity - b->velocity);

	// Calculate the relative velocity along the normal collision vector
	float approachingSpeed = vRel.dot(normal);

	// Now we proceed to calculate the collision impulse
	const Vec2 impulseDirection = normal;
	const float impulseMagnitude = -(1 + e) * approachingSpeed / (a->invMass + b->invMass);

	Vec2 J = impulseDirection * impulseMagnitude;

	// Apply the impulse vector to both objects in opposite direction
	a->applyImpulse(J);
	b->applyImpulse(-J);
}

//------------------------------------------------------------------
// Resolves the collision with rotation using the impulse method
//------------------------------------------------------------------
void Contact::resolveCollisionWithRotation() {
	// Apply positional correction using the projection method
	resolvePenetration();

	//--------- Define elasticity and friction --------------
	float e, f;
	e = std::min(a->restitution, b->restitution);
	f = std::min(a->friction, b->friction);
	//e = (a->restitution + b->restitution) / 2.0;
	//f = (a->friction + b->friction) / 2.0;
	//-------------------------------------------------------
	// Calculate the relative velocity between the two objects
	Vec2 ra = end - a->position;
	Vec2 rb = start - b->position;
	Vec2 vEnd = a->velocity + Vec2(-ra.y, ra.x) * a->angularVelocity;		// the velocity of the point end (a point of a)
	Vec2 vStart = b->velocity + Vec2(-rb.y, rb.x) * b->angularVelocity;	// the velocity of the point start (a point of b)
	const Vec2 vRel = vEnd - vStart;									// the relative velocity between start and end									

	// Calculate the relative velocity along the normal collision vector
	float approachingSpeed = vRel.dot(normal);

	//----------- Calculate the collision impulse along the normal -----------
	const Vec2 impulseDirectionN = normal;

	float impulseMagnitudeN;
	if (a->isStatic())
		impulseMagnitudeN = -(1 + e) * approachingSpeed / (b->invMass + rb.cross(normal) * rb.cross(normal) * b->invI);
	else if (b->isStatic())
		impulseMagnitudeN = -(1 + e) * approachingSpeed / (a->invMass + ra.cross(normal) * ra.cross(normal) * a->invI);
	else
		impulseMagnitudeN = -(1 + e) * approachingSpeed / ((a->invMass + b->invMass)
			+ ra.cross(normal) * ra.cross(normal) * a->invI + rb.cross(normal) * rb.cross(normal) * b->invI);

	Vec2 Jn = impulseDirectionN * impulseMagnitudeN;
	//--------------------------------------------------------------------------

	//-----------Calculate the collision impulse along the tangent -------------
	Vec2 tangent = normal.normal();
	float vRelDotTangent = vRel.dot(tangent);
	const Vec2 impulseDirectionT = tangent;

	float impulseMagnitudeT;
	if (a->isStatic())
		impulseMagnitudeT = f * -(1 + e) * vRelDotTangent / (b->invMass + rb.cross(tangent) * rb.cross(tangent) * b->invI);
	else if (b->isStatic())
		impulseMagnitudeT = f * -(1 + e) * vRelDotTangent / (a->invMass + ra.cross(tangent) * ra.cross(tangent) * a->invI);
	else
		impulseMagnitudeT = f * -(1 + e) * vRelDotTangent / ((a->invMass + b->invMass)
			+ ra.cross(tangent) * ra.cross(tangent) * a->invI + rb.cross(tangent) * rb.cross(tangent) * b->invI);

	Vec2 Jt = impulseDirectionT * impulseMagnitudeT;
	//---------------------------------------------------------------------

	Vec2 J = Jn + Jt;	// final impulse

	// Apply the impulse vector to both objects in opposite direction
	a->applyImpulse(J, ra);
	b->applyImpulse(-J, rb);
}
