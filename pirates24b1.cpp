#include "pirates24b1.h"



Ocean::Ocean() : tree_ships() , tree_pirates(){
}

Ocean::~Ocean(){
}

StatusType Ocean::add_ship(int shipId, int cannons){
    if(shipId <= 0 || cannons < 0){
        return StatusType::INVALID_INPUT ;
    }
    if(tree_ships.find(shipId) != nullptr){
        return StatusType::FAILURE;
    }
    int prev_treeship_size = tree_ships.AVLTree_size ;
    tree_ships.insert(shipId, ship(shipId,cannons)) ;
    if(prev_treeship_size == tree_ships.AVLTree_size){
        return StatusType::ALLOCATION_ERROR ;
    }
    return StatusType::SUCCESS ;
}

StatusType Ocean::remove_ship(int shipId){
    if(shipId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if(tree_ships.find(shipId) == nullptr || tree_ships.find(shipId)->data->getMSize() != 0){
        return StatusType::FAILURE ;
    }
    int prev_treeship_size = tree_ships.AVLTree_size ;
    tree_ships.remove(shipId) ;
    if(prev_treeship_size == tree_ships.AVLTree_size){
        return StatusType::ALLOCATION_ERROR ;
    }
    return StatusType::SUCCESS ;
}

StatusType Ocean::add_pirate(int pirateId, int shipId, int treasure){
    if(shipId <= 0 || pirateId <= 0){
        return StatusType::INVALID_INPUT ;
    }
    if(tree_pirates.find(pirateId) != nullptr || tree_ships.find(shipId) == nullptr){
        return StatusType::FAILURE;
    }
    int pre_tree_size = tree_pirates.AVLTree_size ;
    pirate curr_pirate = pirate(pirateId,shipId,treasure,tree_ships.find(shipId)->data);
    //curr_pirate.change_treasure(-tree_ships.find(shipId)->data->getBonusTreasure());
    tree_pirates.insert(pirateId,curr_pirate );
    if(pre_tree_size == tree_pirates.AVLTree_size){
        return StatusType::ALLOCATION_ERROR ;
    }
    pirate* curr = tree_pirates.find(pirateId)->data ;
    tree_ships.find(shipId)->data->setPirate(curr) ;
    return StatusType::SUCCESS ;
}

StatusType Ocean::remove_pirate(int pirateId){
    if(pirateId <= 0 ){
        return StatusType::INVALID_INPUT ;
    }
    if(tree_pirates.find(pirateId) == nullptr){
        return StatusType::FAILURE ;
    }
    pirate* currpirate = tree_pirates.find(pirateId)->data ;
    currpirate->getBelongToShip()->freePirate(currpirate);
    int pre_tree_size = tree_pirates.AVLTree_size ;
    tree_pirates.remove(pirateId);
    if(pre_tree_size == tree_pirates.AVLTree_size){
        return StatusType::ALLOCATION_ERROR ;
    }
    return StatusType::SUCCESS;
}

StatusType Ocean::treason(int sourceShipId, int destShipId){
    if(sourceShipId <= 0 || destShipId <= 0 || sourceShipId == destShipId){
        return StatusType::INVALID_INPUT ;
    }
    if(tree_ships.find(sourceShipId) == nullptr || tree_ships.find(destShipId) == nullptr){
        return StatusType::FAILURE ;
    }
    ship* source_ship = tree_ships.find(sourceShipId)->data ;
    if(source_ship->getMSize() == 0){
        return StatusType::FAILURE ;
    }
    int pirate_toMove = source_ship->oldest_pirateId() ;
    if(pirate_toMove != -1){
        int prev_source_size = source_ship->getMSize() ;
        pirate* currpirate = tree_pirates.find(pirate_toMove)->data ;
        source_ship->freePirate(currpirate) ;
        if(prev_source_size == source_ship->getMSize()){
            return StatusType::ALLOCATION_ERROR ;
        }
        currpirate->update_treasure() ;
        ship* dest_ship = tree_ships.find(destShipId)->data ;
        currpirate->setBelongToShip(dest_ship);
        currpirate->setBelongShipId(destShipId) ;
        int prev_dest_size = dest_ship->getMSize() ;
        dest_ship->setPirate(currpirate) ;
        if(prev_dest_size == dest_ship->getMSize()){
            return StatusType::ALLOCATION_ERROR ;
        }
        return StatusType::SUCCESS;
    }
    return StatusType::FAILURE;
}

StatusType Ocean::update_pirate_treasure(int pirateId, int change){
    if(pirateId <=0){
        return StatusType::INVALID_INPUT ;
    }
    if(tree_pirates.find(pirateId) == nullptr){
        return StatusType::FAILURE ;
    }
    pirate* curr_pirate = tree_pirates.find(pirateId)->data;
  //  if(curr_pirate->getBelongShipId() != -1) {
        /*curr_pirate->getBelongToShip()->getMShipRichPirates().remove(*curr_pirate);
        curr_pirate->change_treasure(change);
        curr_pirate->getBelongToShip()->getMShipRichPirates().insert(*curr_pirate,curr_pirate->getMPirate());*/
        curr_pirate->getBelongToShip()->updating_richest(curr_pirate,change);
   /* } else{
        curr_pirate->change_treasure(change);
    }*/
    return StatusType::SUCCESS;
}

output_t<int> Ocean::get_treasure(int pirateId){
    if(pirateId <=0){
        return output_t<int>(StatusType::INVALID_INPUT) ;
    }
    if(tree_pirates.find(pirateId) == nullptr){
        return output_t<int>(StatusType::FAILURE) ;
    }
    pirate* ourpirate = tree_pirates.find(pirateId)->data ;
    return output_t<int>(ourpirate->getTreasure()) ;
}

output_t<int> Ocean::get_cannons(int shipId){
    if(shipId <=0){
        return output_t<int>(StatusType::INVALID_INPUT) ;
    }
    if(tree_ships.find(shipId) == nullptr){
        return output_t<int>(StatusType::FAILURE) ;
    }
    ship* ourship = tree_ships.find(shipId)->data ;
    return output_t<int>(ourship->getMCannons()) ;
}

output_t<int> Ocean::get_richest_pirate(int shipId){
    if(shipId <=0){
        return output_t<int>(StatusType::INVALID_INPUT) ;
    }
    if(tree_ships.find(shipId) == nullptr){
        return output_t<int>(StatusType::FAILURE) ;
    }
    ship* ourship = tree_ships.find(shipId)->data ;
    if(ourship->getMSize() == 0){
        return output_t<int>(StatusType::FAILURE) ;
    }
    if(ourship->getRichestPirate() != -1 ){
        return output_t<int>(ourship->getRichestPirate());
    }
    return output_t<int>(StatusType::FAILURE) ;
}

StatusType Ocean::ships_battle(int shipId1,int shipId2){
    if(shipId1 <= 0 || shipId2 <= 0 || shipId1 == shipId2){
        return StatusType::INVALID_INPUT ;
    }
    if(tree_ships.find(shipId1) == nullptr || tree_ships.find(shipId2) == nullptr){
        return StatusType::FAILURE;
    }
    ship* ship_1 = tree_ships.find(shipId1)->data ;
    ship* ship_2 = tree_ships.find(shipId2)->data ;
    if(min(ship_1->getMSize(),ship_1->getMCannons()) > min(ship_2->getMSize(),ship_2->getMCannons())){
        ship_1->addBonusTreasure(ship_2->getMSize()) ;
        ship_2->addBonusTreasure(-(ship_1->getMSize())) ;
        return StatusType::SUCCESS ;
    }
    if(min(ship_1->getMSize(),ship_1->getMCannons()) < min(ship_2->getMSize(),ship_2->getMCannons())){
        ship_2->addBonusTreasure(ship_1->getMSize()) ;
        ship_1->addBonusTreasure(-(ship_2->getMSize())) ;
        return StatusType::SUCCESS ;
    }
    return StatusType::SUCCESS ;
}

