#include <execution>
#include <vector>
#include <chrono>
#include <stdio.h>

volatile size_t start = 0;
volatile size_t end = 16;

int main()
{
    std::vector<uint8_t> data;
    size_t total = 1024ull*1024ull*128;
    data.reserve(total);
    for( auto bytes=4096; bytes<total; bytes = bytes * 2 )
    {
        size_t cycles = total / bytes;
        data.resize(bytes);
        auto t1 = std::chrono::steady_clock::now();
        for( volatile auto v=start; v<end; v++ )
        for( volatile auto i=size_t(0); i<cycles; i++ )
        {
            std::for_each( std::execution::seq, data.begin(), data.end(), [&]( auto& b ){
                b = int(v) * int(i) + 1;
            });    
        }
        auto t2 = std::chrono::steady_clock::now();
        for( volatile auto v=start; v<end; v++ )
        for( volatile auto i=size_t(0); i<cycles; i++ )
        {
            std::for_each( std::execution::par, data.begin(), data.end(), [&]( auto& b ){
                b = int(v) * int(i) + 1;
            });    
        }
        auto t3 = std::chrono::steady_clock::now();
        for( volatile auto v=start; v<end; v++ )
        for( volatile auto i=size_t(0); i<cycles; i++ )
        {
            std::for_each( std::execution::par_unseq, data.begin(), data.end(), [&]( auto& b ){
                b = int(v) * int(i) + 1;
            });    
        }
        auto t4 = std::chrono::steady_clock::now();
        auto d1 = (t2 - t1).count();
        auto d2 = (t3 - t2).count();
        auto d3 = (t4 - t3).count();
        printf( "b=%d %5.3f %5.3f %5.3f secs\n", bytes, (double)d1 / 1e9, double(d2)/1e9, double(d3)/1e9 );
    }
    return 0;
}
