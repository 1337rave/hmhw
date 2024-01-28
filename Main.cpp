#include <iostream>

template<typename T>
class UniquePtr {
private:
    T* ptr;

public:
    UniquePtr(T* p) : ptr(p) {}
    ~UniquePtr() { delete ptr; }

    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr; }

    T* operator->() const { return ptr; }

    explicit operator bool() const { return ptr != nullptr; }
};

template<typename T>
class SharedPtr {
private:
    T* ptr;
    size_t* refCount;

public:
    SharedPtr(T* p) : ptr(p), refCount(new size_t(1)) {}
    ~SharedPtr() {
        if (--(*refCount) == 0) {
            delete ptr;
            delete refCount;
        }
    }

    SharedPtr(const SharedPtr& other) : ptr(other.ptr), refCount(other.refCount) {
        ++(*refCount);
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            if (--(*refCount) == 0) {
                delete ptr;
                delete refCount;
            }
            ptr = other.ptr;
            refCount = other.refCount;
            ++(*refCount);
        }
        return *this;
    }

    T& operator*() const { return *ptr; }

    T* operator->() const { return ptr; }

    explicit operator bool() const { return ptr != nullptr; }
};


int main() {
    UniquePtr<int> uniquePtr(new int(42));
    std::cout << *uniquePtr << std::endl;

    SharedPtr<int> sharedPtr1(new int(100));
    SharedPtr<int> sharedPtr2 = sharedPtr1;
    std::cout << *sharedPtr1 << std::endl;
    std::cout << *sharedPtr2 << std::endl;

    return 0;
}
