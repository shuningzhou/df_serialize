// STL replacement types, to test / ensure STL is not required

#pragma once

#include <string.h>
#include <initializer_list>

template <typename T>
struct MyVector
{
public:
    ~MyVector()
    {
        Clear();
    }

    MyVector()
        : m_array(nullptr)
        , m_size(0)
    {

    }

    MyVector(const MyVector& other)
        : m_array(nullptr)
        , m_size(0)
    {
        resize(other.size());
        for (size_t index = 0; index < m_size; ++index)
            m_array[index] = other[index];
    }

    MyVector(MyVector&& other)
        : m_array(nullptr)
        , m_size(0)
    {
        m_array = other.m_array;
        m_size = other.m_size;

        other.m_array = nullptr;
        other.m_size = 0;
    }

    void operator = (const MyVector& other)
    {
        Clear();
        resize(other.size());
        for (size_t index = 0; index < m_size; ++index)
            m_array[index] = other[index];
    }

    T& operator [](size_t i) { return m_array[i]; }
    const T& operator [](size_t i) const { return m_array[i]; }

public:
    size_t GetSize() const
    {
        return m_size;
    }

    void Clear()
    {
        if (m_array)
        {
            delete[] m_array;
            m_array = nullptr;
        }
        m_size = 0;
    }

    void Resize(size_t n)
    {
        if (n == m_size)
            return;

        T* newArray = new T[n];

        size_t copyCount = (n < m_size) ? n : m_size;
        for (size_t i = 0; i < copyCount; ++i)
            newArray[i] = m_array[i];

        Clear();
        m_array = newArray;
        m_size = n;
    }

    void PushBack(const T& newItem)
    {
        Resize(m_size + 1);
        m_array[m_size - 1] = newItem;
    }

private:
    T* m_array = nullptr;
    size_t m_size = 0;
};

template <typename T, size_t N>
struct MyArray
{
    MyArray()
    {

    }

    MyArray(std::initializer_list<T> il)
    {
        int index = 0;
        for (const T& src : il)
        {
            if (index >= N)
                break;
            m_array[index] = src;
            index++;
        }
    }

    T& operator [](size_t i) { return m_array[i]; }
    const T& operator [](size_t i) const { return m_array[i]; }

private:
    T m_array[N];
};

struct MyString
{
    MyString(MyString&& other)
        : m_string(nullptr)
    {
        m_string = other.m_string;

        other.m_string = nullptr;
    }

    MyString(const MyString& s)
    {
        set(&s[0]);
    }

    MyString(const char* s = nullptr)
    {
        set(s);
    }

    ~MyString()
    {
        Clear();
    }

    void operator = (const MyString& other)
    {
        set(&other[0]);
    }

    void operator = (const char* s)
    {
        set(s);
    }

    bool operator == (const MyString& other) const
    {
        size_t thisLen = Length();
        size_t otherLen = other.Length();

        if (thisLen == 0 && otherLen == 0)
            return true;

        if (thisLen != otherLen)
            return false;

        return strcmp(m_string, &other[0]) == 0;
    }

    bool operator != (const MyString& other) const
    {
        return !(*this == other);
    }

    void operator += (const char* s)
    {
        if (!s)
            return;

        if (!m_string)
        {
            set(s);
            return;
        }

        char* newString = new char[strlen(m_string) + strlen(s) + 1];
        strcpy(newString, m_string);
        strcat(newString, s);
        Clear();
        m_string = newString;
    }

    char& operator [](size_t i) { return m_string[i]; }
    const char& operator [](size_t i) const { return m_string[i]; }

public:

    void Clear()
    {
        if (m_string)
        {
            delete[] m_string;
            m_string = nullptr;
        }
    }

    size_t Length() const
    {
        return m_string ? strlen(m_string) : 0;
    }

    void Resize(size_t newSize)
    {
        if (newSize == 0)
        {
            Clear();
            return;
        }

        char* newString = new char[newSize];
        memset(newString, 0, newSize);
        if (m_string)
        {
            strcpy(newString, m_string);
            delete m_string;
        }
        m_string = newString;
    }

private:
    void set(const char* s)
    {
        Clear();
        if (s)
        {
            m_string = new char[strlen(s) + 1];
            strcpy(m_string, s);
        }
    }

    char* m_string = nullptr;
};