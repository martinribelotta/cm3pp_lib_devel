#ifndef MUTEX_H_
#define MUTEX_H_

namespace RTOS {

class Mutex {
private:
	void *handler;

public:
	Mutex();
	~Mutex();

	void lock();
	bool tryLock();
	void unlock();

private:
	Mutex(const Mutex& m) :
			handler(0l) {
	}

	Mutex &operator=(const Mutex& m) {
		return *this;
	}
};

class ScopedLock {
	Mutex * const pMutex;
public:
	ScopedLock(Mutex& m) :
			pMutex(&m) {
		pMutex->lock();
	}

	~ScopedLock() {
		pMutex->unlock();
	}
};

} /* namespace RTOS */
#endif /* MUTEX_H_ */
