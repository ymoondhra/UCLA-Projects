#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir); //Constructor for the student world creates a new StudentWorld with a GameWorld base. Necessary to create a StudentWorld object.
    virtual int init(); //initializes the world
    virtual int move(); //does something for each actor and checks if the round is over or the NachenBlaster died
    virtual void cleanUp(); //delets all dynamically allocated memory in the world
    void addActor(Actor* a); //adds an actor to the actor list
    void increaseNumAliensDestroyed(int i); //increases the numAliensDestroyed by i
    NachenBlaster* getNach() const; //returns the pointer to the NachenBlaster
    bool allCollisions(Actor* a); //By using the euclidean distance function (which is private), this function checks if the inputted actor (*a) collided with the NachenBlaster or each of the actors on the actorList. If so, it calls the function "collide" for each actor it collided with, except itself. If the collide function returns true, meaning that the object collided with something relevant (i.e. an alien colliding with a NachenBlaster), then allCollisions will be set to true. Otherwise, it will return false. The purpose of this is so that projectiles and goodies can return immediately if they collide with relevant objects.
    virtual ~StudentWorld(); //deletes the StudentWorld object and all of its dynamically allocated storage by calling cleanUp().
private:
    void removeDeadObjects(); //removes the objects on the actor list that are dead (isDead() == true)
    int numAliensOnScreen(); //returns the number of aliens on the screen (aka the number of aliens on the actor list)
    double euclidian_dist(int ax, int ay, int bx, int by); //calculates the euclidian distance between two objects

    int numAliensDestroyed; //counter for the number of aliens destroyed by the NachenBlaster. Gets set to 0 in the init() function
    int numAliensNeedDestroyed; //for every round, this calculates the number of aliens that need to be destroyed in order for the game to move on to the next level
    int maxShips; //calculates the maximum number of ships that can be on the screen at one time
    
    std::list<Actor*> actorList; //a list that stores all the actors of the student world
    NachenBlaster* nach; //pointer to the NachenBlaster
};

#endif // STUDENTWORLD_H_

