//
// Created by omarb on 7/1/2024.
//

#ifndef WET1_SHIP_H
#define WET1_SHIP_H

#include "AVLTree.h"
#include "wet1util.h"
class ship ;
class pirate{
private:
    int m_pirate ;
    int belong_shipId ;
    int treasure ;
    ship* belongToShip ;
    int m_time;

public:
    pirate() = default ;
    pirate(int pirateId ,int shipId,int treasure,ship* belongToShip);
    ~pirate();

    int getMPirate() const;

    int getBelongShipId() const;
    void setBelongShipId(int belongShipId);

    int getTreasure() const;
    void change_treasure(int num) ;
    void update_treasure() ;

    ship *getBelongToShip() const;
    void setBelongToShip(ship *belongToShip);

    int getMTime() const;

    void setMTime(int mTime);

    bool operator==(const pirate &other) const;
    bool operator!=(const pirate &other) const;
    bool operator>(const pirate &rhs) const;
    bool operator<(const pirate &rhs) const;
    friend bool operator>(const pirate* movie1, const pirate &movie2)  ;
};


class ship {
private:
    int m_shipId ;
    int m_cannons ;
    AVLTree<int,pirate> m_shipPirates;
    AVLTree<int,pirate> m_ship_orderedPirates;
    AVLTree<pirate,int> m_ship_richPirates;
    int richest_pirate;
    int m_bonusTreasure ;
    int num_order ;

public:
    ship() = default ;
    ship(int shipId,int cannons ,int richestPirate = -1) ;
    ~ship();

    void setPirate (pirate* addpirate);
    int getMSize() const ;

    int getMCannons() const;

    int getRichestPirate() const;

    void freePirate(pirate* addpirate );

    int getBonusTreasure() const;
    void addBonusTreasure(int addTreasure);


    int oldest_pirateId()  ;

    void updating_richest(pirate* curr_pirate , int coins) ;

};



#endif //WET1_SHIP_H
