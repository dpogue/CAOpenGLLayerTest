class pfGLRenderLayer;

class PipelineLayer
{
    typedef struct objc_object* CALayerType;
    typedef struct objc_class* Class;

protected:
    static Class LayerClass;
    CALayerType fLayer;

    static void IInitLayerClass();
    void IInitLayer();

public:
    PipelineLayer();

    pfGLRenderLayer* getLayer() const;
};
