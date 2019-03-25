#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <list>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//separate this into two base classes
//what am i allowed to define and redefine?
//move all definitions to .cpp file, keep declarations here
class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* s, int health, int imageID, double startX, double startY, int dir, double size, int depth); //Constructor for the actor creates a new Actor with a GraphObject base. Necessary to create an Actor object.
    virtual ~Actor(); //Destructor for the Actor is set to virtual so that it deletes the GraphObject base along with all variables of Actor.
    void nowDead(); //Sets the bool variable "dead" of the Actor to true, meaning that the actor is now dead.
    bool isDead() const; //Returns the bool variable "dead" of the Actor.
    int getHealth() const; //Returns the integer variable "health" of the Actor.
    void subtractHealth(int h); //If the actor is a ship, this function sets the private integer variable "health" to health-h. If the object's health goes below zero, it calls kill(). If the health goes above 50 and it is a NachenBlaster, then recursion is used to subtract the required amount of health from the NachenBlaster.
    void makeAlienTrue(); //Sets the bool variable "alien" of the Actor to true, meaning that the actor is an alien.
    void makeAlienFalse(); //Sets the bool variable "alien" of the Actor to false, meaning that the actor is not an alien.
    bool isAlien() const; //Returns the bool variable "alien" of the Actor.
    bool isSnagglegon() const; //Returns the bool variable "snagglie" of the Actor, which holds whether or not the Actor is a snagglegon. The purpose of having this is to know how much to increase the score by (250 if false or 1000 if true).
    void makeSnagglegonTrue(); //Sets the bool variable "snagglie" of the Actor to true, meaning that the actor is a snagglegon.
    bool flownOffScreen() const; //Returns whether or not the object flew off the screen (X or Y is < 0 or >= VIEW_WIDTH or VIEW_HEIGHT).
    virtual void kill(); //Returns return. Does nothing else. This is different than moving off a screen. This is murder.
    StudentWorld* getWorld(); //Returns a pointer to the StudentWorld object it is in.
    
    virtual void move() = 0; //A pure virtual function that defines the movement of an actor in one tick.
    virtual bool collide(Actor* a) = 0; //A pure virtual function that defines what happens once a collision between another actor and the current actor occurs. It is called in the allCollisions function of StudentWorld.
    virtual void doSomething() = 0; // A pure virtual function that defines what each type of actor should do in one tick. It calls allCollisions to see if that actor collided with any relevant objects.
    
private:
    StudentWorld* stud; //a pointer to the student world
    int m_health; //stores the health of an actor. This is only relevant for ships
    bool dead; //Non-AlienOrNach actors are not considered dead or alive
    bool alien; //Aliens are true, rest are false
    bool snagglie; //snagglegons are true, rest are false
};

class AlienOrNach: public Actor
{
public:
    AlienOrNach(StudentWorld* s, int health, int imageID, double startX, double startY, int dir, double size, int depth, int f, double t); //Constructor for the AlienOrNach creates a new AlienOrNach with a Actor base. Necessary to create an AlienOrNach object. Calls makeAlienTrue(), sets the flightPlanLength to 0, and sets travelSpeed and flightPlan equal to the input.
    virtual ~AlienOrNach(); //Destructor for the AlienOrNach is set to virtual so that it deletes the Actor base along with all variables of AlienOrNach.
    int getFlightPlan(); //Returns the integer variable "flightPlan" of the AlienOrNach.
    void setFlightPlan(int f); //Sets the integer variable "flightPlan" of the AlienOrNach to int f.
    int getFlightPlanLength(); //Returns the integer variable "flightPlanLength" of the AlienOrNach.
    void setFlightPlanLength(int fl); //Sets the integer variable "flightPlanLength" of the AlienOrNach to int fl.
    double getTravelSpeed(); //Returns the integer variable "travelSpeed" of the AlienOrNach.
    void setTravelSpeed(double t); //Sets the integer variable "travelSpeed" of the AlienOrNach to double t.
    void shoot(); //fires the appropriate projectile
    virtual void kill(); //sets the ship to dead. If an alien, follows the appropriate steps for when an alien dies
    
    virtual void dropGoodie() = 0; //This function is only for aliens and has the purpose of dropping a goodie, if the probability calls for such.
private:
    int flightPlan; //direction of motion (0 = left, 45 = down left, 315 = up left)
    int flightPlanLength; //how many ticks to move in the current flight plan
    double travelSpeed; //how many pixels to move per tick
};

//NACHENBLASTER
class NachenBlaster : public  AlienOrNach
{
public:
    NachenBlaster(StudentWorld* s); //Constructor for the NachenBlaster creates a new NachenBlaster with an AlienOrNach base. Necessary to create an NachenBlaster object. Sets cabbagePoints to 30, calls makeAlienFalse(), and sets the amount of torpedoes to 0.
    virtual ~NachenBlaster(); //Destructor for the NachenBlaster is set to virtual so that it deletes the AlienOrNach base along with all variables of NachenBlaster.
    virtual void doSomething(); //does the appropriate set of steps for each tick
    virtual void move(); //moves in the direction of the user key input
    int getCabbagePoints(); //Returns the integer variable cabbagePoints
    void addCabbagePoints(int num); //Adds num to the integer cabbagePoints.
    void shootTorpedo(); //If the integer variable "torpedoes" is greater than 0, then create a new torpedo, add it to the actorList, play sound_torpedo, and decremenet the torpedoes variable by 1.
    void incTorpedoes(int num); //Increase the integer "torpedoes" variable by num.
    int getNumTorpedoes(); //Return the integer variable "torpedoes".
    virtual void dropGoodie(); //Return. A NachenBlaster cannot drop goodies.
    virtual bool collide(Actor* a); //Return. All of the collisions with the NachenBlaster are taken care of in other types of actors' collide functions.
private:
    int cabbagePoints; //number of cabbage points remaining
    int torpedoes; //number of torpedoes remaining
};

//ALIENS
class Smallgon : public AlienOrNach
{
public:
    Smallgon(StudentWorld* s, int health); //Constructor for the Smallgon creates a new Smallgon with an AlienOrNach base. Necessary to create an Smallgon object.
    virtual ~Smallgon(); //Destructor for the Smallgon is set to virtual so that it deletes the AlienOrNach base along with all variables of Smallgon.
    virtual void doSomething(); //does the appropriate set of steps for each tick
    virtual void move(); //moves in the direction as specified by the spec
    virtual bool collide(Actor* a); //If this alien collided with a NachenBlaster, subtract 5 health from the NachenBlaster using subtractHealth(5), call kill() on the alien, and return true. Otherwise, return false, meaning that the alien collided with nothing relevant.
    virtual void dropGoodie(); //Return, because smallgons cannot drop goodies.
};

class Smoregon : public AlienOrNach
{
public:
    Smoregon(StudentWorld* s, int health); //Constructor for the Smoregon creates a new Smoregon with an AlienOrNach base. Necessary to create an Smoregon object.
    virtual ~Smoregon(); //Destructor for the Smoregon is set to virtual so that it deletes the AlienOrNach base along with all variables of Smoregon.
    virtual void doSomething(); //does the appropriate set of steps for each tick
    virtual void move(); //moves in the direction as specified by the spec
    virtual bool collide(Actor* a); //If this alien collided with a NachenBlaster, subtract 5 health from the NachenBlaster using subtractHealth(5), call kill() on the alien, and return true. Otherwise, return false, meaning that the alien collided with nothing relevant.
    virtual void dropGoodie(); //There is a 1/3 chance it will drop a goodie. If it will drop a goodie, there is a 50% chance it will be a FlatulenceTorpedoGoodie and a 50% chance it will be a RepairGoodie.
};

class Snagglegon : public AlienOrNach
{
public:
    Snagglegon(StudentWorld* s, int health); //Constructor for the Snagglegon creates a new Snagglegon with an AlienOrNach base. Necessary to create an Snagglegon object. Calls makeSnagglegonTrue(), which makes the private bool variable of Actor called "snagglie" true.
    virtual ~Snagglegon(); //Destructor for the Snagglegon is set to virtual so that it deletes the AlienOrNach base along with all variables of Snagglegon.
    virtual void doSomething(); //does the appropriate set of steps for each tick
    virtual void move(); //moves in the direction as specified by the spec
    virtual bool collide(Actor* a); //If this alien collided with a NachenBlaster, subtract 15 health from the NachenBlaster using subtractHealth(15), call kill() on the alien, and return true. Otherwise, return false, meaning that the alien collided with nothing relevant.
    virtual void dropGoodie(); //There is a 1/6 ch`ance it will drop an ExtraLifeGoodie.
};

//STARS
//Would it be viewWidth ViewHeight for the x and y?
class Star: public Actor
{
public:
    
    Star(StudentWorld* s); //Constructor for the Star creates a new Star with an Actor base. Necessary to create a star object. Used for creating a new star at beginning of game
    //creating a new star in the middle of the game. No matter what x value is inputted, it will always be 0
    Star(StudentWorld* s, double startX); //Constructor for the Star creates a new Star with an Actor base. Necessary to create a star object for when it is added during or after the first tick (not in the StudentWorld::init() function. This is because the startX is always set to VIEW_WIDTH-1. Used for creating a new star in the middle of the game
    virtual ~Star(); //Destructor for the Star is set to virtual so that it deletes the Actor base along with all variables of Star.
    virtual void doSomething(); //If the star hasn't flown off the left side of the screen, call move(). Otherwise, call nowDead() on the star.
    virtual void move(); //If the star has flown off the screen, call nowDead() and return. Otherwise, move one coordinate to the left (getX() - 1).
    virtual bool collide(Actor* a); //Return false because stars cannot collide with any actors.
    
};

//EXPLOSIONS
class Explosion : public Actor
{
public:
    Explosion(StudentWorld* s, double startX, double startY); //Constructor for the Explosion creates a new Explosion with an Actor base. Necessary to create a Explosion object. Set the numTicksAlive to 0.
    virtual ~Explosion(); //Destructor for the Explosion is set to virtual so that it deletes the Actor base along with all variables of Explosion.
    virtual void doSomething(); //The explosion must show for four ticks and then disappear.
    virtual void move(); //Return. Explosions can't move.
    virtual bool collide(Actor* a); //Return false. Explosions cannot collide with any actors.
private:
    int numTicksAlive; //counts the number of ticks the explosion has been alive for
};

//PROJECTILES
class Projectile : public Actor
{
public:
    Projectile(StudentWorld* s, int imageID, double startX, double startY, int dir, double size, int depth); //Constructor for the Projectile creates a new Projectile with an Actor base. Necessary to create a Projectile object.
    virtual ~Projectile(); //Destructor for the Projectile is set to virtual so that it deletes the Actor base, which it would do even if it's not virtual.
    virtual void doSomething(); //If it is dead or has flown off the right side of the screen, return. If it collided with a relevant object (an alien) return. Call move(), and then if it collided with a relevant object (an alien), return.
    virtual void move() = 0; //Defines the movement of a projectile.
};

class Cabbage : public Projectile
{
public:
    Cabbage(StudentWorld* s, double startX, double startY); //Constructor for the Cabbage creates a new Cabbage with an Projectile base. Necessary to create an Cabbage object.
    virtual ~Cabbage(); //Destructor for the Cabbage is set to virtual so that it deletes the Projectile base along with all variables of Cabbage.
    virtual bool collide(Actor* a); //If the cabbage collided with an alien and is not dead and the alien isn't dead, subtract 2 health from the alien. If the alien did not die, play sound_blast. Mark the cabbage as dead and return whether or not it collided with a relevant object.
    virtual void move(); //If it flew off the screen, set itself as dead and return. Otherwise, move 8 pixels to the right and tilt itself 20 degrees counter-clockwise.
};

class Turnip : public Projectile
{
public:
    Turnip(StudentWorld* s, double startX, double startY); //Constructor for the Turnip creates a new Turnip with an Projectile base. Necessary to create an Turnip object.
    virtual ~Turnip(); //Destructor for the Turnip is set to virtual so that it deletes the Projectile base along with all variables of Turnip.
    bool collide(Actor* a); //If the turnip collided with the NachenBlaster and is not dead and the NachenBlaster isn't dead, subtract 2 health from the NachenBlaster. Play sound_blast. Mark the turnip as dead and return whether or not it collided with a relevant object.
    virtual void move(); //If it flew off the screen, set itself as dead and return. Otherwise, move 6 pixels to the left.
};

class FlatulenceTorpedo : public Projectile
{
public:
    FlatulenceTorpedo(StudentWorld* s, double startX, double startY, int dir); //Constructor for the FlatulenceTorpedo creates a new FlatulenceTorpedo with an Projectile base. Necessary to create an FlatulenceTorpedo object.
    virtual ~FlatulenceTorpedo(); //Destructor for the FlatulenceTorpedo is set to virtual so that it deletes the Projectile base along with all variables of FlatulenceTorpedo.
    virtual bool collide(Actor* a); //gives damage if it collided with the correct ship for its current direction
    virtual void move(); //If it flew off the screen, set itself as dead and return. Otherwise, move 8 pixels to the left or right depending on the direction.
};


//GOODIES
class Goodie : public Actor
{
public:
    Goodie(StudentWorld* s, int imageID, double startX, double startY, int dir, double size, int depth); //Constructor for the Goodie creates a new Goodie with an Actor base. Necessary to create a Goodie object.
    virtual ~Goodie(); //Destructor for the Goodie is set to virtual so that it deletes the Actor base, which it would do even if it's not virtual.
    virtual void move(); //If it flew off the screen, set itself as dead and return. Otherwise, move down and left.
    virtual void doSomething(); //If it flew off the screen or is dead, return. If it collided with a relevant object, return. Call move(), then check again if it collided with a relevant object by calling allCollisions.
    virtual bool collide(Actor* a); //Returns whether or not it collided with a relevant object (NachenBlaster). Call activate() if this is true.
    virtual void activate() = 0; //This function activates the goodie.
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* s, double startX, double startY); //Constructor for the ExtraLifeGoodie creates a new ExtraLifeGoodie with an Goodie base. Necessary to create a ExtraLifeGoodie object.
    virtual ~ExtraLifeGoodie(); //Destructor for the Goodie is set to virtual so that it deletes the Actor base, which it would do even if it's not virtual.
    virtual void activate(); //Increases the score by 100, plays sound_goodie, increases the number of lives, and sets itself to dead using nowDead().
};

class RepairGoodie : public Goodie
{
public:
    RepairGoodie(StudentWorld* s, double startX, double startY); //Constructor for the RepairGoodie creates a new RepairGoodie with an Goodie base. Necessary to create a RepairGoodie object.
    virtual ~RepairGoodie(); //Destructor for the RepairGoodie is set to virtual so that it deletes the Goodie base, which it would do even if it's not virtual.
    virtual void activate(); //Increases the score by 100, plays sound_goodie, subtracts -10 health (basically increasing the health by 10), and sets itself to dead using nowDead().
};

class FlatulenceTorpedoGoodie : public Goodie
{
public:
    FlatulenceTorpedoGoodie(StudentWorld* s, double startX, double startY); //Constructor for the FlatulenceTorpedoGoodie creates a new FlatulenceTorpedoGoodie with an Goodie base. Necessary to create a FlatulenceTorpedoGoodie object.
    virtual ~FlatulenceTorpedoGoodie(); //Destructor for the FlatulenceTorpedoGoodie is set to virtual so that it deletes the Goodie base, which it would do even if it's not virtual.
    virtual void activate(); //Increases the score by 100, plays sound_goodie, increases the number of torpedoes by 5, and sets itself to dead using nowDead().
};

#endif // ACTOR_H_

