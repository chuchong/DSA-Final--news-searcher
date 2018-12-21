template<class T>
struct QueueNode {
public:
	T value;
	QueueNode* next = nullptr;
};

template<class T>
class Queue {
private:
	QueueNode<T> * head = nullptr;
	QueueNode<T> * end = nullptr;
public:
	void pop();
	void push(T c);
	T headValue();
	~Queue();
	bool isEmpty() { return head == nullptr; }
};

template<class T>
inline void  Queue<T>::pop()
{
	QueueNode<T>* sym = head;
	head = head->next;
	delete sym;
}

template<class T>
inline void Queue<T>::push(T c)
{
	QueueNode<T> * sym = new QueueNode<T>;
	sym->value = c;

	if (end == nullptr)
		head = end = sym;
	else {
		end->next = sym;
		end = end->next;
	}
}

template<class T>
inline T Queue<T>::headValue()
{
	return head->value;
}

template<class T>
inline Queue<T>::~Queue()
{
	while (head != nullptr)
		pop();
}