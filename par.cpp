#include <execution>
#include <vector>
#include <chrono>
#include <stdio.h>

volatile std::byte x = std::byte(0x3a);

int main()
{
    std::vector<std::byte> data;
    size_t total = 1024ull*1024ull*1024;
    data.reserve(total);
    for( auto bytes=16384; bytes<total; bytes = bytes * 2 )
    {
        size_t cycles = total / bytes;
        data.resize(bytes);
        auto t1 = std::chrono::steady_clock::now();
        for( auto i=0; i<cycles; i++ )
        {
            x = std::byte(int(x)+1);
            std::for_each( std::execution::seq, data.begin(), data.end(), [&]( std::byte& b ){
                b = std::byte(int(b) ^ int(x));
            });    
        }
        auto t2 = std::chrono::steady_clock::now();
        for( auto i=0; i<cycles; i++ )
        {
            x = std::byte(int(x)+1);
            std::for_each( std::execution::par_unseq, data.begin(), data.end(), [&]( std::byte& b ){
                b = std::byte(int(b) ^ int(x));
            });    
        }
        auto t3 = std::chrono::steady_clock::now();
        auto d1 = (t2 - t1).count();
        auto d2 = (t3 - t2).count();
        printf( "b=%d %5.3f %5.3f secs\n", bytes, (double)d1 / 1e9, double(d2)/1e9 );
    }
    return 0;
}