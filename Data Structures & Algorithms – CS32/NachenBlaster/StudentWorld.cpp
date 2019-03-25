#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <list>
#include <string>
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
#include <iostream>
using namespace std;

//Definitions are found in StudentWorld.h file and report.docx which is attached within this zip file

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

//PUBLIC FUNCTIONS
StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
    //initializing variables to 0
    numAliensDestroyed = 0;
    numAliensNeedDestroyed = 0;
    maxShips = 0;
}

int StudentWorld::init()
{
    //1: Initialize the data structures used to keep track of your game’s world.
    ostringstream oss;
    numAliensDestroyed = 0;
    numAliensNeedDestroyed = 6 + (4 * getLevel());
    maxShips = 4 + (.5 * getLevel());
    //2: Allocate and insert a valid NachenBlaster object into the game world.
    nach = new NachenBlaster(this);
    //3: Allocate and insert star objects into the game world.
    for(int starCount = 0; starCount < 30; starCount++)
    {
        Actor* a = new Star(this);
        actorList.push_back(a);
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    //Calling doSomething() for NachenBlaster
    if(!nach->isDead())
    {
        nach->doSomething();
    }
    //if the NachenBlaster died, return the appropriate value
    if(nach->isDead())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    //if enough aliens were killed, return that the level is completed
    if(numAliensNeedDestroyed - numAliensDestroyed <= 0)
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    //For each actor, call doSomething()
    for(list<Actor*>::iterator it = actorList.begin();it != actorList.end(); ++it)
    {
        if(!((*it)->isDead()))
            (*it)->doSomething();
        //if the NachenBlaster died, return the appropriate value
        if(nach->isDead())
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        //if enough aliens were killed, return that the level is completed
        if(numAliensDestroyed >= numAliensNeedDestroyed)
        {
            numAliensDestroyed = 0;
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    //remove all dead objects
    removeDeadObjects();
    
    //this variable holds the number of remaining aliens that must be destroyed
    int numRemaining = numAliensNeedDestroyed - numAliensDestroyed;
    //code to see if we need to add a new alien
    if(numAliensOnScreen() < min(maxShips, numRemaining))
    {
        double s1 = 60.0;
        double s2 = 20.0 + (getLevel() * 5);
        double s3 = 5.0 + (getLevel() * 10);
        double s = s1 + s2 + s3;
        Actor* a;
        double randy = (((double) randInt(0, 10000000))/10000000.0);
        //probability for which alien to add if we need to add a new one
        if(randy < (double) s1/s)
        {
            int h = 5 * (1 + ((getLevel() - 1) * .1));
            a = new Smallgon(this, h);
        }
        else if(randy < ((double) s2/s) + (s1/s))
        {
            int h = 5 * (1 + ((getLevel() - 1) * .1));
            a = new Smoregon(this, h);
        }
        else
        {
            int h = 10 * (1 + ((getLevel() - 1) * .1));
            a = new Snagglegon(this, h);
            
        }
        //put this actor in the actorList and therefore the world
        actorList.push_back(a);
    }
    
    
    //3. Update the status text on the top of the screen with the latest information (e.g., the user’s current score, the number of torpedoes the NachenBlaster has, the current level, etc.).
    int numLives = getLives();
    int percentHealth = (nach->getHealth()/50.0)*100.0;
    int percentCabbages = (nach->getCabbagePoints()/30.0)*100.0;
    int numTorpies = getNach()->getNumTorpedoes();
    
    stringstream oss;
    oss << "Lives: " << numLives << "  Health: " << percentHealth << "%  Score: " << getScore() << "  Level: " << getLevel() << "  Cabbages: " << percentCabbages << "%  Torpedoes: " << numTorpies;
    string displayedText = oss.str();
    setGameStatText(displayedText);
    
    //4. Test if we need to create a new star using probability
    if(randInt(1, 15) == 1)
    {
        
        Star* s = new Star(this, 0);
        actorList.push_back(s);
    }
    
    //Continue the game
    return GWSTATUS_CONTINUE_GAME; //continue game
}

void StudentWorld::cleanUp()
{
    //delete NachenBlaster
    if(nach != nullptr)
    {
        delete nach;
        nach = nullptr;
    }
    if(actorList.empty())
        return;
    //delete all actors
    for(list<Actor*>::iterator it = actorList.begin();it != actorList.end();)
    {
        if(*it != nullptr)
            delete *it;
        *it = nullptr;;
        it = actorList.erase(it);
    }
}

void StudentWorld::addActor(Actor* a)
{
    actorList.push_back(a);
}

void StudentWorld::increaseNumAliensDestroyed(int i)
{
    numAliensDestroyed += i;
}

NachenBlaster* StudentWorld::getNach() const
{
    return nach;
}

bool StudentWorld::allCollisions(Actor* a)
{
    //if the object collided with the NachenBlaster
    if(euclidian_dist(a->getX(), a->getY(), (nach)->getX(), (nach)->getY()) < .75 * (a->getRadius() + (nach)->getRadius()))
    {
        //call a's collide function
        if(a->collide(nach))
            return true;
    }
    //if the object collided with another object on the actorList
    for(list<Actor*>::iterator it = actorList.begin();it != actorList.end(); ++it)
    {
        //if they collided
        if(euclidian_dist(a->getX(), a->getY(), (*it)->getX(), (*it)->getY()) < .75 * (a->getRadius() + (*it)->getRadius()))
        {
            //if a is itself, continue to the next iteration
            if(a == *it)
                continue;
            //call a's collide function
            if(a->collide(*it))
                return true;
        }
    }
    //return whether or not the object collided with a relevant object
    return false;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

//PRIVATE FUNCTIONS
//Removes all the dead objects from the actorList and therefore the Student World
void StudentWorld::removeDeadObjects()
{
    for(list<Actor*>::iterator it = actorList.begin();it != actorList.end(); ++it)
    {
        if((*it)->isDead())
        {
            delete *it;
            it = actorList.erase(it);
        }
    }
}
//calculates the number of aliens that are on the screen using a counter variable numAliens.
int StudentWorld::numAliensOnScreen()
{
    int numAliens = 0;
    for(list<Actor*>::iterator it = actorList.begin();it != actorList.end(); ++it)
    {
        if( !(*it)->isDead() && (*it)->isAlien() )
            numAliens++;
    }
    return numAliens;
}

//calculates the euclidian distance between two objects
double StudentWorld::euclidian_dist(int ax, int ay, int bx, int by)
{
    return sqrt( ((bx-ax)*(bx-ax)) + ((by-ay)*(by-ay)) );
}
