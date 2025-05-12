//
// Created by omarb on 7/1/2024.
//

#include "Ship.h"

pirate::pirate(int pirateId, int shipId, int treasure, ship *belongToShip ) :
    m_pirate(pirateId) , belong_shipId(shipId) , treasure(treasure) ,
    belongToShip(belongToShip),m_time(0){
}

pirate::~pirate() {
}

int pirate::getMPirate() const {
    return m_pirate;
}

int pirate::getBelongShipId() const {
    return belong_shipId;
}

void pirate::setBelongShipId(int belongShipId) {
    belong_shipId = belongShipId;
}

int pirate::getTreasure() const {
    return treasure + belongToShip->getBonusTreasure() ;
}

void pirate::change_treasure(int num) {
    treasure += num ;
}
void pirate::update_treasure() {
    treasure = treasure + belongToShip->getBonusTreasure() ;
}

ship *pirate::getBelongToShip() const {
    return belongToShip;
}

void pirate::setBelongToShip(ship *belongToShip) {
    pirate::belongToShip = belongToShip;
}

int pirate::getMTime() const {
    return m_time;
}

void pirate::setMTime(int mTime) {
    m_time = mTime;
}

bool pirate::operator==(const pirate& other) const {
    return m_pirate == other.m_pirate ; /*&&
           belong_shipId == other.belong_shipId ;  &&
           treasure == other.treasure ;*/
}

bool pirate::operator!=(const pirate& other) const {
    return !(other == *this);
}

bool pirate::operator>(const pirate& other) const {
    if(treasure < other.treasure){
        return true ;
    }
    if(treasure == other.treasure){
        if(m_pirate < other.m_pirate){
            return true ;
        }
    }
    return false ;
}

bool pirate::operator<(const pirate& other) const {
    if(treasure > other.treasure){
        return true ;
    }
    if(treasure == other.treasure){
        if(m_pirate > other.m_pirate){
            return true ;
        }
    }
    return false ;
}

bool operator>(const pirate* pirate1, const pirate& pirate2) {
    if (pirate1->treasure < pirate2.treasure) {
        return true;
    }
    if (pirate1->treasure == pirate2.treasure) {
        if (pirate1->m_pirate < pirate2.m_pirate) {
            return true;
        }
    }
    return false;
}


//-------------------------------------------
ship::ship(int shipId, int cannons ,int richestPirate) :
    m_shipId(shipId) , m_cannons(cannons) , m_shipPirates() , m_ship_orderedPirates() ,
    richest_pirate(richestPirate),m_bonusTreasure(0) , num_order(0){
}

ship::~ship() {
}

void ship::setPirate(pirate *addpirate) {
    if(m_shipPirates.find(addpirate->getMPirate()) != nullptr){
        return;
    }
    addpirate->setBelongShipId(m_shipId);
    addpirate->setBelongToShip(this) ;
    addpirate->change_treasure(-m_bonusTreasure) ;
    addpirate->setMTime(num_order);
    m_shipPirates.insert(addpirate->getMPirate(),*addpirate) ;
    m_ship_orderedPirates.insert(num_order, *addpirate) ;
    m_ship_richPirates.insert(*addpirate,addpirate->getMPirate());
    richest_pirate = m_ship_richPirates.findMaxRightmostLeaf(m_ship_richPirates.tree_root);
    num_order++ ;
}

int ship::getMSize() const {
    return m_shipPirates.AVLTree_size ;
}

int ship::getMCannons() const {
    return m_cannons ;
}

int ship::getRichestPirate() const {
    return richest_pirate;
}


void ship::freePirate(pirate* addpirate ) {
   int num_order = addpirate->getMTime() ;
   int id = addpirate->getMPirate();
    m_ship_orderedPirates.remove(num_order) ;
    m_ship_richPirates.remove(*addpirate) ;
    m_shipPirates.remove(id) ;
    //updating the get_richest_pirate
    richest_pirate = m_ship_richPirates.findMaxRightmostLeaf(m_ship_richPirates.tree_root);
}


int ship::getBonusTreasure() const {
    return m_bonusTreasure;
}

void ship::addBonusTreasure(int addTreasure) {
    m_bonusTreasure += addTreasure ;
}

int ship::oldest_pirateId()  {
    pirate *curr = m_ship_orderedPirates.findMaxRightmostLeaf2(m_ship_orderedPirates.tree_root)->data ;
    return curr->getMPirate();
}

void ship::updating_richest(pirate* curr_pirate , int coins) {
    m_ship_richPirates.remove(*curr_pirate);
    curr_pirate->change_treasure(coins) ;
    m_ship_richPirates.insert(*curr_pirate,curr_pirate->getMPirate());
    richest_pirate = m_ship_richPirates.findMaxRightmostLeaf(m_ship_richPirates.tree_root);
}
