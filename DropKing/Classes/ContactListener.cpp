/*
 * ContactListener.cpp
 *
 *  Created on: 2014. 2. 8.
 *       Author: bsh
 */

#include "C:\cocos2d-x-2.2.3\cocos2d-x-2.2.3\projects\DropKing\Classes/ContactListener.h"

ContactListener::ContactListener() {
	// TODO Auto-generated constructor stub

}

ContactListener::~ContactListener() {
	// TODO Auto-generated destructor stub
}
void ContactListener::BeginContact(b2Contact* contact){
	_contactStage = CONTACT_BEGIN;
	CCLog("begin");
}
void ContactListener::EndContact(b2Contact* contact){
	_contactStage = CONTACT_END;
	CCLog("end");
}
void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){

}
void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){
}
