#include "DocList.h"



void DocList::Add(int id, int cnt)
{
	if (head == nullptr) {
		PushBack(id, cnt);
		return;
	}

	DocNode *newnode = new DocNode(id, cnt);

	if (cnt >= head->cnt) {
		newnode->next = head;
		head->prior = newnode;
		head = newnode;
		return;
	}

	DocNode * p = head;
	DocNode * q = p->next;
	while (q != nullptr) {
		if (cnt <= p->cnt && cnt > q->cnt) {
			newnode->prior = p;
			newnode->next = q;
			p->next = newnode;
			q->prior = newnode;
			return;
		}
		p = q;
		q = q->next;
	}

	if (cnt <= p->cnt) {
		last = newnode;
		p->next = newnode;
		newnode->prior = p;
		return;
	}

	delete newnode;
	return;
}

int DocList::Search(int id)const
{
	DocNode*node = head;
	while (node != nullptr) {
		if (node->id == id)
			return node->cnt;
		node = node->next;
	}
	return 0;
}

void DocList::Edit(int id, int to_cnt)
{
	Remove(id);
	Add(id, to_cnt);
}

void DocList::Remove(int id)
{
	DocNode * p = head;
	DocNode * q = head;
	if (p == nullptr)
		return;
	else {
		q = p->next;
		if (p->id == id) {// 第一个就是
			head = q;
			q->prior = nullptr;
			delete p;
			return;
		}
		while (q != nullptr) {//第一个不是
			if (q->id == id) {//q位置是
				p->next = q->next;
				if (p->next)
					p->next->prior = p;
				else
					last = p;
				delete q;
				return;
			}
			p = q;
			q = q->next;
		}
	}
}

//按照出现次数排序
void DocList::qSort()
{
	if(head && head->next)
		qSort(head, last);
}

void DocList::qSort(DocNode * left, DocNode * right)
{
	if (head != nullptr && left != right && left != right->next) {
		DocNode *p = Partion(left, right);
		qSort(left, p->prior);
		qSort(p->next, right);
	}
}

DocNode * DocList::Partion(DocNode * left, DocNode * right)
{
	int cnt = right->cnt;
	int id = right->id;


	DocNode *i = left->prior;

	for (DocNode *j = left; j != right; j = j->next) {
		if (j->cnt >= cnt) {
			i = (i == nullptr) ? left : i->next;

			swap(i, j);
		}
	}
	i = (i == nullptr) ? left : i->next;
	swap(i, right);
	return i;
}

void DocList::PushBack(int id, int cnt)
{
	if (head == nullptr) {
		head = last = new DocNode(id, cnt);
	}
	else {
		last->next = new DocNode(id, cnt);
		last->next->prior = last;
		last = last->next;
	}
}

DocList::DocList()
{
}


DocList::~DocList()
{
	clear();
}
