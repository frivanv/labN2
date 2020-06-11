#include <iostream>
#include <stdexcept>
#include <exception>
#include <complex>

using namespace std;

constexpr auto IndexOutOfRange = "Index out of range";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class Sequence {
private:
    int size = 0;

public:
    Sequence() = default;
    virtual ~Sequence() {};

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(int index) const = 0;
    virtual int GetLength() const = 0;
    virtual T& operator [] (int index) const = 0;
    virtual void Append(T item) = 0;
    virtual void Prepend(T item) = 0;
    virtual void InsertAt(T item, int index) = 0;
    virtual void Print() = 0;
    virtual void Enter() = 0;
    virtual Sequence<T>* Concat(Sequence<T>* list) const = 0;
    virtual Sequence<T>* GetSubSequence(int startIndex, int endIndex) const = 0;

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class LinkedList {

private:
    struct Node
    {
        T item;
        Node* next;
    };

    Node* head = nullptr;
    int size = 0;

public:
    LinkedList() : size(0), head(nullptr) {};

    LinkedList(const T* item, const int size) : LinkedList()
    {
        if (size < 0) throw length_error("Index is negative");
        if (size == 0) throw length_error("Node is empty");

        Node* tail = new Node{*item, nullptr};
        head = tail;

        for (size_t i = 1; i < size; i++) {
            Node* body = new Node{*(item + i), nullptr};
            tail->next = body;
            tail = body;
        }

        this->size = size;
    }

    LinkedList(LinkedList <T> const &list) {
        Node* body{ head };
        Node* List{body};

        for (size_t i = 1; i < size; i++, body = body->next) {
            List = new Node{body->item, List};
        }

        this->size = size;
    }

    ~LinkedList() {
        while (this->head != nullptr) {
            Node* body{head->next};
            delete[] head;
            this->head = body;
        }

        delete[] this->head;
    }

    T GetFirst() {
        if (size == 0) throw length_error(IndexOutOfRange);

        return head->item;
    }

    T GetLast() {
        if (size == 0) throw length_error(IndexOutOfRange);
        Node* body{head};

        for (size_t i = 0; i < size - 1; i++) body = body->next;

        return body->item;
    }

    T Get(int index) {
        if (index < 0 || index >= size) throw length_error(IndexOutOfRange);
        Node* body{head};

        for (size_t i = 0; i < index; i++) body = body->next;
        return body->item;
    }

    T& operator [] (int index) {
        if (index < 0 || index >= size) throw length_error(IndexOutOfRange);
        Node* body{head};

        for (size_t i = 0; i < index; i++) body = body->next;

        return body->item;
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex) {
        if (startIndex < 0 || endIndex < 0 || startIndex >= size || endIndex >= size || startIndex > endIndex) throw length_error(IndexOutOfRange);
        Node* body{head};

        for (size_t i = 0; i < startIndex; i++) body = body->next;

        LinkedList<T>* newNode = new LinkedList<T>;

        for (size_t i = static_cast<size_t>(startIndex); i < endIndex + 1; i++) {
            newNode->Prepend(body->item);
            body = body->next;
        }

        newNode->size = endIndex - startIndex + 1;

        return newNode;
    }

    int GetLength() {
        return size;
    }

    void Append(T data) {
        head = new Node{data, head};

        ++size;
    }

    void Prepend(T data) {
        if (head == nullptr)  head = new Node{data, head};
        else {
            Node* body{head};
            for (size_t i = 1; i < size; i++) body = body->next;
            body->next = new Node{data, nullptr};
        }

        ++size;
    }

    void InsertAt(T data, int index) {
        if (index < 0 || index >= size) throw length_error(IndexOutOfRange);
        Node* body{head};

        for (size_t i = 0; i < index - 1; i++) body = body->next;

        Node* newNode = new Node{data, body->next};
        body->next = newNode;

        this->size += 1;
    }

    LinkedList<T>* Concat(LinkedList<T>* list) {
        Node* body{this->head};
        LinkedList<T>* newList = new LinkedList<T>();

        for (int i = 0; i < this->size; i++) {
            newList->Prepend(body->item);
            body = body->next;
        }

        Node* body2{list->head};
        for (int i = this->size; i < (list->size + this->size); i++) {
            newList->Prepend(body2->item);
            body2 = body2->next;
        }

        newList->size = this->size + list->size;

        return newList;
    }

    void Print() {
        for (int i = 0; i < size; i++) {
            cout << Get(i) << " ";
        }
        cout << endl;
    }

    void EnterList() {
        cout << "Please, type List size: ";
        int count;
        cin >> count;

        for (int i = 0; i < count; i++) {
            T data;
            cout << "Element " << i << " - ";
            cin >> data;
            Prepend(data);
        }

    }
};

int Test_Linked_list() {
    int* a{new int[6] {1,4,5,7,4,17}};
    int* b{new int[5] {5,11,8,9,2}};

    LinkedList<int> list1{a, 6};
    LinkedList<int> list2{b, 5};

    if (list1.GetFirst() == 1) cout << "GetFirst succeed" << endl;
    else cout << "GetFirst failed" << endl;

    if (list1.GetLast() == 17) cout << "GetLast succeed" << endl;
    else cout << "GetLast failed" << endl;

    if (list1[2] == 5 && list1[5] == 17) cout << "Operator [] succeed" << endl;
    else cout << "Operator [] failed" << endl;

    if (list1.GetLength() == 6) cout << "GetLength succeed" << endl;
    else cout << "GetLength failed" << endl;

    if (list1.Get(3) == 7) cout << "Get succeed" << endl;
    else cout << "Get failed" << endl;

    list1.Append(5);
    if (list1[0] == 5) cout << "Append succeed" << endl;
    else cout << "Append failed" << endl;

    list2.Prepend(9);
    if (list2[5] == 9) cout << "Prepend succeed" << endl;
    else cout << "Prepend failed" << endl;

    LinkedList<int>* p = list2.GetSubList(1, 4);
    if (p->Get(0) == 11 && p->Get(1) == 8 && p->Get(2) == 9 && p->Get(3) == 2) cout << "GetSubList succeed" << endl;
    else cout << "GetSubList failed" << endl;

    list2.InsertAt(3, 2);
    if (list2[2] == 3) cout << "InsertAt succeed" << endl;
    else cout << "InsertAt failed" << endl;

    LinkedList<int>* p2 = list1.Concat(&list2);
    if (p2->Get(0) == 5 && p2->Get(1) == 1 && p2->Get(2) == 4 && p2->Get(3) == 5
        && p2->Get(4) == 7 && p2->Get(5) == 4 && p2->Get(6) == 17 && p2->Get(7) == 5
        && p2->Get(8) == 11 && p2->Get(9) == 3 && p2->Get(10) == 8 && p2->Get(11) == 9
        && p2->Get(12) == 2 && p2->Get(13) == 9) cout << "Concat succeed" << endl;
    else cout << "Concat failed" << endl;

    delete[] a;
    delete[] b;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class LinkedListSequence : public Sequence<T> {
protected:
    LinkedList<T>* linked_list;

public:

    LinkedListSequence() : Sequence<T>() {
        linked_list = new LinkedList<T>();
    }

    LinkedListSequence(T* item, int size) : Sequence<T>() {
        linked_list = new LinkedList<T>(item, size);
    }

    virtual ~LinkedListSequence() = default;

    virtual T GetFirst() const override {
        if (linked_list->GetLength() == 0) throw out_of_range(IndexOutOfRange);

        return linked_list->GetFirst();
    }

    virtual T GetLast() const override {
        if (linked_list->GetLength() == 0) throw out_of_range(IndexOutOfRange);

        return linked_list->GetLast();
    }

    virtual int GetLength()const   {
        return linked_list->GetLength();
    }

    virtual T Get(int index) const override {
        if (index < 0 || index >= linked_list->GetLength()) throw out_of_range(IndexOutOfRange);

        return linked_list->Get(index);
    }

    virtual T& operator [] (int index) const {
        return  linked_list->operator [] (index);
    }

    virtual Sequence<T>* GetSubSequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex < 0 || startIndex >= linked_list->GetLength() || endIndex >= linked_list->GetLength()) throw out_of_range(IndexOutOfRange);

        LinkedList<T>* subList = linked_list->GetSubList(startIndex, endIndex);
        Sequence<T>* subSequence = new LinkedListSequence<T>();

        for (int i = 0; i < subList->GetLength(); i++)
            subSequence->Prepend(subList->Get(i));

        return subSequence;
    }

    virtual LinkedListSequence<T>* Concat(Sequence<T>* linked_list2) const override{
        LinkedListSequence<T>* ConcatSequence = new LinkedListSequence<T>();

        for (int i = 0; i < linked_list->GetLength(); i++)
            ConcatSequence->Prepend(linked_list->Get(i));

        for (int i = 0; i < linked_list2->GetLength(); i++)
            ConcatSequence->Prepend(linked_list2->Get(i));

        return ConcatSequence;
    }

    virtual void Append(T item) override {
        return linked_list->Append(item);
    }

    virtual void Prepend(T item) override {
        return linked_list->Prepend(item);
    }

    virtual void InsertAt(T item, int index)  {
        if (index < 0 || index >= linked_list->GetLength()) throw out_of_range(IndexOutOfRange);

        return linked_list->InsertAt(item, index);
    }

    virtual void Print() override {
        return linked_list->Print();
    }

    virtual void Enter() override {
        return linked_list->EnterList();
    }

};

int Test_List_Sequence() {
    int* a{new int[6] {1,4,5,7,4,17}};
    LinkedListSequence<int> list1{a, 6};

    int* b{new int[5] {5,11,8,9,2}};
    LinkedListSequence<int> list2{b, 5};

    if (list1.GetFirst() == 1) cout << "GetFirst succeed" << endl;
    else cout << "GetFirst failed" << endl;

    if (list1.GetLast() == 17) cout << "GetLast succeed" << endl;
    else cout << "GetLast failed" << endl;

    if (list1[2] == 5 && list1[5] == 17) cout << "Operator [] succeed" << endl;
    else cout << "Operator [] failed" << endl;

    if (list1.GetLength() == 6) cout << "GetLength succeed" << endl;
    else cout << "GetLength failed" << endl;

    if (list1.Get(3) == 7) cout << "Get succeed" << endl;
    else cout << "Get failed" << endl;

    list1.Append(5);
    if (list1.Get(0) == 5) cout << "Append succeed" << endl;
    else cout << "Append failed" << endl;

    list2.Prepend(9);
    if (list2.Get(5) == 9) cout << "Prepend succeed" << endl;
    else cout << "Prepend failed" << endl;

    Sequence<int>* p = list2.GetSubSequence(1, 4);
    if (p->Get(0) == 11 && p->Get(1) == 8 && p->Get(2) == 9 && p->Get(3) == 2) cout << "GetSubList succeed" << endl;
    else cout << "GetSubList failed" << endl;

    list2.InsertAt(3, 2);
    if (list2.Get(2) == 3) cout << "InsertAt succeed" << endl;
    else cout << "InsertAt failed" << endl;

    LinkedListSequence<int>* p2 = list1.Concat(&list2);
    if (p2->Get(0) == 5 && p2->Get(1) == 1 && p2->Get(2) == 4 && p2->Get(3) == 5
        && p2->Get(4) == 7 && p2->Get(5) == 4 && p2->Get(6) == 17 && p2->Get(7) == 5
        && p2->Get(8) == 11 && p2->Get(9) == 3 && p2->Get(10) == 8 && p2->Get(11) == 9
        && p2->Get(12) == 2 && p2->Get(13) == 9) cout << "Concat succeed" << endl;
    else cout << "Concat failed" << endl;

    delete[] a;
    delete[] b;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
template <class T>
class DynamicArray {
private:
    T* arr = nullptr;
    int size = 0;

public:
    DynamicArray(const int size) {
        this->size = size;
        this->arr = new T[size];
    }

    DynamicArray(const T* arr, const int size) {
        this->size = size;
        this->arr = new T[size];

        if (size < 0) throw length_error("Index is negative");

        for (int i = 0; i < size; i++) this->arr[i] = arr[i];
    }

    DynamicArray(DynamicArray<T> const& dynamic_array) : DynamicArray(T* dynamic_array->arr, dynamic_array->size)
    {}

    DynamicArray<T>& operator = (DynamicArray<T>& dynamic_array) {
        T* new_arr = new T[dynamic_array.GetSize()];
        for (int i = 0; i < dynamic_array.size; i++) new_arr[i] = dynamic_array.arr[i];

        this->size = dynamic_array.size;
        this->arr = new_arr;

        return (*this);
    }

    ~DynamicArray() {
        delete[] arr;
        size = 0;
    }

    T& operator[] (int index) {
        if (index < 0 || index >= this->size) throw out_of_range(IndexOutOfRange);

        return arr[index];
    }

    T Get(int index) {
        if (index < 0 || index >= size) throw out_of_range(IndexOutOfRange);

        return arr[index];
    }

    int GetLength() const {
        return size;
    }

    void Set(int index, T item) {
        if (index < 0 || index >= size) throw out_of_range(IndexOutOfRange);

        arr[index] = item;
    }

    void Resize(int new_size) {
        if (new_size < 0) throw length_error("Index is negative");

        int size2 = new_size < size ? new_size : size;
        T* new_arr = new T[new_size];

        size = new_size;
        for (int i = 0; i < size2; i++) new_arr[i] = arr[i];
        delete[] arr;
        arr = new_arr;
    }

    void Print() {
        for (int i = 0; i < size; i++)
            cout << arr[i]<<" ";
        cout << endl;
    }

    void EnterMas() {
        cout << "Please, type Array size: ";
        int count;
        cin >> count;
        Resize(count);

        for (int i = 0; i < count; i++) {
            int data;
            cout << "Element " << i << " - ";
            cin >> data;
            Set(i, data);
        }
    }
};

int Test_Dynamic_Array() {
    int* pr{new int[5] {11,12,13,14,15}};
    DynamicArray<int> arr1(pr, 5);

    if (arr1[1] == 12 && arr1[4] == 15) cout << "Operator [] succeed" << endl;
    else cout << "Operator [] failed" << endl;

    if (arr1.GetLength() == 5) cout << "GetSize succeed" << endl;
    else cout << "GetSize failed" << endl;

    if (arr1.Get(3) == 14) cout << "Get succeed" << endl;
    else cout << "Get failed" << endl;

    arr1.Resize(7);
    if (arr1.GetLength() == 7) cout << "Resize succeed" << endl;
    else cout << "Resize failed" << endl;

    int data = 44;

    arr1.Set(5, data);
    if (arr1[5] == 44) cout << "Set succeed" << endl;
    else cout << "Set failed" << endl;

    DynamicArray<int> arr2(2);
    if (arr2[0] == -842156789 && arr2[1] == -842156789) cout << "Creation constructor succeed" << endl;
    else cout << "Creation constructor failed" << endl;

    delete[] pr;

    return 0;
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
template <class T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T>* arr = 0;
    int size = 0;

public:
    ArraySequence() : Sequence<T>() {
        arr = new DynamicArray<T>(0);
        this->size = 0;
    }

    ArraySequence(int size) : Sequence<T>() {
        arr = new DynamicArray<T>(size);
        this->size = size;
    }

    ArraySequence(T* item, int size) : Sequence<T>() {
        arr = new DynamicArray<T>(item, size);
        this->size = size;
    }

    virtual T& operator [] (int index) const {
        return  arr->operator [] (index);
    }

    virtual ~ArraySequence() = default;

    virtual T GetFirst() const override {
        if (this->size == 0) throw out_of_range(IndexOutOfRange);

        return arr->Get(0);
    }

    virtual T Get(int index) const override {
        if (index < 0 || index >= size) throw out_of_range(IndexOutOfRange);

        return arr->Get(index);
    }

    virtual T GetLength() const override {
        return size;
    }

    virtual T GetLast() const override {
        return arr->Get(this->size-1);
    }

    virtual void Prepend(T item) override {
        arr->Resize(size+1);
        size = size + 1;
        arr->Set(size-1, item);
    }

    virtual void Append(T item) override {
        arr->Resize(size + 1);
        T data1 = arr->Get(0);
        T data2 = 0;

        for (int i = 0; i < this->size; i++) {
            data2 = data1;
            data1 = arr->Get(i + 1);
            arr->Set(i + 1, data2);
        }

        arr->Set(0, item);
    }

    virtual void InsertAt(T item, int index) override {
        if (index < 0 || index >= size) throw out_of_range(IndexOutOfRange);

        arr->Set(index, item);
    }

    virtual Sequence<T>* GetSubSequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || startIndex >= size|| endIndex < 0 || endIndex >= size) throw out_of_range(IndexOutOfRange);

        Sequence<T>* GetSubSequence = new ArraySequence();
        for (int i = 0; i < (endIndex - startIndex + 1); i++)
            GetSubSequence->Prepend(arr->Get(startIndex + i));

        return GetSubSequence;
    }

    virtual ArraySequence <T>* Concat(Sequence<T>* arr2) const override {
        ArraySequence <T>* new_arr = new ArraySequence();

        for (int i = 0; i < this->size; i++)
            new_arr->Prepend(arr->Get(i));

        for (int i = 0; i < arr2->GetLength(); i++)
            new_arr->Prepend(arr2->Get(i));

        return new_arr;
    }

    virtual void Print() override {
        arr->Print();
    }

    virtual void Enter() override {
        arr->EnterMas();
    }
};

int Test_Array_Sequence() {
    int* pr{new int[5] {11,12,13,14,15}};
    ArraySequence<int> arr1(pr, 5);

    int* prpr{ new int[3] {23,16,4} };
    ArraySequence<int> arr2(prpr, 3);

    if (arr1[1] == 12 && arr1[4] == 15) cout << "Operator [] succeed" << endl;
    else cout << "Operator [] failed" << endl;

    if (arr1.GetFirst() == 11) cout << "GetFirst succeed" << endl;
    else cout << "GetFirst failed" << endl;

    if (arr1.GetLast() == 15) cout << "GetLast succeed" << endl;
    else cout << "GetLast failed" << endl;

    if (arr1.GetLength() == 5) cout << "GetLength succeed" << endl;
    else cout << "GetLength failed" << endl;

    if (arr1.Get(3) == 14) cout << "Get succeed" << endl;
    else cout << "Get failed" << endl;

    arr1.Append(76);
    if (arr1.GetFirst() == 76) cout << "Append succeed" << endl;
    else cout << "Append failed" << endl;

    arr1.Prepend(8);
    if (arr1.GetLast() == 8) cout << "Prepend succeed" << endl;
    else cout << "Prepend failed" << endl;

    arr1.GetSubSequence(2,5);
    if (arr1[2] == 12 && arr1[3] == 13 && arr1[4] == 14 && arr1[5] == 8) cout << "GetSubSequence succeed" << endl;
    else cout << "GetSubSequence failed" << endl;

    int data = 44;

    arr1.InsertAt(data, 4);
    if (arr1[4] == 44) cout << "InsertAt succeed" << endl;
    else cout << "InsertAt failed" << endl;

    ArraySequence<int>* p2 = arr1.Concat(&arr2);
    if (p2->Get(0) == 76 && p2->Get(1) == 11 && p2->Get(2) == 12 && p2->Get(3) == 13
        && p2->Get(4) == 44 && p2->Get(5) == 8 && p2->Get(6) == 23 && p2->Get(7) == 16
        && p2->Get(8) == 4) cout << "Concat succeed" << endl;
    else cout << "Concat failed" << endl;

    DynamicArray<int> arr3(2);
    if (arr3[0] == -842156789 && arr3[1] == -842156789) cout << "Creation constructor succeed" << "\n";
    else cout << "Creation constructor failed" << "\n";

    delete[] pr;
    delete[] prpr;

    return 0;
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class MyClass : public LinkedListSequence<T> {
private:
    LinkedListSequence<T>* list;

public:
    MyClass() {
        list = new LinkedListSequence<T>();
    }

    MyClass(T* item, int size) {
        list = new LinkedListSequence<T>(item, size);
    }

    MyClass(MyClass<T> & list2) {
        list = new LinkedListSequence<T>(list2->GetLength());
        for (int i = 0; i < list->GetLength(); i++)
            list->Append(list2->Get(i));
    }

    ~MyClass() = default;

    T Get(int index) {
        if (index < 0 || index >= list->GetLength()) throw out_of_range(IndexOutOfRange);

        return list->Get(index);
    }

    int GetLength() const {
        return list->GetLength();
    }

    T GetFirst() const {
        if (list->GetLength() == 0) throw out_of_range(IndexOutOfRange);

        return list->GetFirst();
    }

    T GetLast() const {
        if (list->GetLength() == 0) throw out_of_range(IndexOutOfRange);

        return list->GetLast();
    }

    T Get(int index) const {
        if (index < 0 || index >= list->GetLength()) throw out_of_range(IndexOutOfRange);

        return list->Get(index);
    }

    MyClass<T>* Concat(MyClass<T>* list2) {
        MyClass<T>* concatList = new MyClass<T>();
        concatList->list = this->list->Concat(list2->list);

        return concatList;
    }

    MyClass<T>* Map(T(*foo) (T)) {
        MyClass<T>* map_list = new MyClass<T>();
        int size = this->GetLength();

        for (size_t i = 0; i < size; i++) {
            map_list->list->Prepend(foo(list->Get(i)));
        }

        return map_list;
    }

    MyClass<T>* Where(bool (*foo)(T)) {
        int size = this->GetLength();

        MyClass<T>* where_list;
        where_list = new MyClass<T>();

        for (size_t i = 0; i < size; i++) {
            if (foo(Get(i)) == 1)
            {
                where_list->list->Prepend(Get(i));
            }
        }

        return where_list;
    }

    MyClass<T>* GetSubSequence(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex < 0 || startIndex >= list->GetLength() || endIndex >= list->GetLength()) throw out_of_range(IndexOutOfRange);

        Sequence<T>* subList = list->GetSubSequence(startIndex, endIndex);
        MyClass<T>* subSequence = new MyClass<T>();

        for (int i = 0; i < subList->GetLength(); i++)
            subSequence->Prepend(subList->Get(i));

        return subSequence;
    }

    void Append(T item) {
        return list->Append(item);
    }

    void Prepend(T item) {
        return list->Prepend(item);
    }

    int Search(MyClass<T>* list2){
        if (list2->GetLength() < this->GetLength())
            for (int i = 0; i < this->GetLength() - list2->GetLength() + 1; i++) {
                if (this->Get(i) == list2->Get(0)) {
                    if (this->GetLength() - i + 1 < list2->GetLength()) {
                        return -1;
                    }
                    else {
                        int temp = 0;
                        for (int j = 0; j < list2->GetLength(); j++) {
                            if ((this->GetSubSequence(i, list2->GetLength() + i - 1))->Get(j) == list2->Get(j)) {
                                temp++;
                            }
                            if (temp == list2->GetLength()) {
                                return i;
                            }
                        }
                    }
                }
                if (i == this->GetLength() - list2->GetLength()) {
                    return -1;
                }

            }

        return -1;
    }

    void Print() {
        return list->Print();
    }

    void Enter() {
        cout << "Please, type size:";
        int count;
        cin >> count;

        for (int i = 0; i < count; i++) {
            T data;
            cout << "Element" << i << " -";
            cin >> data;
            Prepend(data);
        }

    }

    void InsertAt(T item, int index) {
        if (index < 0 || index >= list->GetLength()) throw out_of_range(IndexOutOfRange);

        return list->InsertAt(item, index);
    }
};



bool test_where(string str) {
    if (str == "one")
    {
        return 1;
    }
    else return 0;
}
string test_map1(string str) {
    str = "cat";
    return str;
}
string test_map2(string str) {
    str = str+str;
    return str;
}

int Test_MyClass() {
    MyClass<string>* map1 = new MyClass<string>();
    MyClass<string>* map2 = new MyClass<string>();
    MyClass<string>* map3 = new MyClass<string>();

    MyClass<string>* list1 = new MyClass<string>();
    list1->Prepend("All");
    list1->Prepend("the");
    list1->Prepend("haters");
    list1->Prepend("love");
    list1->Prepend("me");

    MyClass<string>* list2 = new MyClass<string>();
    list2->Prepend("one");
    list2->Prepend("two");
    list2->Prepend("three");

    MyClass<string>* list3 = new MyClass<string>();
    list3->Prepend("four");
    list3->Prepend("five");
    list3->Prepend("six");

    if (list1->GetFirst() == "All") cout << "GetFirst succeed" << endl;
    else cout << "GetFirst failed" << endl;

    if (list1->GetLast() == "me") cout << "GetLast succeed" << endl;
    else cout << "GetLast failed" << endl;

    if (list1->GetLength() == 5) cout << "GetLength succeed" << endl;
    else cout << "GetLength failed" << endl;

    if (list1->Get(3) == "love") cout << "Get succeed" << endl;
    else cout << "Get failed" << endl;

    list1->Append("OOO");
    if (list1->Get(0) == "OOO") cout << "Append succeed" << endl;
    else cout << "Append failed" << endl;

    list1->Prepend("Oh");
    if (list1->Get(6) == "Oh") cout << "Prepend succeed" << endl;
    else cout << "Prepend failed" << endl;

    list1->InsertAt("Yeah", 3);
    if (list1->Get(3) == "Yeah") cout << "InsertAt succeed" << endl;
    else cout << "InsertAt failed" << endl;

    MyClass<string>* p2 = list2->Concat(list3);
    if (p2->Get(0) =="one" && p2->Get(1) == "two" && p2->Get(2) == "three"
        && p2->Get(3) == "four" && p2->Get(4) == "five"
        && p2->Get(5) == "six") cout << "Concat succeed" << endl;
    else cout << "Concat failed" << endl;

    map1 = list2->Map(test_map1);
    map2 = list3->Map(test_map2);
    if (map1->Get(0) == "cat" && map1->Get(1) == "cat"&& map1->Get(2) == "cat") cout << "Map1 succeed" << endl;
    else cout << "Map1 failed" << endl;

    if (map2->Get(0) == "fourfour" && map2->Get(1) == "fivefive" && map2->Get(2) == "sixsix") cout << "Map2 succeed" << endl;
    else cout << "Map2 failed" << endl;

    map3 = list2->Where(test_where);
    if (map3->Get(0) == "one") cout << "Where succeed" << endl;
    else cout << "Where failed" << endl;

    MyClass<string>* search1;
    search1 = new MyClass<string>();
    search1->Prepend("two");

    if (list2->Search(search1) == 1) cout << "Search succeed" << endl;
    else cout << "Search failed" << endl;

    delete search1;
    delete list1;
    delete list2;
    delete list3;
    delete map1;
    delete map2;
    delete map3;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {

    cout << "What type of data do you want to work with?" << endl;
    cout << "Please, type '1' for complex, '2' for double or '3' for string" << endl;
    int type;

    do {
        cin >> type;
        if ((type != 1) && (type != 2) && (type != 3))
        {
            cout << "You must choose 1 or 2 or 3" << endl;
        }
    } while ((type != 1) && (type != 2) && (type != 3));


    MyClass<complex<double>>* arr1 = new MyClass<complex<double>>();
    MyClass<double>* arr2 = new MyClass<double>();
    MyClass<string>* arr3 = new MyClass<string>();

    MyClass<complex<double>>* con1 = new MyClass<complex<double>>();
    MyClass<double>* con2 = new MyClass<double>();
    MyClass<string>* con3 = new MyClass<string>();

    MyClass<complex<double>>* search1 = new MyClass<complex<double>>();
    MyClass<double>* search2 = new MyClass<double>();
    MyClass<string>* search3 = new MyClass<string>();

    switch (type)
    {
        case 1:
            arr1->Enter();
            break;
        case 2:
            arr2->Enter();
            break;
        case 3:
            arr3->Enter();
            break;
        default: cout << "ERROR! You need choose an action!" << endl;
            break;
    }
    
    int choice = 0;

    cout << "Please, choose the operation:" << endl;
    cout << "1. Print array" << endl;
    cout << "2. Get Length" << endl;
    cout << "3. Concatination" << endl;
    cout << "4. Search" << endl;
    cout << "5. Get" << endl;
    cout << "6. Get First" << endl;
    cout << "7. Get Last" << endl;
    cout << "8. Append" << endl;
    cout << "9. Prepend" << endl;
    cout << "10. InsertAt" << endl;

    cout << endl;
    cout << "Your choice: ";
    cin >> choice;
    cout << endl;

    int ap1(0);
    double ap2(0);
    string ap3("");
    int index(0), startIndex(0), endIndex(0);

    switch (choice) {
       case 1:
            if (type == 1) arr1->Print();
            if (type == 2) arr2->Print();
            if (type == 3) arr3->Print();
            break;
       case 2:
            if (type == 1) {cout << arr1->GetLength() << endl;}
            if (type == 2) {cout << arr2->GetLength() << endl;}
            if (type == 3) {cout << arr3->GetLength() << endl;}
            break;
       case 3:
            cout << "Please, type new list: ";

            if (type == 1) {con1->Enter(); (arr1->Concat(con1))->Print();}
            if (type == 2) {con2->Enter(); (arr2->Concat(con2))->Print();}
            if (type == 3) {con3->Enter(); (arr3->Concat(con3))->Print();}
            break;
       case 4:
            cout << "Please, type piece of list: ";

            if (type == 1) {cin >> ap1; search1->Prepend(ap1); cout << arr1->Search(search1) << endl;}
            if (type == 2) {cin >> ap2; search2->Prepend(ap2); cout << arr2->Search(search2) << endl;}
            if (type == 3) {cin >> ap3; search3->Prepend(ap3); cout << arr3->Search(search3) << endl;}
            break;
       case 5:
            cout << "Please, type index: ";
            cin >> index;

            if (type == 1) {cout << arr1->Get(index) << endl;}
            if (type == 2) {cout << arr2->Get(index) << endl;}
            if (type == 3) {cout << arr3->Get(index) << endl;}
            break;
       case 6:
            if (type == 1) {cout << arr1->GetFirst() << endl;}
            if (type == 2) {cout << arr2->GetFirst() << endl;}
            if (type == 3) {cout << arr3->GetFirst() << endl;}
            break;
       case 7:
            if (type == 1) {cout << arr1->GetLast() << endl;}
            if (type == 2) {cout << arr2->GetLast() << endl;}
            if (type == 3) {cout << arr3->GetLast() << endl;}
            break;
       case 8:
            cout << "Please, type element: ";

            if (type == 1) {cin >> ap1; arr1->Append(ap1);}
            if (type == 2) {cin >> ap2; arr2->Append(ap2);}
            if (type == 3) {cin >> ap3; arr3->Append(ap3);}
            break;
       case 9:
            cout << "Please, type element: ";

            if (type == 1) {cin >> ap1; arr1->Prepend(ap1);}
            if (type == 2) {cin >> ap2; arr2->Prepend(ap2);}
            if (type == 3) {cin >> ap3; arr3->Prepend(ap3);}
            break;
       case 10:
            cout << "Please, type index: ";
            cin >> index;
            cout << "Please, type new element: ";

            if (type == 1) {cin >> ap1; arr1->InsertAt(ap1, index);}
            if (type == 2) {cin >> ap2; arr2->InsertAt(ap2, index);}
            if (type == 3) {cin >> ap3; arr3->InsertAt(ap3, index);}
            break;
       default: cout << "ERROR! You need choose an action!" << endl;
    }

    switch (type)
    {
        case 1:
            delete arr1;
            break;
        case 2:
            delete arr2;
            break;
        case 3:
            delete arr3;
            break;
        default: cout << "ERROR!" << endl;
            break;
    }

    delete con1;
    delete con2;
    delete con3;
    delete search1;
    delete search2;
    delete search3;


    return 0;
}
