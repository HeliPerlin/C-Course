#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <iostream>
#include <istream>
#include <ostream>
#include <vector>
#include <iterator>
#include <utility>

#define INITIAL_CAP 16UL
#define LOWER_FACTOR 0.25
#define UPPER_FACTOR 0.75
#define INCREASE 1
#define DECREASE 0
#define MIN_CAP 1


/**
 * A data structure which holds vectors containing pairs, as each pair holds a key of type KeyT
 * and a related value of type ValueT
 * @tparam KeyT the type of the key
 * @tparam ValueT the type of the value
 */
template<class KeyT, class ValueT>
class HashMap
{

private:

    size_t _capacity;
    size_t _size;
    std::vector<std::pair<KeyT, ValueT>> * _data;

    /**
     * Increases or decreases the hashmap when the upper/ lower factor is reached and updates the
     * the indices of each key and its' value accordingly
     * @param sizeChange indicates what kind of resizing is needed
     */
    void _rehash(int sizeChange)
    {
        size_t tempCapacity = _capacity;
        if (sizeChange == INCREASE)
        {
            _capacity *= 2;
        }
        else
        {
            _capacity /= 2;
        }
        auto tempData = new(std::nothrow) std::vector<std::pair<KeyT, ValueT>>[_capacity];
        if (tempData == nullptr)
        {
            delete[] this->_data;
            throw std::exception();
        }

        for (size_t i = 0; i < tempCapacity; ++ i)
        {
            for (size_t j = 0; j < _data[i].size(); ++ j)
            {
                size_t cell = _findIndex((_data[i][j]).first);
                KeyT key = (_data[i][j]).first;
                ValueT value = (_data[i][j]).second;
                try
                {
                    tempData[cell].push_back(std::pair<KeyT, ValueT>(key, value));
                }
                catch (std::exception &e)
                {
                    delete [] tempData;
                    delete [] this->_data;
                    throw std::exception();
                }
            }
        }
        delete[] this->_data;
        this->_data = tempData;
    }


    /**
     * calculates the index of a given key
     * @param key the key to calculate the index of
     * @return an index in the hashmap
     */
    size_t _findIndex(const KeyT key) const
    {
        std::hash<KeyT> hash_func;
        size_t x = hash_func(key);
        size_t cell = x & (_capacity - 1);
        return cell;
    }


    /**
     * an iterator of the hashmap. Iterates through pairs of key and its' value
     */
    class ConstIterator
    {
    private:
        HashMap<KeyT, ValueT> _hashMap;
        std::pair<KeyT, ValueT>* _cur;
        size_t _vecCounter;
        size_t _innerCounter;

    public:
        /**
         * iterator traits
         */
        typedef std::pair<KeyT, ValueT> value_type;
        typedef std::pair<KeyT, ValueT> &reference;
        typedef std::pair<KeyT, ValueT> *pointer;
        typedef int difference_type;
        typedef std::forward_iterator_tag iterator_category;

        /**
         * constructs an iterator to the hashmap given as an argument
         * @param hashMap the hashmap to iterate over
         */
        ConstIterator(const HashMap<KeyT, ValueT>& hashMap)
        {
            _hashMap = hashMap;
            size_t i = 0;
            for (; i < hashMap._capacity; ++ i)
            {
                if (_hashMap._data[i].size() > 0)
                {
                    _vecCounter = i;
                    _innerCounter = 0;
                    break;
                }
            }
            if(i == _hashMap._capacity)
            {
                this->_cur = nullptr;
                _innerCounter = 0;
                _vecCounter = _hashMap._capacity;

            }
            else
            {
                this->_cur = &(_hashMap._data[_vecCounter][_innerCounter]);
            }
        }

        /**
         * default constructor
         */
        ConstIterator(): _innerCounter(0), _vecCounter(0), _cur(nullptr)
        {
            _hashMap = HashMap<KeyT, ValueT>();
        }

        /**
         * constructs an iterator starting at "pair" in the hashmap given as an argument
         */
        ConstIterator(const HashMap<KeyT, ValueT>& hashMap , std::pair<KeyT, ValueT>* pair) :
        _hashMap(hashMap), _cur(pair), _vecCounter(hashMap._capacity), _innerCounter(0) {}

        /**
         * copy constructor
         * @param other the iterator to copy from
         */
        ConstIterator(const ConstIterator &other): _hashMap(other._hashMap), _vecCounter(other
        ._vecCounter), _innerCounter(other._innerCounter)
        {
            _cur = &(_hashMap._data[_vecCounter][_innerCounter]);
        }

        /**
         * gives the iterator the traits of other
         */
        ConstIterator& operator=(const ConstIterator &other)
        {
            if (this == &other)
            {
                return *this;
            }
            this->_hashMap = other._hashMap;
            if (this->_cur != nullptr)
            {
                *(this->_cur) = *(other._cur);
            }
            else
            {
                this->_cur = &(this->_hashMap._data[other._vecCounter][other._innerCounter]);
            }
            this->_innerCounter = other._innerCounter;
            this->_vecCounter = other._vecCounter;
            return *this;
        }

        /**
         * dereferences the iterator
         * @return the current content of the iterator
         */
        std::pair<KeyT, ValueT> operator*() const
        {
            return *(this->_cur);
        }

        /**
         * provides a pointer to the iterator's content
         * @return a pointer to the current content of the iterator
         */
        std::pair<KeyT, ValueT>* operator->() const
        {
            return this->_cur;
        }

        /**
         * moves the iterator forward
         * @return the iterator before the change
         */
        ConstIterator operator++(int) // map++
        {
            ConstIterator temp = *this;
            if (_innerCounter + 1 < _hashMap._data[_vecCounter].size())
            {
                _innerCounter++;
                this->_cur = &(_hashMap._data[_vecCounter][_innerCounter]);
            }
            else
            {
                size_t i = _vecCounter + 1;
                for (; i < _hashMap._capacity; ++i)
                {
                    if (_hashMap._data[i].size() > 0)
                    {
                        _vecCounter = i;
                        _innerCounter = 0;
                        break;
                    }
                }
                if (i == _hashMap._capacity)
                {
                    this->_cur = nullptr; // if the for loop reached the end, we define the end()
                            // to be a nullptr
                    _innerCounter = 0;
                    _vecCounter = _hashMap._capacity;
                }
                else
                {
                    this->_cur = & (_hashMap._data[_vecCounter][_innerCounter]);
                }
            }
            return temp;
        }

        /**
         * moves the iterator forward
         * @return the iterator after the change
         */
        ConstIterator &operator++()
        {
            if (_innerCounter + 1 < _hashMap._data[_vecCounter].size())
            {
                _innerCounter ++;
                this->_cur = & (_hashMap._data[_vecCounter][_innerCounter]);
            }
            else
            {
                size_t i = _vecCounter + 1;
                for (; i < _hashMap._capacity; ++ i)
                {
                    if (_hashMap._data[i].size() > 0)
                    {
                        _vecCounter = i;
                        _innerCounter = 0;
                        break;
                    }
                }

                if (i == _hashMap._capacity)
                {
                    this->_cur = nullptr; // if the for loop reached the end, we define the end()
                    // to be a nullptr
                    _innerCounter = 0;
                    _vecCounter = _hashMap._capacity;
                }
                else
                {
                    this->_cur = & (_hashMap._data[_vecCounter][_innerCounter]);
                }
            }
            return *this;
        }

        /**
         * compares two iterators by their placement and their hashmap
         * @param other the iterator to compare to
         * @return true if the iterators are equal, false otherwise
         */
        bool operator==(const ConstIterator &other)
        {
            if (this->_cur == nullptr)
            {
                return (other._cur == nullptr);
            }
            else if (other._cur == nullptr)
            {
                return (this->_cur == nullptr);
            }
            return (this->_hashMap == other._hashMap && this->_innerCounter == other._innerCounter
            && this->_vecCounter == other._vecCounter);
        }

        /**
         * compares two iterators by their placement and their hashmap
         * @param other the iterator to compare to
         * @return true if the iterators are unequal, false otherwise
         */
        bool operator!=(const ConstIterator &other)
        {
            return !(this->operator==(other));
        }
    };

public:
    typedef ConstIterator iterator;
    typedef ConstIterator const_iterator;

    /**
     * initializes a hashmap
     */
    HashMap() : _capacity(INITIAL_CAP), _size(0)
    {
        _data = new(std::nothrow) std::vector<std::pair<KeyT, ValueT>>[_capacity];
        if (_data == nullptr)
        {
            throw std::exception();
        }
    }

    /**
     * initializes a hashmap by iterating over two containers and giving each key from the lhs
     * container a value from the rhs container
     * @tparam KeysInputIterator the iterator type of the keyBegin and keyEnd
     * @tparam ValuesInputIterator the iterator type of the valueBegin and ValueEnd
     * @param keysBegin begin iterator to the lhs container
     * @param keyEnd end iterator to the lhs container
     * @param valuesBegin begin iterator to the rhs container
     * @param valuesEnd end iterator to the rhs container
     */
    template<typename KeysInputIterator, typename ValuesInputIterator>
    HashMap(const KeysInputIterator keysBegin, const KeysInputIterator keyEnd, const
            ValuesInputIterator valuesBegin, const ValuesInputIterator valuesEnd)
    {
        _capacity = INITIAL_CAP;
        _size = 0;
        _data = new(std::nothrow) std::vector<std::pair<KeyT, ValueT>>[_capacity];
        if (_data == nullptr)
        {
            throw std::exception();
        }
        auto itVal = valuesBegin;
        for (auto itKey = keysBegin; itKey != keyEnd; itKey++)
        {
            KeyT key = *itKey;
            ValueT value = *itVal;
            size_t cell = _findIndex(key);
            if (contains_key(key))
            {
                this->operator[](key) = value;
            }
            else
            {
                try
                {
                    _data[cell].push_back(std::pair<KeyT, ValueT>(key, value));
                }
                catch (std::exception &e)
                {
                    delete [] this->_data;
                    throw std::exception();
                }
            }
            _size ++;
            if(load_factor() > UPPER_FACTOR)
            {
                this->_rehash(INCREASE);
            }
            itVal ++;
        }
        if (itVal != valuesEnd)
        {
            delete [] this->_data;
            throw std::exception();
        }
    }

    /**
     * a copy constructor
     * @param other the hashmap to copy
     */
    HashMap(const HashMap<KeyT, ValueT> &other) // Copy Constructor
    {
        _capacity = other._capacity;
        _size = other._size;
        _data = new(std::nothrow) std::vector<std::pair<KeyT, ValueT>>[_capacity];
        if (_data == nullptr)
        {
            throw std::exception();
        }
        for (size_t i = 0; i < _capacity; ++ i)
        {
            for (size_t j = 0; j < other._data[i].size(); ++ j)
            {
                try
                {
                    this->_data[i].push_back(other._data[i][j]);
                }
                catch (std::exception &e)
                {
                    delete [] this->_data;
                    throw std::exception();
                }
            }
        }
    }


    /**
     * destructor
     */
    ~HashMap()
    {
        delete [] this->_data;
    }

    /**
     * returns the size of the hashmap
     * @return the size of *this
     */
    size_t size() const
    {
        return _size;
    }

    /**
     * returns the capacity of the hashmap
     * @return the capacity of *this
     */
    size_t capacity() const
    {
        return _capacity;
    }

    /**
     * checks if a hashmap's size is 0
     * @return true if the size is 0, false otherwise
     */
    bool empty() const
    {
        return (_size == 0);
    }

    /**
     * calculate the load factor ot *this
     * @return the load factor ot *this
     */
    double load_factor() const
    {
        return (double )_size / (double) _capacity;
    }

    /**
     * inserts a new pair of key and value to the hashmap
     * @param key the key variable
     * @param value the value variable
     * @return true if the pair was inserted succesfully, false otherwise
     */
    bool insert(const KeyT key, const ValueT value) noexcept(false)
    {
        if (contains_key(key))
        {
            return false;
        }
        size_t cell = _findIndex(key);
        try
        {
            _data[cell].push_back(std::pair<KeyT, ValueT>(key, value));
            _size++;
        }
        catch (std::exception &e)
        {
            throw std::exception();
        }
        if (load_factor() > UPPER_FACTOR)
        {
            this->_rehash(INCREASE);
        }
        return true;
    }

    /**
     * checks if *this contains a given key
     * @param key the key to look up for
     * @return true if the key is inside the hashmap, false otherwise
     */
    bool contains_key(KeyT key) const
    {
        size_t cell = _findIndex(key);
        if (cell >= _capacity || _data[cell].empty())
        {
            return false;
        }
        for (auto it = _data[cell].begin(); it != _data[cell].end(); it++)
        {
            if (it->first == key)
            {
                return true;
            }
        }
        return false;
    }

    /**
     * returns the value of a given key, if exists
     * @param key the key to return its' value
     * @return the matching value of key
     */
    ValueT at(KeyT key) const noexcept(false)
    {
        if (!contains_key(key))
        {
            throw std::exception();
        }
        size_t cell = _findIndex(key);
        for (auto it = _data[cell].begin(); it != _data[cell].end(); it ++)
        {
            if(it->first == key)
            {
                return it->second;
            }
        }
        throw std::exception();
    }

    /**
     * returns the value of a given key, if exists, and allows change.
     * @param key the key to return its' value
     * @return the matching value of key
     */
    ValueT& at(KeyT key) noexcept(false)
    {
        size_t cell = _findIndex(key);
        for (auto it = _data[cell].begin(); it != _data[cell].end(); it ++)
        {
            if(it->first == key)
            {
                return it->second;
            }
        }
        throw std::runtime_error("HashMap<KeyT, ValueT>::at - Unfound key.");
    }

    /**
     * erases a single key and its' value from *this
     * @param key the key to remove
     * @return true if the pair was removed successfully
     */
    bool erase(KeyT key)
    {
        size_t cell = _findIndex(key);
        for (auto it = _data[cell].begin(); it != _data[cell].end(); it++)
        {
            if (it->first == key)
            {
                try
                {
                    _data[cell].erase(it);
                }
                catch (std::exception &e)
                {
                    throw std::exception();
                }
                _size -= 1;
                if (load_factor() < LOWER_FACTOR && _capacity > MIN_CAP)
                {
                    this->_rehash(DECREASE);
                }
                return true;
            }
        }
        return false;
    }

    /**
     * finds the index of the bucket of a given key, if exists in *this
     * @param key
     * @return the index of the bucket of key
     */
    size_t bucket_index(const KeyT key) const noexcept(false)

    {
        size_t cell = _findIndex(key);
        for (auto it = this->_data[cell].begin(); it != this->_data[cell].end(); it++)
        {
            if (it->first == key)
            {
                return cell;
            }
        }
        throw std::exception();
    }

    /**
     * finds the size of the bucket of a given key, if exists in *this
     * @param key
     * @return the size of the bucket of key
     */
    size_t bucket_size(const KeyT key) const noexcept(false)
    {
        if (!contains_key(key))
        {
            throw std::exception();
        }
        return _data[_findIndex(key)].size();
    }

    /**
     * clears the given hashmap
     */
    void clear()
    {
        for (size_t i = 0; i < _capacity; ++ i)
        {
            this->_data[i].clear();
        }
        _size = 0;
    }

    /**
     * gives *this the traits of other
     * @param other the hashmap to give its' traits to *this
     * @return
     */
    HashMap<KeyT, ValueT> &operator=(const HashMap<KeyT, ValueT> &other) noexcept(false)
    {
        if (this == &other)
        {
            return *this;
        }
        _size = other._size;
        _capacity = other._capacity;
        auto tempData = new (std::nothrow)std::vector<std::pair<KeyT, ValueT>>[_capacity];
        if (tempData == nullptr)
        {
            delete[] this->_data;
            throw std::exception();
        }
        for (size_t i = 0; i < _capacity; ++ i)
        {
            for (size_t j = 0; j < other._data[i].size(); ++ j)
            {
                try
                {
                    tempData[i].push_back(other._data[i][j]);
                }
                catch (std::exception &e)
                {
                    delete[] tempData;
                    delete [] this->_data;
                    throw std::exception();
                }
            }
        }
        delete[] this->_data;
        this->_data = tempData;
        return *this;
    }

    /**
     * allows access to the value of key
     * @param key the key to access its' value
     * @return the value of key
     */
    ValueT &operator[](KeyT key) noexcept(false)
    {
        size_t cell = _findIndex(key);
        for (auto it = _data[cell].begin(); it != _data[cell].end(); it ++)
        {
            if(it->first == key)
            {
                return it->second;
            }
        }
        try
        {
            _data[cell].push_back(std::pair<KeyT, ValueT>(key, ValueT()));
            _size++;
            return _data[cell].back().second;
        }
        catch (std::exception &e)
        {
            throw std::exception();
        }

    }

    /**
     * returns to the value of key
     * @param key the key to access its' value
     * @return the value of key
     */
    ValueT operator[](KeyT key) const
    {
        size_t cell = _findIndex(key);
        for (auto it = _data[cell].begin(); it != _data[cell].end(); it ++)
        {
            if(it->first == key)
            {
                return it->second;
            }
        }
        return ValueT();
    }


    /**
     * compares two hashmaps
     * @param lhs the lhs to compare
     * @param rhs the rhs to compare
     * @return true if the hashmaps are equal, false otherwise
     */
    friend bool operator==(const HashMap<KeyT, ValueT> &lhs, const HashMap<KeyT, ValueT> &rhs)
    {
        if (lhs._size != rhs._size || lhs._capacity != rhs._capacity)
        {
            return false;
        }
        for (size_t i = 0; i < lhs._capacity; i++)
        {
            for (size_t j = 0; j < lhs._data[i].size(); ++ j)
            {
                KeyT currKey = lhs._data[i][j].first;
                ValueT currVal = lhs._data[i][j].second;
                // check keys
                if ( !(rhs.contains_key(currKey)) )
                {
                    return false;
                }
                // check values.
                if (rhs.at(currKey) != currVal)
                {
                    return false;
                }
            }
        }
        return true;

    }

    /**
     * compares two hashmaps
     * @param lhs the lhs to compare
     * @param rhs the rhs to compare
     * @return true if the hashmaps are unequal, false otherwise
     */
    friend bool operator!=(const HashMap<KeyT, ValueT> &lhs, const HashMap<KeyT, ValueT> &rhs)
    {
        bool eq = (lhs == rhs);
        return !eq;
    }

    /**
     * returns an iterator to the start of the hashmpap
     * @return an iterator to the start of the hashmpap
     */
    const_iterator cbegin() const
    {
        const_iterator it(*this);
        return it;
    }

    /**
     * returns an iterator to the end of the hashmpap
     * @return an iterator to the end of the hashmpap
     */
    const_iterator cend() const
    {
        const_iterator it(*this, nullptr);
        return it;
    }

    /**
     * returns an iterator to the start of the hashmpap
     * @return an iterator to the start of the hashmpap
     */
    const_iterator begin() const
    {
        const_iterator it(*this);
        return it;
    }

    /**
     * returns an iterator to the end of the hashmpap
     * @return an iterator to the end of the hashmpap
     */
    const_iterator end() const
    {
        const_iterator it(*this, nullptr);
        return it;
    }

};

#endif //HASHMAP_HPP
