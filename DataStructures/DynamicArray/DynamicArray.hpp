#include<iostream>

template<class T>
class DynamicArray{

private :

	T *data;
	size_t CurSize;
	size_t Capacity;

	bool isSorted;

public:

	//THE BIG FOUR
	DynamicArray() ;
	~DynamicArray();
	DynamicArray(const DynamicArray<T>&);
	DynamicArray<T>& operator=(const DynamicArray<T>&);

	//helpers
private:
	void CopyFrom(const DynamicArray<T>&);
    void Free();
	void Resize(size_t NewSize);

	int BinarySearch(const T&, size_t, size_t )const;
	int LinearSearch(const T&)const;


public:

	//add a new elem in the end
	void PushBack(const T&);
	//removes the last elem
	void PopBack(); //both with amortized O(1) complexity,
					//because memory reallocations


	//new elem on random position
	void InsertAt(size_t,const T&);

	//removes an elem on random position
	void RemoveAt(size_t);

	const T& GetAt(size_t)const;
	void SetAt(size_t,const T&);
	const T& operator[](size_t)const;
	T& operator[](size_t);

	size_t GetSize()const;
	size_t GetCapacity()const;

	//searches for an element and returns it's index if founded  , -1 else
	int Search(const T&)const;

	//sorts the vector using the simple insertion sort algorithm
	void Sort();


	//simple print functions
	void PrintInfo()const;

	void PrintElems()const;

};

template<class T>
DynamicArray<T>::DynamicArray():data(nullptr), CurSize(0), Capacity(0){

	/*...*/
}



template<class T>
DynamicArray<T>::~DynamicArray(){
	Free();
}

template<class T>
void DynamicArray<T>::Free(){
	
	delete[] data;

	data = nullptr;
	CurSize = 0;
	Capacity = 0;

}

template<class T>
void DynamicArray<T>::CopyFrom(const DynamicArray<T> &other){

	Free();

	data = new T[other.CurSize];

	//memcpy(data, other.data, sizeof(T)*other.CurSize);
	//will not do the trick, because it will make a shallow copy of
	//our objects

	for (int i = 0; i < other.CurSize; i++){

		data[i] = other.data[i];
	}

	CurSize = other.CurSize;
	Capacity = other.Capacity;
	isSorted = other.isSorted;
}
template<class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &other):data(nullptr),CurSize(0),Capacity(0){

	CopyFrom(other);

}

template<class T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T> &other){

	if (this != &other){

		CopyFrom(other);

	}

	return *this;

}

template<class T>
void DynamicArray<T>::Resize(size_t NewCap){


	T *temp = data;

	data = new T[NewCap];

	for (int i = 0; i < CurSize; i++){

		data[i] = temp[i];
	}

	Capacity = NewCap;

	delete[] temp;
}

template<class T>
void DynamicArray<T>::PushBack(const T& elem){

	if (CurSize >= Capacity){

		size_t NewCap = (Capacity == 0 ? 2 : Capacity * 2);
		Resize(NewCap);
	}

	data[CurSize++] = elem;

	if (CurSize == 1) {

		isSorted = true;
		return;
	}

	if(data[CurSize - 2] > data[CurSize - 1])
		isSorted = false;

}

template<class T>
void DynamicArray<T>::PopBack() {

	if (CurSize)
		CurSize--;
	else
		throw std::length_error("already empty!");

	if (CurSize * 2 <= Capacity) {

		size_t NewCap = ((CurSize == 0) ? 0 : Capacity / 2);
		Resize(NewCap);
	}

}

template<class T>
void DynamicArray<T>::InsertAt(size_t pos, const T& newElem){
	
	bool SortedBefore = isSorted;

	PushBack(newElem); //will take care of the memory 

	if (pos >= CurSize) //the element should be the last one
		return;

	if (SortedBefore)
		isSorted = true; //maybe our elem will go to its sorted position

	//now we should 'roll' our element back to the wanted position 
	for (size_t i = CurSize - 1; i > pos; i--) {

		if (data[i] > data[i - 1])
			isSorted = false;

		std::swap(data[i], data[i - 1]);
	}

}



template<class T>
void DynamicArray<T>::SetAt(size_t pos, const T& elem){


	if (pos >= CurSize)
		throw std::out_of_range("Can't set a non-existent element");

	data[pos] = elem;

}

template<class T>
void DynamicArray<T>::RemoveAt(size_t pos) {

	if (pos >= CurSize || CurSize == 1) { //just removes the last elem
		PopBack();
		return;
	}

	if (!isSorted) { //faster version O(1), but can be unexpected
		std::swap(data[CurSize - 1], data[pos]);	 //from the class' client
		PopBack();
		return;
	}

	//else ..
	for(size_t i = pos; i < CurSize-1 ;i++)
		std::swap(data[i], data[i+1]);

	PopBack();

}
template<class T>
const T& DynamicArray<T>::GetAt(size_t pos)const{

	if (pos  >= CurSize){

		throw std::out_of_range("out of range!!!");

	}

	return data[pos];
}

template<class T>
const T& DynamicArray<T>::operator[](size_t pos)const{

	GetAt(pos);
}

template<class T>
T& DynamicArray<T>::operator[](size_t pos){

	if (pos >= CurSize){
		throw std::out_of_range("out of range!!!");
	}

	return data[pos];
}

template<class T>
size_t DynamicArray<T>::GetSize()const{

	return CurSize;
}

template<class T>
size_t DynamicArray<T>::GetCapacity()const{

	return Capacity;
}


template<class T>
void DynamicArray<T>::Sort() {

	if (isSorted)
		return;

	//complexity O(n^2)
	for (int i = 1; i < CurSize; i++)
		for (int j = i; j >= 1; j--)
			if (data[j] < data[j - 1])
				std::swap(data[j], data[j - 1]);

	isSorted = true;

}

template<class T>
int DynamicArray<T>::Search(const T& e) const{

	if (isSorted) {

		std::cout << "using binary search ";
		return BinarySearch(e, 0, CurSize); // O(logn)
	}
	else {

		std::cout << "using linear search ";
		return LinearSearch(e);            //O(n)
	}
}

template<class T>
int DynamicArray<T>::BinarySearch(const T& e, size_t L, size_t R)const {

	//there is no such element
	if (L >= R)
		return -1;

	size_t med = (L + R) / 2;

	if (data[med] == e)
		return med;

	if (data[med] > e)
		return BinarySearch(e, L, med - 1);

	if(data[med] < e)
		return BinarySearch(e, med + 1, R);


}

template<class T>
int DynamicArray<T>::LinearSearch(const T& e) const{

	for (int i = 0; i < CurSize; i++)
		if (data[i] == e)
			return i;

	return -1;

}

template<class T>
void DynamicArray<T>::PrintInfo()const{

	std::cout << "obj at: Ox" << this
		<< " buffer starts at: Ox" << data
		<< " length:" << CurSize
		<< " capacity:" << Capacity << std::endl;

}


//T should have operator<< redefined
template<class T>
void DynamicArray<T>::PrintElems()const {

	for (int i = 0; i < CurSize; i++)
		std::cout << data[i] << "/";

	std::cout << std::endl;

}