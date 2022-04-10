#include"layer.cpp"

class Brain
{
public:
    std::vector<Layer> layers;
    int count;

    Brain()
    {
        
    }

    Brain(int layers)
    {
        this -> layers.resize(layers);
        count = layers;
    }

    void Decide(const Matrix &input)
    {
        layers[0].WeightedSums = layers[0].Weights * input + layers[0].Bias;
        layers[0].activate();

        for(int i = 1; i < count; i++)
        {
            layers[i].WeightedSums = layers[i].Weights * layers[i - 1].WeightedSums + layers[i].Bias;
            layers[i].activate();
        }
    }

    void Train(float *targets, const Matrix &input)
    {
        float error = 0;

        for(int i = 0; i < layers[ count - 1 ].Weights.rows ; i++)
        {
            error += pow( targets[i] - layers[ count - 1 ].WeightedSums.arr[i] ,2);
        }

        float *cachDeriv = nullptr, *nextCachDeriv = new float [ layers[ count - 2 ].Weights.rows ];

        layers[count - 1].backPropOutput(targets, layers[count - 2], nextCachDeriv, 1, error);

        for(int i = count - 2; i > 0; i--)
        {
            cachDeriv = nextCachDeriv;

            nextCachDeriv = new float[ layers[ i - 1 ].Weights.rows ];

            layers[i].backPropHidden(cachDeriv, layers[ i - 1 ], 1, error, nextCachDeriv);

            delete[] cachDeriv;
        }

        cachDeriv = nextCachDeriv;

        layers[0].backPropHidden(cachDeriv, input, 1, error);
        
        delete[] cachDeriv;
    }
};