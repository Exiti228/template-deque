#include <iostream>
#include <vector>
#include<algorithm>
using std::vector;
template <typename T>

class vector2diter{
private:
   static const int chunk_size=(512ULL>sizeof(T)? 512ULL: sizeof(T))/sizeof(T);
   typename vector<T*>::iterator chunk_iter;
   T* elem_iter;
public:
   vector2diter(typename vector<T*>::iterator c_it,T* e_it) : chunk_iter(c_it), elem_iter(e_it) {}
   typedef std::random_access_iterator_tag iterator_category;
   typedef T value_type;
   typedef T difference_type;
   typedef T& reference;
   typedef T* pointer;

   vector2diter& operator++() {
      if ((*chunk_iter) + chunk_size - 1 > elem_iter) {
        elem_iter++;
      }
      else {
        chunk_iter++;
        elem_iter = (*chunk_iter);
      }
      return *this;
   }
   vector2diter& operator--() {
      if ((*chunk_iter) < elem_iter)
        elem_iter--;
      else {
        chunk_iter--;
        elem_iter = (*chunk_iter) + chunk_size - 1;
      }
      return *this;
   }


   T& operator*() {
    return *elem_iter;
   }

   bool operator==(const vector2diter<T> &a) {
      return elem_iter == a.elem_iter;
   }
   bool operator!=(const vector2diter<T> &a) {
      return a.elem_iter != elem_iter;
   }

   vector2diter& operator+=(int k) {
      k += elem_iter - *chunk_iter;
      int r = k % chunk_size;
      chunk_iter += k / chunk_size;
      if (r < 0) {
        chunk_iter--;
        r += chunk_size;
      }
      elem_iter = *chunk_iter + r;
      return *this;
   }
   vector2diter<T> operator+( int k) {
      vector2diter<T> other(*this);
      return other+=k;

   }
   int operator-(const vector2diter<T> &a) {
      int aoffs = (a.elem_iter - *a.chunk_iter );
      int boffs = (elem_iter - *chunk_iter);
      int chunks = (chunk_iter - a.chunk_iter);
      return chunks * chunk_size -aoffs + boffs;
   }
   vector2diter<T> operator-(int k) {
      return (*this)+=-k;
   }

   vector2diter& operator-=(int k) {

      return (*this) += -k;
   }

   bool operator<(const vector2diter<T> &a) {
    //std::cout << *elem_iter << " " << *a.elem_iter << std::endl;
    return *elem_iter < *a.elem_iter;
   }

   vector2diter<T> operator=(const vector2diter<T> &a) {

     chunk_iter = a.chunk_iter;
     elem_iter = a.elem_iter;
     return (*this);
   }
};

template <typename T>

class vector2d {
   private:
     static const int chunk_size=(512ULL>sizeof(T)? 512ULL: sizeof(T))/sizeof(T);
     int n;
     vector<T*> chunks;
   public:
     vector2d() : n(0) {
        chunks.push_back(new T[chunk_size]);
    }
    vector2d(int _n, T val = T()) : n(_n) {
        int chunk_count = 1 + n / chunk_size;
        for (int i = 0; i < chunk_count; ++i)
            chunks.push_back(new T[chunk_size]);
        for (int i = 0; i < n; ++i) {
            chunks[i / chunk_size][i % chunk_size] = val;
        }
    }
    ~vector2d() {
        for (T* yk : chunks)
            delete[] yk;

    }
    T& operator[](int i) {
        return chunks[i / chunk_size][i % chunk_size];
    }

    int size() {
        return n;
    }
     void resize(int newsize,const T &val=T()) {
         int new_chunk_count=1+newsize/chunk_size;
         if (n >= newsize) {
            while (chunks.size() > new_chunk_count) {
                delete[] chunks.back();
                chunks.pop_back();
            }
         }
         else {

            while (chunks.size() < new_chunk_count) {
              chunks.push_back(new T[chunk_size]);
            }
            for (int i = n; i < newsize; ++i) {
                chunks[i / chunk_size][i % chunk_size] = val;
            }

         }
         n=newsize;
     }
   void push_back(const T&val) {
      if ( chunk_size * chunks.size() == n + 1) {
        chunks.push_back(new T[chunk_size]);
      }
      chunks[n / chunk_size][n % chunk_size] = val;
      n++;


   }
   void pop_back() {
      if (chunk_size * chunks.size() - chunk_size == n) {
        delete[] chunks.back();
        chunks.pop_back();
      }
      n--;
   }

    vector2diter<T> begin() {
      return vector2diter<T>(chunks.begin(), *chunks.begin());
   }
   vector2diter<T> end() {
      int y = n / chunk_size;
      int x = n % chunk_size;
      return vector2diter<T>(chunks.begin() + y, (*(chunks.begin() + y)) + x);
   }
   void log() {
      std::cout << "size=" << n << std::endl;
      std::cout << "chunks="<< chunks.size() << std::endl;
      for (auto p:chunks)
         std::cout << p << ' ';
      std::cout << std::endl;
   }
};


using namespace std;
int main() {
   vector2d<int> v(100,1);
//   v.log();
//   v.resize(50);
//   v.log();
//   v.resize(128,0.1);
//   v.log();
   int d=0;
   for (auto &x:v)
      x=d-=1;
    sort(v.begin(), v.end() - 10);
    for (auto x : v)
        cout << x << " ";
    cout << endl;


   auto it = lower_bound(v.begin(), v.end() - 10, -50);
   cout<<*it<<endl;
   it+=35;
   cout<<*it<<endl;
   it+=-55;
   cout<<*it<<endl;

}

















