/*
 * ContactListener.h
 *
 *  Created on: 2014. 2. 8.
 *       Author: bsh
 */

#ifndef CONTACTLISTENER_H_
#define CONTACTLISTENER_H_
#include "Box2D/Box2D.h"
#include "cocos2d.h"
using namespace cocos2d;

enum contactStage{
	CONTACT_BEGIN, CONTACT_END, CONTACT_NULL,
};
class ContactListener : public b2ContactListener{
public:
	ContactListener();
	virtual ~ContactListener();
	int _contactStage;


	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

};

#endif /* CONTACTLISTENER_H_ */
