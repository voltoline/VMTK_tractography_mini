#include "Transformation3DEventHandler.h"

//#include "ui/components/RenderingSurfaceSet.h"

//template <class renderer_type>
//Transformation3DEventHandler<renderer_type>::Transformation3DEventHandler()
//{

//}

//template<class renderer_type>
//void Transformation3DEventHandler<renderer_type>::update(Model3DTransformation *, Model3DTransformationEvent event_type)
//{
//    switch(event_type)
//    {

////    case Model3DTransformationEvent::THRESHOLD_DATA_CHANGED:
////    {
////        mRenderingSurfaceSet->getRenderingSurface3D()->setRenderingContext();

////        mRenderer->setEnableThreshold(m_model3DTransformation->getThresholdState());
////        mRenderer->setNoiseThreshold(m_model3DTransformation->getThreshold());

////        mRenderingSurfaceSet->update3DRendering();
////        break;
////    }

//    default:
//        break;

//    }
//}

//template <class renderer_type>
//void Transformation3DEventHandler<renderer_type>::setRenderer(renderer_type *renderer)
//{
//    mRenderer = renderer;

////    mRenderer->setEnableThreshold(m_model3DTransformation->getThresholdState());

////    if(m_model3DTransformation->getThresholdState())
////        mRenderer->setNoiseThreshold(m_model3DTransformation->getThreshold());
//}

//template <class renderer_type>
//void Transformation3DEventHandler<renderer_type>::setRenderingSurfaceSet(RenderingSurfaceSet *renderingSurfaceSet)
//{
//    mRenderingSurfaceSet = renderingSurfaceSet;
//}

//template <class renderer_type>
//void Transformation3DEventHandler<renderer_type>::setModel3DTransformation(Model3DTransformation *model3DTransformation)
//{
//    m_model3DTransformation = model3DTransformation;
////    m_model3DTransformation->subscribe(this, Model3DTransformationEvent::THRESHOLD_DATA_CHANGED);
//}
