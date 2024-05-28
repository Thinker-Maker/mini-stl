//
// Created by 董鸿博 on 2024/5/24.
//

#ifndef BITSET_BITSET_HPP
#define BITSET_BITSET_HPP
#include <iostream>
#include <vector>
namespace dhb
{
    template<size_t N>
    class bitset
    {
    public:
        bitset();
        bool test(size_t pos)const;
        void set(size_t pos);
        void reset(size_t pos);
    private:
        std::vector<int> bits;
    };

    template<size_t N>
    bitset<N>::bitset()
    {
        bits.resize(N / 32 + 1, 0);
    }

    template<size_t N>
    void bitset<N>::set(size_t pos)
    {
        size_t i = pos / 32;
        size_t j = pos % 32;
        bits[i] |= (1 << j);
    }

    template<size_t N>
    void bitset<N>::reset(size_t pos)
    {
        size_t i = pos / 32;
        size_t j = pos % 32;
        bits[i] &= ~(1 << j);
    }

    template<size_t N>
    bool bitset<N>::test(size_t pos)const
    {
        return ( bits[pos / 32] & ( 1 << ( pos % 32 ) ) ) != 0;
    }
    void test_bitset()
    {
        bitset<100> bitmap;
        bitmap.set(40);
        bitmap.set(39);
        std::cout << bitmap.test(37) << std::endl;
        std::cout << bitmap.test(38) << std::endl;
        std::cout << bitmap.test(39) << std::endl;
        std::cout << bitmap.test(40) << std::endl;
        std::cout << bitmap.test(41) << std::endl;
        std::cout << bitmap.test(42) << std::endl<< std::endl;

        bitmap.reset(40);
        std::cout << bitmap.test(37) << std::endl;
        std::cout << bitmap.test(38) << std::endl;
        std::cout << bitmap.test(39) << std::endl;
        std::cout << bitmap.test(40) << std::endl;
        std::cout << bitmap.test(41) << std::endl;
        std::cout << bitmap.test(42) << std::endl << std::endl;
    }

    template<size_t N>
    class bitsets_two
    {
    public:
        void set(size_t pos)
        {
            if(bitmap1.test(pos) == false && bitmap2.test(pos) == false)
                bitmap2.set(pos);
            else if(bitmap1.test(pos) == false && bitmap2.test(pos) == true){
                bitmap1.set(pos);
                bitmap2.reset(pos);
            }
            else if(bitmap1.test(pos) == true && bitmap2.test(pos) == false)
                bitmap2.set(pos);


        }
        void print(size_t pos) const
        {

            if(bitmap1.test(pos) == false && bitmap2.test(pos) == false)
                std::cout << "00(0)" << std::endl;
            else if(bitmap1.test(pos) == false && bitmap2.test(pos) == true)
                std::cout << "01(1)" << std::endl;
            else if(bitmap1.test(pos) == true && bitmap2.test(pos) == false)
                std::cout << "10(2)" << std::endl;
            else if(bitmap1.test(pos) == true && bitmap2.test(pos) == true)
                std::cout << "11(>=3)" << std::endl;
        }

    private:
        bitset<N> bitmap1;
        bitset<N> bitmap2;
    };

    void test_bitsets_two()
    {
        bitsets_two<100> bitmaps;
        bitmaps.print(40);
        bitmaps.set(40);
        bitmaps.print(40);
        bitmaps.set(40);
        bitmaps.print(40);
        bitmaps.set(40);
        bitmaps.print(40);
    }
}

#endif //BITSET_BITSET_HPP
