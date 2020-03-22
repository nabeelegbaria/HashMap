#include <iostream>
#include <vector>
#include <set>
#include <cassert>

#define KEY_NOT_FOUND "KEY NOT FOUND"
#define INVALID_FACTOR " FACTOR IS INVALID"
#define INVALIDMSG "INVALID"
#define DIFFERRENTSIZE "DIFFERENT VECTOR SIZE"

template<class keyT, class valueT>
/**
 * A class which represents a HashMap.
 * @tparam keyT
 * @tparam valueT
 */
class HashMap
{
public:
    /**
     * default constructor which creates a map with capacity 16,lowerfactor = 0.25, upperfactor = 0.75.
     */
    HashMap()
    {
        hashTable = *new std::vector<std::vector<std::pair<keyT, valueT>>>(16);
        hashcapacity = 16;
        hashsize = 0;
        upperfacor = 0.75;
        lowerfactor = 0.25;
    }

    /**
     * This func checks if the given factor is legal .
     * @param factor
     * @return
     */
    bool legal(double factor)
    {
        return !(factor < 0 || factor > 1);
    }

    /**
     * constructor which receives two factors and creates a hashmap with capacity 16.
     * @param lowerfactor
     * @param upperfactor
     */
    HashMap(double lowerfactor, double upperfactor) : HashMap()
    {
        if (upperfactor < lowerfactor || !legal(lowerfactor) || !legal(upperfactor))
        {
            throw std::invalid_argument(INVALID_FACTOR);
        }
        else
        {
            this->upperfacor = upperfactor;
            this->lowerfactor = lowerfactor;
        }
    }

    /**
     * constructor which receives two vectors and creates a hashmap according them .
     * @param keysvec
     * @param valvec
     */
    HashMap(std::vector<keyT> keysvec, std::vector<valueT> valvec) : HashMap()
    {

        if (keysvec.size() != valvec.size())
        {
            throw std::invalid_argument(DIFFERRENTSIZE);
        }
        for (int j = 0; j < (int) keysvec.size(); ++j)
        {
            int hashIndex = hashFunc(keysvec[j]);
            if (!containsKey(keysvec[j]))
            {
                std::pair<keyT, valueT> target = {keysvec[j], valvec[j]};
                hashTable[hashIndex].push_back(target);
                hashsize++;
                if (getLoadFactor() > upperfacor)
                {
                    resize(hashTable, true);
                }
            }
            else
            {
                for (typename std::vector<std::pair<keyT, valueT>>::iterator it = hashTable[hashIndex].begin();
                     it != hashTable[hashIndex].end(); ++it)
                {
                    if (it->first == keysvec[j])
                    {
                        it->second = valvec[j];
                    }
                }
            }
        }
    }

    /**
     * a copy constructor.
     * @param other
     */
    HashMap(const HashMap &other)
    {
        this->hashsize = other.hashsize;
        this->hashcapacity = other.hashcapacity;
        this->upperfacor = other.upperfacor;
        this->lowerfactor = other.lowerfactor;
        this->hashTable = *new std::vector<std::vector<std::pair<keyT, valueT>>>(other.capacity());
        for (int i = 0; i < other.hashcapacity; ++i)
        {
            if (!other.hashTable[i].empty())
            {
                this->hashTable[i] = other.hashTable[i];
            }
        }
    }

    /**
     * a move constructor.
     * @param other
     */
    HashMap(HashMap && other)
    {
        this->hashTable = other.hashTable;
        this->hashsize = other.hashsize;
        this->hashcapacity = other.hashcapacity;
        this->upperfacor = other.upperfacor;
        this->lowerfactor = other.lowerfactor;
        other.clear();
    }

    /**
     * destructor for the hashmap.
     */
    ~HashMap()
    {
        clear();
    }

    //********************************* METHODS *********************************

    /**
     * This function returns the hash size.
     * @return size.
     */
    int size() const
    {
        return hashsize;
    }

    /**
     * This function returns the hash capacity.
     * @return capacity .
     */
    int capacity() const
    {
        return this->hashcapacity;
    }

    /**
     * This function represents the hash function .
     * @param key
     * @return digit which represents the place of the key in the hashTable.
     */
    int hashFunc(const keyT &key) const
    {
        std::hash<keyT> hash_fn;
        int _hash = hash_fn(key);
        return _hash & (capacity() - 1);
    }

    /**
     * This function returns the load factor.
     * @return load factor.
     */
    double getLoadFactor() const
    {
        return ((double) hashsize / hashcapacity);
    }

    /**
     * This function checks if the hash is empty .
     * @return true if it is empty , false otherwise.
     */
    bool empty() const
    {
        return hashsize == 0;
    }

    /**
     * A rehash function which replace the hashTable with bigger/smaller capacity .
     * @param arr
     * @param flag
     */
    void resize(std::vector<std::vector<std::pair<keyT, valueT>>> &arr, bool flag)
    {
        int currentcapacity = hashcapacity;
        std::vector<std::vector<std::pair<keyT, valueT>>> currentarr = arr;
        if (flag)
        {
            arr = *new std::vector<std::vector<std::pair<keyT, valueT>>>(hashcapacity * 2);
            hashcapacity *= 2;
            hashsize = 0;
        }
        if (!flag)
        {
            arr = *new std::vector<std::vector<std::pair<keyT, valueT>>>(hashcapacity / 2);
            hashcapacity /= 2;
            hashsize = 0;
        }
        for (int i = 0; i < currentcapacity; ++i)
        {
            if (!currentarr[i].empty())
            {
                for (int j = 0; j < (int) currentarr[i].size(); ++j)
                {
                    keyT f = currentarr[i][j].first;
                    valueT v = currentarr[i][j].second;
                    insert(f, v);
                }
            }
        }
    }

    /**
     * This function receives a key and value , adds them to the hashTable.
     * @param key
     * @param value
     * @return
     */
    bool insert(const keyT &key, const valueT &value)
    {
        if (containsKey(key))
        {
            return false;
        }
        auto temp = std::pair<keyT, valueT>(key, value);
        int hashIndex = hashFunc(key);
        hashTable[hashIndex].push_back(temp);
        hashsize++;
        if (getLoadFactor() > upperfacor)
        {
            resize(hashTable, true);
        }
        return true;
    }

    /**
     * This function receives a key and check if it exists in the hashTable.
     * @param key
     * @return true if it contained , false otherwise .
     */
    bool containsKey(const keyT &key)
    {
        int hashIndex = hashFunc(key);
        if (!hashTable[hashIndex].empty())
        {
            for (typename std::vector<std::pair<keyT, valueT>>::iterator it = hashTable[hashIndex].begin();
                 it != hashTable[hashIndex].end(); ++it)
            {
                if (it->first == key)
                {
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * This function receives a key and return its value in the hashTable.
     * @param key
     * @return the value if the key exists , throwing exception otherwise.
     */
    valueT &at(const keyT &key)
    {
        if (containsKey(key))
        {
            int hashIndex = hashFunc(key);
            for (typename std::vector<std::pair<keyT, valueT>>::iterator it = hashTable[hashIndex].begin();
                 it != hashTable[hashIndex].end(); ++it)
            {
                if (it->first == key)
                {
                    return it->second;
                }
            }
        }
        else
        {

            throw std::invalid_argument(KEY_NOT_FOUND);
        }
    }

    /**
     * This function receives a key and return its value in the hashTable, the function is for reading only.
     * @param key
     * @return the value if the key exists , throwing exception otherwise.
     */
    const valueT &at(const keyT &key) const
    {
        if (containsKey(key))
        {
            int hashIndex = hashFunc(key);
            for (typename std::vector<std::pair<keyT, valueT>>::iterator it = hashTable[hashIndex].begin();
                 it != hashTable[hashIndex].end(); ++it)
            {
                if (it->first == key)
                {
                    return it->second;
                }
            }
        }
        else
        {

            throw std::invalid_argument(KEY_NOT_FOUND);
        }
    }

    /**
     * This function receives a key and erase it from the hashTable.
     * @param key
     * @return true if it is deleted , false otherwise.
     */
    bool erase(const keyT &key)
    {
        int hashIndex = hashFunc(key);
        if (!hashTable[hashIndex].empty())
        {
            for (typename std::vector<std::pair<keyT, valueT>>::iterator it = hashTable[hashIndex].begin();
                 it != hashTable[hashIndex].end(); ++it)
            {
                if (it->first == key)
                {
                    hashTable.at(hashIndex).erase(it);
                    hashsize--;
                    if (getLoadFactor() < lowerfactor)
                    {
                        resize(hashTable, false);
                    }
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * This function receives a key and returns the size of the bucket of the key .
     * @param key
     * @return bucket size.
     */
    int bucketSize(const keyT &key)
    {
        if (!containsKey(key))
        {
            throw std::invalid_argument(INVALIDMSG);
        }
        int hashIndex = hashFunc(key);
        return (int) hashTable[hashIndex].size();
    }

    /**
     * The function clears the HashMap .
     */
    void clear()
    {
        for (int i = 0; i < hashcapacity; ++i)
        {
            hashTable[i].clear();
        }
        hashTable.clear();
        hashsize = 0;
    }

    //*************IMPLEMENTATION INNER CLASS ITERATOR ************
    /**
     * inner iterator class.
     */
    class const_iterator
    {
    public:
        /**
         * a default constructor .
         * @param ptr
         * @param _hash
         */
        const_iterator(std::pair<keyT, valueT> *ptr = nullptr,
                       const HashMap<keyT, valueT> *_hash = nullptr)
        {
            this->_pointer = ptr;
            this->my_hash = _hash;
        }

        /**
         * the * operator.
         * @return
         */
        std::pair<keyT, valueT> &operator*() const
        {
            return *_pointer;
        }

        /**
         * the -> operator.
         * @return
         */
        std::pair<keyT, valueT> *operator->() const
        {
            return _pointer;
        }

        /**
         * the ++ operator, which replace the this with the next value.
         * @return
         */
        const const_iterator &operator++()
        {
            int index = my_hash->hashFunc(_pointer->first);
            if (counter == my_hash->hashsize - 1)
            {
                *this = const_iterator(nullptr);
                return *this;
            }
            for (int i = 0; i < (int) my_hash->hashTable.at(index).size(); ++i)
            {
                int k = (int) my_hash->hashTable.at(index).size();
                if (my_hash->hashTable.at(index).at(i).first == _pointer->first &&
                    i != k - 1)
                {
                    this->_pointer = (std::pair<keyT, valueT> *) &my_hash->hashTable.at(index).at(i + 1);
                    counter++;
                    break;
                }
                else if (my_hash->hashTable.at(index).at(i).first == _pointer->first)
                {
                    int a = 1;
                    while (my_hash->hashTable.at(index + a).empty())
                    {
                        a++;
                    }
                    this->_pointer = (std::pair<keyT, valueT> *) &my_hash->hashTable.at(index + a).at(0);
                    counter++;
                    break;
                }
            }
            return *this;
        }

        /**
         * the ++ operator.
         * @return
         */
        const const_iterator &operator++(int)
        {
//            const_iterator ret = *this;
            ++*this;
            return *this;
        }

        /**
         * the == operator for comparing .
         * @param rhs
         * @return
         */
        bool operator==(const_iterator const &rhs) const
        {
            return _pointer == rhs._pointer;
        }

        /**
         * the != operator for comparing .
         * @param rhs
         * @return
         */
        bool operator!=(const_iterator const &rhs) const
        {
            return _pointer != rhs._pointer;
        }

    private:
        std::pair<keyT, valueT> *_pointer;
        const HashMap<keyT, valueT> *my_hash;
        int counter = 0;
    };

    /**
     * This function returns an iterator for the first element in the HashMap.
     * @return const_iterator.
     */
    const const_iterator begin() const
    {
        for (int i = 0; i < hashcapacity; ++i)
        {
            if (!hashTable[i].empty())
            {
                const_iterator it = const_iterator((std::pair<keyT, valueT> *) &hashTable[i][0], this);
                return it;
            }
        }
        return const_iterator(nullptr);
    }

    /**
     * This function returns an iterator for nullptr.
     * @return const_iterator.
     */
    const const_iterator end() const
    {
        return const_iterator(nullptr);
    }

    /**
     * This function returns an iterator for the first element in the HashMap.
     * @return const_iterator.
     */
    const const_iterator cbegin() const
    {
        for (int i = 0; i < hashcapacity; ++i)
        {
            if (!hashTable[i].empty())
            {
                const_iterator it = const_iterator((std::pair<keyT, valueT> *) &hashTable[i][0], this);
                return it;
            }
        }
        return const_iterator(nullptr);
    }

    /**
    * This function returns an iterator for nullptr.
    * @return const_iterator.
    */
    const const_iterator cend() const
    {
        return const_iterator(nullptr);
    }

    //*************** OPERATORS *************
    /**
     * The assigning operator .
     * @param other
     * @return HashMap refference .
     */
    HashMap<keyT, valueT> &operator=(const HashMap<keyT, valueT> &other)
    {
        this->hashTable = new std::vector<std::vector<std::pair<keyT, valueT>>>(other.capacity());
        for (int i = 0; i < other.hashcapacity; ++i)
        {
            if (!other.hashTable[i].empty())
            {
                this->hashTable[i] = other.hashTable[i];
            }
        }
        this->hashcapacity = other.hashcapacity;
        this->hashsize = other.hashsize;
        this->upperfacor = other.upperfacor;
        this->lowerfactor = other.lowerfactor;
        return *this;
    }

    /**
     * The [] operator , this one enable writing .
     * @param key
     * @return value of the key .
     */
    valueT &operator[](const keyT key)
    {
        if (!containsKey(key))
        {
            insert(key, valueT());
        }
        int hashIndex = hashFunc(key);
        for (typename std::vector<std::pair<keyT, valueT>>::iterator it = hashTable[hashIndex].begin();
             it != hashTable[hashIndex].end(); ++it)
        {
            if (it->first == key)
            {
                return it->second;
            }
        }
        return hashTable[0][0].second;
    }

    /**
    * The [] operator , this one for reading .
    * @param key
    * @return value of the key .
    */
    const valueT &operator[](const keyT key) const
    {
        if (containsKey(key))
        {
            int hashIndex = hashFunc(key);
            for (typename std::vector<std::pair<keyT, valueT>>::iterator it = hashTable[hashIndex].begin();
                 it != hashTable[hashIndex].end(); ++it)
            {
                if (it->first == key)
                {

                    return it->second;
                }
            }
        } else
        {
            // undefined behavior.
            return valueT();
        }
    }

    /**
     * The == operator for comparing between two HashMaps .
     * @param map1
     * @param map2
     * @return true if they are equal , false otherwise.
     */
    friend bool operator==(HashMap<keyT, valueT> const &map1, HashMap<keyT, valueT> const &map2)
    {
        if (map1.size() != map2.size() || map1.capacity() != map2.capacity() || map1.upperfacor != map2.upperfacor
            || map1.lowerfactor != map2.lowerfactor)
        {
            return false;
        }
        std::set<std::pair<keyT, valueT>> set1;
        std::set<std::pair<keyT, valueT>> set2;
        for (int i = 0; i < map1.capacity(); ++i)
        {
            for (int j = 0; j < map1.hashTable[i].size(); ++j)
            {
                set1.insert(map1.hashTable.at(i).at(j));
            }
            for (int j = 0; j < map2.hashTable[i].size(); ++j)
            {
                set2.insert(map2.hashTable.at(i).at(j));
            }
        }
        return set1 == set2;
    }

    /**
    * The != operator for comparing between two HashMaps .
    * @param map1
    * @param map2
    * @return true if they are not equal , false otherwise.
    */
    friend bool operator!=(HashMap<keyT, valueT> const &map1, HashMap<keyT, valueT> const &map2)
    {
        return !(map1 == map2);
    }

private:
    /**
     * data members for the HashMap.
     */
    std::vector<std::vector<std::pair<keyT, valueT>>> hashTable;
    int hashcapacity;
    int hashsize;
    double upperfacor;
    double lowerfactor;
};

