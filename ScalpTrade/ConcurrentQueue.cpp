#include "ConcurrentQueue.h"
#include "Trade.h"
#include "MessageHeader.h"
#include "Order.h"

template <typename T>
ConcurrentQueue<T>::~ConcurrentQueue(){}

template <typename T>
void ConcurrentQueue<T>:: push(T item)
{

		// Acquire lock
		std::unique_lock<std::mutex> lock(m_mutex);

		// Add item
		m_queue.push(item);

		// Notify one thread that
		// is waiting
		m_cond.notify_one();
}

template <typename T>
T ConcurrentQueue<T>:: pop()
{

		// acquire lock
		std::unique_lock<std::mutex> lock(m_mutex);

		// wait until queue is not empty
		m_cond.wait(lock,
			[this]() { return !m_queue.empty(); });

		// retrieve item
		T item = m_queue.front();
		m_queue.pop();

		// return item
		return item;
}

template <typename T>
bool ConcurrentQueue<T>::isEmpty()
{
	return m_queue.empty();
}

template class ConcurrentQueue<Trade>;
template class ConcurrentQueue<MessageHeader*>;
template class ConcurrentQueue<Order>;