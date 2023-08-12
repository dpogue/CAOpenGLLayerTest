class PipelineLayer
{
    typedef struct objc_object* CALayerType;

protected:
    static void* LayerClass;
    CALayerType fLayer;

    static void IInitLayerClass();
    void IInitLayer();

public:
    PipelineLayer();

    CALayerType getLayer() const;
};
