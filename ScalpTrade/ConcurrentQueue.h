#pragma once
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>


template <typename T>
class ConcurrentQueue {
private:
	std::queue<T> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_cond;

public:
	void push(T item);
	T pop();
	bool isEmpty();
	ConcurrentQueue() = default; 
	~ConcurrentQueue();
};

