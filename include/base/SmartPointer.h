#ifndef CSW_SMART_PTR_H_
#define CSW_SMART_PTR_H_

#include <assert.h>
#include <cstddef>            // for std::ptrdiff_t

#include <stdint.h>

#include <algorithm>        // for std::swap
#include <functional>       // for std::less

#include "Common/Logger/Log/include/PlatformDefine.h"

namespace Logger
{
    //  scoped_ptr mimics a built-in pointer except that it guarantees deletion
    //  of the object pointed to, either on destruction of the scoped_ptr or via
    //  an explicit reset(). scoped_ptr is a simple solution for simple needs;
    template<class T> class unique_ptr
    {
    private:

        T * px;
        // noncopyable
        unique_ptr(unique_ptr const &);
        unique_ptr & operator=(unique_ptr const &);

        typedef unique_ptr<T> this_type;

        void operator==( unique_ptr const& ) const;
        void operator!=( unique_ptr const& ) const;

    public:

        typedef T element_type;

        explicit unique_ptr( T * p = 0 ): px( p ) // never throws
        {

        }

        ~unique_ptr() // never throws
        {
            delete px;
        }

        void reset(T * p = 0) // never throws
        {
            assert( p == 0 || p != px ); // catch self-reset errors
            //this_type(p).swap(*this);
			px = p;
        }

        T & operator*() const // never throws
        {
            assert( px != 0 );
            return *px;
        }

        T * operator->() const // never throws
        {
            assert( px != 0 );
            return px;
        }

        T * get() const // never throws
        {
            return px;
        }

        void swap(unique_ptr & b) // never throws
        {
            T * tmp = b.px;
            b.px = px;
            px = tmp;
        }
    };

    template<class T> inline void swap(unique_ptr<T> & a, unique_ptr<T> & b) // never throws
    {
        a.swap(b);
    }

    // get_pointer(p) is a generic way to say p.get()
    template<class T> inline T * get_pointer(unique_ptr<T> const & p)
    {
        return p.get();
    }


    //************************shared_ptr**************************//
    template<class T> 
	class shared_ptr
    {
    //private:
    public:
#if defined(WIN32) || defined(WIN64)
        class atomic_count
        {
        public:
            explicit atomic_count( long v ): value_( v )
            {
            }

            long operator++()
            {
                return InterlockedIncrement( &value_ );
            }

            long operator--()
            {
                return InterlockedDecrement( &value_ );
            }

            operator long() const
            {
                return static_cast<long const volatile &>( value_ );
            }

        private:
            atomic_count( atomic_count const & );
            atomic_count & operator=( atomic_count const & );
            long value_;
        };
#else //vxworks
        class atomic_count
        {
        public:

            explicit atomic_count( long v ) : value_( static_cast< int >( v ) ) {}

            long operator++()
            {
                return atomic_exchange_and_add( &value_, +1 ) + 1;
            }

            long operator--()
            {
                return atomic_exchange_and_add( &value_, -1 ) - 1;
            }

            operator long() const
            {
                return atomic_exchange_and_add( &value_, 0 );
            }

        private:
            atomic_count(atomic_count const &);
            atomic_count & operator=(atomic_count const &);
            mutable int value_;

        private:
            static int atomic_exchange_and_add( int * pw, int dv )
            {
                // int r = *pw;
                // *pw += dv;
                // return r;
                int r;
                __asm__ __volatile__
                    (
                    "lock\n\t"
                    "xadd %1, %0":
                "+m"( *pw ), "=r"( r ): // outputs (%0, %1)
                "1"( dv ): // inputs (%2 == %1)
                "memory", "cc" // clobbers
                    );
                return r;
            }
        };
#endif

        // porting for boost, shared_ptr_nmt.hpp
        template<class Y> inline void checked_delete(Y * x)
        {
            // intentionally complex - simplification causes regressions
            typedef char type_must_be_complete[sizeof(Y)? 1: -1 ];
            (void) sizeof(type_must_be_complete);
            delete x;
        }

        typedef atomic_count count_type;

    public:
        typedef T element_type;
        typedef T value_type;

        explicit shared_ptr(T * p = 0): px(p)
        {
            try  // prevent leak if new throws
            {
                pn = new count_type(1);
            }
            catch(...)
            {
                checked_delete(p);
                throw;
            }
        }

        ~shared_ptr()
        {
            if(--*pn == 0)
            {
                checked_delete(px);
                delete pn;
            }
        }

        shared_ptr(shared_ptr const & r): px(r.px)  // never throws
        {
            pn = r.pn;
            ++*pn;
        }

        shared_ptr & operator=(shared_ptr const & r)
        {
            shared_ptr(r).swap(*this);
            return *this;
        }

        // atuo_ptr
        explicit shared_ptr(std::auto_ptr<T> & r)
        { 
            pn = new count_type(1); // may throw
            px = r.release(); // fix: moved here to stop leak if new throws
        } 

        shared_ptr & operator=(std::auto_ptr<T> & r)
        {
            shared_ptr(r).swap(*this);
            return *this;
        }


        void reset(T * p = 0)
        {
            assert(p == 0 || p != px);
            shared_ptr(p).swap(*this);
        }

        T & operator*() const  // never throws
        {
            assert(px != 0);
            return *px;
        }

        T * operator->() const  // never throws
        {
            assert(px != 0);
            return px;
        }

        T * get() const  // never throws
        {
            return px;
        }

        long use_count() const  // never throws
        {
            return *pn;
        }

        bool unique() const  // never throws
        {
            return *pn == 1;
        }

        void swap(shared_ptr<T> & other)  // never throws
        {
            std::swap(px, other.px);
            std::swap(pn, other.pn);
        }

    private:

        T * px;            // contained pointer
        count_type * pn;   // ptr to reference counter
    };

    template<class T, class U> inline bool operator==(shared_ptr<T> const & a, shared_ptr<U> const & b)
    {
        return a.get() == b.get();
    }

    template<class T, class U> inline bool operator!=(shared_ptr<T> const & a, shared_ptr<U> const & b)
    {
        return a.get() != b.get();
    }

    template<class T> inline bool operator<(shared_ptr<T> const & a, shared_ptr<T> const & b)
    {
        return std::less<T*>()(a.get(), b.get());
    }

    template<class T> void swap(shared_ptr<T> & a, shared_ptr<T> & b)
    {
        a.swap(b);
    }

    // get_pointer() enables boost::mem_fn to recognize shared_ptr

    template<class T> inline T * get_pointer(shared_ptr<T> const & p)
    {
        return p.get();
    }

}

    


#endif
