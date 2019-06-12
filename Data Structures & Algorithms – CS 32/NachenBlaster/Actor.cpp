#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//ACTOR
//Definitions are found in StudentWorld.h file and report.docx which is attached within this zip file
Actor::Actor(StudentWorld* s, int health, int imageID, double startX, double startY, int dir, double size, int depth)
:GraphObject(imageID, startX, startY, dir, size, depth)
{
    stud = s;
    dead = false;
    alien = false;
    snagglie = false;
    m_health = health;
}

Actor::~Actor()
{}

int Actor::getHealth() const
{
    return m_health;
}
void Actor::subtractHealth(int h) //only for ships tho
{
    //if the actor is an AlienOrNach
    if(isAlien() || this == getWorld()->getNach())
    {
        //if it's dead, don't subtract health
        if(isDead())
            return;
        //subtract h health from the health of the actor
        m_health -= h;
        //if health goes lower, than 0, kill the AlienOrNach and set its health to zero
        if(m_health <= 0)
        {
            m_health = 0;
            kill();
        }
        else if(this == getWorld()->getNach() && m_health > 50)
            this->subtractHealth(m_health % 50);
        //otherwise, if NachenBlaster's health goes above 50, reset it to 50 using recursion
    }
}
void Actor::makeAlienTrue()
{
    alien = true;
}
void Actor::makeAlienFalse()
{
    alien = false;
}
bool Actor::isAlien() const
{
    return alien;
}
void Actor::nowDead() //changes status to dead (true)
{
    dead = true;
}
bool Actor::isDead() const //returns status as dead (true) or not (false)
{
    return dead;
}
StudentWorld* Actor::getWorld()
{
    return stud;
}
bool Actor::isSnagglegon() const
{
    return snagglie;
}
void Actor::makeSnagglegonTrue()
{
    snagglie = true;
}
bool Actor::flownOffScreen() const
{
    if(getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT)
        return true;
    return false;
}
void Actor::kill()
{
    return;
}

//ALIENORNACH
AlienOrNach::AlienOrNach(StudentWorld* s, int health, int imageID, double startX, double startY, int dir, double size, int depth, int f, double t)
:Actor(s, health, imageID, startX, startY, dir, size, depth)
{
    //alien is set to true, flightPlan and travelSpeed are given inputted values, and flightPlanLength is set to 0.
    makeAlienTrue();
    flightPlan = f;
    travelSpeed = t;
    flightPlanLength = 0;
}
AlienOrNach::~AlienOrNach()
{}

int AlienOrNach::getFlightPlan()
{
    return flightPlan;
}
void AlienOrNach::setFlightPlan(int f)
{
    if(isAlien())
        flightPlan = f;
}
int AlienOrNach::getFlightPlanLength()
{
    return flightPlanLength;
}
void AlienOrNach::setFlightPlanLength(int fl)
{
    if(isAlien())
        flightPlanLength = fl;
}
double AlienOrNach::getTravelSpeed()
{
    return travelSpeed;
}
void AlienOrNach::setTravelSpeed(double t)
{
    if(isAlien())
        travelSpeed = t;
}
void AlienOrNach::shoot()
{
    //if the alien isn't dead
    if(!isDead())
    {
        //NachenBlaster: Shoot a cabbage
        if(this == getWorld()->getNach())
        {
            if(getWorld()->getNach()->getCabbagePoints() >= 5)
            {
                
                Actor* cab = new Cabbage(getWorld(), getX()+12, getY());
                getWorld()->addActor(cab);
                getWorld()->playSound(SOUND_PLAYER_SHOOT);
                getWorld()->getNach()->addCabbagePoints(-5);
            }
        }
        else if(isAlien())
        {
            //Alien: shoot a projectile (torpedo if Snagglegon, otherwise turnip)
            Actor* projectile;
            if(isSnagglegon())
            {
                getWorld()->playSound(SOUND_TORPEDO);
                projectile = new FlatulenceTorpedo(getWorld(), getX() - 14, getY(), 180);
            }
            else
            {
                getWorld()->playSound(SOUND_ALIEN_SHOOT);
                projectile = new Turnip(getWorld(), getX() - 14, getY());
            }
            getWorld()->addActor(projectile);
        }
    }

}
void AlienOrNach::kill()
{
    //if the AlienOrNach is not dead and is an AlienOrNach
    if( !isDead() && (isAlien() || (this == getWorld()->getNach())) )
    {
        //set it to dead
        nowDead();
        //if it's an alien
        if(isAlien())
        {
            Actor* exp = new Explosion(getWorld(), getX(), getY());
            getWorld()->addActor(exp);
            //^create a new explosion
            dropGoodie(); //drop a goodie
            getWorld()->playSound(SOUND_DEATH); //play the appropriate sound
            getWorld()->increaseNumAliensDestroyed(1); //increase the number of aliens destroyed by 1
            if(isSnagglegon())
                getWorld()->increaseScore(1000); //increase the score by 1000 if it's a Snagglegon
            else
                getWorld()->increaseScore(250); //if it's a Smallgon or Smoregon, increase the score by 250
        }
    }
}

//NACHENBLASTER
NachenBlaster::NachenBlaster(StudentWorld* s)
:AlienOrNach(s, 50, IID_NACHENBLASTER, 0, 128, 0, 1.0, 0, 0, 0)
{
    //initializing variables and making the alien variable of Actor set to false
    cabbagePoints = 30;
    makeAlienFalse();
    torpedoes = 0;
}
NachenBlaster::~NachenBlaster()
{}
void NachenBlaster::doSomething()
{
    //if the NachenBlaster isn't dead, add a cabbagePoint, move in the appropriate direction, and if the health went below zero then kill itself.
    if(!isDead())
    {
        addCabbagePoints(1);
        move();
        if(this->getHealth() <= 0)
            this->kill();
    }
}
void NachenBlaster::move()
{
    //receives a char from the user, and moves in the appropriate direction or fires a cabbage or torpedo.
    int ch;
    if(getWorld()->getKey(ch))
    {
        // user hit a key during this tick!
        switch (ch)
        {
            case KEY_PRESS_LEFT:
            {
                if(getX() - 6 >= 0)
                    moveTo(getX() - 6, getY());
                break;
                
            }
            case KEY_PRESS_RIGHT:
            {
                if(getX() + 6 < VIEW_WIDTH)
                    moveTo(getX() + 6, getY());
                break;
            }
            case KEY_PRESS_UP:
            {
                if(getY() + 6 < VIEW_HEIGHT)
                    moveTo(getX(), getY() + 6);
                break;
            }
            case KEY_PRESS_DOWN:
            {
                if(getY() - 6 >= 0)
                    moveTo(getX(), getY()-6);
                break;
            }
            case KEY_PRESS_SPACE:
            {
                shoot(); //cabbage
                break;
            }
            case KEY_PRESS_TAB:
            {
                shootTorpedo(); //torpedo
                break;
            }
        }
    }
}
int NachenBlaster::getCabbagePoints()
{
    return cabbagePoints;
}
void NachenBlaster::addCabbagePoints(int num)
{
    //add cabbage points, checking that it does not exceed 30 or go below 0.
    cabbagePoints += num;
    if(cabbagePoints < 0)
        cabbagePoints = 0;
    else if(cabbagePoints > 30)
        cabbagePoints = 30;
}
void NachenBlaster::shootTorpedo()
{
    //if there is 1 or more torpedo, fire a torpedo
    if(torpedoes > 0)
    {
        Actor* torpie = new FlatulenceTorpedo(getWorld(), getX()+12, getY(), 0);
        getWorld()->addActor(torpie);
        getWorld()->playSound(SOUND_TORPEDO);
        torpedoes--;
    }
}
void NachenBlaster::incTorpedoes(int num)
{
    torpedoes += num;
}
int NachenBlaster::getNumTorpedoes()
{
    return torpedoes;
}
void NachenBlaster::dropGoodie()
{
    return;
}
bool NachenBlaster::collide(Actor* a)
{
    return false;
}

//ALIENS
//Smallgon
Smallgon::Smallgon(StudentWorld* s, int health)
:AlienOrNach(s, health, IID_SMALLGON, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), 0, 1.5, 1, 0, 2.0)
{}
Smallgon::~Smallgon()
{}
void Smallgon::doSomething()
{
    //if it's not dead and did not fly off the screen, continue with the doSomething() function
    if(isDead())
        return;
    if(getX() < 0)
    {
        nowDead();
        return;
    }

    //check if it collided with anything
    getWorld()->allCollisions(this);
    //return if it died
    if(isDead())
        return;
    if(getFlightPlanLength() == 0 || getY() >= VIEW_HEIGHT-1 || getY() <= 0)
    {
        if(getY() >= VIEW_HEIGHT-1)
        {
             //if hit top of screen
            //set travel direction to down and left
            setFlightPlan(45);
        }
        else if(getY() <= 0)
        {
             //if hit bottom of screen
            //set travel direction to up and left
            setFlightPlan(315);
        }
        else if(getFlightPlanLength() == 0)
        {
            int randee = randInt(1, 3);
            if(randee == 1)
                setFlightPlan(0); //setTravelSpeed(due left)
            else if(randee == 2)
                setFlightPlan(315); //setTravelSpeed(up and left)?
            else if(randee == 3)
                setFlightPlan(45); //setTravelSpeed(down and left)?
        }
        setFlightPlanLength(randInt(1, 32)); //get new flight plan length
    }
     //if within 4 coordinates of NachenBlaster's getY(), and is to the right of NachenBlaster, shoot at it and return
    if((this->getWorld()->getNach()->getX() < getX()))
    {
        if((getY() <= getWorld()->getNach()->getY() + 4) && (getY() >= getWorld()->getNach()->getY() - 4))
        {
            double shootOrNa = randInt( 1, (5*getWorld()->getLevel())+20.0 );
            if(shootOrNa <= getWorld()->getLevel())
            {
                shoot(); //fire turnip!
                return;
            }
        }
    }

     
    //moving in current direction:
    move();
    //check if it collided with anything
    getWorld()->allCollisions(this);
}
void Smallgon::move()
{
    //if flew off the left side of screen, set itself to dead and return
    if(getX() < 0)
    {
        nowDead();
        return;
    }
    if(getFlightPlan() == 315) //up and left
    {
        moveTo(getX() - getTravelSpeed(), getY() + getTravelSpeed());
    }
    else if(getFlightPlan() == 45) //down and left
    {
        moveTo(getX() - getTravelSpeed(), getY() - getTravelSpeed());
    }
    else if(getFlightPlan() == 0) //due left
        moveTo(getX() - getTravelSpeed(), getY());
    setFlightPlanLength(getFlightPlanLength() - 1); //decrement flight plan length
    
}

bool Smallgon::collide(Actor* a)
{
    bool collided = false;
    if(a == nullptr)
        return false;
    if( !isDead() && !(a->isDead()) )
    {
        if(a == getWorld()->getNach())
        {
            //if collided with nachenblaster and isn't dead and nachenblaster isn't dead, subtract 5 health from the nachenblaster and kill itself
            getWorld()->getNach()->subtractHealth(5);
            kill();
            collided = true;
        }
    }
    //returns whether or not the alien collided with anything relevant
    return collided;
}
void Smallgon::dropGoodie()
{
    //can't drop goodies so do nothing
    return;
}


//Smoregon
Smoregon::Smoregon(StudentWorld* s, int health)
:AlienOrNach(s, health, IID_SMOREGON, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), 0, 1.5, 1, 0, 2.0)
{}
Smoregon::~Smoregon()
{}
void Smoregon::doSomething()
{
    //if it's not dead and did not fly off the screen, continue with the doSomething() function
    if(isDead())
        return;
    if(getX() < 0)
    {
        nowDead();
        return;
    }
    //check if it collided with anything
    getWorld()->allCollisions(this);
    //return if it died
    if(isDead())
        return;

     if(getFlightPlanLength() == 0 || getY() >= VIEW_HEIGHT-1 || getY() <= 0)
     {
         if(getY() >= VIEW_HEIGHT-1)
         {
             //if hit top of screen
             setFlightPlan(45); //set flight plan to down and left
         }
         
         else if(getY() <= 0)
         {
             //if hit bottom of screen
             setFlightPlan(315); //set travel direction to up and left
         }
         else if(getFlightPlanLength() == 0)
         {
             //randomly create a new flight plan length
             int randee = randInt(1, 3);
             if(randee == 1)
                 setFlightPlan(0); //setTravelSpeed(due left)
             else if(randee == 2)
                 setFlightPlan(45); //setTravelSpeed(up and left)?
             else if(randee == 3)
                 setFlightPlan(315); //setTravelSpeed(down and left)?
         }
         setFlightPlanLength(randInt(1, 32)); //get new flight plan length
     }
    
    //if within 4 coordinates of NachenBlaster's getY(), and is to the right of NachenBlaster, shoot at it and return. If don't shoot, there is a chance the alien will to go to ramming speed
    if((this->getWorld()->getNach()->getX() < getX()))
    {
        if((getY() <= getWorld()->getNach()->getY() + 4) && (getY() >= getWorld()->getNach()->getY() - 4))
        {
            int endPt = (5.0*getWorld()->getLevel()) + 20.0;
            int shootOrNa = randInt(1, endPt);
            if((getY() <= getWorld()->getNach()->getY() + 4) && (getY() >= getWorld()->getNach()->getY() - 4))
            {
                if(shootOrNa <= getWorld()->getLevel())
                {
                    shoot(); //fire turnip!
                    return;
                }
            }
            int changeFlightOrNa = randInt(1, endPt);
            if(changeFlightOrNa <= getWorld()->getLevel())
            {
                setFlightPlan(0); //due left
                setFlightPlanLength(VIEW_WIDTH); //until it reaches the end of the screen
                setTravelSpeed(5);
                //continue on with the next step..? what does that mean? return?
            }
        }
    }
    
     //moving in current direction:
    move();
    //check if it collided with anything
    getWorld()->allCollisions(this);
}

void Smoregon::move()
{
    //if flew off the left side of screen, set itself to dead and return
    if(getX() < 0)
    {
        nowDead();
        return;
    }
    
    if(getFlightPlan() == 315) //up and left
    {
        moveTo(getX() - getTravelSpeed(), getY() + getTravelSpeed());
    }
    else if(getFlightPlan() == 45) //down and left
    {
        moveTo(getX() - getTravelSpeed(), getY() - getTravelSpeed());
    }
    else if(getFlightPlan() == 0) //left
        moveTo(getX() - getTravelSpeed(), getY());
    
    setFlightPlanLength(getFlightPlanLength() - 1); //decrement flight plan length
}

bool Smoregon::collide(Actor* a)
{
    bool collided = false;
    if(a == nullptr)
        return false;
    if(!isDead() && !(a->isDead()))
    {
        if(a == getWorld()->getNach()) //only nachenblaster, not cabbage or torpedoes.
        {
            //if collided with nachenblaster and isn't dead and nachenblaster isn't dead, subtract 5 health from the nachenblaster and kill itself
            collided = true;
            getWorld()->getNach()->subtractHealth(5);
            kill();
        }
    }
    //returns whether or not the alien collided with anything relevant
    return collided;
}
void Smoregon::dropGoodie()
{
    //1/3 chance of dropping a goodie
    int r = randInt(1, 3);
    if(r == 1)
    {
        //50% chance of dropping each of these two goodies
        int r2 = randInt(1, 2);
        if(r2 == 1)
        {
            //Flatulence Torpedo Goodie.
            Actor* flatTorpGoodie = new FlatulenceTorpedoGoodie(getWorld(), getX(), getY());
            getWorld()->addActor(flatTorpGoodie);
        }
        else // if(r2 == 2)
        {
            //Repair Goodie
            Actor* repairGoodie = new RepairGoodie(getWorld(), getX(), getY());
            getWorld()->addActor(repairGoodie);
        }
    }
}

//Snagglegon
Snagglegon::Snagglegon(StudentWorld* s, int health)
:AlienOrNach(s, health, IID_SNAGGLEGON, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), 0, 1.5, 1, 315, 1.75)
{
    makeSnagglegonTrue(); //snagglie of class Actor becomes true for this object
}
Snagglegon::~Snagglegon()
{}
void Snagglegon::doSomething()
{
    //if it's not dead and did not fly off the screen, continue with the doSomething() function
    if(isDead())
        return;
    if(getX() < 0)
    {
        nowDead();
        return;
    }
    
    //check if it collided with anything
    getWorld()->allCollisions(this);
    //return if it died
    if(isDead())
        return;
    if(getY() >= VIEW_HEIGHT-1)
    {
        //if hit top of screen
        //set travel direction to down and left
        setFlightPlan(45);
    }
    
    else if(getY() <= 0)
    {
        //if hit bottom of screen
        //set travel direction to up and left
        setFlightPlan(315);
    }
    
    //if within 4 coordinates of NachenBlaster's getY(), and is to the right of NachenBlaster, shoot at it and return
    if((this->getWorld()->getNach()->getX() < getX()))
    {
        if((getY() <= getWorld()->getNach()->getY() + 4) && (getY() >= getWorld()->getNach()->getY() - 4))
        {
            int shootOrNa = randInt( 1, 15+(10*getWorld()->getLevel()) );
            if(shootOrNa <= getWorld()->getLevel())
            {
                shoot(); //fire torpedo!
                return;
            }
        }
    }
    
    //moving in current direction:
    move();
    //check if it collided with anything
    getWorld()->allCollisions(this);
}
void Snagglegon::move()
{
    //if flew off the left side of screen, set itself to dead and return
    if(getX() < 0)
    {
        nowDead();
        return;
    }
    
    //moving in current direction:
    if(getFlightPlan() == 315) //up and left
        moveTo(getX() - getTravelSpeed(), getY() + getTravelSpeed());
    else if(getFlightPlan() == 45) //down and left
        moveTo(getX() - getTravelSpeed(), getY() - getTravelSpeed());
    else if(getFlightPlan() == 0) //due left
        moveTo(getX() - getTravelSpeed(), getY());
}

bool Snagglegon::collide(Actor* a)
{
    bool collided = false;
    if(a == nullptr)
        return false;
    if(!isDead() && !(a->isDead()))
    {
        if(a == getWorld()->getNach())
        {
            //if collided with nachenblaster and isn't dead and nachenblaster isn't dead, subtract 15 health from the nachenblaster and kill itself
            getWorld()->getNach()->subtractHealth(15);
            collided = true;
            kill();
        }

    }
    return collided;
}
void Snagglegon::dropGoodie()
{
    //1/6 chance of a extra life goodie being dropped
    int r = randInt(1, 6);
    if(r == 1)
    {
        Actor* extraGoodie = new ExtraLifeGoodie(getWorld(), getX(), getY());
        getWorld()->addActor(extraGoodie);
    }
}

//STARS
Star::Star(StudentWorld* s)
:Actor(s, 0, IID_STAR, randInt(0, VIEW_WIDTH - 1), randInt(0, VIEW_HEIGHT - 1), 0, randInt(5, 50)/100.0, 3)
{}
Star::~Star()
{}
Star::Star(StudentWorld* s, double startX)
:Actor(s, 0, IID_STAR, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT), 0, randInt(5, 50)/100.0, 3)
{
    
}
void Star::doSomething()
{
    if(!flownOffScreen())
        move();
    else
        nowDead();
}
void Star::move()
{
    if(flownOffScreen())
    {
        nowDead();
        return;
    }
    moveTo(getX() - 1, getY());
}
bool Star::collide(Actor* a)
{
    return false;
}

//Explosion
Explosion::Explosion(StudentWorld* s, double startX, double startY)
:Actor(s, 0, IID_EXPLOSION, startX, startY, 0, 1, 0)
{
    numTicksAlive = 0;
}
Explosion::~Explosion()
{}
void Explosion::doSomething()
{
    if(numTicksAlive <= 0)
    {
        numTicksAlive = 1;
        return;
    }
    else if(numTicksAlive < 4)
    {
        setSize(getSize() * 1.5);
        numTicksAlive++;
    }
    else
        nowDead();
}
void Explosion::move()
{
    return;
}
bool Explosion::collide(Actor* a)
{
    return false;
}

//PROJECTILES
Projectile::Projectile(StudentWorld* s, int imageID, double startX, double startY, int dir, double size, int depth)
:Actor(s, 0, imageID, startX, startY, dir, size, depth)
{}
Projectile::~Projectile()
{}
void Projectile::doSomething()
{
    if(isDead())
        return;
    if(flownOffScreen())
    {
        nowDead();
        return;
    }
    if(getWorld()->allCollisions(this))
        return;
    move();
    if(getWorld()->allCollisions(this))
        return;
}

//Cabbage
Cabbage::Cabbage(StudentWorld* s, double startX, double startY)
:Projectile(s, IID_CABBAGE, startX, startY, 0, 0.5, 1)
{}
Cabbage::~Cabbage()
{}
bool Cabbage::collide(Actor* a)
{
    //if it's alive check if it collided with an alien that isn't dead and subtract 2 health from the alien, play a sound, and mark itself as dead
    bool collided = false;
    if(a == nullptr)
        return false;
    if(!isDead() && !(a->isDead()) && a->isAlien())
    {
        collided = true;
        a->subtractHealth(2);
        if(!a->isDead())
            getWorld()->playSound(SOUND_BLAST);
        nowDead();
    }
    return collided;
}

void Cabbage::move()
{
    if(flownOffScreen())
    {
        nowDead();
        return;
    }
    //move to the right
    moveTo(getX()+8, getY());
    //tilt 20 degrees counter clockwise
    setDirection(getDirection() + 20);
}

//Turnip
Turnip::Turnip(StudentWorld* s, double startX, double startY)
:Projectile(s, IID_TURNIP, startX, startY, 0, 0.5, 1)
{}
Turnip::~Turnip()
{}
bool Turnip::collide(Actor* a)
{
    //check if it collided with the nachenblaster
    bool collided = false;
    if(a == getWorld()->getNach())
    {
        collided = true;
        a->subtractHealth(2);
        getWorld()->playSound(SOUND_BLAST);
        nowDead();
    }
    return collided;
}
void Turnip::move()
{
    if(flownOffScreen())
    {
        nowDead();
        return;
    }
    
    moveTo(getX()-6, getY());
    setDirection(getDirection() - 20);
}


//FlatulenceTorpedo
FlatulenceTorpedo::FlatulenceTorpedo(StudentWorld* s, double startX, double startY, int dir)
:Projectile(s, IID_TORPEDO, startX, startY, dir, 0.5, 1)
{}
FlatulenceTorpedo::~FlatulenceTorpedo()
{}

bool FlatulenceTorpedo::collide(Actor* a)
{
    //if its direction is 0, check if it collided with an alien. Otherwise, if the direction is 180, check if it collided with the NachenBlaster.
    bool collided = false;
    if(a == nullptr)
        return false;
    if( !isDead() && !a->isDead() )
   {
       if( (getDirection() == 0 && a->isAlien()) || (getDirection() == 180 && (a == getWorld()->getNach())) )
       {
           collided = true;
           a->subtractHealth(8);
           nowDead();
           if(a == getWorld()->getNach())
               getWorld()->playSound(SOUND_BLAST);
           else if(a->isAlien() && !(a->isDead()))
               getWorld()->playSound(SOUND_BLAST);
       }
   }
    return collided;
}
    
void FlatulenceTorpedo::move()
{
    if(flownOffScreen())
    {
        nowDead();
        return;
    }
    if(getDirection() == 180) //fired by alien
        moveTo(getX()-8, getY());
    else if(getDirection() == 0)
        moveTo(getX()+8, getY());
}

//GOODIES
Goodie::Goodie(StudentWorld* s, int imageID, double startX, double startY, int dir, double size, int depth)
:Actor(s, 0, imageID, startX, startY, dir, size, depth)
{}
Goodie::~Goodie()
{}
void Goodie::move()
{
    if(flownOffScreen())
    {
        nowDead();
        return;
    }
    moveTo(getX()-0.75, getY()-0.75);
}
void Goodie::doSomething()
{
    if(isDead())
        return;
    if(flownOffScreen())
    {
        nowDead();
        return;
    }
    
    if(getWorld()->allCollisions(this))
        return;
    
    move();
    
    if(getWorld()->allCollisions(this))
        return;
}
bool Goodie::collide(Actor* a)
{
    bool collided = false;
    if(!isDead() && (a == getWorld()->getNach()))
    {
        collided = true;
        activate();
    }
    return collided;
}

//Note: definitions found in Actor.h file
//ExtraLifeGoodie
ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* s, double startX, double startY)
:Goodie(s, IID_LIFE_GOODIE, startX, startY, 0, 0.5, 1)
{}
ExtraLifeGoodie::~ExtraLifeGoodie()
{}
void ExtraLifeGoodie::activate()
{
    getWorld()->increaseScore(100);
    getWorld()->playSound(SOUND_GOODIE);
    getWorld()->incLives();
    nowDead();
}

//RepairGoodie
RepairGoodie::RepairGoodie(StudentWorld* s, double startX, double startY)
:Goodie(s, IID_REPAIR_GOODIE, startX, startY, 0, 0.5, 1)
{}
RepairGoodie::~RepairGoodie()
{}
void RepairGoodie::activate()
{
    getWorld()->increaseScore(100);
    getWorld()->playSound(SOUND_GOODIE);
    getWorld()->getNach()->subtractHealth(-10);
    nowDead();
}

//FlatulenceTorpedoGoodie
FlatulenceTorpedoGoodie::FlatulenceTorpedoGoodie(StudentWorld* s, double startX, double startY)
:Goodie(s, IID_TORPEDO_GOODIE, startX, startY, 0, 0.5, 1)
{}
FlatulenceTorpedoGoodie::~FlatulenceTorpedoGoodie()
{}
void FlatulenceTorpedoGoodie::activate()
{
    getWorld()->increaseScore(100);
    getWorld()->playSound(SOUND_GOODIE);
    getWorld()->getNach()->incTorpedoes(5);
    nowDead();
}
