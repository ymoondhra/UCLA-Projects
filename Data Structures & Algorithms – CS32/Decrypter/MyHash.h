///DO WE HAVE TO DO IFNDEF, CAN WE JUST DEFINE THE IMPLEMENTATIONS WITHIN THE CLASS DEFINITION?

#ifndef HASH_H_
#define HASH_H_

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;
    const ValueType* find(const KeyType& key) const; // for a map that can't be modified, return a pointer to const ValueType
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key);

    // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;
    
    int numNodesDeleted = 0;
    int numNodesAdded = 0;

private:
    struct Node
    {
        Node()
        {
            next = nullptr;
        }
        ~Node()
        {
            if(next != nullptr)
            {
                delete next;
            }
        }
        Node(KeyType key, ValueType value)
        {
            k = key;
            v = value;
            next = nullptr;
        }
        KeyType k;
        ValueType v;
        Node* next;
    };
    int numBuckets;
    int numItems;
    double maxLoad; //maximum num items divided by num buckets
    Node** hashTable;
    
    unsigned int getBucketNumber(const KeyType& key) const;
    bool addToBucket(const KeyType& key, const ValueType& value);
    void cleanUp();
};

template<typename KeyType, typename ValueType>
MyHash<KeyType,ValueType>::MyHash(double maxLoadFactor)
{
    numBuckets = 100;
    numItems = 0;
    
    if(maxLoadFactor <= 0)
        maxLoad = 0.5;
    else if(maxLoadFactor > 2.0)
        maxLoad = 2.0;
    else
        maxLoad = maxLoadFactor;
    
    hashTable = new Node*[numBuckets];
    for(int i = 0; i < numBuckets; i++)
    {
        hashTable[i] = nullptr;
    }
}

template<typename KeyType, typename ValueType>
MyHash<KeyType,ValueType>::~MyHash()
{
    //free all memory, unless already freed
    cleanUp();
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType,ValueType>::reset()
{
    //free all memory
    cleanUp();
    //create new empty hash table with size 100 buckets
    hashTable = new Node*[100];
    for(int i = 0; i < numBuckets; i++)
    {
        hashTable[i] = nullptr;
    }
    /////QUESTION: Why do we set each pointer of the hash array to nullptr?
    /////  Answer: So that each element of the hash array will either be nullptr or pointing to a Node, so that we can search for elements (look at find function code). If we don't give the pointers the nullptr value then they will be pointing to some random place in memory and will technically have a value.
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType,ValueType>::associate(const KeyType& key, const ValueType& value)
{
    //add new association to the hash table
    //if the key is already in the map, then update
    ValueType* v = find(key);
    if(v != nullptr)
    {
        //discard the old value that the key mapped to and replace it with the new value.
        *v = value;
    }
    else //if the key is not in the hashTable
    {
        bool wasAdded = addToBucket(key, value);
        if(!wasAdded) //did not get added because load factor
        {
            //allocate a new dynamic array with numBuckets*2 buckets. Move all items to new array, might need to re-compute proper location for every key/value association in the new array. Delete contents of old array
            int newNumBucks = 2*numBuckets;
            numItems = 0;
            Node** newHashTable = new Node*[newNumBucks];
            for(int i = 0; i < newNumBucks; i++)
            {
                newHashTable[i] = nullptr;
            }
            Node** oldHashTable = hashTable;
            hashTable = newHashTable;
            for(int i = 0; i < numBuckets; i++)
            {
                if(oldHashTable[i] != nullptr)
                {
                    addToBucket(oldHashTable[i]->k, oldHashTable[i]->v);
                    Node* tempPtr = oldHashTable[i]->next;
                    while(tempPtr != nullptr)
                    {
                        addToBucket(tempPtr->k, tempPtr->v);
                        /////2018 edit: dont we have to delete the dynamically allocated nodes within each element of the hash array?
                        /////i.e.:  Node* old = tempPtr;
                        tempPtr = tempPtr->next;
                        /////       delete old;
                    }
                    delete oldHashTable[i];
                }
            }
            delete [] oldHashTable;
            numBuckets = newNumBucks;
            associate(key, value);
        }
    }
}

template<typename KeyType, typename ValueType>
int MyHash<KeyType,ValueType>::getNumItems() const
{
    return numItems;
}

template<typename KeyType, typename ValueType>
double MyHash<KeyType,ValueType>::getLoadFactor() const
{
    double loadFactor = ((double) numItems/ (double) numBuckets);
    return loadFactor;
}

template<typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType,ValueType>::find(const KeyType& key) const
{
    unsigned int bucketNum = getBucketNumber(key);
    Node* ptr = hashTable[bucketNum];
    
    
    if(ptr == nullptr)
        return nullptr;
    
    while(ptr != nullptr)
    {
        if(ptr->k == key)
        {
            ValueType* VTptr = &(ptr->v);
            return VTptr;
        }
        ptr = ptr->next;
    }
    return nullptr;
}

template<typename KeyType, typename ValueType>
ValueType* MyHash<KeyType,ValueType>::find(const KeyType& key)
{
    return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
}


//-----   PRIVATE FUNCTIONS   ------
template<typename KeyType, typename ValueType>
unsigned int MyHash<KeyType,ValueType>::getBucketNumber(const KeyType& key) const
{
    unsigned int hash(const KeyType& k);
    unsigned int h = hash(key);
    return h % numBuckets;
}

template<typename KeyType, typename ValueType>
bool MyHash<KeyType,ValueType>::addToBucket(const KeyType& key, const ValueType& value)
{
    if(((numItems + 1.0)/(double) numBuckets) > maxLoad)
        return false;
    
    unsigned int bucketNum = getBucketNumber(key);
    Node* ptr = hashTable[bucketNum];
    if(ptr == nullptr)
    {
        ptr = new Node(key, value);
        hashTable[bucketNum] = ptr;
    }
    else
    {
        while(ptr->next != nullptr)
        {
            ptr = ptr->next;
        }
        ptr->next = new Node(key, value);
    }
    
    numItems++;
    return true;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType,ValueType>::cleanUp()
{
    if(hashTable != nullptr)
    {
        for(int i = 0; i < numBuckets; i++)
        {
            if(hashTable[i] != nullptr)
            {
                delete hashTable[i];
            }
        }
    }
    delete [] hashTable;
    hashTable = nullptr;
    numBuckets = 100;
    numItems = 0;
}

#endif // HASH_H_
