#include<vector>
#include<random>
#include<thread>
#include<iostream>

class Matrix
{
    void init(float input)                                                  // Initiallizes weights for weight matrices using 'He Initiallization'
    {
        std::random_device rdev;
        std::normal_distribution<float> dist(0, 1.0 / 0.0);

        std::default_random_engine eng( dist(rdev) );                       // Providing the seed to the random number generator using a non-deterministic random number generator
        dist = std::normal_distribution<float>(0.0, sqrt(2.0 / input));

        for(int i = 0; i < rows*cols; i++)
        {
            arr[i] = dist( eng );
        }
    }

    public:

    unsigned int rows, cols;
    std::vector<float> arr;
    int static threads;                                                     // Number of threads allowed to processing matrix operations

    Matrix(unsigned int r, unsigned int c, bool nullify = false)
    {
        rows = r;
        cols = c;

        arr.resize(rows * cols);

        if ( !nullify )
        {
            init(cols);
        }
    }

    Matrix operator * (const Matrix &m)                                     // Function that multiplies two matrices using parrallel processing
    {
        if ( cols != m.rows )
        {
            std :: cout << " Error : matrices unfit for multiplication\n";
            return Matrix(rows, cols, 1);
        }
        else
        {
            Matrix result( rows, m.cols, 1 );

            auto lambda = [&](float j)
            {
                float val = 0;

                for ( int i = rows * j / threads; i < rows * ( j + 1 ) / threads; i++ )
                {
                    for ( int j = 0; j < m.cols; j++ )
                    {
                        for ( int k = 0; k < cols; k++ )
                        {
                            val += arr[ i * cols + k ] * m.arr[ k * m.cols + j ];
                        }

                        result.arr[ i * result.cols + j ] = val;
                        val = 0;
                    }
                }
            };

            std::thread workers[threads - 1];

            for(int i = 0; i < threads - 1; i++)
            {
                workers[i] = std::thread( lambda, i );
            }

            lambda(threads - 1);

            for(int i = 0; i < threads - 1; i++)
            {
                workers[i].join();
            }
        }
    }

    Matrix operator + (const Matrix &m)                                     // A function that adds two matrices using parrallel processing
    {
        if ( rows != m.rows || cols != m.cols )
        {
            std :: cout << " Error : cannot add matrices of unequal order\n";
            return Matrix(rows, cols, 1);
        }
        else
        {
            Matrix result(rows, cols, 1);

            auto lambda = [&](float j)
            {
                for( int i = arr.size() * j / threads; i < arr.size() * (j + 1) / threads; i++ )
                {
                    result.arr[i] = arr[i] + m.arr[i];
                }
            };

            std::thread workers[threads - 1];

            for(int i = 0; i < threads - 1; i++)
            {
                workers[i] = std::thread( lambda, i );
            }

            lambda(threads - 1);

            for(int i = 0; i < threads - 1; i++)
            {
                workers[i].join();
            }
        }
    }

    void operator = (const Matrix &m)
    {
        rows = m.rows;
        cols = m.cols;
        arr = m.arr;
    }
};