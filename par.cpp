#include <execution>
#include <vector>
#include <chrono>
#include <stdio.h>

#ifdef _MSC_VER
#include <ppl.h>
#endif

volatile size_t start = 0;
volatile size_t end = 4;

struct alignas(1024) element {
    volatile uint8_t b;
};

static void process( element& t )
{
    t.b = 1;
}

int main()
{
    std::vector<element> data;
    size_t total = 1024ull*1024ull*4;
    data.resize(total);
    for( auto& b : data )
        b.b = rand()%256;
    auto data1 = data;
    auto data2 = data;
    auto data3 = data;
    auto data4 = data;
    for( auto elements=128; elements<total; elements = elements * 2 )
    {
        size_t cycles = total / elements;
        auto t1 = std::chrono::steady_clock::now();
        for( volatile auto v=start; v<end; v++ )
        for( volatile auto i=size_t(0); i<cycles; i++ )
        {
            std::for_each( std::execution::seq, data1.begin(), data1.begin() + elements, process );
        }
        auto t2 = std::chrono::steady_clock::now();
        for( volatile auto v=start; v<end; v++ )
        for( volatile auto i=size_t(0); i<cycles; i++ )
        {
            std::for_each( std::execution::par, data2.begin(), data2.begin() + elements, process );
        }
        auto t3 = std::chrono::steady_clock::now();
        for( volatile auto v=start; v<end; v++ )
        for( volatile auto i=size_t(0); i<cycles; i++ )
        {
            std::for_each( std::execution::par_unseq, data3.begin(), data3.begin() + elements, process );
        }
        auto t4 = std::chrono::steady_clock::now();
        for( volatile auto v=start; v<end; v++ )
        for( volatile auto i=size_t(0); i<cycles; i++ )
        {
            concurrency::parallel_for_each( data4.begin(), data4.begin() + elements, process );
        }
        auto t5 = std::chrono::steady_clock::now();
        auto d1 = (t2 - t1).count();
        auto d2 = (t3 - t2).count();
        auto d3 = (t4 - t3).count();
        auto d4 = (t5 - t4).count();
        printf( "b=%d %5.3f %5.3f %5.3f %5.3f secs, %d elements\n", elements, (double)d1 / 1e9, double(d2)/1e9, double(d3)/1e9, double(d4)/1e9, elements );
    }
    return 0;
}
