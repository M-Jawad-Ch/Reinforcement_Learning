#include"matrices.hpp"

class Layer
{
public:
    Matrix Weights;
    Matrix WeightedSums;
    Matrix Bias;

    Layer(int size, int weightPerNode)
    {
        Weights = Matrix(size,weightPerNode);
        WeightedSums = Matrix(size);
        Bias = Matrix(size);
    }

    void rel()
    {
        for(int i = 0; i < WeightedSums.rows; i++)
        {
            if ( WeightedSums.arr[i] < 0 ) WeightedSums = 0;
        }
    }

    void backPropOutput(float *targetVals, const Layer& secondLastLayer, float *cachDeriv, float &learningRate, const float &error)
    {
        auto lambda = [&](int interval)
        {
            float del = 0, val = 0, temp = 0;
            for(int i = interval * (float)Weights.rows / Matrix::threads ; i < (interval + 1) * (float)Weights.rows / Matrix::threads; i++)
            {
                for(int j = 0; j < Weights.cols; j++)
                {
                    del = 2 * (WeightedSums.arr[i] - targetVals[i]) * secondLastLayer.WeightedSums.arr[j];
                    if ( WeightedSums.arr[i] < 0 ) del = 0;
                    val = Weights.arr[i * Weights.cols + j] - learningRate * del;
                    Weights.arr[i * Weights.cols + j] = val;
                    
                    temp = 2*(WeightedSums.arr[i] - targetVals[i]) * Weights.arr[i * Weights.cols + j];
                    if ( WeightedSums.arr[i] < 0 ) temp = 0;
                    cachDeriv[j] += temp;
                }

                del = 2 * (WeightedSums.arr[i] - targetVals[i]);
                if ( WeightedSums.arr[i] < 0 ) del = 0;
                val = Bias.arr[i] - learningRate * del;
                Bias.arr[i] = val;
            }
        };

        std :: thread t[Matrix::threads - 1];

        for(int i = 0; i < Matrix::threads - 1; i++)
        {
            t[i] = std :: thread(lambda, i);
        }

        lambda(Matrix::threads - 1);

        for(int i = 0; i < Matrix::threads - 1; i++)
        {
            t[i].join();
        }
    }

    void backPropHidden(float* cachDeriv, const Layer &previousLayer, float learningRate, const float &error, float *nextCachDeriv = nullptr)
    {
        auto lambda = [&](int interval)
        {
            float del = 0, val = 0, temp = 0;
            for(int i = interval * (float)Weights.rows / Matrix::threads; i < (interval + 1) * (float)Weights.rows / Matrix::threads; i++)
            {
                for(int j = 0; j < Weights.cols; j++)
                {
                    del = cachDeriv[i] * previousLayer.WeightedSums.arr[j];
                    val = Weights.arr[i * Weights.cols + j] - learningRate * del;
                    Weights.arr[i * Weights.cols + j] = val;

                    if(nextCachDeriv)
                    {
                        temp = cachDeriv[i] * Weights.arr[i * Weights.cols + j];
                        if ( WeightedSums.arr[i] < 0 ) temp = 0;
                        nextCachDeriv[j] += temp;
                    }
                }

                del = cachDeriv[i];
                if ( WeightedSums.arr[i] < 0 ) del = 0;
                val = Bias.arr[i] - learningRate * del;
                Bias.arr[i] = val;
            }
        };

        std :: thread t[Matrix::threads - 1];

        for(int i = 0; i < Matrix::threads - 1; i++)
        {
            t[i] = std :: thread(lambda, i);
        }

        lambda(Matrix::threads - 1);

        for(int i = 0; i < Matrix::threads - 1; i++)
        {
            t[i].join();
        }
    }

    void backPropHidden(float* cachDeriv, const Matrix &previousLayer, float learningRate, const float &error, float *nextCachDeriv = nullptr)
    {
        auto lambda = [&](int interval)
        {
            float del = 0, val = 0, temp = 0;
            for(int i = interval * (float)Weights.rows / Matrix::threads; i < (interval + 1) * (float)Weights.rows / Matrix::threads; i++)
            {
                for(int j = 0; j < Weights.cols; j++)
                {
                    del = cachDeriv[i] * previousLayer.arr[j];
                    if ( previousLayer.arr[j] < 0 ) del = 0;
                    val = Weights.arr[i * Weights.cols + j] - learningRate * del;
                    Weights.arr[i * Weights.cols + j] = val;

                    if(nextCachDeriv)
                    {
                        temp = cachDeriv[i] * Weights.arr[i * Weights.cols + j];
                        if ( WeightedSums.arr[i] < 0 ) temp = 0;

                        nextCachDeriv[j] += temp;
                    }
                }

                del = cachDeriv[i];
                if ( WeightedSums.arr[i] < 0 ) del = 0;
                val = Bias.arr[i] - learningRate * del;
                Bias.arr[i] = val;
            }
        };

        std :: thread t[Matrix::threads - 1];

        for(int i = 0; i < Matrix::threads - 1; i++)
        {
            t[i] = std :: thread(lambda, i);
        }

        lambda(Matrix::threads - 1);

        for(int i = 0; i < Matrix::threads - 1; i++)
        {
            t[i].join();
        }
    }
};